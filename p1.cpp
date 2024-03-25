// File: p1.cpp
// Author: Sam Udall
// Date: September 17, 2023

#include <cstdio>     // Provides standard output and input
#include <iostream>   // Provides cout
#include <cstdlib>    // Provides EXIT_SUCCESS
#include <ctime>      // Provides time library
#include <cmath>      // Provides math library
#include "p1.h"       // Provides all class files

using namespace std;  // Allows all standard library items to be used

// ___________________________Node class_______________________________

node::node(int v, node *pn, node *pp) {
	val = new int[3];   // [pid, start, end]
    val[0] = v;
	next = pn;
    previous = pp;
}

// ___________________________List class_______________________________

memList::memList(int bCount, int attempt, int tp) {   
    
    firstFit *FF = new firstFit(bCount, attempt, tp); // try the first fit alogrithm
    FF->runIt();
    cout << "FF done" << endl;
    nextFit *NF = new nextFit(bCount, attempt, tp);
    NF->runIt();
    cout << "NF done" << endl;
    bestFit *BF = new bestFit(bCount, attempt, tp);
    BF->runIt();
    cout << "BF done" << endl;
    worstFit *WF = new worstFit(bCount, attempt, tp);
    WF->runIt();
    cout << "WF done" << endl;
    // display all stats
    FF->displayStats();
    NF->displayStats();
    BF->displayStats();
    WF->displayStats();
}

memList::~memList() {

}

// ___________________________FirstFit class_______________________________

firstFit::firstFit(int bCount, int attempt, int tp) {
    blockCount = bCount;    // the total number of blocks
    attempts = attempt;     // the total number of attempts to allocate everything
    totalProcesses = tp;    // the total number of processes
    pCount = 0;             // the total processes curently allocated 
    processes = new int[totalProcesses]; // the array for allocation states.
    alloCalls = 0;
    alloFails = 0;      //stats
    nodeTraversed = 0;
    totalFragTraversed = 0;

    first = new node(-1, last, NULL);   //these are not actual processes just reference points to start and end
    last = new node(-1, NULL, first);
    first->next =last;
    first->val[1] = 0;
    first->val[2] = blockCount - 1;         // filling in there varibales 
    last->val[1] = blockCount - 1;
    last->val[2] = 0;
    
}

    firstFit::~firstFit() {
    
}
void firstFit::clear(node *pn){
    node *t;
    if (pn == first) { // clear every node but first and last
        t = pn->next;
        clear(t);
    } else if (pn != last) {
        t = pn->next;
        delete pn;
        clear(t);
    }
    first->next = last;
    last->previous = first;
    pCount = 0;
}

void firstFit::runIt() {
    int pid;
    int blocks;
    int traversalTemp = 0;

    for(int i = 0; i < totalProcesses; i++) { // set all processes states to deallocated
        processes[i] = 0;
    }
    for (int i = 0; i < attempts; i++) {
        traversalTemp = nodeTraversed;          // temp value to reset traversal if allocation fails
        pid = rand() % totalProcesses;
        blocks = rand() % 15 +1;        // blocks between 1 to 16
        if (processes[pid] == 0) {      // if its not allocated yet
            alloCalls++;
            
            if (!allocate(pid, blocks)) {   // if it fail to allocate
                alloFails++;
                nodeTraversed = traversalTemp;  // this doesn't change if allocation fails
            } else {                // it successfully allocated
                processes[pid] = 1;
                pCount++; 
                findHole();     // find fragments
            }            
        } else if (processes[pid] == 1) { // it has been de allocated
            deallocate(pid);
            processes[pid] = 0;
        }
    }
}

