Project 2: DMalloc
===================

<!-- TODO: Fill this out. -->
## Design Overview:

## Collaborators:
Yiran Zhou
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
About 12 hours

## Extra Credits?
1.Realloction
In dmalloc.cc

2. Sanitizer
GCC AddressSanitizer
AddressSanitizer is a very useful tool that can dectect many memory and other type of bugs. It can find use-after-freeand heap , stack, global-buffer overflow bugs in C programs.
The algorithm of the Address Sanitizer to approach memory error detection is similar to that of the Valgrind-based tool AddrCheck: use shadow memory to record whether each byte of application memory is safe to access. Then use instrumentation to check the shadow memory on each application load or store. But AddressSanitizer uses a more efficient shadow mapping and a more compact shadow encoding, detects errors in stack and global variables in addition to the heap and is an order of magnitude faster than AddrCheck. About the shadow memory, the Address Sanitizer will dedicate one-eighth of the virtual address space to its shadow memmory and uses a direct mapping with a scale and offset to translate an application address to its corresponding shadow address. There is also a library called run time library. The main purpose of the run-time library is to manage the shadow memory. At application startup the entire shadow region is mapped so that no other part of the program can use it. The Bad segment of the shadow memory is protected.The malloc and free functions are replaced with a specialized implementation. The malloc function allocates extra memory, the redzone, around the returned region. The redzones are marked as unaddressable, or poisoned. The larger the redzone, the larger the overflows or underflows that will be detected. The memory regions inside the allocator are organized as an array of freelists corresponding to a range of object sizes. When a freelist that corresponds to a requested object size is empty, a large group of memory regions with their redzones is allocated from the operating system. The left redzone is used to store the internal data of the allocator; consequently, the minimum size of the heap redzone is currently 32 bytes. This internal data can not be corrupted by a buffer underflow, because such underflows are detected immediately prior to the actual store (if the underflow happens in the instrumented code). The free function poisons the entire memory region and puts it into quarantine, such that this region will not be allocated by malloc any time soon. Currently, the quarantine is implemented as a FIFO queue which holds a fixed amount of memory at any time. By default, malloc and free record the current call stack in order to provide more informative bug reports. The malloc call stack is stored in the left redzone (the larger the redzone, the larger the number of frames that can be stored) while the free call stack is stored in the beginning of the memory region itself. In order to detect out-of-bounds accesses to globals and stack objects, AddressSanitizer must create poisoned redzones around such objects. For globals, the redzones are created at compile time and the addresses of the redzones are passed to the runtime library at application startup. The run-time library function poisons the redzones and records the addresses for further error reporting. For stack objects, the redzones are created and poisoned at run-time. Currently, redzones of 32 bytes (plus up to 31 bytes for alignment) are used. AddressSanitizer is thread-safe. The shadow memory is modified only when the corresponding application memory is not accessible (inside malloc or free, during creation or destruction of a stack frame, during module initialization). All other accesses to the shadow memory are reads. The malloc and free functions use thread-local caches to avoid locking on every call (as most modern malloc implementations do). If the original program has a race between a memory access and deletion of that memory, AddressSanitizer may sometimes detect it as a use-after-free bug, but is not guaranteed to. 

source:AddressSanitizer: A Fast Address Sanity Checker https://static.googleusercontent.com/media/research.google.com/zh-CN//pubs/archive/37752.pdf
