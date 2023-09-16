 
#ifndef __MYOS__MEMORYMANAGEMENT_H
#define __MYOS__MEMORYMANAGEMENT_H

#include <common/types.h>


namespace myos
{
    struct PageTableEntry
    {
        public:
            bool modified;
            bool referenced;
            bool valid;
            int index;
            int * address;
    };

    struct MemoryChunk
    {
        MemoryChunk *next;
        MemoryChunk *prev;
        bool allocated;
        common::size_t size;
    };
    
    
    class MemoryManager
    {
        
    protected:
        MemoryChunk* first;
    public:
        
        int numberOfHits = 0;
        int numberOfMisses = 0;
        int numberOfPagesLoaded = 0;
        int numberOPagesfWrittenBackToDisk = 0;
        int totalOps = 0;

        int AlgorithmChoice;
        int lruArr[1000];
        int fifoCounter = 0;
        int secondChancePointer = 0;
        static MemoryManager *activeMemoryManager;
        
        MemoryManager(common::size_t first, common::size_t size);
        ~MemoryManager();
        
        void* malloc(common::size_t size);
        void free(void* ptr);
        void prepareDefaultPageTable(PageTableEntry * pageTable, int * memory, int size);

        int fifoHelper(PageTableEntry * pageTable, int size);
        void fifoMain(PageTableEntry * pageTable, int * disk, int size, int index, int value);

        int secondChanceHelper(PageTableEntry * pageTable, int size);
        void secondChanceMain(PageTableEntry * pageTable, int * disk, int size, int index, int value);

        int lruHelper(PageTableEntry * pageTable, int * lruArray, int size);
        int lruMain(PageTableEntry * pageTable, int * disk, int * lruArray, int size, int index, int value);

        int searchMemoryWithoutValue(PageTableEntry * pageTable, int size, int index);
        int searchMemoryWithValue(PageTableEntry * pageTable, int size, int index, int value);

        int mainGet(PageTableEntry * pageTable, int * disk, int * lruArray, int size, int index);
        void mainSet(PageTableEntry * pageTable, int * disk, int * lruArray, int size, int index, int value);

        int getValue(PageTableEntry * pageTable, int * disk, int * lruArray, int size, int index);
        void setValue(PageTableEntry * pageTable, int * disk, int * lruArray, int size, int index, int value);

        void bubbleSort(PageTableEntry * pageTable, int * disk, int * lruArray, int diskSize, int memSize);
        void insertionSort(PageTableEntry * pageTable, int * disk, int * lruArray, int diskSize, int memSize);
        void quickSort(PageTableEntry * pageTable, int * disk, int * lruArray, int diskSize, int memSize, int low, int high);
        int partition (PageTableEntry * pageTable, int * disk, int * lruArray, int diskSize, int memSize, int low, int high);
        void resetBits();
    };
}


void* operator new(unsigned size);
void* operator new[](unsigned size);

// placement new
void* operator new(unsigned size, void* ptr);
void* operator new[](unsigned size, void* ptr);

void operator delete(void* ptr);
void operator delete[](void* ptr);


#endif