#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <math.h>
#define MAX_TREE_HT 100 

  
struct MinHeapNode { 
  
    char data; 
  
    unsigned freq; 
  
    struct MinHeapNode *left, *right; 
}; 
  
struct MinHeap { 
  
    unsigned size; 
  
    unsigned capacity; 
  
    struct MinHeapNode** array; 
}; 
  
struct MinHeapNode* newNode(char data, unsigned freq); 
struct MinHeap* createMinHeap(unsigned capacity);
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b);
void minHeapify(struct MinHeap* minHeap, int idx);
int isSizeOne(struct MinHeap* minHeap);
struct MinHeapNode* extractMin(struct MinHeap* minHeap);
void insertMinHeap(struct MinHeap* minHeap, struct MinHeapNode* minHeapNode);
void buildMinHeap(struct MinHeap* minHeap);
void printArr(int arr[], int n);
int isLeaf(struct MinHeapNode* root);
struct MinHeap* createAndBuildMinHeap(char data[], int freq[], int size);
struct MinHeapNode* buildHuffmanTree(char data[], int freq[], int size);
void printCodes(struct MinHeapNode* root, int arr[], int top, char c);
void HuffmanCodes(char data[], int freq[], int size, char filename_message[], char filename_encoded[]);
void read_file (char filename[], char * string, int * array, int * counter);
void decoder (struct MinHeapNode* root, int arr[], int top, int n[], int counter);


int main() 
{  
    int i, num = 1;
    char filename_huffman_tree[20] = "reference.txt";
    char filename_message[20] = "message.txt";
    char filename_encoded[20] = "encoded.dat";
    char *words;
    int *frequency;
    words = (char *) malloc (sizeof (char));
    frequency = (int *) malloc (sizeof (int));

    read_file (filename_huffman_tree, words, frequency, &num);
    HuffmanCodes (words, frequency, num, filename_message, filename_encoded);
    

    return 0;
}

struct MinHeapNode* newNode(char data, unsigned freq) 
{ 
    struct MinHeapNode* temp = (struct MinHeapNode*) malloc(sizeof(struct MinHeapNode)); 
  
    temp->left = temp->right = NULL; 
    temp->data = data; 
    temp->freq = freq; 
  
    return temp; 
} 
  
struct MinHeap* createMinHeap(unsigned capacity) 
{ 
  
    struct MinHeap* minHeap = (struct MinHeap *) malloc(sizeof(struct MinHeap)); 
  
    minHeap->size = 0; 
  
    minHeap->capacity = capacity; 
  
    minHeap->array 
        = (struct MinHeapNode**)malloc(minHeap-> 
capacity * sizeof(struct MinHeapNode*)); 
    return minHeap; 
} 
  
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b) 
  
{ 
  
    struct MinHeapNode* t = *a; 
    *a = *b; 
    *b = t; 
} 
  
void minHeapify(struct MinHeap* minHeap, int idx) 
  
{ 
  
    int smallest = idx; 
    int left = 2 * idx + 1; 
    int right = 2 * idx + 2; 
  
    if (left < minHeap->size && minHeap->array[left]-> 
freq < minHeap->array[smallest]->freq) 
        smallest = left; 
  
    if (right < minHeap->size && minHeap->array[right]-> 
freq < minHeap->array[smallest]->freq) 
        smallest = right; 
  
    if (smallest != idx) { 
        swapMinHeapNode(&minHeap->array[smallest], 
                        &minHeap->array[idx]); 
        minHeapify(minHeap, smallest); 
    } 
} 
  
int isSizeOne(struct MinHeap* minHeap) 
{ 
  
    return (minHeap->size == 1); 
} 
  
struct MinHeapNode* extractMin(struct MinHeap* minHeap) 
  
{ 
  
    struct MinHeapNode* temp = minHeap->array[0]; 
    minHeap->array[0] 
        = minHeap->array[minHeap->size - 1]; 
  
    --minHeap->size; 
    minHeapify(minHeap, 0); 
  
    return temp; 
} 

void insertMinHeap(struct MinHeap* minHeap, struct MinHeapNode* minHeapNode) 
  
{ 
  
    ++minHeap->size; 
    int i = minHeap->size - 1; 
  
    while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq) { 
  
        minHeap->array[i] = minHeap->array[(i - 1) / 2]; 
        i = (i - 1) / 2; 
    } 
  
    minHeap->array[i] = minHeapNode; 
} 
  
void buildMinHeap(struct MinHeap* minHeap) 
  
{ 
  
    int n = minHeap->size - 1; 
    int i; 
  
    for (i = (n - 1) / 2; i >= 0; --i) 
        minHeapify(minHeap, i); 
} 
  