bool firstFit::allocate(int pid, int blocks) {
    bool rc = false;
    node *t = first;    //temp nodes 
    node *t2;
     // if no proccesses exist or if there is space between "first" and the actual first process
    if (t->next->val[1] > blocks) { // if there is space between first and the first proccess
        t2 = new node(pid, t->next, t);
        t->next->previous = t2;         //t2's next points to t2 for it pervious
        t->next = t2;
        t2->val[1] = 0;
        t2->val[2] = blocks - 1;    // process takes memory space 0 to blocks-1 
        rc = true; 
    } else {
        t = t->next;
        nodeTraversed++;
        while (t != last) {
            if (((t->next->val[1]) - t->val[2]) > blocks) { // if space between t ends and t->next starts is greater or equal to blocks
                t2 = new node(pid, t->next, t);
                t->next->previous = t2; // change nexts previous to be new node
                t->next = t2; // change previous next to be new node
                t2->val[1] = t->val[2] + 1;
                t2->val[2] = t->val[2] + blocks;
                rc = true;
                break;
            }
            nodeTraversed++;
            t = t->next;
        }
    }
    return rc;
}

void firstFit::deallocate(int pid) {
    // assume pid is already allocated
    node *t = first->next;
    for (int i = 0; i < pCount; i++) {
        if (t->val[0] == pid) {
            t->previous->next = t->next;    //fix pointers to go around pid to delete
            t->next->previous = t->previous;
            break;
        } else {
            t =t->next; // find pid
        }
    }
    delete t;
    pCount--;
}

void firstFit::displayStats() {
    cout << "\nStats  for FirstFit:" << endl;
    cout << "Avg Frag count " << totalFragTraversed / (alloCalls-alloFails) << endl;
    cout << "Avg traversal count " << nodeTraversed / (alloCalls-alloFails) <<endl;
    cout << "Fail rate " << alloFails / alloCalls << endl;
}

void firstFit::findHole() {
    node *t = first;    //temp nodes 
    while (t != last)
    {
        // if space between t nexts start and t's end is 1 or 2 found fragment
        if ((t->next->val[1]-t->val[2]) == 1 || (t->next->val[1]-t->val[2]) == 2 ) {
            totalFragTraversed++;
        }
        t = t->next;
    }
    
}

// ___________________________NextFit class_______________________________


nextFit::nextFit(int bCount, int attempt, int tp) {
    blockCount = bCount;    // the total number of blocks
    attempts = attempt;     // the total number of attempts to allocate everything
    totalProcesses = tp;    // the total number of processes
    pCount = 0;             // the total processes curently allocated 
    processes = new int[totalProcesses]; // the array for allocation states.
    alloCalls = 0;
    alloFails = 0;      //stats
    nodeTraversed = 0;
    totalFragTraversed = 0;

    first = new node(-1, last, NULL);   //these are not actual processes just reference points to start and end
    last = new node(-1, NULL, first);
    first->next =last;
    first->val[1] = 0;
    first->val[2] = blockCount - 1;         // filling in there varibales 
    last->val[1] = blockCount - 1;
    last->val[2] = 0;
}

    nextFit::~nextFit() {
    
}

void nextFit::runIt() {
    int pid;
    int blocks;
    int traversalTemp = 0;
    placeHolder = first;

    for(int i = 0; i < totalProcesses; i++) { // set all processes states to deallocated
        processes[i] = 0;
    }
    for (int i = 0; i < attempts; i++) {
        traversalTemp = nodeTraversed;          // temp value to reset traversal if allocation fails
        pid = rand() % totalProcesses;
        blocks = rand() % 15 +1;        // blocks between 1 to 16
      
        if (processes[pid] == 0) {      // if its not allocated yet
            alloCalls++;
            
            if (!allocate(pid, blocks)) {   // if it fail to allocate
                alloFails++;
                nodeTraversed = traversalTemp;  // this doesn't change if allocation fails
            } else {                // it successfully allocated
                processes[pid] = 1;
                pCount++;
                findHole();     // find fragments
            }
        } else if (processes[pid] == 1) { // it has been de allocated
            deallocate(pid);
            processes[pid] = 0;
        }
    }
}

void nextFit::clear(node *pn){
    node *t;
    if (pn == first) { // clear every node but first and last
        t = pn->next;
        clear(t);
    } else if (pn != last) {
        t = pn->next;
        delete pn;
        clear(t);
    }
    first->next = last;
    last->previous = first;
    pCount = 0;
}

