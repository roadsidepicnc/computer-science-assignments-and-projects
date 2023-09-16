#include <stdio.h>
#include <sys/socket.h>
#include <sys/queue.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include "server.h"

////////////////////////////////////////////////////

ServantData * servantData; 

 /*  For printing timestamp*/
char timeStamp[100];   

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;             /* Condition Variable */ 
pthread_mutex_t mutexQueue = PTHREAD_MUTEX_INITIALIZER;     /* Mutex For The Synchronizing Queue Operations */ 
pthread_mutex_t mutexServant = PTHREAD_MUTEX_INITIALIZER;   /* Mutex For The Synchronizing Servant Operations */

pthread_t mainThread;     /* Main thread */
pthread_t * poolThreads;  /* Pool threads */ 

struct Queue socketIDs;   /* Queue For Keeping Socket IDs */

int SIGINTHappened = 0;  /*  Integer For SIGINT*/
int SIGUSR1Happened = 0; /*  Integer For SIGUSR1*/
int numberOfThreads;    /*  For number of threads at the server*/
int numberOfServants = 0; /*  For counting number of servants at the server*/
int numberOfTotalOps = 0; /*  For counting number of total operations at the server*/
int LISTEN_BACKLOG = 100;
int port;               /* PORT input */



////////////////////////////////////////////////////

void SIGINTHANDLER()    /* SIGINT Signal Handler*/
{
    pthread_kill(mainThread, SIGUSR1);  
    SIGINTHappened = 1;
}

void SIGUSR1HANDLER()   /* SIGUSR1 Signal Handler*/
{
    SIGUSR1Happened = 1;
}

int findNumberOfServant(char * city)
{
    for (int i = 0; i < numberOfServants; i++)
    {
        for (int j = 0; j < servantData[i].numberOfCities; j++)
        {
            if(strcmp(city, servantData[i].cities[j]) == 0) return i;               
        }
    }
    return -1;
}

