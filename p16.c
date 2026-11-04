#include<stdio.h>
#include<mpi.h>

int main(int argc , char ** argv){
    int rank,size, local,manual = 0, scan_sum;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    local = (rank + 1) * 10;
    MPI_Scan(&local, &scan_sum, 1, MPI_INT,MPI_SUM, MPI_COMM_WORLD);
    printf("p%d : local = %d, MPI_scan sum = %d\n",rank, local,scan_sum);

    if(rank == 0){
        int prefix = local;
        printf("P0 Manual Prefix = %d",prefix);

        for(int i = 1; i < size ; i++){
            int value;
            MPI_Recv(&value, 1, MPI_INT, i, 160, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            prefix += value;
            MPI_Send(&prefix, 1, MPI_INT, i, 161, MPI_COMM_WORLD);
        }
    }else{
        MPI_Send(&local, 1, MPI_INT, 0,  160,MPI_COMM_WORLD);
        MPI_Recv(&manual, 1, MPI_INT, 0, 161, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("P%d manual Prefix = %d\n",rank, manual);
    }
    MPI_Finalize();
    return 0;
}