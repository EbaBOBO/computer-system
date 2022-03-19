#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <stdarg.h>

#include "../io300.h"



/*
    student.c

    Fill in the following stencils

*/

/*
    When starting, you might want to change this for testing on small files.
*/
#ifndef CACHE_SIZE
#define CACHE_SIZE 4096
#endif

#if(CACHE_SIZE < 4)
#error "internal cache size should not be below 4."
#error "if you changed this during testing, that is fine."
#error "when handing in, make sure it is reset to the provided value"
#error "if this is not done, the autograder will not run"
#endif

/*
   This macro enables/disables the dbg() function. Use it to silence your
   debugging info.

   Use the dbg() function instead of printf debugging if you don't want to
   hunt down 30 printfs when you want to hand in
*/
#define DEBUG_PRINT 0
#define DEBUG_STATISTICS 1

struct io300_file {
    /* read,write,seek all take a file descriptor as a parameter */
    int fd;
    /* this will serve as our cache */
    char *cache;


    // TODO: Your properties go here
    off_t file_offset; //the position of file
    off_t file_head; // the head position of the cache
    size_t cache_rest; //the rest number of the cache
    int write_stat; // whether we need to flush
    char *rest; //the rest of the cache


    /* Used for debugging, keep track of which io300_file is which */
    char *description;
    /* To tell if we are getting the performance we are expecting */
    struct io300_statistics {
        int read_calls;
        int write_calls;
        int seeks;
    } stats;
};

/*
    Assert the properties that you would like your file to have at all times.
    Call this function frequently (like at the beginning of each function) to
    catch logical errors early on in development.
*/
static void check_invariants(struct io300_file *f) {
    assert(f != NULL);
    assert(f->cache != NULL);
    assert(f->fd >= 0);

    // TODO: Add more invariants
}

/*
    Wrapper around printf that provides information about the
    given file. You can silence this function with the DEBUG_PRINT macro.
*/
static void dbg(struct io300_file *f, char *fmt, ...) {
    (void)f; (void)fmt;
#if(DEBUG_PRINT == 1)
    static char buff[300];
    size_t const size = sizeof(buff);
    int n = snprintf(
        buff,
        size,
        // TODO: Add the fields you want to print when debugging
        "{desc:%s, } -- ",
        f->description
    );
    int const bytes_left = size - n;
    va_list args;
    va_start(args, fmt);
    vsnprintf(&buff[n], bytes_left, fmt, args);
    va_end(args);
    printf("%s", buff);
#endif
}



struct io300_file *io300_open(const char *const path, char *description) {
    if (path == NULL) {
        fprintf(stderr, "error: null file path\n");
        return NULL;
    }

    int const fd = open(path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        fprintf(stderr, "error: could not open file: `%s`: %s\n", path, strerror(errno));
        return NULL;
    }

    struct io300_file *const ret = malloc(sizeof(*ret));
    if (ret == NULL) {
        fprintf(stderr, "error: could not allocate io300_file\n");
        return NULL;
    }

    ret->fd = fd;
    ret->cache = malloc(CACHE_SIZE);
    if (ret->cache == NULL) {
        fprintf(stderr, "error: could not allocate file cache\n");
        close(ret->fd);
        free(ret);
        return NULL;
    }
    ret->description = description;
    // TODO: Initialize your file
    // ret->file_offset = 0;
    ret->file_head = 0;
    // ret->cache_rest = CACHE_SIZE;
    ret->write_stat = 0;
    // read(ret->fd, ret->cache, CACHE_SIZE);
    // // ret->rest = NULL;
    ret->rest = ret->cache;
    // ret->cache_rest = read(ret->fd, ret->cache, CACHE_SIZE);
    ret->cache_rest = pread(ret->fd, ret->cache, CACHE_SIZE,ret->file_head);


    check_invariants(ret);
    dbg(ret, "Just finished initializing file from path: %s\n", path);
    return ret;
}

int io300_seek(struct io300_file *const f, off_t const pos) {
    check_invariants(f);
    f->stats.seeks++;

    // TODO: Implement this
    
    if (pos >= f->file_head && pos <= (f->file_head + CACHE_SIZE - 1))
    {
        f->cache_rest -= pos - f->file_head;
        f->rest = pos - f->file_head + f->cache; 
    }
    else
    {
        if (f->write_stat == 1)
        {
            io300_flush(f);
            f->write_stat = 0;
        }
        f->rest = f->cache;
        f->file_head = pos;
        // f->cache_rest = read(f->fd, f->cache, CACHE_SIZE); // fill in the cache
        f->cache_rest = pread(f->fd, f->cache, CACHE_SIZE,f->file_head);

    }


    return lseek(f->fd, pos, SEEK_SET);
}

