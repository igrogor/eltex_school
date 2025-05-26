all: client server

client: client_tcp.o
	gcc client_tcp.c -o client

server: server_tcp.o
	gcc server_tcp.c -o server

clean:
	rm ./server ./client client_tcp.o server_tcp.o