Project 4 - WeensyOS
====================

<!-- TODO: Fill this out. -->

## Design Overview:
None
## Collaborators:

## Conceptual Questions:
1. What is the disadvantage of having an identity mapping between virtual and physical memory? What is the purpose of mapping virtual memory addresses to different physical addresses?
Multiple page tables need to be traversed, and memory needs to be accessed multiple times to achieve high complexity. The purpose of mapping virtual memory addresses to different physical addresses is it can save memory and page tables can be allocated on demand.

2.How does a new page table get prepared for a new process? Talk both about processes born from fork() and those not born from fork().
First we initialize the ptable and then assign a pagetable to the process. Then we mapping data before the process start address with kernel's data and permission.
The process that used the fork() system call is the parent process. In other words, a parent process is one that creates a child process. A parent process may have multiple child processes but a child process only one parent process.On the success of a fork() system call, the PID of the child process is returned to the parent process and 0 is returned to the child process. On the failure of a fork() system call, -1 is returned to the parent process and a child process is not created.
3.During the normal execution of the process, how does the process refer to its memory? Go through an example of translating a virtual memory address to an actual physical memory address.
The process was first allocated a physical address ad then mapping a virtual address. 
4.Upon exiting, what kind of resources have to be cleaned up and freed?

## Extra Credit attempted:

## How long did it take to complete WeensyOS?

<!-- Enter an approximate number of hours that you spent actively working on the project. -->