int io300_close(struct io300_file *const f) {
    check_invariants(f);

#if(DEBUG_STATISTICS == 1)
    printf("stats: {desc: %s, read_calls: %d, write_calls: %d, seeks: %d}\n",
            f->description, f->stats.read_calls, f->stats.write_calls, f->stats.seeks);
#endif
    // TODO: Implement this
    io300_flush(f);
    close(f->fd);
    free(f->cache);
    free(f);
    return 0;
}

off_t io300_filesize(struct io300_file *const f) {
    check_invariants(f);
    struct stat s;
    int const r = fstat(f->fd, &s);
    if (r >= 0 && S_ISREG(s.st_mode)) {
        return s.st_size;
    } else {
        return -1;
    }
}



int io300_readc(struct io300_file *const f) {
    check_invariants(f);
    // TODO: Implement this
    unsigned char c;
    // char c;
    if(f->cache_rest <= 0)
    {
        if(f->write_stat == 1)
        {
            // f->write_stat = 0;
            io300_flush(f);
            f->write_stat = 0;
        }
        f->file_head += CACHE_SIZE;
        f->rest = f->cache;
        // f->cache_rest = read(f->fd, f->cache, CACHE_SIZE);
        f->cache_rest = pread(f->fd, f->cache, CACHE_SIZE,f->file_head);
        
    }
        c = *(f->rest);
        f->rest += 1;
        f->cache_rest -= 1;

    return c;

    // if(f->cache_rest > 0)
    // {
    //     c = *(f->rest);
    //     f->rest += 1;
    //     f->cache_rest -= 1;
        
    // }
    // else
    // {
    //     //if full, flush ad read to fill in the cache
    //     if(f->write_stat == 1)
    //     {
    //         // f->write_stat = 0;
    //         io300_flush(f);
    //     }
    //     f->file_head += CACHE_SIZE;
    //     f->rest = f->cache;
    //     // f->cache_rest = read(f->fd, f->cache, CACHE_SIZE);
    //     f->cache_rest = pread(f->fd, f->cache, CACHE_SIZE,f->file_head);

    //     //read
    //     // c = *(f->rest);
    //     // f->rest += 1;
    //     // f->cache_rest -= 1;
    
    // }
    // printf("%c\n", c);
    // return c;

    // if (read(f->fd, &c, 1) == 1) {
    //     return c;
    // } else {
    //     return -1;
    // }
}
int io300_writec(struct io300_file *f, int ch) {
    check_invariants(f);
    // TODO: Implement this
    unsigned char const c = (unsigned char)ch;
    // char const c = (unsigned char)ch;
    if(f->rest >= f->cache + CACHE_SIZE)
    {
        if(f->write_stat == 1)
        {
            // f->write_stat = 0;
            io300_flush(f);
            f->write_stat = 0;
        }
        f->file_head += CACHE_SIZE;
        f->rest = f->cache;
        // f->cache_rest = read(f->fd, f->cache, CACHE_SIZE);
        f->cache_rest = pread(f->fd, f->cache, CACHE_SIZE,f->file_head);
    }
        memcpy(f->rest, &c, 1);
        f->rest += 1;
        f->cache_rest -= 1;
        f->write_stat = 1;
    return ch;

    // if(f->cache_rest > 0)
    // {
    //     memcpy(f->rest, &c, 1);
    //     f->rest += 1;
    //     f->cache_rest -= 1;
    //     f->write_stat = 1;
    // }
    // else
    // {
    //     //if full, flush ad read to fill in the cache
    //     if(f->write_stat == 1)
    //     {
    //         io300_flush(f);
    //     }
    //     f->file_head += CACHE_SIZE;
    //     f->rest = f->cache;
    //     // f->cache_rest = read(f->fd, f->cache, CACHE_SIZE);
    //     f->cache_rest = pread(f->fd, f->cache, CACHE_SIZE,f->file_head);
    //     //write
    //     memcpy(f->rest, &c, 1);
    //     f->rest += 1;
    //     f->cache_rest -= 1;
    //     f->write_stat = 1;
    // }
    // return ch;

    
    // return write(f->fd, &c, 1) == 1 ? ch : -1;
}

