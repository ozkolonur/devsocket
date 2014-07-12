ARGS = -g -Wall -Wextra -lpthread

all: client server

client: client.c
	gcc $(ARGS) -o client client.c

server: server.c
	gcc $(ARGS) -o server server.c

clean:
	rm -f server
	rm -f client
