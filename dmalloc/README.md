Project 2: DMalloc
===================

<!-- TODO: Fill this out. -->
## Design Overview:

## Collaborators:

## Conceptual Questions:
1.Describe a situation when it is much better to use dynamic memory allocation over static or automatic memory allocation.
Dynamic memory allocation can control the life cycle of the variables. When we only need the variable in a particular time, it’s a good choice to use dynamic memory allocation rather than static or automatic memory allocation. Dynamic memory allocation don’t need to allocate memory previously. We can change the size of the memory according to our program.


2.How does the heap grow in memory? How is this different from how the stack grows? Why is this the case?
The heap grows up in memory. The stack grows down in the memory. Because when a return address is pushed onto the stack, the pointer of the stack variable would decrease. But when a new memory is allocated to a heap, it will be added on the top of the last chunk of heap memory allocated.

3.Consider some pointer, ptr, that points to memory address 0x12f99a64. What address does (char *) ptr + 1 point to? What address does (int *) ptr + 1 point to? Assume 32 bit (4 byte) integers.
Address (char *) ptr + 1 point to memory address 0x12f99a65. 
Address (int *) ptr + 1 point to 0x12f99a64.

4.Name and describe two potential memory errors that can arise from using malloc and free incorrectly.
(1)LeakSanitizer error: the program didn’t call free( ) for a heap-allocated memory region before exiting.
(2)AddressSanitizer(heap-use-after-free): the code accessed memory in a heap allocation after it was already freed.

5.How is checking for integer overflow in addition different than checking for integer overflow in multiplication?
In addition, we need to calculate sum. If both numbers are positive and sum is negative, we will return false. Else, if both numbers are negative and sum is positive then return false. 
In multiplication, there will be some special case such as 0, which will never exceed. If the product of two divided by one equals another, it won’t exceed. For some other cases, it will exceed.
## How many hours did it take to complete DMalloc?
