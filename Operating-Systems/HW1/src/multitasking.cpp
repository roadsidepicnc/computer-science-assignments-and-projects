
#include <multitasking.h>

#define MAX_SIZE 255

using namespace myos;
using namespace myos::common;

void print(char* str)
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

void printHex(uint8_t key)
{
    char* foo = "00";
    char* hex = "0123456789ABCDEF";
    foo[0] = hex[(key >> 4) & 0xF];
    foo[1] = hex[key & 0xF];
    print(foo);
}

Thread :: Thread()
{
    //threadState = BLOCKED;
    typeID = 0;

}

Thread :: Thread(int typeID, uint8_t amount, int threadID)
{
    //threadState = BLOCKED;
    this->typeID = typeID;
    this->threadID = threadID;
    this->amount = amount;
}

Thread :: ~Thread()
{
}

void Thread :: createThread()
{
    printHex(threadID);
    print("Thread Created\n");
    threadState = READY;
}

void Thread :: joinThread()
{
    if(threadState == READY)
    {
       printHex(threadID);
        print("Thread Joined\n");
        threadState = RUNNING; 
    }   
}

void Thread :: yieldThread()
{
    if(threadState == RUNNING)
    {
        printHex(threadID);
        print("Thread Yielded\n");
        threadState = READY;
    }
}

void enterCriticalReigon(int threadID, int * turn, int interested[2])
{
    int num;
    int other = 1 - threadID;
    interested[threadID] = 1;
    * turn = threadID;
    while(* turn == threadID && interested[other] == 1);
}

void leaveCriticalRiegion(int threadID, int interested[2])
{
    interested[threadID] = 0;
}

void Thread:: apply(uint8_t * num, int * turn, int interested[2])
{
    enterCriticalReigon(threadID, turn, interested);

    if(typeID == 0 && amount < MAX_SIZE)
    {
        *num += amount;
        printHex(threadID);
        print(" Producer Thread Produced: ");
        printHex(*num);
        print("\n");
    }
    else if(typeID == 1 && amount > 0)
    {
        *num -= amount;
        printHex(threadID);
        print(" Thread Consumed: ");
        printHex(*num);
        print("\n");
    }

    leaveCriticalRiegion(threadID, interested);
}

void Thread:: applyX(uint8_t * num, int * turn, int interested[2])
{
    int other = 1 - threadID;
    interested[threadID] = 1;
    * turn = threadID;
    
    while(* turn == threadID && interested[other] == 1)
    {
        print("WAITING!\n");
    }

    if(typeID == 0 && amount < MAX_SIZE)
    {
        *num += amount;
        printHex(threadID);
        print(" Producer Thread Produced: ");
        printHex(*num);
        print("\n");
    }
    else if(typeID == 1 && amount > 0)
    {
        *num -= amount;
        printHex(threadID);
        print(" Thread Consumed: ");
        printHex(*num);
        print("\n");
    }
}

ThreadManager::ThreadManager()
{
    numThreads = 0;
    currentThread = -1;
}

ThreadManager::~ThreadManager()
{
}

bool ThreadManager::AddThread(Thread * thread)
{
    if(numThreads >= 256)
        return false;
    threads[numThreads++] = thread;
    return true;
}

void ThreadManager::Schedule()
{

    if(numThreads <= 0)
    {
        return;
    }

    /*  Yield  */
        
    if(++currentThread >= numThreads)
    {
        currentThread %= numThreads;
    }

    threads[currentThread]->yieldThread();

}

/////////////////////////////////////////////////////

Task::Task(GlobalDescriptorTable *gdt, void entrypoint())
{
    counter = 0;
    cpustate = (CPUState*)(stack + 4096 - sizeof(CPUState));
    
    cpustate -> eax = 0;
    cpustate -> ebx = 0;
    cpustate -> ecx = 0;
    cpustate -> edx = 0;

    cpustate -> esi = 0;
    cpustate -> edi = 0;
    cpustate -> ebp = 0;
    
    /*
    cpustate -> gs = 0;
    cpustate -> fs = 0;
    cpustate -> es = 0;
    cpustate -> ds = 0;
    */
    
    // cpustate -> error = 0;    
   
    // cpustate -> esp = ;
    cpustate -> eip = (uint32_t)entrypoint;
    cpustate -> cs = gdt->CodeSegmentSelector();
    // cpustate -> ss = ;
    cpustate -> eflags = 0x202;
    
}

void Task::x()
{
}

Task::~Task()
{
}

        
TaskManager::TaskManager()
{
    numTasks = 0;
    currentTask = -1;
}

TaskManager::~TaskManager()
{
}

bool TaskManager::AddTask(Task* task)
{
    if(numTasks >= 256)
        return false;
    tasks[numTasks++] = task;
    return true;
}

CPUState* TaskManager::Schedule(CPUState* cpustate)
{
    tasks[currentTask]->threadManager.Schedule();

    if(numTasks <= 0)
    {
        return cpustate;
    }
    
    if(currentTask >= 0)
    {
        tasks[currentTask]->cpustate = cpustate;
    }
    
    if(++currentTask >= numTasks)
    {
        currentTask %= numTasks;
    }

    return tasks[currentTask]->cpustate;
}

    