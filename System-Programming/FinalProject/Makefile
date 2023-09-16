all: server client servant

server: server.c
	gcc -o server server.c -pthread -lrt -Wall

client: client.c
	gcc -o client client.c -pthread -lrt -Wall

servant: servant.c
	gcc -o servant servant.c -pthread -lrt -Wall

clean:
	$(RM) server
	$(RM) client
	$(RM) servant