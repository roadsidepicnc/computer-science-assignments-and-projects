 
#include <memorymanagement.h>

using namespace myos;
using namespace myos::common;

void printf(char * str);
void printfHex(uint8_t key);
void printfHex2(uint16_t key);

////////////////////////////////////////////////////////////////

/* At the start sets page table to its default.
*/

void MemoryManager :: prepareDefaultPageTable(PageTableEntry * pageTable, int * memory, int size)
{
    for(int i = 0; i < size; i++)
    {
        (pageTable + i)->address = memory + i;  // i'th page table entry points to the i'th memory slot. 
        (pageTable + i)->index = -1;            // Default index for page table entry is -1
        (pageTable + i)->valid = false;         // Default valid bit is false
        (pageTable + i)->referenced = false;    // Default referenced bit is false
        (pageTable + i)->modified = false;      // Default modified bit is false
    }  
}

/* Determines the which index of the page table to be replaced. Returns the index as integer
   Determines the index by checking the lruArray. Details are explained at the report.
*/

int MemoryManager::lruHelper(PageTableEntry * pageTable, int * lruArray, int size)
{
    for(int i = 0; i < size; i++)
    {
        if(pageTable[i].valid == false)
        {
            return i;
        }
    }

    int res = 0;
    int min = lruArr[0];

    for(int i = 0; i < size; i++)
    {       
        if(pageTable[i].valid == true && lruArr[i] < min)
        {
            res = i;
            min = lruArr[i];
        }
    }

    return res;
}

/* Uses helper function and executes page replacement with the lru algorithm
*/

int MemoryManager :: lruMain(PageTableEntry * pageTable, int * disk, int * lruArray, int size, int index, int value)
{
    int requiredIndex;
    requiredIndex = lruHelper(pageTable, lruArray, size);

    if(pageTable[requiredIndex].modified)   // WRITE BACK TO DISK
    {
        int backupIndex = pageTable[requiredIndex].index;
        int backupValue = pageTable[requiredIndex].address[0];
        disk[backupIndex] = backupValue;
        numberOPagesfWrittenBackToDisk++;
    }

    lruArr[requiredIndex] = 0; 
    (pageTable + requiredIndex) -> index = index;
    (pageTable + requiredIndex) -> valid = true;
    (pageTable + requiredIndex) -> modified = false;
    *pageTable[requiredIndex].address = value;
    return requiredIndex;
}

/* Determines the which index of the page table to be replaced. Returns the index as integer
   Determines the index by checking the page secondChancePointer points. Details are provided at the report.
*/

int MemoryManager::secondChanceHelper(PageTableEntry * pageTable, int size)
{
    int res;

    while(true)
    {
        if(pageTable[secondChancePointer].valid == false || (pageTable[secondChancePointer].valid == true && pageTable[secondChancePointer].referenced == false))
        {
            res = secondChancePointer;
            break;
        }
        else if(pageTable[secondChancePointer].valid == true && pageTable[secondChancePointer].referenced == true)
        {
            pageTable[secondChancePointer].referenced = false;
            secondChancePointer++;
            secondChancePointer %= size;
        }
    }

    return (res) % size;
}

/* Uses helper function and executes page replacement with the second chance algorithm
*/

void MemoryManager :: secondChanceMain(PageTableEntry * pageTable, int * disk, int size, int index, int value)
{
    int requiredIndex;
    requiredIndex = secondChanceHelper(pageTable, size);

    if(pageTable[requiredIndex].modified)   // WRITE BACK TO DISK
    {
        int backupIndex = pageTable[requiredIndex].index;
        int backupValue = pageTable[requiredIndex].address[0];
        disk[backupIndex] = backupValue;
        numberOPagesfWrittenBackToDisk++;
    }

    (pageTable + requiredIndex) -> index = index;
    (pageTable + requiredIndex) -> valid = true;
    (pageTable + requiredIndex) -> modified = false;
    (pageTable + requiredIndex) -> referenced = true;
    *pageTable[requiredIndex].address = value;

}

/* Determines the which index of the page table to be replaced. Returns the index as integer
   Determines the index by checking the fifoCounter. Details are provided at the report.
*/

int MemoryManager::fifoHelper(PageTableEntry * pageTable, int size)
{   
    for(int i = 0; i < size; i++)
    {
        if(pageTable[i].valid == false)
        {
            return i;
        }
    }

    int res;
    
    fifoCounter++;

    res = fifoCounter;

    fifoCounter %= size;
    return (res - 1) % size;
}

/* Uses helper function and executes page replacement with the FIFO algorithm.
*/

void MemoryManager::fifoMain(PageTableEntry * pageTable, int * disk, int size, int index, int value)
{   
    int requiredIndex;

    requiredIndex = fifoHelper(pageTable, size);

    if(pageTable[requiredIndex].modified)   // WRITE BACK TO DISK
    {
        int backupIndex = pageTable[requiredIndex].index;
        int backupValue = pageTable[requiredIndex].address[0];
        disk[backupIndex] = backupValue;
        numberOPagesfWrittenBackToDisk++;
    }

    (pageTable + requiredIndex) -> index = index;
    (pageTable + requiredIndex) -> valid = true;
    (pageTable + requiredIndex) -> modified = false;
    *pageTable[requiredIndex].address = value;
}

