#include <stdio.h>
#include <mpi.h>

double f(double x) { return x * x; }

int main(int argc, char **argv)
{
    int rank, size, n = 1000;
    double a = 0.0, b = 1.0, h, local = 0.0, total;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    h = (b - a) / n;

    for (int i = rank + 1; i < n; i += size)
        local += f(a + i * h);
    local = local * 2.0 + (rank == 0 ? f(a) : 0.0) + (rank == 0 ? f(b) : 0.0);

    /* The endpoint terms must be counted once; interior terms are split by rank. */
    double interior = 0.0;
    for (int i = rank + 1; i < n; i += size)
        interior += f(a + i * h);
    double partial = 2.0 * interior;
    if (rank == 0)
        partial += f(a) + f(b);
    partial *= h / 2.0;

    printf("P%d partial integral = %f\n", rank, partial);
    MPI_Reduce(&partial, &total, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank == 0)
    {
        double exact = 1.0 / 3.0;
        printf("Numerical integral = %f\n", total);
        printf("Exact integral = %f\n", exact);
        printf("Absolute error = %.6f\n", (total > exact) ? total - exact : exact - total);
    }
    MPI_Finalize();
    return 0;
}