bool nextFit::allocate(int pid, int blocks) {
    bool rc = false;
    node *t = placeHolder;  // where we left off
    node *t2; 
    if (t == first && t->next->val[1] > blocks) { // if no proccesses exist 
        t2 = new node(pid, t->next, t);
        t->next->previous = t2;
        t->next = t2;
        t2->val[1] = 0;
        t2->val[2] = blocks - 1;    // process takes memory space 0 to blocks-1 
        placeHolder = t2;
        rc = true; 

    } else {
        while (t != last) {
            if (((t->next->val[1]) - t->val[2]) > blocks) { // if space between t ends and t->next starts is greater or equal to blocks
                t2 = new node(pid, t->next, t);
                t->next->previous = t2; // change nexts previous to be new node
                t->next = t2; // change previous next to be new node
                t2->val[1] = t->val[2] + 1;
                t2->val[2] = t->val[2] + blocks;
                rc = true;
                placeHolder = t2;
                break;
            } else {    // it could not fit in this space
                nodeTraversed++;
                t = t->next;
            }
        }   // t = last
        if (!rc) {
            t = first;  //if it hasn't been allocated yet search first back to placeholder
            if (t->next->val[1] > blocks) { // if there is space between first and the first proccess
                t2 = new node(pid, t->next, t);
                t->next->previous = t2;         //t2's next points to t2 for it pervious
                t->next = t2;
                t2->val[1] = 0;
                t2->val[2] = blocks - 1;    // process takes memory space 0 to blocks-1 
                rc = true;  
            } else {
                t = t->next;
                while (t != placeHolder) { // go back to place holder
                    nodeTraversed++;
                    if (((t->next->val[1]) - t->val[2]) > blocks) { // if space between t ends and t->next starts is greater or equal to blocks
                        t2 = new node(pid, t->next, t);
                        t->next->previous = t2; // change nexts previous to be new node
                        t->next = t2; // change previous next to be new node
                        t2->val[1] = t->val[2] + 1;
                        t2->val[2] = t->val[2] + blocks;
                        placeHolder = t2;
                        rc = true;
                        break;
                    } else {    // it could not fit in this space
                        t = t->next;
                    }
                }
            }
        }
    }
    return rc;
}

void nextFit::deallocate(int pid) {
        // assume pid is already allocated
    node *t = first->next;
    for (int i = 0; i < pCount; i++) {
        if (t->val[0] == pid) {
            if (t == placeHolder) { // if pid is placeholder, move placeholder back
                placeHolder = placeHolder->previous;
            }
            t->previous->next = t->next;    //fix pointers to go around pid to delete
            t->next->previous = t->previous;
            break;
        } else {
            t =t->next; // find pid
        }
    }
    delete t;
    pCount--;
}

void nextFit::displayStats() {
    cout << "\nStats  for NextFit:" << endl;
    cout << "Avg Frag count " << totalFragTraversed / (alloCalls-alloFails) << endl;
    cout << "Avg traversal count " << nodeTraversed / (alloCalls-alloFails) <<endl;
    cout << "Fail rate " << alloFails / alloCalls << endl;
}

void nextFit::findHole() {
    node *t = first;    //temp nodes 
    while (t != last)
    {
        // if space between t nexts start and t's end is 1 or 2 found fragment
        if ((t->next->val[1]-t->val[2]) == 1 || (t->next->val[1]-t->val[2]) == 2 ) {
            totalFragTraversed++;
        }
        t = t->next;
    }
    
}

// ___________________________BestFit class_______________________________

bestFit::bestFit(int bCount, int attempt, int tp) {
    blockCount = bCount;    // the total number of blocks
    attempts = attempt;     // the total number of attempts to allocate everything
    totalProcesses = tp;    // the total number of processes
    pCount = 0;             // the total processes curently allocated 
    processes = new int[totalProcesses]; // the array for allocation states.
    alloCalls = 0;
    alloFails = 0;      //stats
    nodeTraversed = 0;
    totalFragTraversed = 0;

    first = new node(-1, last, NULL);   //these are not actual processes just reference points to start and end
    last = new node(-1, NULL, first);
    first->next =last;
    first->val[1] = 0;
    first->val[2] = blockCount - 1;         // filling in there varibales 
    last->val[1] = blockCount - 1;
    last->val[2] = 0;
    
}