/* Searches the page table and returns the ith page with the same index value. Returns -1 if not found.
*/

int MemoryManager::searchMemoryWithoutValue(PageTableEntry * pageTable, int size, int index)
{
    for(int i = 0; i < size; i++)
    {
        if(pageTable[i].valid == true && pageTable[i].index == index)
        {
            return i;
        }
    }

    return -1;
}

int MemoryManager::searchMemoryWithValue(PageTableEntry * pageTable, int size, int index, int value)
{
    for(int i = 0; i < size; i++)
    {
        if(pageTable[i].valid == true && pageTable[i].index == index && pageTable[i].address[0] == value)
        {
            return i;
        }
    }

    return -1;
}

/* Main method for the getValue.
*/

int MemoryManager :: mainGet(PageTableEntry * pageTable, int * disk, int * lruArray, int size, int index)
{   
    int temp = searchMemoryWithoutValue(pageTable, size, index);

    if(temp == -1)
    {
        int val = disk[index];

        if(AlgorithmChoice == 0) fifoMain(pageTable, disk, size, index, val); // MODIFY PAGE TABLE    
        else if(AlgorithmChoice == 1) secondChanceMain(pageTable, disk, size, index, val); // MODIFY PAGE TABLE    
        else if(AlgorithmChoice == 2) lruMain(pageTable, disk, lruArray, size, index, val); // MODIFY PAGE TABLE 

        numberOfMisses++;
        numberOfPagesLoaded++;


        return val;
    }
    else
    {
        numberOfHits++;

        (lruArr[temp])++; 
        return pageTable[temp].address[0];
    }
}

/* Main method for the setValue.
*/

void MemoryManager :: mainSet(PageTableEntry * pageTable, int * disk, int * lruArray, int size, int index, int value)
{
    int temp = searchMemoryWithoutValue(pageTable, size, index);

    if(temp == -1)
    {
        int ff;

        if(AlgorithmChoice == 0) fifoMain(pageTable, disk, size, index, value); // MODIFY PAGE TABLE    
        else if(AlgorithmChoice == 1) secondChanceMain(pageTable, disk, size, index, value); // MODIFY PAGE TABLE    
        else if(AlgorithmChoice == 2) ff = lruMain(pageTable, disk, lruArray, size, index, value); // MODIFY PAGE TABLE 

        numberOfMisses++;
        numberOfPagesLoaded++;

        pageTable[ff].modified = true; 
    }
    else
    {
        numberOfHits++;

        (lruArr[temp])++; 
        pageTable[temp].modified = true;
        *pageTable[temp].address = value;
    }
}

/* Gets the i'th element of the disk. Details are provided at the report.
*/

int MemoryManager :: getValue(PageTableEntry * pageTable, int * disk, int * lruArray, int size, int index)
{
    int res;
    totalOps++;
    res = mainGet(pageTable, disk, lruArray, size, index);
    return res;
}

/* Sets the i'th element of the disk with the input value. Details are provided at the report.
*/

void MemoryManager :: setValue(PageTableEntry * pageTable, int * disk, int * lruArray, int size, int index, int value)
{
    totalOps++;
    mainSet(pageTable, disk, lruArray, size, index, value);
}

////////////////////////////////////////////////////////////////

void MemoryManager :: bubbleSort(PageTableEntry * pageTable, int * disk, int * lruArray, int diskSize, int memSize)
{
    for(int i = 0; i < diskSize - 1; i++)
    {
        for(int j = 0; j < diskSize - 1 - i; j++)
        {
            int x1 = getValue(pageTable, disk, lruArray, memSize, j);
            int x2 = getValue(pageTable, disk, lruArray, memSize, j + 1);

            if(x1 > x2)
            {
                setValue(pageTable, disk, lruArray, memSize, j, x2);
                setValue(pageTable, disk, lruArray, memSize, j + 1, x1);
            }
        }
    }    
}

void MemoryManager :: insertionSort(PageTableEntry * pageTable, int * disk, int * lruArray, int diskSize, int memSize)
{
    int i, key, j;
    for (i = 1; i < diskSize; i++)
    {
        key = getValue(pageTable, disk, lruArray, memSize, i);
        j = i - 1;

        int tt = getValue(pageTable, disk, lruArray, memSize, j);

        while (j >= 0 && tt > key)
        {
            int temp2 = getValue(pageTable, disk, lruArray, memSize, j);
            setValue(pageTable, disk, lruArray, memSize, j + 1, temp2);
            j = j - 1;
            tt = getValue(pageTable, disk, lruArray, memSize, j);
        }

        setValue(pageTable, disk, lruArray, memSize, j + 1, key);
    }
}

