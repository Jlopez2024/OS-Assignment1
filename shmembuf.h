#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
                        } while (0)

#define BUF_SIZE 1024   // Maximum size for exchanged string

// Define a structure that will be imposed on the shared memory object

struct shmbuf {
    sem_t  sem1;
    sem_t  sem2;            
    size_t count;             // Number of bytes used in 'buf'
    int    buf[2];            // Data being transferred - modified to store integers
};
