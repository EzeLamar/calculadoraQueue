all: server client
.PHONY: all

server: server.c msje.h
	gcc -o server server.c 

client: client.c msje.h
	gcc -o client client.c