ssize_t io300_read(struct io300_file *const f, char *const buff, size_t const sz) {
    check_invariants(f);
    // TODO: Implement this
////////////////////////////////////////////////////
    //1. rest > sz
    if (f->cache_rest >= sz)
    {
        memcpy(buff, f->rest, sz);
        f->rest += sz;
        f->cache_rest -= sz;
        return sz;
    }
    else
    {
    //2. rest < sz
    size_t title = 0;
    //2-1 
    if(f->cache_rest > 0)
    {
        title = f->cache_rest;
        memcpy(buff, f->rest, title);
        f->rest += title;
    }
    if(f->write_stat == 1)
    {
        io300_flush(f);
        f->write_stat = 0;
    }
    f->file_head += CACHE_SIZE;
    f->rest = f->cache;
    f->cache_rest = pread(f->fd, f->cache, CACHE_SIZE,f->file_head);  

    if (f->cache_rest <=0)
    {
        return title;
    }

    // use a while loop to read repeatly
    // while(f->cache_rest == CACHE_SIZE && (sz - title) >= CACHE_SIZE)
    while((sz - title) >= CACHE_SIZE)
    {
        memcpy(title + buff, f->rest, CACHE_SIZE);
        f->rest += CACHE_SIZE;
        if(f->write_stat == 1)
        {
            io300_flush(f);
            f->write_stat = 0;
        }
        f->file_head += CACHE_SIZE;
        f->rest = f->cache;
        f->cache_rest = pread(f->fd, f->cache, CACHE_SIZE,f->file_head); 
        title += CACHE_SIZE;

        if(f->cache_rest <= 0)
        {
            return title;
        }
    }
    //tail
    size_t tail = sz - title;

    if (tail <= f->cache_rest)
    {
        memcpy(buff + title, f->rest, tail);
        f->cache_rest -= tail;
        f->rest += tail;
        return title + tail;
    }
    else
    {
        memcpy(buff + title, f->rest, f->cache_rest);
        size_t temp = f->cache_rest;
        f->rest += temp;
        f->cache_rest = 0;
        return title + temp; 
    }

    }
}

//////////////////////// stride
// ssize_t io300_read(struct io300_file *const f, char *const buff, size_t const sz, int stride) {
//     check_invariants(f);
//     // TODO: Implement this
//     off_t file_size = io300_filesize(f);
//     size_t file_offset = f->file_head + CACHE_SIZE - f->cache_rest;
//     //1. rest > sz
//     if (f->cache_rest >= sz)
//     {
//         for(int i=0; i < sz; i++)
//         {
//             if (file_offset > file_size)
//             {
//                 return -1;
//             }
//             memcpy(buff, f->rest, 1);
//             f->rest += 1;
//             f->cache_rest -= 1;
//             file_offset += stride;
//         }
//         return sz;
//     }
//     else
//     {
//     //2. rest < sz
//     size_t title = 0;
//     //2-1 
//     if(f->cache_rest > 0)
//     {
//         title = f->cache_rest;
//         // memcpy(buff, f->rest, title);
//         for(int i=0; i < title; i++)
//         {
//             if (file_offset > file_size)
//             {
//                 return -1;
//             }
//             memcpy(buff, f->rest, 1);
//             f->rest += 1;
//             f->cache_rest -= 1;
//             file_offset += stride;
//         }

//         // f->rest += title;
//     }
//     if(f->write_stat == 1)
//     {
//         io300_flush(f);
//         f->write_stat = 0;
//     }
//     f->file_head += CACHE_SIZE;
//     f->rest = f->cache;
//     f->cache_rest = pread(f->fd, f->cache, CACHE_SIZE,f->file_head);  

//     if (f->cache_rest <=0)
//     {
//         return title;
//     }

//     // use a while loop to read repeatly
//     // while(f->cache_rest == CACHE_SIZE && (sz - title) >= CACHE_SIZE)
//     while((sz - title) >= CACHE_SIZE)
//     {
//         // memcpy(title + buff, f->rest, CACHE_SIZE);
//         // f->rest += CACHE_SIZE;
//         for(int i=0; i < CACHE_SIZE; i++)
//         {
//             if (file_offset > file_size)
//             {
//                 return -1;
//             }
//             memcpy(buff + title + i, f->rest, 1);
//             f->rest += 1;
//             f->cache_rest -= 1;
//             file_offset += stride;
//         }

//         if(f->write_stat == 1)
//         {
//             io300_flush(f);
//             f->write_stat = 0;
//         }
//         f->file_head += CACHE_SIZE;
//         f->rest = f->cache;
//         f->cache_rest = pread(f->fd, f->cache, CACHE_SIZE,f->file_head); 
//         title += CACHE_SIZE;

