# CSC351-Operating-Systems-P1-Memory-Management
This was a programming assignment with the following instructions:

Referring to the Simulation-experiments.pdf, the task of this assignment is to implement Simulation Exercise 2: Main Memory Allocation.  In turn, the PDF refers to four allocation strategies; First Fit, Next Fit, Best Fit, and Worst Fit. This shall be implemented in C++.  The g++ compiler shall be used, as well as the "make" utility with an associated makefile.
Goal: To simulate and evaluate different memory allocation/deallocation techniques, first
fit, next fit, best fit, and worst fit, when a linked list is used to keep track of memory
usage.

The method I chose to solve this was with a doubly linked list with the holes in memory being implied and could be computationally calculated to find the spaces between used blocks. this method saves space I'm in memory usage at the cost of slightly more computation time. My Professor gave this assignment a 95%.
