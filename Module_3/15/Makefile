all: client server

client: client_tcp.o
	gcc client_tcp.c -o client_tcp

server: server_tcp.o
	gcc server_tcp.c -o server_tcp

clean:
	rm ./server_tcp ./client_tcp server_tcp.o client_tcp.o