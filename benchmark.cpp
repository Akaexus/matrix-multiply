#include <omp.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <iostream>

// #define MATRIX_SIZE 1000

float** generateMatrix(int rows, int cols, bool generate = true) {
    float** matrix = new float*[rows];
    #pragma omp parallel for shared(matrix, rows, cols, generate) default(none)
    for (int i = 0; i < rows; i++) {
        matrix[i] = new float[cols];
        if (generate) {
            for (int j = 0; j < cols; j++) {
                matrix[i][j] = (float) rand() / RAND_MAX;
            }
        }
    }
    return matrix;
}

void deleteMatrix(float** matrix, int rows, int cols) {
    #pragma omp parallel for shared(matrix, rows, cols) default(none)
    for(int i = 0; i < rows; i++) {
        delete matrix[i];
    }
    delete matrix;
}

int main() {
    int chunks[] = {10, 20, 25, 40, 50, 100, 200, 250, 400, 500, 1000};
    for (int r = 0; r < 11; r += 1) {
      omp_set_num_threads(4);
      // printf("chunk_size: %d\n", chunks[r]);
      for(int MATRIX_SIZE = 2000; MATRIX_SIZE <= 2000; MATRIX_SIZE += 400) {
      srand(time(NULL));
        float** a = generateMatrix(MATRIX_SIZE, MATRIX_SIZE);
        float** b = generateMatrix(MATRIX_SIZE, MATRIX_SIZE);
        float** c = generateMatrix(MATRIX_SIZE, MATRIX_SIZE, false);

        auto start_time = std::chrono::high_resolution_clock::now();
       //  // METODA 3 pętlowa
       // #pragma omp parallel for shared(a, b, c, MATRIX_SIZE) default (none)
       // for(int j = 0; j < MATRIX_SIZE; j++) {
       //     for (int i = 0; i < MATRIX_SIZE; i++) {
       //         float s = 0;
       //         for (int k = 0; k < MATRIX_SIZE; k++) {
       //             s += a[i][k] * b[k][j];
       //         }
       //         c[i][j] = s;
       //     }
       // }
       auto end_time = std::chrono::high_resolution_clock::now();
       auto time = end_time - start_time;
       std::cout << chunks[r] << " " << time/std::chrono::milliseconds(1);
       // METODA 6 pętlowa
       start_time = std::chrono::high_resolution_clock::now();
        int chunk = chunks[r];
        #pragma omp parallel for shared(a, b, c, chunk, MATRIX_SIZE) default (none)
        for(int i = 0; i < MATRIX_SIZE; i += chunk) {
            for (int j = 0; j < MATRIX_SIZE; j += chunk) {
                for (int k = 0; k < MATRIX_SIZE; k += chunk) {
                    for (int ii = i; ii < i + chunk; ii++) {
                        for (int kk = k; kk < k + chunk; kk++) {
                            for (int jj = j; jj < j + chunk; jj++) {
                                c[ii][jj] += a[ii][kk] * b[kk][jj];
                            }
                        }
                    }
                }
            }
        }
        end_time = std::chrono::high_resolution_clock::now();
        time = end_time - start_time;
        std::cout << " " << time/std::chrono::milliseconds(1) << "\n";

        deleteMatrix(a, MATRIX_SIZE, MATRIX_SIZE);
        deleteMatrix(b, MATRIX_SIZE, MATRIX_SIZE);
        deleteMatrix(c, MATRIX_SIZE, MATRIX_SIZE);
      }
    }

    return 0;
}
