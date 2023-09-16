
#include <common/types.h>
#include <gdt.h>
#include <memorymanagement.h>
#include <hardwarecommunication/interrupts.h>
#include <hardwarecommunication/pci.h>
#include <drivers/driver.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>
#include <drivers/vga.h>
#include <gui/desktop.h>
#include <gui/window.h>
#include <multitasking.h>

// #define GRAPHICSMODE

using namespace myos;
using namespace myos::common;
using namespace myos::drivers;
using namespace myos::hardwarecommunication;
using namespace myos::gui;

void printf(char* str)
{
    static uint16_t* VideoMemory = (uint16_t*)0xb8000;

    static uint8_t x=0,y=0;

    for(int i = 0; str[i] != '\0'; ++i)
    {
        switch(str[i])
        {
            case '\n':
                x = 0;
                y++;
                break;
            default:
                VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | str[i];
                x++;
                break;
        }

        if(x >= 80)
        {
            x = 0;
            y++;
        }

        if(y >= 25)
        {
            for(y = 0; y < 25; y++)
                for(x = 0; x < 80; x++)
                    VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | ' ';
            x = 0;
            y = 0;
        }
    }
}

void printfHex(uint8_t key)
{
    char* foo = "00";
    char* hex = "0123456789ABCDEF";
    foo[0] = hex[(key >> 4) & 0xF];
    foo[1] = hex[key & 0xF];
    printf(foo);
}

void printfHex2(uint16_t key)
{
    char* foo = "000";
    char* hex = "0123456789ABCDEF";
    foo[0] = hex[(key >> 8) & 0xF];
    foo[1] = hex[(key >> 4) & 0xF];
    foo[2] = hex[key & 0xF];
    printf(foo);
}

int SortChoice = -1;
int AlgorithmChoice = -1;
int Sorted = -1;


class PrintfKeyboardEventHandler : public KeyboardEventHandler
{
    public:

        void OnKeyDown(char c)
        {
            if(SortChoice == -1){
                if(c - 48 < 4 && c - 48 > 0) SortChoice = c - 48;                   
            }
            else if(AlgorithmChoice == -1)
            {
                if(c - 48 < 4 && c - 48 > 0) AlgorithmChoice = c - 48;                   
            }
            else if(Sorted == -1)
            {
                if(c - 48 < 4 && c - 48 > 0) Sorted = c - 48;                    
            }
        }
};

class MouseToConsole : public MouseEventHandler
{
    int8_t x, y;
public:
    
    MouseToConsole()
    {
        uint16_t* VideoMemory = (uint16_t*)0xb8000;
        x = 40;
        y = 12;
        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                            | (VideoMemory[80*y+x] & 0xF000) >> 4
                            | (VideoMemory[80*y+x] & 0x00FF);        
    }
    
    virtual void OnMouseMove(int xoffset, int yoffset)
    {
        static uint16_t* VideoMemory = (uint16_t*)0xb8000;
        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                            | (VideoMemory[80*y+x] & 0xF000) >> 4
                            | (VideoMemory[80*y+x] & 0x00FF);

        x += xoffset;
        if(x >= 80) x = 79;
        if(x < 0) x = 0;
        y += yoffset;
        if(y >= 25) y = 24;
        if(y < 0) y = 0;

        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                            | (VideoMemory[80*y+x] & 0xF000) >> 4
                            | (VideoMemory[80*y+x] & 0x00FF);
    }
    
};

void taskA()
{
    while(true)
        printf("A");
}
void taskB()
{
    while(true)
        printf("B");
}

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors()
{
    for(constructor* i = &start_ctors; i != &end_ctors; i++)
        (*i)();
}

