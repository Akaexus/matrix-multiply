#define MATRIX_SIZE 10000

#include <omp.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>

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
    srand(time(NULL));
    if (MATRIX_SIZE != MATRIX_SIZE) {
        printf("Rozmiary macierzy nie są właściwe!\n");
        return 1;
    }

    float** a = generateMatrix(MATRIX_SIZE, MATRIX_SIZE);
    float** b = generateMatrix(MATRIX_SIZE, MATRIX_SIZE);
    float** c = generateMatrix(MATRIX_SIZE, MATRIX_SIZE, false);

    // METODA 3 pętlowa
//    #pragma omp parallel for shared(a, b, c) default (none)
//    for(int j = 0; j < MATRIX_SIZE; j++) {
//        for (int i = 0; i < MATRIX_SIZE; i++) {
//            float s = 0;
//            for (int k = 0; k < MATRIX_SIZE; k++) {
//                s += a[i][k] * b[k][j];
//            }
//            c[i][j] = s;
//        }
//    }

//    // METODA 6 pętlowa
    int chunk = 100;
    #pragma omp parallel for shared(a, b, c, chunk) default (none)
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

    deleteMatrix(a, MATRIX_SIZE, MATRIX_SIZE);
    deleteMatrix(b, MATRIX_SIZE, MATRIX_SIZE);
    deleteMatrix(c, MATRIX_SIZE, MATRIX_SIZE);

    return 0;
}
