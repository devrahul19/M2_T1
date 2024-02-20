#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <fstream>
#include <pthread.h>

using namespace std::chrono;

const int MATRIX_SIZE = 400;
#define MAX_THREADS 16

int matrix1[MATRIX_SIZE][MATRIX_SIZE];
int matrix2[MATRIX_SIZE][MATRIX_SIZE];
int matrix3[MATRIX_SIZE][MATRIX_SIZE];
int thread_num = 0;
int partition_size = MATRIX_SIZE / MAX_THREADS;

void fillRandomMatrix(int matrix[][MATRIX_SIZE]) {
    for (int i = 0; i < MATRIX_SIZE; ++i) {
        for (int j = 0; j < MATRIX_SIZE; ++j) {
            matrix[i][j] = rand() % 100;
        }
    }
}

void *multiplyMatrix(void *args) {
    int thread = thread_num++;

    for (int i = thread * partition_size; i < (thread + 1) * partition_size; ++i) {
        for (int j = 0; j < MATRIX_SIZE; ++j) {
            matrix3[i][j] = 0;
            for (int k = 0; k < MATRIX_SIZE; ++k) {
                matrix3[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
    return NULL;
}

int main() {
    srand(time(0));

    fillRandomMatrix(matrix1);
    fillRandomMatrix(matrix2);

    auto start = high_resolution_clock::now();

    pthread_t matrix_thread[MAX_THREADS];

    for (int i = 0; i < MAX_THREADS; ++i) {
        pthread_create(&matrix_thread[i], NULL, &multiplyMatrix, NULL);
    }

    for (int i = 0; i < MAX_THREADS; ++i) {
        pthread_join(matrix_thread[i], NULL);
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    printf("Execution time: %d microseconds", duration.count());

    std::ofstream outputFile("PThread_Results.txt");
    for (int i = 0; i < MATRIX_SIZE; ++i) {
        for (int j = 0; j < MATRIX_SIZE; ++j) {
            outputFile << matrix3[i][j] << "\t";
        }
        outputFile << std::endl;
    }
    outputFile << "Execution time: " << duration.count() << " microseconds";
    outputFile.close();

    return 0;
}