//         if(f->cache_rest <= 0)
//         {
//             return title;
//         }
//     }
//     //tail
//     size_t tail = sz - title;

//     if (tail <= f->cache_rest)
//     {
//         // memcpy(buff + title, f->rest, tail);
//         // f->cache_rest -= tail;
//         // f->rest += tail;
//         for(int i=0; i < tail; i++)
//         {
//             if (file_offset > file_size)
//             {
//                 return -1;
//             }
//             memcpy(buff + title + i, f->rest, 1);
//             f->rest += 1;
//             f->cache_rest -= 1;
//             file_offset += stride;
//         }

//         return title + tail;
//     }
//     else
//     {
//         memcpy(buff + title, f->rest, f->cache_rest);
//         size_t temp = f->cache_rest;
//         // f->rest += temp;
//         f->cache_rest = 0;

//         for(int i=0; i < temp; i++)
//         {
//             if (file_offset > file_size)
//             {
//                 return -1;
//             }
//             memcpy(buff + title + i, f->rest, 1);
//             f->rest += 1;
//             // f->cache_rest -= 1;
//             file_offset += stride;
//         }

//         return title + temp; 
//     }

//     }
// }

ssize_t io300_write(struct io300_file *const f, const char *buff, size_t const sz) {
    check_invariants(f);
    // TODO: Implement this
    //1. rest > sz
    // if (f->cache_rest >= sz)
    if ((int)(f->cache + CACHE_SIZE - f->rest) > (int)sz)
    {
        memcpy(f->rest, buff, sz);
        f->rest += sz;
        f->cache_rest -= sz;
        f->write_stat = 1;
        return sz;
    }
    else
    {
    //2. rest < sz
    // size_t title = 0;
    size_t title = (size_t)(f->cache + CACHE_SIZE - f->rest);
    memcpy(f->rest, buff, title);
    f->rest += title;
    f->write_stat = 1;
    // memcpy()
    //2-1 
    // if(f->cache_rest > 0)
    // {
    //     // title = f->cache_rest;
    //     // title = (size_t)(f->cache + CACHE_SIZE - f->rest);
    //     memcpy(f->rest, buff, title);
    //     f->rest += title;
    //     f->write_stat = 1;
    // }
    if(f->write_stat == 1)
    {
        io300_flush(f);
        f->write_stat = 0;
    }
    f->file_head += CACHE_SIZE;
    f->rest = f->cache;
    f->cache_rest = pread(f->fd, f->cache, CACHE_SIZE,f->file_head);  
    // if (f->cache_rest <=0)
    // {
    //     return title;
    // }

    // use a while loop to read repeatly
    while((sz - title) >= CACHE_SIZE)
    {
        memcpy(f->rest , title + buff, CACHE_SIZE);
        f->write_stat = 1;
        f->rest += CACHE_SIZE;
        if(f->write_stat == 1)
        {
            io300_flush(f);
            f->write_stat = 0;
        }
        f->file_head += CACHE_SIZE;
        f->rest = f->cache;
        f->cache_rest = pread(f->fd, f->cache, CACHE_SIZE,f->file_head); 
        title += CACHE_SIZE;
        // if(f->cache_rest <= 0)
        // {
        //     return title;
        // }
    }
    //tail
    size_t tail = sz - title;
    if (tail <= f->cache_rest)
    {
        memcpy(f->rest, buff + title, tail);
        f->cache_rest -= tail;
        f->rest += tail;
        f->write_stat = 1;
    }
    else
    {
        memcpy(f->rest, buff + title, tail);
        // size_t temp = f->cache_rest;
        // f->rest += temp;
        f->rest += tail;
        f->cache_rest = 0;
        f->write_stat = 1;
    }
    return sz;

    }

    // return write(f->fd, buff, sz);
}

int io300_flush(struct io300_file *const f) {
    check_invariants(f);
    // TODO: Implement this
    //rewrite : head, rest, cache_rest, write_stat
    // size_t r = write(f->fd, f->rest, CACHE_SIZE);

    // if(f->write_stat == 1) {
    //     size_t rs = write(f->fd, f->cache, (int)(f->rest - f->cache));
    //     f->write_stat = 0;
    //     return rs;
    // }
    // return 0;

    size_t rs = pwrite(f->fd, f->cache, (int)(f->rest - f->cache), f->file_head);
    return rs;

    // f->write_stat = 0;
    // f->file_head += CACHE_SIZE;
    // f->rest = f->cache;
    // f->cache_rest = read(f->fd, f->cache, CACHE_SIZE);

    
}
