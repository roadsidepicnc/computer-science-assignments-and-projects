 
#ifndef __MYOS__MULTITASKING_H
#define __MYOS__MULTITASKING_H

#include <common/types.h>
#include <gdt.h>


namespace myos
{
    enum ThreadState { READY, RUNNING, BLOCKED };

    
    struct CPUState
    {
        common::uint32_t eax;
        common::uint32_t ebx;
        common::uint32_t ecx;
        common::uint32_t edx;

        common::uint32_t esi;
        common::uint32_t edi;
        common::uint32_t ebp;

        /*
        common::uint32_t gs;
        common::uint32_t fs;
        common::uint32_t es;
        common::uint32_t ds;
        */
        common::uint32_t error;

        common::uint32_t eip;
        common::uint32_t cs;
        common::uint32_t eflags;
        common::uint32_t esp;
        common::uint32_t ss;        
    } __attribute__((packed));
    
    
    class Thread
    {
        friend class ThreadManager;
        private:
            common::uint8_t amount = 1;
            int typeID;
            int threadID;
            
        public:
            ThreadState threadState;
            Thread();
            Thread(int typeID, common::uint8_t amount, int threadID);
            ~Thread();
            void apply(common::uint8_t * num, int * turn, int interested[2]);
            void applyX(common::uint8_t * num, int * turn, int interested[2]);
            void joinThread();
            void createThread();
            void yieldThread();
            
    };

    class ThreadManager
    {
        private:        
            
        public:
            int numThreads;
            int currentThread;
            Thread* threads[256];
            void Schedule();
            ThreadManager();
            ~ThreadManager();
            bool AddThread(Thread * thread);     
    };

    /////////////////////////////////////////////////

    class Task
    {
    friend class TaskManager;
    private:          
        common::uint8_t stack[4096]; // 4 KiB
        CPUState* cpustate;
    public:
        int turn;
        int interested[2];
        common::uint8_t counter; 
        ThreadManager threadManager;
        Task(GlobalDescriptorTable *gdt, void entrypoint());
        ~Task();
        void x();
    };
    
    
    class TaskManager
    {
    private:
        
        int numTasks;
        int currentTask;
    public:

        int taskCounter;
        Task* tasks[256];
        TaskManager();
        ~TaskManager();
        bool AddTask(Task* task);      

        CPUState* Schedule(CPUState* cpustate);
    };
      
}


#endif