void bestFit::runIt() {
    int pid;
    int blocks;
    int traversalTemp = 0;

    for(int i = 0; i < totalProcesses; i++) { // set all processes states to deallocated
        processes[i] = 0;
    }
    for (int i = 0; i < attempts; i++) {
        traversalTemp = nodeTraversed;          // temp value to reset traversal if allocation fails
        pid = rand() % totalProcesses;
        blocks = rand() % 15 +1;        // blocks between 1 to 16
        if (processes[pid] == 0) {      // if its not allocated yet
            alloCalls++;
            if (!allocate(pid, blocks)) {   // if it fail to allocate
                alloFails++;
                nodeTraversed = traversalTemp;  // this doesn't change if allocation fails
            } else {                // it successfully allocated
                processes[pid] = 1;
                pCount++;
                findHole();     // find fragments
            }            
        } else if (processes[pid] == 1) { // it has been de allocated
            deallocate(pid);
            processes[pid] = 0;
        }
    }
}

bestFit::~bestFit() {
    
}

void bestFit::clear(node *pn){
    node *t;
    if (pn == first) { // clear every node but first and last
        t = pn->next;
        clear(t);
    } else if (pn != last) {
        t = pn->next;
        delete pn;
        clear(t);
    }
    first->next = last;
    last->previous = first;
    pCount = 0;
}

bool bestFit::allocate(int pid, int blocks) {
    bool rc = false;
    node *t = first;
    node *best = first;
    node *t2; 
    int bestval = blockCount; // defualt out of bounds
    
    if (t->next == last) { // if no process exists
        t2 = new node(pid, last, first);
        first->next = t2;
        last->previous = t2;
        t2->val[1] = 0;
        t2->val[2] = blocks - 1;    // process takes memory space 0 to blocks-1 
        rc = true; 

    } else {
        t = t->next;
        while(t != last) {
            if ((t->next->val[1] - t->val[2]) > blocks && (t->next->val[1] - t->val[2]) < bestval) {
                best = t;
                bestval = (t->next->val[1] - t->val[2]);
                nodeTraversed++;
            }

            t = t->next;
        }
        if (bestval < blockCount) {
            t2 = new node(pid, best->next, best);
            best->next->previous= t2;
            best->next = t2;
        t2->val[1] = best->val[1] + 1;
        t2->val[2] = best->val[1] + blocks;    // process takes memory space 0 to blocks-1
        
        rc = true;
        }
    }
    return rc;
}

void bestFit::deallocate(int pid) {
    // assume pid is already allocated
    node *t = first->next;
    for (int i = 0; i < pCount; i++) {
        if (t->val[0] == pid) {
            t->previous->next = t->next;    //fix pointers to go around pid to delete
            t->next->previous = t->previous;
            break;
        } else {
            t =t->next; // find pid
        }
    }
    delete t;
    pCount--;
}

void bestFit::displayStats() {
    cout << "\nStats  for BestFit:" << endl;
    cout << "Avg Frag count " << totalFragTraversed / (alloCalls-alloFails) << endl;
    cout << "Avg traversal count " << nodeTraversed / (alloCalls-alloFails) <<endl;
    cout << "Fail rate " << (alloFails / alloCalls) << endl;
}

void bestFit::findHole() {
    node *t = first;    //temp nodes 
    while (t != last)
    {
        // if space between t nexts start and t's end is 1 or 2 found fragment
        if ((t->next->val[1]-t->val[2]) == 1 || (t->next->val[1]-t->val[2]) == 2 ) {
            totalFragTraversed++;
        }
        t = t->next;
    }
    
}

// ___________________________WorstFit class_______________________________

worstFit::worstFit(int bCount, int attempt, int tp) {
    blockCount = bCount;    // the total number of blocks
    attempts = attempt;     // the total number of attempts to allocate everything
    totalProcesses = tp;    // the total number of processes
    pCount = 0;             // the total processes curently allocated 
    processes = new int[totalProcesses]; // the array for allocation states.
    alloCalls = 0;
    alloFails = 0;      //stats
    nodeTraversed = 0;
    totalFragTraversed = 0;

    first = new node(-1, last, NULL);   //these are not actual processes just reference points to start and end
    last = new node(-1, NULL, first);
    first->next =last;
    first->val[1] = 0;
    first->val[2] = blockCount - 1;         // filling in there varibales 
    last->val[1] = blockCount - 1;
    last->val[2] = 0;
    
}

