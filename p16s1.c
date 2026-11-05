
#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv) {
    int rank, size, local, scan_min, manual;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int values[] = {45, 30, 50, 20, 35};

    if (size != 5) {
        if (rank == 0)
            printf("Please run with 5 processes.\n");
        MPI_Finalize();
        return 0;
    }

    local = values[rank];

    // Built-in prefix minimum
    MPI_Scan(&local, &scan_min, 1, MPI_INT,
             MPI_MIN, MPI_COMM_WORLD);

    printf("P%d: Local value = %d, Prefix min = %d\n",
           rank, local, scan_min);

    // Manual prefix minimum using Send and Recv
    if (rank == 0) {
        int prefix = local;

        manual = prefix;
        printf("P0 Manual Prefix = %d\n", manual);

        for (int i = 1; i < size; i++) {
            int value;

            MPI_Recv(&value, 1, MPI_INT, i, 160,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            prefix = (prefix < value) ? prefix : value;

            MPI_Send(&prefix, 1, MPI_INT, i, 161,
                     MPI_COMM_WORLD);
        }
    }
    else {
        MPI_Send(&local, 1, MPI_INT, 0, 160,
                 MPI_COMM_WORLD);

        MPI_Recv(&manual, 1, MPI_INT, 0, 161,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        printf("P%d Manual Prefix = %d\n", rank, manual);
    }

    MPI_Finalize();
    return 0;
}
