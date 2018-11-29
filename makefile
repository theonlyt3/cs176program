CC = gcc
ARGS = -Wall

all: server_c_tcp client_c_tcp

server_c_tcp: server_c_tcp.c
	$(CC) $(ARGS) -o server_c_tcp server_c_tcp.c

client_c_tcp: client_c_tcp.c
	$(CC) $(ARGS) -o client_c_tcp client_c_tcp.c

clean:
	rm -f *.o server_c_tcp *~
	rm -f *.o client_c_tcp *~
