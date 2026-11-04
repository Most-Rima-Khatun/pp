#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv)
{
    int rank, size;

    int items[8] = {10, 20, 30, 40, 50, 60, 70, 80};

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 3)
    {
        if (rank == 0)
        {
            printf("Run with exactly 3 processes.\n");
        }
        MPI_Finalize();
        return 0;
    }

    if (rank == 0)
    {
        for (int i = 0; i < 8; i++)
        {
            int dest = (i % 2 == 0) ? 1 : 2;
            MPI_Send(&items[i], 1, MPI_INT, dest, 10, MPI_COMM_WORLD);
            printf("p%d produced : %d\n", rank, items[i]);
        }
    }
    else
    {
        int count = 4;
        for (int i = 0; i < count; i++)
        {
            int value;
            MPI_Recv(&value, 1, MPI_INT, 0, 10, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("p%d consumed : %d\n", rank, value);
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0)
    {
        printf("All 8 items processed successfully.\n");
    }
    MPI_Finalize();
    return 0;
}