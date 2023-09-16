all: server_Y client server_Z

server_Y: server_Y.c
	gcc -o server_Y server_Y.c -lpthread -lm

client: client.c
	gcc -o client client.c

server_Z: server_Z.c
	gcc -o server_Z server_Z.c -lpthread -lm -lrt

clean:
	$(RM) server_Y
	$(RM) client
	$(RM) server_Z