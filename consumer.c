#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "shmembuf.h"

int main() {
    int            fd;
    char           *shmpath = "/test"; // Use a fixed shared memory path
    struct shmbuf  *shmp;
    int            data[2]; // Define an array to store two integers
    size_t         len = sizeof(data); // Calculate the size of the data array

    // Open the existing shared memory object and map it into the caller's address space.
    fd = shm_open(shmpath, O_RDWR, 0);
    if (fd == -1)
        errExit("shm_open");

    shmp = mmap(NULL, sizeof(*shmp), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    
    if (shmp == MAP_FAILED)
        errExit("mmap");

    for (int i = 0; i < 10; i++) {
        // Wait for 'sem2' to be posted by peer before touching shared memory.
        if (sem_wait(&shmp->sem2) == -1)
            errExit("sem_wait");

        // Read data from shared memory into the data array.
        memcpy(data, &shmp->buf, len);

        // Print the integers stored in shared memory.
        printf("Received integers: %d %d\n", data[0], data[1]);

        // Post 'sem1' to tell the peer that it can now access the modified data in shared memory.
        if (sem_post(&shmp->sem1) == -1)
            errExit("sem_post");


        // Sleep to simulate some processing time.
        sleep(1);
    }

    // Unlink the shared memory object.
    if (shm_unlink(shmpath) == -1) {
      printf("Consumer: Shared memory unlinking...\nHit the enter key to exit.");
      errExit("shm_unlink");
    }

    exit(EXIT_SUCCESS);
}
