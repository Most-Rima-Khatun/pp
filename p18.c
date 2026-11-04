#include<stdio.h>
#include<mpi.h>

int main(int argc , char **argv){
    int rank, size, local_hist[10] = {0}, global_hist[10];

    int data[4];
    int samples[4][4] ={{1, 2, 2, 3},
                        {2, 3, 4, 4},
                        {1, 4, 5, 5},
                        {0, 2, 5, 9}};

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(size != 4){
        if(rank == 0)
            printf("Run with exactly 4 processes.\n");
        MPI_Finalize();
        return 0;
    }

    for(int i = 0; i < 4; i++)
        data[i] = samples[rank][i];
    for(int i = 0; i < 4; i++)
        local_hist[data[i]]++;

    MPI_Allreduce(local_hist, global_hist,10, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    if(rank == 0){
        printf("value frequency \n");
        for(int i = 0; i < 10; i++)
            printf("%5d %9d\n", i, global_hist[i]);
        printf("total elements = %d\n", sizeof(global_hist) / 2);
    }
    MPI_Finalize();
    return 0;
}