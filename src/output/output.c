/**
 * @file output.c
 * @brief Реализация функций ввода и вывода матриц
 *
 * @details
 * Содержит реализацию операций с файлами и консольным выводом для
 * матричных операций
 */

#include "output.h"

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Функция для вывода матрицы
 *
 * @param rows Количество строк
 * @param cols Количество стоблцов
 * @param data Указатель на массив данных
 */
void output_print_matrix (int rows, int cols, const double* data) {
    if (!data) printf ("Данные матрицы отсутствуют.");
    else {
        printf ("Матрица %dx%d:\n", rows, cols);
        for (int index_row = 0; index_row < rows; index_row++) {
            for (int index_col = 0; index_col < cols; index_col++) {
                printf ("%.2f ", data[index_row * cols + index_col]);
            }
            printf ("\n");
        }
    }
}

/**
 * @brief Функция сохранения матрицы в файл
 *
 * @param rows Количество строк
 * @param cols Количество столбцов
 * @param data Указатель на массив данных
 * @param filename Указатель куда нужно сохранить данные
 *
 * @return NULL при ошибке или указатель на созданную матрицу
 */
int output_save_matrix_to_file (int rows, int cols, const double* data,
                                const char* filename) {
    int   result = -1;
    FILE* file   = NULL;

    if (data) {
        file = fopen (filename, "w");
        if (file) {
            fprintf (file, "%d %d\n", rows, cols);
            for (int index_row = 0; index_row < rows; index_row++) {
                for (int index_col = 0; index_col < cols; index_col++) {
                    fprintf (file, "%.2f ", data[index_row * cols + index_col]);
                }
                fprintf (file, "\n");
            }
            result = 0;
        } else {
            fprintf (stderr, "Ошибка открытия файла.\n");
        }
    } else {
        printf ("Данные матрицы отсутствуют.\n");
    }

    if (file) fclose (file);

    return result;
}

/**
 * @brief Загружает матрицу из файла
 *
 * @param rows Количество строк
 * @param cols Количество столбцов
 * @param filename Указатель куда нужно сохранить данные
 * @return NULL при ошибке или указатель на созданную матрицу
 */
double* output_load_matrix_from_file (int* rows, int* cols, const char* filename) {
    FILE*   file = NULL;
    double* data = NULL;
    int     res  = 1;

    file = fopen (filename, "r");
    if (!file) {
        fprintf (stderr, "Ошибка чтения файла.\n");
        res = 0;
    }

    if (res) {
        if (fscanf (file, "%d %d", rows, cols) != 2) {
            fprintf (stderr, "Ошибка чтения размеров матрицы.\n");
            res = 0;
        }
    }

    if (res) {
        data = (double*) malloc ((*rows) * (*cols) * sizeof (double));
        if (!data) res = 0;
    }

    if (res) {
        for (int index_row = 0; index_row < *rows && res; index_row++) {
            for (int index_col = 0; index_col < *cols && res; index_col++) {
                if (fscanf (file, "%lf", &data[index_row * (*cols) + index_col]) !=
                    1) {
                    fprintf (stderr, "Ошибка чтения элементов матрицы.\n");
                    res = 0;
                }
            }
        }
    }

    if (file) fclose (file);

    if (!res && data) {
        free (data);
        data = NULL;
    }

    return data;
}
