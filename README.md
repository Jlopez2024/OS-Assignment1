# OS Assignment1
First assignment of Operating Systems course

The compiled code is already included so "./producer & ./consumer &" can be executed.

If the code needs recompiled, run the code specified in the assignment doc:

    gcc producer.c -pthread -lrt -o producer
    gcc consumer.c -pthread -lrt -o consumer

Once the code runs, the producer will start with a data buffer containing 0, 1. After each critical section modification that the producer performs, the data within the buffer will be incremented. This makes it easier to see how the consumer accesses the critical section to print the data within shared memory. I set the loop to run for 10 iterations because it seems that ctrl-c can't kill processes that manipulate shared memory. After the loops have finished, hit enter and the process will end and the terminal will allow for input again.

Unexpected behavior:

For the most part, everything runs as intended. The only strange bug that I was unable to fix was where the terminal prints out a new terminal/command line (For lack of a better word). Here is an example:

    Producer copied data
    Received integers: 0 1
    Producer copied data
    @Jlopez2024 ➜ /workspaces/OS-Assignment1 (main) $ Received integers: 1 2
    Producer copied data
    Received integers: 2 3
    Producer copied data
    Received integers: 3 4

It doesn't seem to effect anything, but it is a bit annoying to look at.