void printArr(int arr[], int n) 
{ 
    FILE * ptr2 = fopen ("encoded.dat", "a");
    int i; 
        for (i = 0; i < n; ++i)     fprintf(ptr2, "%d",arr[i] );
    fprintf(ptr2, " ");
    fclose (ptr2);
} 

int isLeaf(struct MinHeapNode* root) 
  
{ 
  
    return !(root->left) && !(root->right); 
} 

struct MinHeap* createAndBuildMinHeap(char data[], int freq[], int size) 
  
{ 
  
    struct MinHeap* minHeap = createMinHeap(size); 
  
    for (int i = 0; i < size; ++i) 
        minHeap->array[i] = newNode(data[i], freq[i]); 
  
    minHeap->size = size; 
    buildMinHeap(minHeap); 
  
    return minHeap; 
} 
  
struct MinHeapNode* buildHuffmanTree(char data[], int freq[], int size) 
  
{ 
    struct MinHeapNode *left, *right, *top; 
  
    struct MinHeap* minHeap = createAndBuildMinHeap(data, freq, size); 
  
    while (!isSizeOne(minHeap)) { 
  
        left = extractMin(minHeap); 
        right = extractMin(minHeap); 
        top = newNode('$', left->freq + right->freq); 
        top->left = left; 
        top->right = right; 
  
        insertMinHeap(minHeap, top); 
    } 

    return extractMin(minHeap); 
} 
  

void printCodes(struct MinHeapNode* root, int arr[], int top, char c)
{ 
  
    if (root->left)
    { 
        arr[top] = 0; 
        printCodes(root->left, arr, top + 1, c); 
    } 
  
        if (root->right) 
        {
            arr[top] = 1;
            printCodes(root->right, arr, top + 1, c);
        }
  
            if (isLeaf(root) && root -> data == c)
            {
                printf("%c: ", root->data);
                for (int i = 0; i < top; i++)  printf("%d",arr[i] ); 
                    printf("\n");
                printArr(arr, top);     
            }
}

void HuffmanCodes(char data[], int freq[], int size, char filename_message[], char filename_encoded[])  
{ 
    int num[100];
    int n = 0;
    char c;
    FILE * ptr1 = fopen (("%s", filename_message), "r");
    FILE * ptr2 = fopen (("%s", filename_encoded), "r");
    struct MinHeapNode* root = buildHuffmanTree(data, freq, size); 
    int arr[MAX_TREE_HT], top = 0; 
    

        while (c != EOF)
        {
            c = fgetc (ptr1);
            printCodes(root, arr, top, c);
        }
    fclose(ptr1);
    
    

            while (c != EOF)
            {

                if (c != ' ')
                {
                    c = fgetc (ptr2);
                    num[n] = c - 48;
                    n++;
                }

                    else if (c == ' ')
                    {
                        decoder (root, arr, top, num, n);
                        printf("pipip\n");
                        n = 0;
                    }
            }

    fclose(ptr2); 
           
} 

void read_file (char filename[], char *string, int * array, int * counter)
{
    int i, is_same, num, length;
    FILE * ptr = fopen (("%s",filename), "r");
    char c = fgetc (ptr);
    string[0] = c;
    array[0] = 1;

        while (c != EOF)
        {
            c = fgetc (ptr);
            is_same = 0;

                for (i = 0; i < *counter; i++)
                {
                   if (string[i] == c)
                   {
                       is_same  = 1;
                       num = i;
                       break;
                   }
                }

                    if (is_same == 1)
                    {
                        array[num] += 1;
                    }    
                        else if (is_same == 0)
                        {
                            string = (char *) realloc (string, (*counter + 1) * sizeof (char));
                            array = (int *) realloc (array, (*counter + 1) * sizeof (int));
                            string[*counter] = c;
                            array[*counter] = 1;
                            (*counter)++;
                        }         
        }

*counter -= 1;
string[*counter] = '\0';
array[*counter] = '\0';
fclose (ptr);
}


void decoder (struct MinHeapNode* root, int arr[], int top, int n[], int counter)
{ 


    if (root->left)
    { 
        arr[top] = 0; 
        decoder(root->left, arr, top + 1, n, counter); 
    } 
  
        if (root->right) 
        {
            arr[top] = 1;
            decoder(root->right, arr, top + 1, n, counter);
        }
  
            if (isLeaf(root))
            {
                FILE * ptr = fopen ("decoded.txt", "a");
                int x;
                int is_same = 1;
                printf("%d\n", counter);

                        for (x = 0; x < counter; x++)
                        {
                            if (arr[x] != n[x])
                            {
                                is_same = 0;
                                break;
                            }
                        }

                if (is_same)
                {

                    fprintf(ptr, "%c", root->data);
                }    

                fclose(ptr);
            }
    
}
 