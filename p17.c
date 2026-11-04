#include <stdio.h>
#include<stdlib.h>
#include <mpi.h>

int main(int argc, char **argv)
{
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int *sendbuf = (int *)malloc((size_t)size * sizeof(int));
    int *recvbuf = (int *)malloc((size_t)size * sizeof(int));

    if (!sendbuf || !recvbuf)
        MPI_Abort(MPI_COMM_WORLD, 1);

    for (int dest = 0; dest < size; dest++)
    {
        sendbuf[dest] = rank * 10 + dest;
    }
    MPI_Alltoall(sendbuf, 1, MPI_INT, recvbuf, 1, MPI_INT, MPI_COMM_WORLD);

    printf("P%d received :", rank);
    for (int i = 0; i < size; i++)
    {
        printf(" %d", recvbuf[i]);
    }
    printf("\n");
    free(sendbuf);
    free(recvbuf);
    MPI_Finalize();
    return 0;
}