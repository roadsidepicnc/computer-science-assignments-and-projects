typedef struct node
{
    int item;
    int index;
    struct node* next;
}node;

typedef struct Queue
{
    node* front;
    node* rear;
}Queue;

typedef struct ServantData
{
    int uniquePort;
    char ** cities;
    int numberOfCities;
    char ip[30];
    int pid;
}ServantData;

////////////////////////////////////////////////////

void getTime(char* c_time_string)
{
    time_t current_time;
    current_time = time(NULL);
    if(current_time == ((time_t)-1)){
        fprintf(stderr, "Failure to obtain the current time\n");
        exit(EXIT_FAILURE);
    }

    strcpy(c_time_string,ctime(&current_time));
    if(c_time_string == NULL){
        fprintf(stderr, "Failure to convert the current time\n");
        exit(EXIT_FAILURE); 
    }
    c_time_string[strlen(c_time_string) - 1] = '\0';
}

int empty(Queue *q)
{
    if(q-> front == NULL) return 0;     
    else return 1;       
}

void enqueue(Queue *q, int item)
{
    node* tempNode = (node *) malloc(sizeof(node));
    tempNode-> next = NULL;
    tempNode-> item = item;

    if(empty(q) != 0) q->rear->next = tempNode;     
    else q->front = tempNode;  
    q -> rear = tempNode;
}

int dequeue(Queue * q)
{
    int client;
    node * tempNode;

    if(empty(q) != 0)
    {
        client = q -> front -> item;
        tempNode = q -> front;
        q -> front = q -> front -> next;        
        free(tempNode);       
        if(q->front==NULL) q->rear=NULL;          
    }

    return client;
}

void freeQueue(Queue *q)
{
    while(empty(q) == 1) dequeue(q);      
}