void resetArrays(MemoryManager memoryManager1, int * arrDisk1, int * arrDisk2, int * arrDisk3, int * arrMem, int *lruArray, int size)
{
    memoryManager1.resetBits();

    for(int i = 0; i < size; i++) lruArray[i] = 0;

    arrDisk1[0] = 0;
    arrDisk1[1] = 0;
    arrDisk1[2] = 0;
    arrDisk1[3] = 0;
    arrDisk1[4] = 0;    

    arrDisk1[0] = 2;
    arrDisk1[1] = 3;
    arrDisk1[2] = 31;
    arrDisk1[3] = 1;
    arrDisk1[4] = 23;
    arrDisk1[5] = 7;
    arrDisk1[6] = 23;
    arrDisk1[7] = 89;
    arrDisk1[8] = 69;
    arrDisk1[9] = 4;
    arrDisk1[10] = 5;
    arrDisk1[11] = 8;
    arrDisk1[12] = 6;
    arrDisk1[13] = 11;
    arrDisk1[14] = 22;
    arrDisk1[15] = 66;
    arrDisk1[16] = 77;
    arrDisk1[17] = 34;
    arrDisk1[18] = 87;
    arrDisk1[19] = 10;

    arrDisk2[0] = 1;
    arrDisk2[1] = 2;
    arrDisk2[2] = 3;
    arrDisk2[3] = 4;
    arrDisk2[4] = 5;
    arrDisk2[5] = 6;
    arrDisk2[6] = 7;
    arrDisk2[7] = 8;
    arrDisk2[8] = 9;
    arrDisk2[9] = 10;
    arrDisk2[10] = 11;
    arrDisk2[11] = 12;
    arrDisk2[12] = 13;
    arrDisk2[13] = 14;
    arrDisk2[14] = 15;
    arrDisk2[15] = 16;
    arrDisk2[16] = 17;
    arrDisk2[17] = 18;
    arrDisk2[18] = 19;
    arrDisk2[19] = 20;

    arrDisk3[0] = 33;
    arrDisk3[1] = 4;
    arrDisk3[2] = 33;
    arrDisk3[3] = 5;
    arrDisk3[4] = 1;
    arrDisk3[5] = 8;
    arrDisk3[6] = 33;
    arrDisk3[7] = 66;
    arrDisk3[8] = 69;
    arrDisk3[9] = 41;
    arrDisk3[10] = 3;
    arrDisk3[11] = 75;
    arrDisk3[12] = 54;
    arrDisk3[13] = 11;
    arrDisk3[14] = 21;
    arrDisk3[15] = 23;
    arrDisk3[16] = 74;
    arrDisk3[17] = 7;
    arrDisk3[18] = 19;
    arrDisk3[19] = 64;
}

