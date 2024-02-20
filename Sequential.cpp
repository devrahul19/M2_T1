#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>

using namespace std;
using namespace std::chrono;

const int N = 50; // matrix size

void intializing_matrix(int matrix[N][N])
{
    // initialize matrix with random values
    srand(time(NULL));
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            matrix[i][j] = rand() % 10;
        }
    }
}

void multiply_matrix(int x[N][N], int y[N][N], int z[N][N])
{
    // perform matrix multiplication
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            z[i][j] = 0;
            for (int k = 0; k < N; k++)
            {
                z[i][j] += x[i][k] * y[k][j];
            }
        }
    }
}

int main()
{
    int X[N][N], Y[N][N], Z[N][N];

    // initialize matrices A and B with random values
    intializing_matrix(X);
    intializing_matrix(Y);

    // calculate execution time
    auto start_time = high_resolution_clock::now();
    // perform matrix multiplication
    multiply_matrix(X, Y, Z);
    auto stop_time = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(stop_time - start_time);

    // print execution time
    cout << "Execution time: " << duration.count() << " nanoseconds\n";

    return 0;
}