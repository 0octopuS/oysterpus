# Message Passing Interface(MPI)

## 1. Introduction

🔘 **What is MPI?**

Programs pass messages through some processes in order to finish parallel tasks. In practice, it's easy to implement parallel tasks by MPI. For example manager process can send a message to describe a new job, and assign the job to the latter.

🔘 **Concepts in MPI**

- **communicator**: define a set of processes which can send message to each other
- **rank**: inside a communicator, each of the process has a number(called rank). Processes communicate **explicitly** by specifying ranks.
- **tag**: each message has its unique tag. 

## 2. Hello-world

!!! abstract "Take Away"
    - `MPI_Init(&argc, &argv)`: init the MPI environment. All the global variable and local variable will be created(communicator, ranks...).
    - `MPI_Comm_size(MPI_Comm communicator, int* size)`: get the size of communicator.
    - `MPI_Comm_rank()`: get the rank of current process.
    - `MPI_Get_processor_name()`: get name of processor.
    - `MPI_Finalize()`: clean MPI environment. After this no other MPI command can be used.
    - `MPI_COMM_WORLD`: MPI automatically generate communicator.
    - `mpicc -o my_mpi_exe test.c`: compile.
    - `mpiexec -n 4 ./my_mpi_exe`: run program with 4 processes.
    - 

### 2.1 Program

=== ":octicons-terminal-16: Code"

    ```c
    #include <stdio.h>
    #include <stdlib.h>
    #include <mpi.h>

    int main(int argc, char *argv[])
    {
        // TODO: say hello! in parallel
        MPI_Init(&argc,&argv);

        // Get the number of processes
        int n;
        MPI_Comm_size( MPI_COMM_WORLD , & n);

        // Get the rank of processes
        int rank;
        MPI_Comm_rank( MPI_COMM_WORLD ,& rank);

        char processor_name[MPI_MAX_PROCESSOR_NAME];
        int name_len;
        MPI_Get_processor_name( processor_name , &name_len);
        printf("Hello world from processor %s, rand %d out of %d processor\n", processor_name, rank, n);

        MPI_Finalize();
        return 0;
    }
    ```

=== ":octicons-code-review-16: Local Result"
    ```sh
    [xiezhong@puhti-login11 mpi]$ mpiexec -n 4 ./hello-world/hello_world.exe 
    MPI startup(): Warning: I_MPI_PMI_LIBRARY will be ignored since the hydra process manager was found
    MPI startup(): Warning: I_MPI_PMI_LIBRARY will be ignored since the hydra process manager was found
    MPI startup(): Warning: I_MPI_PMI_LIBRARY will be ignored since the hydra process manager was found
    MPI startup(): Warning: I_MPI_PMI_LIBRARY will be ignored since the hydra process manager was found
    Hello world from processor puhti-login11.bullx, rand 0 out of 4 processor
    Hello world from processor puhti-login11.bullx, rand 1 out of 4 processor
    Hello world from processor puhti-login11.bullx, rand 2 out of 4 processor
    Hello world from processor puhti-login11.bullx, rand 3 out of 4 processor
    ```
=== ":octicons-code-review-16: Puhti Result"
    ```sh
    [xiezhong@puhti-login11 mpi]$ squeue -u $USER
             JOBID PARTITION     NAME     USER ST       TIME  NODES NODELIST(REASON)
          20170858     small hello-wo xiezhong  R       0:11      1 r02c32
    ```
    Here is the result in file `slurm-20170858.out`
    ```tex
    Hello world from processor r02c32.bullx, rand 1 out of 4 processor
    Hello world from processor r02c32.bullx, rand 2 out of 4 processor
    Hello world from processor r02c32.bullx, rand 3 out of 4 processor
    Hello world from processor r02c32.bullx, rand 0 out of 4 processor
    ```

## 3. Message exchange

!!! abstract "Take Away"
    - `MPI_Barrier`: Synchronization between processes wait until everybody within the communicator reaches the call.
    - `MPI_Send(void* data, int count, MPI_Datatype datatype, int destination, int tag, MPI_Comm communicator)`: send `count` data of type `datatype`. The `destination` is the rank of receiver, and `tag` belongs to the message.
    - `MPI_Recv(void* data, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm communicator, MPI_Status* status)`: arguments are similar to `MPI_Send`, with an additional `status` specifying the status of whether the message is received successfully.
    - `MPI_Get_count( const MPI_Status *status, MPI_Datatype datatype, int *count )`: number of received elements.

### 3.1 Program

