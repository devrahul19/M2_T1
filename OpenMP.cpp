#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <omp.h>

#define N 50


using namespace std;

class Timer
{
public:
    Timer() : beg_(clock_::now()) {}
    void reset() { beg_ = clock_::now(); }
    double elapsed() const
    {
        return chrono::duration_cast<second_>(clock_::now() - beg_).count();
    }

private:
    typedef chrono::high_resolution_clock clock_;
    typedef chrono::duration<double, ratio<1>> second_;
    chrono::time_point<clock_> beg_;
};

// Populates the given matrix with random numbers.
void populateMatrix(long x[][N])
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            x[i][j] = rand() % 100;
        }
    }
}

// Multiplies row x with column col from y, returning the result.
long multiplyRowCol(long x[], long y[][N], long col)
{
    long result = 0;

    for (int i = 0; i < N; i++)
    {
        result += (x[i] * y[i][col]);
    }

    return result;
}

// Multiplies matrices x and y, storing the result in z.
void multiplyMatrices(long x[][N], long y[][N], long z[][N])
{
#pragma omp parallel for schedule(static)
    for (int row = 0; row < N; row++)
    {
        for (int col = 0; col < N; col++)
        {
            z[col][row] = multiplyRowCol(x[row], y, col);
        }
    }
}

// Writes the given matrix to file.
void persistToFile(string path, long m[][N])
{
    ofstream outfile;
    outfile.open("result.txt");

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            outfile << m[i][j] << " ";
        }

        outfile << endl;
    }

    outfile.close();
}

int main(int argc, char **argv)
{
    long x[N][N];
    long y[N][N];
    long z[N][N];

    populateMatrix(x);
    populateMatrix(y);

    Timer tmr;

    multiplyMatrices(x, y, z);

    double t = tmr.elapsed();

    persistToFile("result.txt", z);

    cout << "Elapsed time: " << (t * 1000) << endl;

    return 0;
}
