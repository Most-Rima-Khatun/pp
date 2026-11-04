#include <stdio.h>
#include <mpi.h>

double f(double x)
{
    return x * x;
}
int main(int argc, char **argv)
{
    int rank, size;
    int n = 1000;
    double a = 0.0, b = 1.0, h;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    h = (b - a) / n;

    double temp = 0.0, result;

    for (int i = rank + 1; i < n; i += size)
    {
        temp += f(a + i * h); // f(a+xh)
    }
    // 2*integral(f(x))[0,1]+ f(a)+f(b)
    temp = temp * 2.0 + (rank == 0 ? f(a) : 0.0) + (rank == 0 ? f(b) : 0.0);

    double interior = 0.0;

    for (int i = rank + 1; i < n; i += size)
    {
        interior += f(a + i * h);
    }
    double partial = 2.0 * interior;

    if (rank == 0)
    {
        partial += f(a) + f(b);
    }
    partial *= h / 2.0;

    printf("P%d partial integral = %f\n", rank, partial);
    MPI_Reduce(&partial, &result, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        double exact = 1.0 / 3.0;
        printf("Numerial integral = %f\n", result);
        printf("Exact integral = %f\n", exact);
        printf("Absolute error = %.6f\n", (result > exact) ? result - exact : exact - result);
    }
    MPI_Finalize();
    return 0;
}