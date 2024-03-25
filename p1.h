// File: p1.h
// Author: Sam Udall
// Date: September 17, 2023
// Purpose: memory management
#ifndef P1_H
#define P1_H

class node {
    public:
        node(int v = 0, node *pn = NULL, node *pp = NULL);
        node *next;
        node *previous;
        int *val;
    friend class memList;
};

class memList{
    public:
       
        memList(int bCount = 128, int attempt = 10000, int tp = 50);
        ~memList();
    private:
};


class firstFit {
    public:
        firstFit(int bCount, int attempt, int tp);
        ~firstFit();
        void displayStats();
        void runIt();
    private:
     node *first;
        node *last;
        int blockCount;     //the total number of blocks (128)
        int attempts;       // the number of attempt to allocate.
        int totalProcesses;
        int pCount;
        float alloCalls;
        float alloFails;
        float nodeTraversed;
        float totalFragTraversed;
        int *processes;

        void findHole();
        bool allocate(int pid, int blocks);
        void deallocate(int pid);
        void clear(node *pn);
        
};

class nextFit {
    public:
        nextFit(int bCount, int attempt, int tp);
        ~nextFit();
        void displayStats();
        void runIt();
    private:
     node *first;
        node *last;
        int blockCount;     //the total number of blocks (128)
        int attempts;       // the number of attempt to allocate.
        int totalProcesses;
        int pCount;
        float alloCalls;
        float alloFails;
        float nodeTraversed;
        float totalFragTraversed;
        int *processes;
        node *placeHolder; 

        void findHole();
        bool allocate(int pid, int blocks);
        void deallocate(int pid);
        void clear(node *pn);
};

class bestFit {
    public:
        bestFit(int bCount, int attempt, int tp);
        ~bestFit();
        void displayStats();
        void runIt();

    private:
     node *first;
        node *last;
        int blockCount;     //the total number of blocks (128)
        int attempts;       // the number of attempt to allocate.
        int totalProcesses;
        int pCount;
        float alloCalls;
        float alloFails;
        float nodeTraversed;
        float totalFragTraversed;
        int *processes;

        void findHole();
        bool allocate(int pid, int blocks);
        void deallocate(int pid);
        void clear(node *pn);
};

class worstFit {
    public:
        worstFit(int bCount, int attempt, int tp);
        ~worstFit();
        void displayStats();
        void runIt();

    private:
     node *first;
        node *last;
        int blockCount;     //the total number of blocks (128)
        int attempts;       // the number of attempt to allocate.
        int totalProcesses;
        int pCount;
        float alloFails;
        float alloCalls;
        float nodeTraversed;
        float totalFragTraversed;
        int *processes;

        void findHole();
        bool allocate(int pid, int blocks);
        void deallocate(int pid);
        void clear(node *pn);
};

#endif