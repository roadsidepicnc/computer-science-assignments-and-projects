#include <stdio.h>
#include <sys/socket.h>
#include <sys/queue.h>
#include <string.h>
#include <sys/mman.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/sem.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

////////////////////////////////////////////////////

int mutexCounter = 0;                                       /* Counter For Making Making Threads write at the same time */
pthread_mutex_t mutexClient = PTHREAD_MUTEX_INITIALIZER;    /* Mutex For Making Threads write at the same time */
pthread_cond_t condEmpty = PTHREAD_COND_INITIALIZER;        /* Condition variable For Making Threads write at the same time */

int LISTEN_BACKLOG = 100;
int numberOfThreads = 0;  /* Number of threads at the server */

int port;    /* PORT input */
char ip[30]; /* ip input */

////////////////////////////////////////////////////

void * clientThreadFunction(void * var)
{   
    char id[20];
    char s[2] = ",";    // For tokenizing
    char buffer[1024];
    strcpy(buffer, var);
    char * token = strtok ((char *) var, s);
    strcpy(buffer, token);   
    token = strtok (NULL, s);
    strcpy(id, token);  // id of the thread
    fprintf(stdout,"Client-Thread-%s: Thread-%s has been created\n", id,id);    
    
    // Creats Socket
    struct sockaddr_in my_addr;
    memset(&my_addr, 0, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(port);
    
    if(inet_pton(AF_INET, ip, &my_addr.sin_addr) == -1)
    {
        fprintf(stderr, "inet Error!\n");
        exit(EXIT_FAILURE);
    }

    pthread_mutex_lock(&mutexClient);

    mutexCounter++; // Counts for the mutex

    if(mutexCounter >= numberOfThreads) pthread_cond_broadcast(&condEmpty); // For making write threads at the same time

    while(mutexCounter < numberOfThreads)
    {
        pthread_cond_wait(&condEmpty, &mutexClient);    // Threads wait here
    } 

    pthread_mutex_unlock(&mutexClient);    

    int mainSocket;

    while(1)
    {
        mainSocket = socket(AF_INET, SOCK_STREAM, 0);

        if(mainSocket == -1)
        {
            fprintf(stderr, "Socket Error!\n");
            exit(EXIT_FAILURE);
        }

        if(connect(mainSocket, (struct sockaddr*) &my_addr, sizeof(my_addr)) == -1)
        {
            fprintf(stderr, "Connect Error!\n");
            close(mainSocket);
            continue;
        }

        break;
 	}

        fprintf(stdout,"Client-Thread-%s: I am requesting \"\\%s\"\n", id, buffer);
        sprintf(buffer, "%sc", buffer);     /*  Put 'c' to the end of the buffer for making it server understand it is client's input*/
        write(mainSocket, buffer, strlen(buffer));

        char returnString[100];
        buffer[strlen(buffer) - 1] = '\0';  /*  Removes 'c' from the end of the buffer for printing*/
        memset(returnString, 0, sizeof(returnString));
        read(mainSocket, returnString, sizeof(returnString));    /* Reads response from the server */

        if(strcmp(returnString,"no_servant") == 0) fprintf(stdout,"Client-Thread-%s: The servers response to \"%s\" is no servant is responsible for that city\n", id, buffer);
        else fprintf(stdout,"Client-Thread-%s: The servers response to \"\\%s\" is %s is responsible for that city\n", id, buffer, returnString); 

        close(mainSocket); /* Closes the socket */

        fprintf(stdout,"Client-Thread-%s terminating.\n", id);

        pthread_exit(NULL);
}

////////////////////////////////////////////////////

int main(int argc, char * argv[])
{
    if(argc != 7)
    {
        fprintf(stderr, "Wrong Number of Inputs!");
        exit(EXIT_FAILURE);
    }

    if(strcmp(argv[1],"-r") != 0 || strcmp(argv[3], "-q") != 0 || strcmp(argv[5],"-s") != 0)
    {
        fprintf(stderr, "Wrong Inputs!");
        exit(EXIT_FAILURE);
    }
    
    int fd = open(argv[2], O_RDONLY);

    if(fd == -1)
    {
        fprintf(stderr, "File Error!\n");
        exit(EXIT_FAILURE);
    }

    port = atoi(argv[4]);
    strcpy(ip, argv[6]);
 
    char line[100];         // Reads data on the line at the input file
    char buffer[1];     
    int lineCounter = 0;    // Counts the number at the each line
    char ** lines = (char **) malloc(sizeof(char *));

    /* Reads the data from the request file */
    while(1)
    {
        size_t readSize = read(fd, buffer, sizeof(buffer));

        if(readSize != sizeof(buffer)) break;
          
        if(buffer[0] == '\n' && lineCounter == 0)
        {
            continue;
        }

        if(buffer[0] == '\n')
        {
            lines = (char **) realloc(lines, sizeof(char*) * (numberOfThreads + 1));
            lines[numberOfThreads] = (char *) malloc(sizeof(char) * lineCounter);
            strcpy(lines[numberOfThreads], line);
            numberOfThreads++;
            memset(&line, 0, sizeof(line));
            lineCounter = 0;
            continue;
        }

        line[lineCounter] = buffer[0];
        lineCounter++;
    }

    fprintf(stdout, "Client: I have loaded %d requests and I’m creating %d threads.\n", numberOfThreads, numberOfThreads);

    /* Creates threads */
    pthread_t * clientThreads = (pthread_t*)malloc(sizeof(pthread_t) * numberOfThreads);

    for (int i = 0; i < numberOfThreads; i++)
    {
        char temp[5];
        sprintf(temp,",%d",i);
        strcat(lines[i],temp);

        if(pthread_create(clientThreads + i, NULL, clientThreadFunction, (void*)lines[i]) != 0) 
        {
            fprintf(stderr, "Create Error!\n");
            exit(EXIT_FAILURE);
        }
    }

    /* Waits for the threads to end */
    for(int i = 0; i < numberOfThreads; ++i)
    {
        if(pthread_join(clientThreads[i], NULL) != 0)
        {
            fprintf(stderr, "Join Error!\n");
            exit(EXIT_FAILURE);
        }
    }

    /* Frees the allocated space */
    fprintf(stdout, "All threads have terminated, goodbye.\n");  
    free(lines);
    free(clientThreads);
	close(fd);

	return 0;
}