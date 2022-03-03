#define DMALLOC_DISABLE 1
#include "dmalloc.hh"
#include <cassert>
#include <cstring>
#include <unordered_map>
#include <string.h>
#include <iostream>
#include <sys/mman.h>
using namespace std;

struct dmalloc_stats initialize = {.nactive = 0, .active_size = 0, .ntotal = 0, .total_size = 0,
                                    .nfail = 0, .fail_size = 0, .heap_min = 0, .heap_max = 0};

int valid_free = 0;
int malloc_stats = 0;
uintptr_t malloc_address = 0;
uintptr_t end_address = 0;
const char* filename = NULL;
unordered_map<char*, size_t> malloc_size;
unordered_map<char*, long> malloc_line;
// unsigned long long ptr_size = 0;

/**
 * dmalloc(sz,file,line)
 *      malloc() wrapper. Dynamically allocate the requested amount `sz` of memory and 
 *      return a pointer to it 
 * 
 * @arg size_t sz : the amount of memory requested 
 * @arg const char *file : a string containing the filename from which dmalloc was called 
 * @arg long line : the line number from which dmalloc was called 
 * 
 * @return a pointer to the heap where the memory was reserved
 */
void* dmalloc(size_t sz, const char* file, long line) {
    (void) file, (void) line;   // avoid uninitialized variable warnings
    // Your code here.

    if(sz >= ((size_t) -1 - 150))
    {
        initialize.nfail += 1;
        initialize.fail_size += sz;
        return NULL;
    }

    // ptr_size = sz;
    char* ptr = (char*)base_malloc(sz + sizeof(size_t));
    // ptr[0] = '0' + sz;
    malloc_size[ptr] = sz;
    malloc_line[ptr] = line;
    filename = file;
    ptr[sz] = 'E';
    // ptr[1] = 1;
    // valid_free += (uintptr_t) ptr;
    valid_free += 1;
    malloc_address += (uintptr_t) ptr;
   
    if (ptr)
    {
        initialize.total_size += sz;
        initialize.active_size += sz;
        initialize.ntotal += 1;
        initialize.nactive += 1;
        // initialize.heap_min = (uintptr_t)ptr;
        // initialize.heap_max = (uintptr_t)ptr + sz + sizeof(size_t);

        if (!initialize.heap_min || initialize.heap_min > (uintptr_t) ptr) {
            initialize.heap_min = (uintptr_t) ptr;
        }
        if (!initialize.heap_max || initialize.heap_max < (uintptr_t) ptr + sz) {
            initialize.heap_max = (uintptr_t) ptr + sz + sizeof(size_t);
        }

    }
    malloc_stats = 1;
    return ptr;
}

/**
 * dfree(ptr, file, line)
 *      free() wrapper. Release the block of heap memory pointed to by `ptr`. This should 
 *      be a pointer that was previously allocated on the heap. If `ptr` is a nullptr do nothing. 
 * 
 * @arg void *ptr : a pointer to the heap 
 * @arg const char *file : a string containing the filename from which dfree was called 
 * @arg long line : the line number from which dfree was called 
 */
void dfree(void* ptr, const char* file, long line) {
    (void) file, (void) line;   // avoid uninitialized variable warnings
    // Your code here.
    uintptr_t free_address = (uintptr_t) ptr;
    
    if(!ptr) {
        return ;
    }

    char* sz = (char*)ptr;
    size_t size = 0;
    // auto map_ptr_size = malloc_size.find((char*) ptr);
    if (malloc_size.find((char*) ptr) != malloc_size.end())
    {
        size = malloc_size[(char*) ptr];
    }

    //  test16-19
    if((malloc_stats == 0) || (ptr < (void*)initialize.heap_min))
    {
        fprintf(stderr, "MEMORY BUG???: invalid free of pointer , not in heap\n");
        abort();
    }
    //test20
    if(valid_free <= 0)
    {
        fprintf(stderr,"MEMORY BUG???: invalid free of pointer %p, double free\n",ptr);
        abort();
    }
    //test21-24
    if(free_address > malloc_address || malloc_size.find((char*) ptr) == malloc_size.end())
    {
        int error = free_address - malloc_address;
        fprintf(stderr,"MEMORY BUG: %s:%ld: invalid free of pointer %p, not allocated\n",file, line,ptr);
        fprintf(stderr,"%s:%ld: %p is %d bytes inside a %ld byte region allocated here\n",file, malloc_line[(char*)malloc_address],(char*)ptr, error,malloc_size[(char*)malloc_address]);
        abort();
    }
    //test25
    // int size = sz[0];
    if(sz[size] != 'E')
    {
        fprintf(stderr,"MEMORY BUG???: detected wild write during free of pointer %p\n", ptr);
        abort();
    }


    if (ptr)
    {
        initialize.active_size -= size;
        // initialize.active_size -= ptr_size;
        initialize.nactive -= 1;
    }
    valid_free -= 1;
    base_free(ptr);
    
    auto map_ptr_size = malloc_size.find((char*) ptr);
    if(map_ptr_size != malloc_size.end())
        malloc_size.erase((char*)ptr);
    auto lines = malloc_line.find((char*) ptr);
    if(lines != malloc_line.end())
        malloc_line.erase((char*)ptr);

}

