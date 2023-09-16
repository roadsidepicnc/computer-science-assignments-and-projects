#include <stdio.h>
#include <sys/socket.h>
#include <sys/queue.h>
#include <string.h>
#include <sys/mman.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/sem.h>
#include <pthread.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include "servant.h"

////////////////////////////////////////////////////

int PORTCONSTANT = 16000;
int LISTEN_BACKLOG = 100;
int port;		/* input port */
int uniquePort;	/* port of this unique servant */
char ip[30];	/* ip input */
char * pid;		/*  Process id provided with /proc */
char directoryPath[100];

int numberOfDirectories = 0; /*  Number of directories  */
int SIGINTHappened = 0;		/*  Integer For SIGINT*/
int numberOfTotalOps = 0;	/*  Number of connections done */

LinkedList list;	/*  Filenames between lower limit and upper limit stored in the linked list*/

pthread_t servantThread; /* thread */

////////////////////////////////////////////////////

void SIGUSR1HANDLER() /*  SIGUSR1 handler  */
{
    SIGINTHappened = 1;
}

void SIGINTHANDLER() /*  SIGINT handler  */
{
    pthread_kill(servantThread, SIGUSR1); /*  Sends SIGUSR1 to the Thread to end */
    SIGINTHappened = 1;
}

void * servantThreadFunction(void * var)
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

    if((sigemptyset(&sa.sa_mask) == -1) || (sigaction(SIGUSR1, &sa, NULL) == -1))
    {
        fprintf(stderr, "Servant: Failed to install signal sa_handler");
        exit(EXIT_FAILURE);
    }

    // Creats Socket
	struct sockaddr_in new_addr;
    int opt = 1;
    int mySocket = socket(AF_INET, SOCK_STREAM, 0);

    if(mySocket == -1)
    {
        fprintf(stderr, "Servant: socket Error!\n");
        exit(EXIT_FAILURE);
    }

    if(setsockopt(mySocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        fprintf(stderr, "Servant: setsockopt Error!\n");
        shutdown(mySocket, SHUT_RDWR);
        exit(EXIT_FAILURE);
    }

    memset(&new_addr, 0, sizeof(new_addr));
    new_addr.sin_family = AF_INET;
    new_addr.sin_addr.s_addr = INADDR_ANY;
    new_addr.sin_port = htons(uniquePort);

    if(bind(mySocket, (struct sockaddr*) &new_addr, sizeof(new_addr)) == -1)
    {
        fprintf(stderr, "Servant: Bind Error!\n");
        shutdown(mySocket, SHUT_RDWR);
        exit(EXIT_FAILURE);
    }

    if(listen(mySocket, LISTEN_BACKLOG) == -1)
    {
        fprintf(stderr, "Servant: Listen Error!\n");
        shutdown(mySocket, SHUT_RDWR);
        exit(EXIT_FAILURE);
    }

    socklen_t new_addr_size = sizeof(new_addr);
    fprintf(stdout, "Servant %s: listening at port %d\n", pid, uniquePort);

    while(SIGINTHappened == 0)	/*  Continues until SIGINT occurs */
    {
    	int newSocket = accept(mySocket, (struct sockaddr*) &new_addr, &new_addr_size);

        if (newSocket == -1)
        {
            if(SIGINTHappened) break;               
            fprintf(stderr, "Servant: Accept Error!\n");
            continue;
        }

        char buffer[1024];
        memset(buffer,0,sizeof(buffer));
        read(newSocket, buffer,sizeof(buffer));	/*  Read client string from the server   */
      
      	int result = 0; 
      	DIR * directory;
        int inputCounter = 0;
        char * token;
        char input[5][30];
        memset(input, 0, 150);  
        char s[2] = " ";

        token = strtok(buffer, s);

        while(token != NULL)
        {   
            strcpy(input[inputCounter], token);
            token = strtok(NULL, s);
            inputCounter++;
        }

        if(inputCounter < 4)
        {
        	fprintf(stderr, "Input Error!\n");
        }

        /*  Check cities inside files names for time check and
            check if inside them has the requested type  */
        if(inputCounter == 5)	
        {
        	struct dirent * dir2;
        	char temp[50] = " ";
        	sprintf(temp,"%s/%s",directoryPath, input[4]);
        	
        	directory = opendir(temp);

        	if(directory != NULL)
        	{
	        	while(1)
	        	{
	        		dir2 = readdir(directory);

	        		if(dir2 == NULL) break;

	        		if(dir2 -> d_name[0] == '.') continue;	 

                    if(checkTime(input[2], input[3], dir2 -> d_name))
                    {
                        char validPathTemp[300];
                        sprintf(validPathTemp, "%s/%s", temp, dir2 -> d_name);
                        result += checkPath(input[1], validPathTemp);
                    }
	        	}
	        }
	        else fprintf(stderr, "Servant: NULL!\n");
        }

        else if(inputCounter == 4)	/* Do the checks on the previous comment for all the cities */
        {
        	for(int i = 0; i < numberOfDirectories; i++)
        	{
                char directoryTemp[300] = " ";
                sprintf(directoryTemp,"%s/%s", directoryPath, get(list, i));
                struct dirent * dirTemp;
                directory = opendir(directoryTemp);

                if(directory != NULL)
                {
                    while(1)
                    {
                        dirTemp = readdir(directory);

                        if(dirTemp == NULL) break;
                            
                        if(dirTemp-> d_name[0] == '.') continue;
                            
                        if(checkTime(input[2], input[3], dirTemp-> d_name))
                        {
                            char validPathTemp[300];
                            sprintf(validPathTemp,"%s/%s", directoryTemp, dirTemp -> d_name);
                            result += checkPath(input[1], validPathTemp);
                        }
                    }
                }
            }		
        }	
      

        char returnString[20];
        memset(returnString, 0, sizeof(returnString));	/* Print value to the buffer and write it to the server socket */
		sprintf(returnString, "%d", result);
        memset(buffer, 0, sizeof(buffer));        
        numberOfTotalOps++;
        write(newSocket, returnString, strlen(returnString));
        close(newSocket);
    }

    shutdown(mySocket, SHUT_RDWR);	/*  Shutdown the socket   */
    pthread_exit(NULL);
}

