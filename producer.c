#include <string.h>
#include <ctype.h>
#include "shmembuf.h"
#include <string.h>

int main() {
    int            fd;
    char           *shmpath = "/test"; // Use the same fixed shared memory path
    struct shmbuf  *shmp;
    int            data[2] = {0, 1}; // Example data to be sent
    size_t         len = sizeof(data); // Calculate the size of the data array

    // Create shared memory object and set its size to the size of our structure.
    fd = shm_open(shmpath, O_CREAT | O_EXCL | O_RDWR, 0600);
    if (fd == -1)
        errExit("shm_open");

    if (ftruncate(fd, sizeof(struct shmbuf)) == -1)
        errExit("ftruncate");

    // Map the object into the caller's address space.
    shmp = mmap(NULL, sizeof(*shmp), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    
    if (shmp == MAP_FAILED)
        errExit("mmap");

    // Initialize semaphores as process-shared, with value 0.
    if (sem_init(&shmp->sem1, 1, 0) == -1)
        errExit("sem_init-sem1");
    if (sem_init(&shmp->sem2, 1, 0) == -1)
        errExit("sem_init-sem2");

    for (int i = 0; i < 10; i++) {
        // Copy data into the shared memory object.
        memcpy(&shmp->buf, data, len);
	    printf("Producer copied data\n");

        // Increment sample data
        data[0]++;
        data[1]++;
	
        // Tell consumer that it can now access shared memory.
        if (sem_post(&shmp->sem2) == -1)
            errExit("sem_post");

        // Wait until consumer says that it has finished accessing the shared memory.
        if (sem_wait(&shmp->sem1) == -1)
            errExit("sem_wait");
        
    }

    // Unlink the shared memory object.
    if (shm_unlink(shmpath) == -1) {
      printf("Producer: Shared memory unlinking...\n");
      errExit("shm_unlink");
    }

    exit(EXIT_SUCCESS);
}
