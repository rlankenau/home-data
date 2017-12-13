CC=gcc
CFLAGS=-O3

all: parser

install: parser
	adduser --quiet --system --group --no-create-home home_data
	adduser home_data dialout
	mkdir -p /usr/local/bin/home_data/
	mkdir -p /usr/local/src/home_data/
	cp parser /usr/local/bin/home_data/parser
	chown home_data:home_data /usr/local/bin/home_data/parser
	cp parser.c /usr/local/src/home_data/parser.c
	chown home_data:home_data /usr/local/src/home_data/parser.c
	cp home_data.service /lib/systemd/system/home_data.service
	ln -s /lib/systemd/system/home_data.service /etc/systemd/system/home_data.service
	/bin/systemctl daemon-reload
	/bin/systemctl enable home_data.service

clean:
	-rm parser

parser: parser.c