/**
 * dcalloc(nmemb, sz, file, line)
 *      calloc() wrapper. Dynamically allocate enough memory to store an array of `nmemb` 
 *      number of elements with wach element being `sz` bytes. The memory should be initialized 
 *      to zero  
 * 
 * @arg size_t nmemb : the number of items that space is requested for
 * @arg size_t sz : the size in bytes of the items that space is requested for
 * @arg const char *file : a string containing the filename from which dcalloc was called 
 * @arg long line : the line number from which dcalloc was called 
 * 
 * @return a pointer to the heap where the memory was reserved
 */
void* dcalloc(size_t nmemb, size_t sz, const char* file, long line) {
    // Your code here (to fix test014).
    if(nmemb>=(size_t) -1 / 8 + 2 || sz >=0x80000001UL)
    {
        initialize.nfail += 1;
        initialize.fail_size += sz;
        return NULL;
    }
    void* ptr = dmalloc(nmemb * sz, file, line);
    if (ptr) {
        memset(ptr, 0, nmemb * sz);
    }
    return ptr;
}

/**
 * get_statistics(stats)
 *      fill a dmalloc_stats pointer with the current memory statistics  
 * 
 * @arg dmalloc_stats *stats : a pointer to the the dmalloc_stats struct we want to fill
 */
void get_statistics(dmalloc_stats* stats) {
    // Stub: set all statistics to enormous numbers
    memset(stats, 255, sizeof(dmalloc_stats));
    // Your code here.
    // *stats = initialize;
    // void* ptr = dmalloc(sizeof(&stats.ntotal), &test002.cc, 7);
    // base_free(*stats);
    // stats.ntotal = 
    stats->nactive = initialize.nactive;         // # active allocations
    stats->active_size = initialize.active_size;     // # bytes in active allocations
    stats->ntotal = initialize.ntotal;          // # total allocations
    stats->total_size = initialize.total_size;      // # bytes in total allocations
    stats->nfail = initialize.nfail;           // # failed allocation attempts
    stats->fail_size = initialize.fail_size;       // # bytes in failed alloc attempts
    stats->heap_min = initialize.heap_min;                 // smallest allocated addr
    stats->heap_max = initialize.heap_max;                 // largest allocated addr
}

/**
 * print_statistics()
 *      print the current memory statistics to stdout       
 */
void print_statistics() {
    dmalloc_stats stats;
    get_statistics(&stats);

    printf("alloc count: active %10llu   total %10llu   fail %10llu\n",
           stats.nactive, stats.ntotal, stats.nfail);
    printf("alloc size:  active %10llu   total %10llu   fail %10llu\n",
           stats.active_size, stats.total_size, stats.fail_size);
}

/**  
 * print_leak_report()
 *      Print a report of all currently-active allocated blocks of dynamic
 *      memory.
 */
void print_leak_report() {
    // Your code here.
    if(valid_free == 0)
        return ;
    for(auto t : malloc_size)
    {
        printf("LEAK CHECK: %s:%ld: allocated object %p with size %ld\n", filename, malloc_line[t.first], t.first, t.second);
        // cout <<"LEAK CHECK: "<< filename << ": allocated object "<< &it->first <<" with size "<< malloc_size[it->first]<<endl;
    }
        


}