=== ":octicons-terminal-16: Code"
    ```c
    if (myid == 0) {
        MPI_Send(message, size, MPI_INT, 1,1,MPI_COMM_WORLD);
        MPI_Recv(receiveBuffer, size, MPI_INT, 1,2,MPI_COMM_WORLD, &status);
        int nrecv;
        MPI_Get_count(&status, MPI_INT, &nrecv);
        printf("Rank %i received %i elements, first %i\n", myid, nrecv, receiveBuffer[0]);
    } else if (myid == 1) {
        MPI_Send(message, size, MPI_INT, 0,2,MPI_COMM_WORLD);
        MPI_Recv(receiveBuffer, size, MPI_INT, 0,1,MPI_COMM_WORLD, &status);
        int nrecv;
        MPI_Get_count(&status, MPI_INT, &nrecv);
        printf("Rank %i received %i elements, first %i\n", myid, nrecv, receiveBuffer[0]);
    }
    ```
=== ":octicons-code-review-16: Puhti Result"
    ```sh
    Rank 0 received 100 elements, first 1
    Rank 1 received 100 elements, first 0
    ```

### 3.2 Question

!!! question "Try increasing the message size (e.g. to 100000), recompile and run. What happens?"
    Get the result of time limit
    ```sh
    slurmstepd: error: *** STEP 20173362.0 ON r07c20 CANCELLED AT 2024-01-22T12:46:27 DUE TO TIME LIMIT ***
    ```
!!! question "What if you reorder the send and receive calls in one of the processes?"
    The program still run and get the correct answer.
    ```sh
    Rank 0 received 100 elements, first 1
    Rank 1 received 100 elements, first 0
    ```

## 4. Message Chain

!!! abstract "Take Away"
    -  `MPI_Sendrecv(const void *sendbuf, int sendcount, MPI_Datatype sendtype, int dest, int sendtag, void *recvbuf, int recvcount, MPI_Datatype recvtype, int source, int recvtag, MPI_Comm comm, MPI_Status * status)` This function wrap `send/recv`, but the communicator must be the same.

### 4.1 Program

=== ":octicons-terminal-16: Send & Recv"
    - Every sender sends message with tag `tag+1` to `rank+1` receiver
    - Every receiver gets message with tag `tag` from `rank-1` sender
    ```c
    destination = myid < ntasks - 1 ? myid + 1 : MPI_PROC_NULL;
    source = myid > 0 ? myid - 1 : MPI_PROC_NULL;
    MPI_Send(message.data(), size, MPI_INT, destination, myid+1, MPI_COMM_WORLD);
    printf("Sender: %d. Sent elements: %d. Tag: %d. Receiver: %d\n",
           myid, size, myid + 1, destination);
    MPI_Recv(receiveBuffer.data(), size, MPI_INT, source, myid, MPI_COMM_WORLD,&status);
    printf("Receiver: %d. first element %d.\n",
           myid, receiveBuffer[0]);
    ```
=== ":octicons-terminal-16: Sendrecv"
    - Use `Sendrecv` function can wrap two function together.
    ```c
    destination = myid + 1; // ignore edge cases 
    source = myid - 1;
    MPI_Sendrecv(message, size, MPI_INT, destination, myid + 1,
                receiveBuffer, size, MPI_INT, source, MPI_ANY_TAG,
                MPI_COMM_WORLD, &status);
    ```
=== ":octicons-code-review-16: Puhti Result"
    ```sh
    Sender: 0. Sent elements: 10000000. Tag: 1. Receiver: 1
    Receiver: 0. first element -1.
    Sender: 1. Sent elements: 10000000. Tag: 2. Receiver: 2
    Receiver: 1. first element 0.
    Sender: 2. Sent elements: 10000000. Tag: 3. Receiver: 3
    Receiver: 2. first element 1.
    Sender: 3. Sent elements: 10000000. Tag: 4. Receiver: -2
    Receiver: 3. first element 2.
    Time elapsed in rank  0:  0.023
    Time elapsed in rank  1:  0.023
    Time elapsed in rank  2:  0.015
    Time elapsed in rank  3:  0.008
    ```


### 4.2 question

!!! question "Investigate the timings with different numbers of MPI tasks (e.g. 2, 4, 8, 16, ...)"
    With `recv` and `send`:
      - 2 nodes -> Time elapsed in rank  0:  0.007
      - 4 nodes -> Time elapsed in rank  0:  0.023
      - 8 nodes -> Time elapsed in rank  0:  0.119
      - 16 nodes -> Time elapsed in rank  0:  0.177
      - Some of nodes always spend more time than others, time differs a lot.
    With `Sendrecv`:
      - 2 nodes -> TIME LIMIT?
      - 4 nodes -> Time elapsed in rank  0:  0.042
      - 8 nodes -> Time elapsed in rank  0:  0.086
      - 16 nodes -> Time elapsed in rank  0:  0.014
      - All the node spend almost same time.