extern "C" void kernelMain(const void* multiboot_structure, uint32_t /*multiboot_magic*/)
{
    //printf("Hello World! --- http://www.AlgorithMan.de\n");

    GlobalDescriptorTable gdt;
           
    TaskManager taskManager;
    /*
    Task task1(&gdt, taskA);
    Task task2(&gdt, taskB);
    taskManager.AddTask(&task1);
    taskManager.AddTask(&task2);
    */
    
    InterruptManager interrupts(0x20, &gdt, &taskManager);
    
    //printf("Initializing Hardware, Stage 1\n");
    
    #ifdef GRAPHICSMODE
        Desktop desktop(320,200, 0x00,0x00,0xA8);
    #endif
    
    DriverManager drvManager;
    
        #ifdef GRAPHICSMODE
            KeyboardDriver keyboard(&interrupts, &desktop);
        #else
            PrintfKeyboardEventHandler kbhandler;
            KeyboardDriver keyboard(&interrupts, &kbhandler);
        #endif
        drvManager.AddDriver(&keyboard);
        
    
        #ifdef GRAPHICSMODE
            MouseDriver mouse(&interrupts, &desktop);
        #else
            MouseToConsole mousehandler;
            MouseDriver mouse(&interrupts, &mousehandler);
        #endif
        drvManager.AddDriver(&mouse);
        
        PeripheralComponentInterconnectController PCIController;
        PCIController.SelectDrivers(&drvManager, &interrupts);

        VideoGraphicsArray vga;
        
    //printf("Initializing Hardware, Stage 2\n");
        drvManager.ActivateAll();
        
    //printf("Initializing Hardware, Stage 3\n");

    #ifdef GRAPHICSMODE
        vga.SetMode(320,200,8);
        Window win1(&desktop, 10,10,20,20, 0xA8,0x00,0x00);
        desktop.AddChild(&win1);
        Window win2(&desktop, 40,15,30,30, 0x00,0xA8,0x00);
        desktop.AddChild(&win2);
    #endif

    interrupts.Activate();

    uint32_t* memupper = (uint32_t*)(((size_t)multiboot_structure) + 8);
    size_t memory = 10*1024*1024;
    MemoryManager memoryManager1(memory, (*memupper)* 1024 - memory - 10*1024);

    PageTableEntry * pageTable = (PageTableEntry *) memoryManager1.malloc(5 * sizeof(PageTableEntry));

    int size = 10;

    int * arrMem = (int *) memoryManager1.malloc(20);
    int * arrDisk1 = (int *) memoryManager1.malloc(80);
    int * arrDisk2 = (int *) memoryManager1.malloc(80);
    int * arrDisk3 = (int *) memoryManager1.malloc(80);
    int * lruArray = (int *) memoryManager1.malloc(20);

    int diskSize = 20;
    int memorySize = 5;

    bool Entry = false;
    while(1)
    {
        resetArrays(memoryManager1, arrDisk1, arrDisk2, arrDisk3, arrMem, lruArray, memorySize);
        memoryManager1.prepareDefaultPageTable(pageTable, arrMem, memorySize);

        SortChoice = -1;
        AlgorithmChoice = -1;
        Sorted = -1;
        printf("\n\nChoose Sort Type:\n");
        printf("    1) Bubble Sort\n");
        printf("    2) Quick Sort\n");
        printf("    3) Insertion Sort\n");
        while(SortChoice == -1);

        printf("\n\nChoose Page Replacement Algorithm:\n");
        printf("    1) FIFO\n");
        printf("    2) Second Chance\n");
        printf("    3) LRU\n");
        while(AlgorithmChoice == -1);

        printf("\n\nArray Choice:\n");
        printf("    1) Not Sorted1\n");
        printf("    2) Not Sorted 2\n");
        printf("    3) Sorted\n");

        while(Sorted == -1);

        memoryManager1.AlgorithmChoice = AlgorithmChoice - 1;
        SortChoice--;
        Sorted--;

        if(Entry)
        {
            memoryManager1.free(arrDisk1);
            memoryManager1.free(arrDisk2);
            memoryManager1.free(arrDisk3);
            memoryManager1.free(arrMem);
            memoryManager1.free(lruArray);
        }
            
        Entry = true;
        //memoryManager1.Instantiate(numberOfDiskPages, numberOfMemoryPages,Sorted);
        printf("SORT: ");
        printfHex(Sorted);
        printf("\nDisk Before Sort: ");

        if(Sorted == 0)
        {
            for(int i = 0; i < diskSize;++i)
            {
                printfHex(memoryManager1.getValue(pageTable, arrDisk1, lruArray, memorySize, i));
                printf(",");
            }
            printf("\n");
        }

        else if(Sorted == 1)
        {
            for(int i = 0; i < diskSize;++i)
            {
                printfHex(memoryManager1.getValue(pageTable, arrDisk3, lruArray, memorySize, i));
                printf(",");
            }
            printf("\n");
        }

        else if(Sorted == 2)
        {
            for(int i = 0; i < diskSize;++i)
            {
                printfHex(memoryManager1.getValue(pageTable, arrDisk2, lruArray, memorySize, i));
                printf(",");
            }
            printf("\n");
        }

        switch(SortChoice)
        {
            case 0:
                    
                if(Sorted == 0)
                {
                    memoryManager1.bubbleSort(pageTable, arrDisk1, lruArray, diskSize, memorySize);
                }
                else if(Sorted == 1)
                {
                    memoryManager1.bubbleSort(pageTable, arrDisk3, lruArray, diskSize, memorySize);
                }
                else if(Sorted == 2)
                {
                    memoryManager1.bubbleSort(pageTable, arrDisk2, lruArray, diskSize, memorySize);
                }

            break;

            case 1:
                printf("quickSort\n");
                if(Sorted == 0)
                {
                    memoryManager1.quickSort(pageTable, arrDisk1, lruArray, diskSize, memorySize, 0, diskSize -1);
                }
                else if(Sorted == 1)
                {
                    memoryManager1.quickSort(pageTable, arrDisk3, lruArray, diskSize, memorySize, 0, diskSize -1);
                }
                else if(Sorted == 2)
                {
                    memoryManager1.quickSort(pageTable, arrDisk2, lruArray, diskSize, memorySize, 0, diskSize -1);
                }

            break;
            case 2:

                if(Sorted == 0)
                {
                    memoryManager1.insertionSort(pageTable, arrDisk1, lruArray, diskSize, memorySize);
                }
                else if(Sorted == 1)
                {
                    memoryManager1.insertionSort(pageTable, arrDisk3, lruArray, diskSize, memorySize);
                }
                else if(Sorted == 2)
                {
                    memoryManager1.insertionSort(pageTable, arrDisk2, lruArray, diskSize, memorySize);
                }

            break;
        }
        
        for(int k = 0; k < memorySize; k++)
        {
            if(pageTable[k].modified)
            {
                if(Sorted == 0) 
                {
                    arrDisk1[pageTable[k].index] = pageTable[k].address[0];
                    pageTable[k].modified = false;
                }
                else if(Sorted == 1) 
                {
                    arrDisk3[pageTable[k].index] = pageTable[k].address[0];
                    pageTable[k].modified = false;
                }
                else if(Sorted == 2) 
                {
                    arrDisk2[pageTable[k].index] = pageTable[k].address[0];
                    pageTable[k].modified = false;
                }
            }
        }
    
        printf("\n\nDisk After Sort : ");

        if(Sorted == 0)
        {
            for(int i = 0; i < diskSize;++i)
            {
                //printfHex(memoryManager1.getValue(pageTable, arrDisk1, lruArray, memorySize, i));
                printfHex(arrDisk1[i]);
                printf(",");
            }
            printf("\n");
        }

        else if(Sorted == 1)
        {
            for(int i = 0; i < diskSize;++i)
            {
                //printfHex(memoryManager1.getValue(pageTable, arrDisk3, lruArray, memorySize, i));
                printfHex(arrDisk3[i]);
                printf(",");
            }
            printf("\n");
        }

        else if(Sorted == 2)
        {
            for(int i = 0; i < diskSize;++i)
            {
                //printfHex(memoryManager1.getValue(pageTable, arrDisk2, lruArray, memorySize, i));
                printfHex(arrDisk2[i]);
                printf(",");
            }
            printf("\n");
        }

        printf("The number of hits: ");
        printfHex2(memoryManager1.numberOfHits);
        printf(" From Number of tries: ");
        printfHex2(memoryManager1.totalOps);
        printf("\n");

        printf("The number of misses: ");
        printfHex2(memoryManager1.numberOfMisses);
        printf(" From Number of tries: ");
        printfHex2(memoryManager1.totalOps);
        printf("\n");

        printf("The number of pages loaded: ");
        printfHex2(memoryManager1.numberOfPagesLoaded);
        printf("\n");

        printf("The number of pages written back to the disk: ");
        printfHex2(memoryManager1.numberOPagesfWrittenBackToDisk);
        printf("\n");

        memoryManager1.resetBits();
        
    }
   
    while(1)
    {
        #ifdef GRAPHICSMODE
            desktop.Draw(&vga);
        #endif
    }
}
