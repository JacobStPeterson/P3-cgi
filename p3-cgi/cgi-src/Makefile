# -*- makefile -*-

include Make.src

default: $(CGI)

test: $(CGI)
	make -C tests test

CC=gcc
CFLAGS=-O0 -Wall --std=gnu99 -pedantic
LDFLAGS=-O0

%.cgi: %.c
	$(CC) $(CFLAGS) -o ../cgi-bin/$@ $<

clean:
	rm -f ../cgi-bin/*.cgi
