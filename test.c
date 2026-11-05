
#include <stdio.h>
#include <mpi.h>

#define N 12
#define WORKERS 3
#define CHUNK (N / WORKERS)

int main(int argc, char *argv[])
{
    int rank, size;
    int arr[N] = {
        12, 45, 23, 67,
        34, 89, 56, 78,
        91, 25, 43, 60
    };

    int local[CHUNK];
    int local_max;
    int overall_max;
    int worker_max;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 4) {
        if (rank == 0)
            printf("Run with exactly 4 processes.\n");

        MPI_Finalize();
        return 0;
    }

    // Master process
    if (rank == 0) {

        printf("P0 distributed array elements to workers.\n\n");

        // Distribute 4 elements to each worker
        for (int i = 1; i <= WORKERS; i++) {
            MPI_Send(
                &arr[(i - 1) * CHUNK],
                CHUNK,
                MPI_INT,
                i,
                0,
                MPI_COMM_WORLD
            );
        }

        // Receive maximum from first worker
        MPI_Recv(
            &overall_max,
            1,
            MPI_INT,
            1,
            1,
            MPI_COMM_WORLD,
            MPI_STATUS_IGNORE
        );

        // Receive maximum from remaining workers
        for (int i = 2; i <= WORKERS; i++) {

            MPI_Recv(
                &worker_max,
                1,
                MPI_INT,
                i,
                1,
                MPI_COMM_WORLD,
                MPI_STATUS_IGNORE
            );

            if (worker_max > overall_max)
                overall_max = worker_max;
        }

        printf("\nP0 received maximum values from all workers.\n");
        printf("Overall maximum element = %d\n", overall_max);
    }

    // Worker processes
    else {

        // Receive assigned elements
        MPI_Recv(
            local,
            CHUNK,
            MPI_INT,
            0,
            0,
            MPI_COMM_WORLD,
            MPI_STATUS_IGNORE
        );

        // Find local maximum
        local_max = local[0];

        for (int i = 1; i < CHUNK; i++) {
            if (local[i] > local_max)
                local_max = local[i];
        }

        printf("P%d found maximum: %d\n",
               rank, local_max);

        // Send local maximum to master
        MPI_Send(
            &local_max,
            1,
            MPI_INT,
            0,
            1,
            MPI_COMM_WORLD
        );

        printf("P%d sent maximum: %d to P0\n",
               rank, local_max);
    }

    MPI_Finalize();

    return 0;
}