void * mainThreadFunction(void * var)
{
    // Creats Socket
    struct sockaddr_in my_addr;
    int opt = 1;
    int mainSocket = socket(AF_INET, SOCK_STREAM, 0);

    if(mainSocket == -1)
    {
        fprintf(stderr, "socket yok\n");
        exit(EXIT_FAILURE);
    }

    if(setsockopt(mainSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    { 
        fprintf(stderr, "setsockopt yok\n");
        exit(EXIT_FAILURE);
    }

    memset(&my_addr, 0, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_addr.s_addr = INADDR_ANY;
    my_addr.sin_port = htons(port);

    if(bind(mainSocket, (struct sockaddr*) &my_addr, sizeof(my_addr)) == -1)
    {
        fprintf(stderr, "Server: Bind Error\n");
        shutdown(mainSocket, SHUT_RDWR);
        exit(EXIT_FAILURE);
    }

    if(listen(mainSocket, LISTEN_BACKLOG) == -1)
    {
        fprintf(stderr, "Server: Listen Error\n");
        exit(EXIT_FAILURE);
    }

    socklen_t my_addr_size = sizeof(my_addr);
    
    while(SIGUSR1Happened == 0) /* Continues Until SIGINT Occurs*/
    {
        int newSocket = accept(mainSocket, (struct sockaddr*) &my_addr, &my_addr_size);
        if (newSocket == -1)
        {
            fprintf(stderr, "Server: Accept Error!\n");
            continue;
        }

        pthread_mutex_lock(&mutexQueue);
        enqueue(&socketIDs, newSocket);     /* Adds new socket is to the queue */
        pthread_cond_broadcast(&cond);      /* Wakes pool threads to handle the newly came socketid */
        pthread_mutex_unlock(&mutexQueue);
	}

    printf("BOK\n");

    pthread_mutex_lock(&mutexQueue); /* If SIGINT occurs, process will send SIGUSR1 to main Thread then main thread sends SIGUSR1 to the pool threads */
    pthread_cond_broadcast(&cond);   
    for(int i = 0;i < numberOfThreads;++i) pthread_kill(poolThreads[i],SIGUSR1);     
    pthread_mutex_unlock(&mutexQueue);

    shutdown(mainSocket, SHUT_RDWR);  /*Closes main Socket*/
    pthread_exit(NULL);

}

void * poolThreadFunction(void * var)
{
    /*  SIGUSR1 handler  */
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &SIGUSR1HANDLER;
    sa.sa_flags = 0;
    if((sigemptyset(&sa.sa_mask) == -1) || (sigaction(SIGUSR1, &sa, NULL) == -1))
    {
        fprintf(stderr, "Failed to install signal sa_handler\n");
        exit(EXIT_FAILURE);
    }

    while(1)
    {
        pthread_mutex_lock(&mutexQueue);
        while(empty(&socketIDs) == 0 && SIGUSR1Happened == 0) pthread_cond_wait(&cond, &mutexQueue);
          
        if(SIGUSR1Happened == 1)
        {
            pthread_mutex_unlock(&mutexQueue);
            break;
        }

        int socketID = dequeue(&socketIDs); /* Gets the top of the queue and removes it from the queue. It is the latest socket id */
        char buffer[1024];
        char buffer2[1024];
        char s[2] = " ";
        memset(buffer, 0, sizeof(buffer));
        read(socketID, buffer,sizeof(buffer));

        if(buffer[strlen(buffer) - 1] == 'c')   /* If Inputs is From Client */
        {
            char input[5][30];
            char* token;
            int inputCounter = 0;
            int numberOfServant = -31;
            buffer[strlen(buffer) - 1] = '\0';
            getTime(timeStamp);
            fprintf(stdout, "[%s] Request arrivered \"%s\"\n", timeStamp, buffer);
            strcpy(buffer2, buffer);            
            memset(input, 0, 150);

            token = strtok(buffer2, s);
    
            while(token != NULL)
            {   
                strcpy(input[inputCounter], token);
                token = strtok(NULL, s);
                inputCounter++;
            }

            if(inputCounter < 4) fprintf(stderr, "Input Error!\n");

            if(inputCounter == 5)   /* If City is Provided at the input */
            {
                numberOfServant = findNumberOfServant(input[4]);

                if(numberOfServant == -1)   /* If There is No Suitable Servant at the system */
                { 
                    memset(buffer, 0, sizeof(buffer));
                    sprintf(buffer, "no_servant");
                }
                else
                {
                    getTime(timeStamp);
                    fprintf(stdout, "[%s] Contacting servant %d\n", timeStamp, servantData[numberOfServant].pid);
                    struct sockaddr_in servant_addr;
                    memset(&servant_addr, 0, sizeof(servant_addr));
                    servant_addr.sin_family = AF_INET;
                    servant_addr.sin_port = htons(servantData[numberOfServant].uniquePort);
                    
                    if(inet_pton(AF_INET, servantData[numberOfServant].ip, &servant_addr.sin_addr) == -1)
                    {
                        fprintf(stderr, "inet Error!\n");
                        exit(EXIT_FAILURE);
                    }

                    int servantSocket = socket(AF_INET, SOCK_STREAM, 0);

                    if(servantSocket == -1)
                    {
                        fprintf(stderr, "Socket Error!\n");
                        exit(EXIT_FAILURE);
                    }
                    if(connect(servantSocket, (struct sockaddr*) &servant_addr, sizeof(servant_addr)) == -1)
                    {
                        fprintf(stderr, "Connect Error!\n");
                        close(servantSocket);
                        exit(EXIT_FAILURE);
                    }

                    write(servantSocket, buffer, strlen(buffer));   /* Sends the request to the certain servant */
                    memset(buffer, 0, strlen(buffer));
                    read(servantSocket, buffer, sizeof(buffer));    /* Reads the respond from the certain servant */   
                }
            }
            else if(inputCounter == 4)  /* If City is Not Provided at the input. Check all of the servants*/
            {
                getTime(timeStamp);
                fprintf(stdout, "[%s] Contacting ALL servants\n", timeStamp);
                int res = 0;
                struct sockaddr_in servant_addr;
                memset(&servant_addr, 0, sizeof(servant_addr));
                servant_addr.sin_family = AF_INET;

                for(int i = 0; i < numberOfServants; i++)
                {
                    servant_addr.sin_port = htons(servantData[i].uniquePort);
                    
                    if(inet_pton(AF_INET, servantData[i].ip, &servant_addr.sin_addr) == -1)
                    {
                        fprintf(stderr, "inet Error!\n\n");
                        exit(EXIT_FAILURE);
                    }

                    int servantSocket = socket(AF_INET, SOCK_STREAM, 0);

                    if(servantSocket == -1)
                    {
                        fprintf(stderr, "Socket Error!\n");
                        exit(EXIT_FAILURE);
                    }

                    if(connect(servantSocket, (struct sockaddr*) &servant_addr, sizeof(servant_addr)) == -1)
                    {
                        fprintf(stderr, "Connect Error!\n");
                        close(servantSocket);
                        exit(EXIT_FAILURE);
                    }

                    write(servantSocket, buffer, strlen(buffer));   /* Sends the request to all of the servants */
                    char resultBuffer[20];
                    memset(resultBuffer, 0, sizeof(resultBuffer));
                    read(servantSocket, resultBuffer, sizeof(resultBuffer));    /* Reads from the each servant at the system */
                    res += atoi(resultBuffer);
                    close(servantSocket);
                }

                memset(buffer,0, strlen(buffer));
                sprintf(buffer,"%d", res);
            }

            write(socketID, buffer, strlen(buffer));        /* Sends respond to the client */ 
            getTime(timeStamp);       
            fprintf(stdout, "[%s] Response received: %s, forwarded to client\n", timeStamp, buffer);
            numberOfTotalOps++; /* Increments total number of operations by 1 */

        }
        else if(buffer[strlen(buffer) - 1] == 's') /* For getting the servant informations from the servants */
        {
            pthread_mutex_lock(&mutexServant);  /* For synchronizing servant operations */
            servantData = (ServantData *) realloc(servantData, sizeof(ServantData) * (numberOfServants + 1));
            servantData[numberOfServants].cities = (char **) malloc(sizeof(char *));
            servantData[numberOfServants].cities[0] = (char *) malloc(sizeof(char));

            buffer[strlen(buffer) - 1] = '\0';

            int counter = 0;
            char * token = NULL;
            char s[2] = ",";    

            token = strtok(buffer, s);
    
            while(token != NULL)    /* Gets the servant infos and stores them at the ServantData struct i created at the header file*/
            {   
                if(counter == 0) servantData[numberOfServants].uniquePort = atoi(token);
                else if(counter == 1) strcpy(servantData[numberOfServants].ip, token);
                else if(counter == 2) servantData[numberOfServants].pid =  atoi(token);
                else
                {
                    servantData[numberOfServants].cities = (char **) realloc(servantData[numberOfServants].cities, sizeof(char *) * (servantData[numberOfServants].numberOfCities + 1));
                    servantData[numberOfServants].cities[servantData[numberOfServants].numberOfCities] = (char *) malloc(20);
                    strcpy(servantData[numberOfServants].cities[servantData[numberOfServants].numberOfCities], token);
                    ++servantData[numberOfServants].numberOfCities;
                }

                token = strtok(NULL, s);
                counter++;
            }

            getTime(timeStamp);
            fprintf(stdout, "[%s] Servant %d present at port %d handling cities %s-%s\n", timeStamp, servantData[numberOfServants].pid, servantData[numberOfServants].uniquePort, servantData[numberOfServants].cities[0], 
                    servantData[numberOfServants].cities[servantData[numberOfServants].numberOfCities - 1]);
            numberOfServants++; /* Increments the number of servants at the server by 1*/
            pthread_mutex_unlock(&mutexServant);
        }

        pthread_mutex_unlock(&mutexQueue);
        close(socketID);    /* Closes socket*/
    }

    pthread_exit(NULL);
}

////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
    if(argc != 5)
    {
        fprintf(stderr, "Wrong Number of Inputs!");
        exit(EXIT_FAILURE);
    }

    if(strcmp(argv[1], "-p") != 0 || strcmp(argv[3], "-t") != 0)
    {
        fprintf(stderr, "Wrong Inputs!");
        exit(EXIT_FAILURE);
    }    

    /*  SIGINT handler  */
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &SIGINTHANDLER;
    sa.sa_flags = 0;
    if((sigemptyset(&sa.sa_mask) == -1) || (sigaction(SIGINT, &sa, NULL) == -1)){
        fprintf(stderr, "Failed to install signal sa_handler\n");
        exit(EXIT_FAILURE);
    }

    if((sigemptyset(&sa.sa_mask) == -1) || (sigaction(SIGINT, &sa, NULL) == -1))
    {
        fprintf(stderr, "Failed to install signal sa_handler\n");
        exit(EXIT_FAILURE);
    }

    port = atoi(argv[2]);
    numberOfThreads = atoi(argv[4]);

    servantData = (ServantData *) malloc(sizeof(ServantData) * (numberOfServants + 1));

    /*  Creates main thread  */
    if(pthread_create(&mainThread, NULL, mainThreadFunction, NULL) != 0)
    {
        fprintf(stderr, "Create Error!\n");
        exit(EXIT_FAILURE);
    }
    
    /*  Creates pool threads  */
    poolThreads = (pthread_t*)malloc(sizeof(pthread_t) * (numberOfThreads));
    char Index[numberOfThreads][10];

    for(int i = 0;i < numberOfThreads;++i)
    {
        sprintf(Index[i],"%d", i);

        if(pthread_create(poolThreads + i, NULL, poolThreadFunction, (void*)Index[i]) != 0)
        {
            fprintf(stderr, "Create Error!\n");
            exit(EXIT_FAILURE);
        }   
    }

    /*  Waits for pool threads to end  */
    for(int i = 0; i < numberOfThreads; i++)
    {
        if(pthread_join(poolThreads[i], NULL) != 0)
        {
            fprintf(stderr, "Join Error!\n");
            exit(EXIT_FAILURE);
        }
    }

    /*  Waits for main thread to end  */
    if(pthread_join(mainThread, NULL) != 0)
    {
        fprintf(stderr, "Join Error!\n");
        exit(EXIT_FAILURE);
    }

    getTime(timeStamp);
    fprintf(stdout, "[%s] SIGINT has been received. I handled a total of %d requests. Goodbye.\n", timeStamp, numberOfTotalOps);

    for(int i = 0;i < numberOfServants;++i) 
    {
        kill(servantData[i].pid, SIGINT);
    }
        
    /*  Frees allocated space */
    for(int i = 0;i < numberOfServants; i++)
    {
        for (int j = 0; j < servantData[i].numberOfCities; j++)
        {
            free(servantData[i].cities[j]);
        }
        free(servantData[i].cities);
    }
    
    free(servantData);
    free(poolThreads);
    freeQueue(&socketIDs);
    return 0;
}