void MemoryManager :: quickSort(PageTableEntry * pageTable, int * disk, int * lruArray, int diskSize, int memSize, int low, int high)
{
    if (low < high)
    {
        int pi = partition(pageTable, disk, lruArray, diskSize, memSize, low, high);
        quickSort(pageTable, disk, lruArray, diskSize, memSize, low, pi - 1);
        quickSort(pageTable, disk, lruArray, diskSize, memSize, pi + 1, high);
    }
}

int MemoryManager :: partition (PageTableEntry * pageTable, int * disk, int * lruArray, int diskSize, int memSize, int low, int high)
{

    int pivot = getValue(pageTable, disk, lruArray, memSize, high);
    int i = (low - 1);
    int x1, x2;

    for (int j = low; j <= high - 1; j++)
    {
        int temp1 = getValue(pageTable, disk, lruArray, memSize, j);

        if (temp1 < pivot)
        {
            i++;
            x1 = getValue(pageTable, disk, lruArray, memSize, i);
            x2 = getValue(pageTable, disk, lruArray, memSize, j);
            setValue(pageTable, disk, lruArray, memSize, i, x2);
            setValue(pageTable, disk, lruArray, memSize, j, x1);
        }
    }

    x1 = getValue(pageTable, disk, lruArray, memSize, i + 1);
    x2 = getValue(pageTable, disk, lruArray, memSize, high);
    setValue(pageTable, disk, lruArray, memSize, i + 1, x2);
    setValue(pageTable, disk, lruArray, memSize, high, x1);
    return (i + 1);
}

void MemoryManager :: resetBits()
{
    totalOps = 0;
    numberOfHits = 0;
    numberOfMisses = 0;
    numberOfPagesLoaded = 0;
    numberOPagesfWrittenBackToDisk = 0;
}

////////////////////////////////////////////////////////////////

MemoryManager* MemoryManager::activeMemoryManager = 0;
        
MemoryManager::MemoryManager(size_t start, size_t size)
{
    activeMemoryManager = this;
    
    if(size < sizeof(MemoryChunk))
    {
        first = 0;
    }
    else
    {
        first = (MemoryChunk*)start;
        
        first -> allocated = false;
        first -> prev = 0;
        first -> next = 0;
        first -> size = size - sizeof(MemoryChunk);
    }
}

MemoryManager::~MemoryManager()
{
    if(activeMemoryManager == this)
        activeMemoryManager = 0;
}
        
void* MemoryManager::malloc(size_t size)
{
    MemoryChunk *result = 0;
    
    for(MemoryChunk* chunk = first; chunk != 0 && result == 0; chunk = chunk->next)
        if(chunk->size > size && !chunk->allocated)
            result = chunk;
        
    if(result == 0)
        return 0;
    
    if(result->size >= size + sizeof(MemoryChunk) + 1)
    {
        MemoryChunk* temp = (MemoryChunk*)((size_t)result + sizeof(MemoryChunk) + size);
        
        temp->allocated = false;
        temp->size = result->size - size - sizeof(MemoryChunk);
        temp->prev = result;
        temp->next = result->next;
        if(temp->next != 0)
            temp->next->prev = temp;
        
        result->size = size;
        result->next = temp;
    }
    
    result->allocated = true;
    return (void*)(((size_t)result) + sizeof(MemoryChunk));
}

void MemoryManager::free(void* ptr)
{
    MemoryChunk* chunk = (MemoryChunk*)((size_t)ptr - sizeof(MemoryChunk));
    
    chunk -> allocated = false;
    
    if(chunk->prev != 0 && !chunk->prev->allocated)
    {
        chunk->prev->next = chunk->next;
        chunk->prev->size += chunk->size + sizeof(MemoryChunk);
        if(chunk->next != 0)
            chunk->next->prev = chunk->prev;
        
        chunk = chunk->prev;
    }
    
    if(chunk->next != 0 && !chunk->next->allocated)
    {
        chunk->size += chunk->next->size + sizeof(MemoryChunk);
        chunk->next = chunk->next->next;
        if(chunk->next != 0)
            chunk->next->prev = chunk;
    }
    
}

void* operator new(unsigned size)
{
    if(myos::MemoryManager::activeMemoryManager == 0)
        return 0;
    return myos::MemoryManager::activeMemoryManager->malloc(size);
}

void* operator new[](unsigned size)
{
    if(myos::MemoryManager::activeMemoryManager == 0)
        return 0;
    return myos::MemoryManager::activeMemoryManager->malloc(size);
}

void* operator new(unsigned size, void* ptr)
{
    return ptr;
}

void* operator new[](unsigned size, void* ptr)
{
    return ptr;
}

void operator delete(void* ptr)
{
    if(myos::MemoryManager::activeMemoryManager != 0)
        myos::MemoryManager::activeMemoryManager->free(ptr);
}

void operator delete[](void* ptr)
{
    if(myos::MemoryManager::activeMemoryManager != 0)
        myos::MemoryManager::activeMemoryManager->free(ptr);
}