void worstFit::runIt() {
    int pid;
    int blocks;
    int traversalTemp = 0;

    for(int i = 0; i < totalProcesses; i++) { // set all processes states to deallocated
        processes[i] = 0;
    }
    for (int i = 0; i < attempts; i++) {
        traversalTemp = nodeTraversed;          // temp value to reset traversal if allocation fails
        pid = rand() % totalProcesses;
        blocks = rand() % 15 +1;        // blocks between 1 to 16
        if (processes[pid] == 0) {      // if its not allocated yet
            alloCalls++;
            
            if (!allocate(pid, blocks)) {   // if it fail to allocate
                alloFails++;
                nodeTraversed = traversalTemp;  // this doesn't change if allocation fails
            } else {                // it successfully allocated
                processes[pid] = 1;
               pCount++; 
               findHole();      // find fragments
            }
        } else if (processes[pid] == 1) { // it has been de allocated
            deallocate(pid);
            processes[pid] = 0;
        }
    }
}

worstFit::~worstFit() {
    
}

void worstFit::clear(node *pn){
    node *t;
    if (pn == first) { // clear every node but first and last
        t = pn->next;
        clear(t);
    } else if (pn != last) {
        t = pn->next;
        delete pn;
        clear(t);
    }
    first->next = last;
    last->previous = first;
    pCount = 0;
}

bool worstFit::allocate(int pid, int blocks) {
    bool rc = false;
    node *t = first;
    node *worst = first;
    node *t2; 
    int worstval = 0; // defualt out of bounds
    
    if (t->next == last) { // if no process exists
        t2 = new node(pid, t->next, t);
        first->next = t2;
        last->previous = t2;
        t2->val[1] = 0;
        t2->val[2] = blocks - 1;    // process takes memory space 0 to blocks-1 
        rc = true; 

    } else {
        
        if (t->next->val[1] > blocks) {
            worstval = t->next->val[1];
            worst = t;
            t = t->next;
            nodeTraversed++;
        }
        while(t != last) {
            if ((t->next->val[1] - t->val[2]) > blocks && (t->next->val[1] - t->val[2]) > worstval) {
                worst = t;
                worstval = (t->next->val[1] - t->val[2]);
                nodeTraversed++;
            }

            t = t->next;
        }
        if (worstval > 0) {
            t2 = new node(pid, worst->next, worst);
            worst->next->previous= t2;
            worst->next = t2;
        t2->val[1] = worst->val[1] + 1;
        t2->val[2] = worst->val[1] + blocks;    // process takes memory space 0 to blocks-1
        rc = true;
        }
    }
    return rc;
}

void worstFit::deallocate(int pid) {
    // assume pid is already allocated
    node *t = first->next;
    for (int i = 0; i < pCount; i++) {
        if (t->val[0] == pid) {
            t->previous->next = t->next;    //fix pointers to go around pid to delete
            t->next->previous = t->previous;
            break;
        } else {
            t =t->next; // find pid
        }
    }
    delete t;
    pCount--;
}

void worstFit::displayStats() {
    cout << "\nStats  for WorstFit:" << endl;
    cout << "Avg Frag count " << totalFragTraversed / (alloCalls-alloFails) << endl;
    cout << "Avg traversal count " << nodeTraversed / (alloCalls-alloFails) <<endl;
    cout << "Fail rate " << alloFails / alloCalls << endl;
}

void worstFit::findHole() {
    node *t = first;    //temp nodes 
    while (t != last)
    {
        // if space between t nexts start and t's end is 1 or 2 found fragment
        if ((t->next->val[1]-t->val[2]) == 1 || (t->next->val[1]-t->val[2]) == 2 ) {
            totalFragTraversed++;
        }
        t = t->next;
    }
    
}