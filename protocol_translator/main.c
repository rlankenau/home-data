#include "unistd.h"
#include "stdio.h"
#include "stdlib.h"
#include "errno.h"
#include "string.h"
#include "signal.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "sys/socket.h"
#include "fcntl.h"
#include "netinet/in.h"
#include "arpa/inet.h"

char *  locations[255];
char *  measurements[255];

int running = 1;

void handle_signal(int signum)
{
	if(signum == SIGINT)
	{
		running = 0;
	}
}

int main(int argc, char **argv)
{
	int		in_fd, sock_fd;
	char 	*buffer;
	char	*start, *end, *p;
	size_t	buffsz = 1024;
	struct 	sigaction sigact;
	struct  sockaddr_in peer;
	int 	rtn = 0;
	char	outbuf[1024];

	memset(locations, 0, sizeof(char *)*255);
	memset(measurements, 0, sizeof(char *)*255);

	locations[1] = "Library";
	locations[2] = "Ashley_Office";
	locations[3] = "Living_Room";
	locations[4] = "Dining_Room";
	locations[5] = "Wood_Shop";
	locations[6] = "Laundry";
	locations[7] = "Storage_Area";
	locations[8] = "Furnace_Area";
	locations[9] = "Breaker_Box_Area";
	locations[10] = "Master_Bedroom";
	locations[11] = "Guest_Bedroom";
	locations[12] = "Molly_Bedroom";
	locations[13] = "Attic";
	locations[14] = "Front_Yard";
	locations[15] = "Back_Yard";
	locations[16] = "Garden";
	locations[100] = "Russ_Office";
	locations[101] = "Kitchen";
	locations[102] = "Garage";

	measurements[1] = "Temperature";
	measurements[2] = "Humidity";

	if(argc != 2)
	{
		fprintf(stderr, "Usage: %s <file>\n", argv[0]);
		return -1;
	}

	errno = 0;
	in_fd = open(argv[1], O_RDONLY);
	if(in_fd == -1)
	{
		fprintf(stderr, "Error opening file: %s.", strerror(errno));
		return -1;
	}

	sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sock_fd == -1)
	{
		fprintf(stderr, "Error opening socket: %s\n", strerror(errno));
		close(in_fd);
		return -1;
	}

	peer.sin_family = AF_INET;
	peer.sin_port 	= htons(8094);
	inet_pton(AF_INET, "127.0.0.1", &peer.sin_addr.s_addr);
	memset(&peer.sin_zero, 0, sizeof(peer.sin_zero));

	/* Connect to the telegraf socket */
	if(connect(sock_fd, (const struct sockaddr*)&peer, sizeof(struct sockaddr_in)) == -1)
	{
		fprintf(stderr, "Could not connect to peer: %s\n", strerror(errno));
		close(sock_fd);
		close(in_fd);
		return -1;
	}

	buffer = malloc(1024);
	if(!buffer)
	{
		fprintf(stderr, "Could not allocate memory for buffer: %s", strerror(errno));
		close(sock_fd);
		close(in_fd);
		return -1;
	}

	start = buffer;
	end = buffer;

	sigact.sa_handler = handle_signal;
	sigemptyset(&sigact.sa_mask);
	if(sigaction(SIGINT, &sigact, NULL) == -1)
	{
		fprintf(stderr, "Could not register signal handler: %s\n", strerror(errno));
		close(sock_fd);
		close(in_fd);
		return -1;
	}

	while(running)
	{
		ssize_t n_bytes = read(in_fd, start, 1024-(start-buffer)); 
		if(n_bytes == 0)
		{
			usleep(10000);
			continue;
		}
		end = start+n_bytes;
		/* Scan from start to end looking for \n */
		for(p = start; p < end;p++)
		{
			if(*p == '\n')
			{
				int loc, measurement;
				int cmd, type, ack; 
				float value;

				*p='\0';
				if (sscanf(start, "%d;%d;%d;%d;%d;%f", &loc, &measurement, &cmd, &ack, &type, &value) == 6) {
					if(locations[loc] && measurements[measurement]) {
						int out_bytes = snprintf(outbuf, 
								sizeof(outbuf), 
								"home_data,location=%s,sensortype=%s %s=%f\n", 
								locations[loc], 
								measurements[measurement], measurements[measurement], 
								value);

						if(out_bytes > 0)
						{
							send(sock_fd, outbuf, out_bytes, 0);
						}
						else
						{
							fprintf(stderr, "No data to send for string [%s]\n", start);
						}
					}
					else
					{
						fprintf(stderr, "No location for %d or measurement for %d\n", loc, measurement);
					}
				}
				else
				{
					fprintf(stderr, "Input error in string: [%s]\n", start);
				}

				start = p + 1;
			}
		}
		/* Got to the end of the buffer.  Move it back to the beginning. */
		n_bytes = end-p;
		memmove(buffer, start, n_bytes);
		start = buffer + n_bytes;
	}

	free(buffer);

	if(errno != 0)
	{
		fprintf(stderr, "Exiting with error: %s", strerror(errno));
		rtn = -1;;
	}

	close(sock_fd);
	close(in_fd);
	return rtn;
}
