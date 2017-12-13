CC=gcc
CFLAGS=-O3

all: home_data_gw_service

install: home_data_gw_service 
	adduser --quiet --system --group --no-create-home home_data
	adduser home_data dialout
	mkdir -p /usr/local/home_data/bin/
	mkdir -p /usr/local/home_data/src/protocol_translator/
	cp home_data_gw_service /usr/local/home_data/bin/
	cp main.c /usr/local/home_data/src/protocol_translator/main.c
	chown -R home_data:home_data /usr/local/home_data/
	cp home_data.service /lib/systemd/system/home_data.service
	ln -s /lib/systemd/system/home_data.service /etc/systemd/system/home_data.service
	/bin/systemctl daemon-reload
	/bin/systemctl enable home_data.service

clean:
	-rm home_data_gw_service

home_data_gw_service: main.c
