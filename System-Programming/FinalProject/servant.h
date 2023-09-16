#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>

typedef struct node
{
    char data[30];
    struct node * next;
}node;

typedef struct LinkedList
{
    node * head;
}LinkedList;

////////////////////////////////////////////////////

// Linked List Functions

node * createnode(char * data)
{
    node * newNode = malloc(sizeof(node));

    if (!newNode) return NULL;
    
    strcpy(newNode -> data, data);
    newNode -> next = NULL;
    return newNode;
}

void add(LinkedList * list, char * data)
{
    node * current = NULL;

    if(list->head == NULL)
    {
        list->head = createnode(data);
    }
    else 
    {
        current = list->head; 

        while(current -> next != NULL)
        {
            current = current->next;
        }

        current -> next = createnode(data);
    }
}

char * get(LinkedList list, int index)
{
    node * current = list.head;

    for (int i = 0; i < index; i++)
    {
        current = current -> next;    
    }

    return current -> data;
}

void freeLinkedList(LinkedList * list)
{
    node * current = list -> head;
    node * temp;

    while(current != NULL)
    {
        temp = current;
        current = current -> next;
        free(temp);
    }
}

// HELPER FUNCTIONS

/* Uses scandir to ignore system files starting with '.' */
int fileNameSelect(const struct dirent * dir)
{
    char * x = strstr(dir -> d_name, ".");
    if(x) return 0;       
    else return 1;       
}

/* input and checks if fileDate is between directory1 and directory2 */
int checkTime(char* directory1, char* directory2, char * fileDate){
    char temp[3][30];
    strcpy(temp[0], directory1);
    strcpy(temp[1], directory2);
    strcpy(temp[2], fileDate);
    int arr[9];
    char* token;

    token = strtok(temp[0],"-");
    arr[0] = atoi(token);

    token = strtok(NULL, "-");
    arr[1] = atoi(token);

    token = strtok(NULL, "\0");
    arr[2] = atoi(token);

    token = strtok(temp[1], "-");
    arr[3] = atoi(token);

    token = strtok(NULL, "-");
    arr[4] = atoi(token);

    token = strtok(NULL, "\0");
    arr[5] = atoi(token);

    token = strtok(temp[2], "-");
    arr[6] = atoi(token);

    token = strtok(NULL, "-");
    arr[7] = atoi(token);

    token = strtok(NULL, "\0");
    arr[8] = atoi(token);

    if(arr[8] < arr[2] || arr[8] > arr[5]) return 0;      
    if(arr[8] == arr[2])
    {
        if(arr[7] < arr[1]) return 0;          
        if(arr[7] == arr[1] && arr[6] < arr[0]) return 0;            
    }
    if(arr[8] == arr[5])
    {
        if(arr[7] > arr[4]) return 0;          
        if(arr[7] == arr[3] && arr[6] > arr[3]) return 0;           
    }

    return 1;
}

/* Checks if the directory file contains any lines that contains type t */
int checkPath(char * t, char * directory)
{
    unsigned char buffer[1];
    char line[200];
    int counter = 0;
    int res = 0;
    char s[2] = " ";
    size_t size;

    int fd = open(directory, O_RDONLY);
    if(fd == -1)
    {
        fprintf(stderr, "Input file couldn't be opened!\n");
        exit(EXIT_FAILURE);
    }

    while(1)
    {
        size = read(fd, buffer, sizeof(buffer));

        if(size != sizeof(buffer)) break;
            
        if(buffer[0] == '\n')
        {
            if(strlen(line) != 0)
            {
                char* token;
                token = strtok(line, s);
                token = strtok(NULL, s);
                if(strcmp(token, t) == 0) res++;                  
            }

            memset(line,0,sizeof(line));
            counter = 0;
            continue;
        }

        line[counter] = buffer[0];
        counter++;
    }

    if(strlen(line) != 0)
    {
        char* token;
        token = strtok(line, s);
        token = strtok(NULL, s);
        if(strcmp(token, t) == 0) res++;         
    }

    if(close(fd) == -1)
    {
        fprintf(stderr, "Input file couldn't be closed!\n");
        exit(EXIT_FAILURE);
    }

    return res;
}