////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
	if(argc != 9)
	{
        fprintf(stderr, "Servant: Wrong Number of Inputs!");
        exit(EXIT_FAILURE);
    }

    if(strcmp(argv[1],"-d") != 0 || strcmp(argv[3], "-c") != 0 || strcmp(argv[5], "-r") != 0 || strcmp(argv[7], "-p") != 0)
    {
        fprintf(stderr, "Servant: Wrong Inputs!");
        exit(EXIT_FAILURE);
    }

    /*  SIGINT handler  */
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &SIGINTHANDLER;
    sa.sa_flags = 0;
    if((sigemptyset(&sa.sa_mask) == -1) || (sigaction(SIGINT, &sa, NULL) == -1))
    {
        fprintf(stderr, "Failed to install signal sa_handler\n");
        exit(EXIT_FAILURE);
    }

    if((sigemptyset(&sa.sa_mask) == -1) || (sigaction(SIGINT, &sa, NULL) == -1))
    {
        fprintf(stderr, "Servant: Failed to install signal sa_handler");
        exit(EXIT_FAILURE);
    }

     /*  Find process id with /proc */
    int fp = open("/proc/self/stat", O_RDONLY);

    if(fp == -1)
    {
        fprintf(stderr, "Servant: Proc Open Error!\n");
        exit(EXIT_FAILURE);
    }

    char line[20];
    read(fp, line, sizeof(line));
    pid = strtok(line, " ");

    if(close(fp) == -1)
    {
        fprintf(stderr, "Servant: Proc Close Error!\n");
        exit(EXIT_FAILURE);
    }

    int lowerLimit, upperLimit, counter = 0;
    char * token;
	char temp1[50];
	char s[2] = "-";

	/*  Initialise arguments  */
	strcpy(directoryPath, argv[2]);
    strcpy(ip, argv[6]);
    port = atoi(argv[8]);

	strcpy(temp1, argv[4]);
	token = strtok(temp1, s);
	
	while(token != NULL)	/*  Initialise the lower and upper limits for the servants*/
	{	
		if(counter == 0) lowerLimit = atoi(token);
		else if(counter == 1) upperLimit = atoi(token);
		token = strtok(NULL, s);
		counter++;
	}

	/*  Make unique port with the lower limit + PORTCONSTANT since all intervals will be unique as mentioned at the Teams */
	uniquePort = PORTCONSTANT + lowerLimit;

	/*  For making sure that it isn't same as port input*/
	if(uniquePort == port) uniquePort += 2;

	if(lowerLimit >= upperLimit || lowerLimit < 1)
	{
        fprintf(stderr, "Servant: Wrong Gap!\n");
        freeLinkedList(&list);
        exit(EXIT_FAILURE);
    }

    /* Socket variables */
	struct sockaddr_in my_addr;
    memset(&my_addr, 0, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(port);
    
    if(inet_pton(AF_INET, ip, &my_addr.sin_addr) == -1)
    {
        fprintf(stderr, "Servant: inet Error!\n");
        exit(EXIT_FAILURE);
    }

    numberOfDirectories = 1 + upperLimit - lowerLimit;    /*  Scans all of the files and store the requested ones to a linked list   */    
 	struct dirent ** names;
 	int n = scandir(directoryPath, &names, fileNameSelect, alphasort);

 	if(n == -1)
    {
        fprintf(stderr, "Servant: Scandir Error!\n");
        freeLinkedList(&list);
        exit(EXIT_FAILURE);
    }
    else
    {
        if(upperLimit > n + 1)
        {
            fprintf(stderr, "Servant: Upper limit is greater than number of directories!\n");
            freeLinkedList(&list);
            for(int i = 0; i < n; i++) free(names[i]);             
            free(names);
            exit(EXIT_FAILURE);
        }
    }

 	for(int i = 0; i < numberOfDirectories; i++) add(&list, names[i + lowerLimit - 1]-> d_name);
  
    fprintf(stdout, "Servant %s: loaded dataset, cities %s-%s\n", pid, get(list,0), get(list, numberOfDirectories - 1));

	/*  Prints requested values from server to the buffer and write it to the server socket, put 's' to the end of buffer to making server understand it is from servant */
    char buffer[1024];
    sprintf(buffer,"%d,%s,%s,", uniquePort, ip, pid);

    for(int i = 0;i < numberOfDirectories - 1;++i)
    {
    	char cityTemp[20];
    	strcpy(cityTemp,get(list,i));
    	cityTemp[strlen(cityTemp) + 1] = '\0';
    	cityTemp[strlen(cityTemp)] = ',';
    	sprintf(buffer,"%s%s", buffer, cityTemp);
    }

    sprintf(buffer,"%s%ss", buffer, get(list, numberOfDirectories - 1));

    int mainSocket;

	while(1)	/*  Try connect until it happens */
	{
    	mainSocket = socket(AF_INET, SOCK_STREAM, 0);

    	if(mainSocket == -1)
    	{
        	fprintf(stderr, "Servant: Socket Error!\n");
        	exit(EXIT_FAILURE);
    	}

    	if(connect(mainSocket, (struct sockaddr*) &my_addr, sizeof(my_addr)) == -1)
    	{
        	fprintf(stderr, "Servant: Connect Error!\n");
        	close(mainSocket);
    	}
    	break;
    }

    write(mainSocket, buffer, strlen(buffer));	/*  Connection happened. Then, send buffer to the server  */

    close(mainSocket);	/*Close Socket */

    if(pthread_create(&servantThread, NULL, servantThreadFunction, NULL) != 0)
    {
       	fprintf(stderr, "Servant: Thread Error!\n");
       	exit(EXIT_FAILURE);
    }

    /* Waits for the thread to end */
    if(pthread_join(servantThread, NULL) != 0)
    {
        fprintf(stderr, "Servant: Join Error!\n");
        exit(EXIT_FAILURE);
    }
 
	fprintf(stdout, "Servant %s: termination message received, handled %d requests in total\n", pid, numberOfTotalOps);

	/* Frees allocated space */
    for(int i = 0;i < n;++i) free(names[i]);    
    free(names);
	freeLinkedList(&list);      
	return 0;

}