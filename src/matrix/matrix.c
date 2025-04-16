/**
 * @file matrix.c
 * @brief Реализация основных операций с матрицами
 *
 * @details
 * Этот файл содержит реализацию основных операций над матрицами,
 * объявленных в matrix.h:
 * - Создание/удаление матриц
 * - Базовые арифметические операции (сложение, вычитание, умножение,
 * деление)
 * - Транспонирование и вычисление детерминанта матрицы
 * - Освобождение памяти
 * - Вывод матрицы в консоль
 * - Чтение, запись и копирование матрицы из файла
 * - Копирование матрицы
 *
 * @note Все функции выполняют проверку входных параметров
 *
 * @see matrix.h
 */

#include "matrix.h"
#include "../errors/errors.h"

#include "../output/output.h"

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Создает матрицу заданного размера
 *
 * @param rows Количество строк (должно быть > 0)
 * @param cols Количетство столбцов (должно быть > 0)
 * @return Структура Matrix при успехе, нулевая матрица при ошибке
 */
Matrix create_matrix (int rows, int cols) {
    Matrix mat = {0, 0, NULL};   // Инициализация пустой матрицы
    int res = 1;   // Флаг успешности выполнения

    // Проверка корректности размеров
    if (rows <= 0 || cols <= 0) {
        res = 0;
        print_error (ERROR_INVALID_DIMENSIONS, "create_matrix");
    } else {
        mat.rows = rows;
        mat.cols = cols;
        mat.data = (MATRIX_TYPE**) malloc (rows * sizeof (MATRIX_TYPE*));

        if (mat.data == NULL) {
            res = 0;   // Ошибка выделения
            print_error (ERROR_MEMORY_ALLOCATION, "create_matrix");
        } else {
            // Выделение памяти под каждую строку
            for (int index_row = 0; index_row < rows && res; index_row++) {
                mat.data[index_row] =
                    (MATRIX_TYPE*) malloc (cols * sizeof (MATRIX_TYPE));
                if (mat.data[index_row] == NULL) {
                    res = 0;   // Ошибка выделения памяти
                    print_error (ERROR_MEMORY_ALLOCATION, "create_matrix");
                }
            }
        }
    }

    if (!res) {   // Освобождаем память в случае ошибки
        if (mat.data != NULL) {
            for (int index_row = 0; index_row < mat.rows; index_row++) {
                if (mat.data[index_row] != NULL) free (mat.data[index_row]);
            }
            free (mat.data);
        }
        mat.data = NULL;
        mat.cols = 0;
        mat.rows = 0;
    }

    return mat;
}

/**
 * @brief Освобождает память занятую матрицей
 *
 * @param matrix Указатель на Matrix
 */
void free_matrix (Matrix* matrix) {
    if (matrix->data != NULL && matrix != NULL) {
        for (int index_row = 0; index_row < matrix->rows; index_row++) {
            free (matrix->data[index_row]);
        }
        free (matrix->data);
        matrix->data = NULL;
        matrix->rows = 0;
        matrix->cols = 0;
    }
}

/**
 * @brief Загружает матрицу из файла
 *
 * @param filename Путь к файлу с матрицей
 *
 * @return Загруженную матрицу или нулевую матрицу при ошибке
 */
Matrix load_matrix_from_file (const char* filename) {
    int     rows, cols;
    double* data = NULL;
    Matrix mat = {0, 0, NULL};   // Инициализация пустой матрицы
    int res = 1;   // Флаг успешности выполнения

    // Загрузка данных из файла через функцию из output.c
    data = output_load_matrix_from_file (&rows, &cols, filename);
    if (!data) {
        res = 0;   // Ошибка загрузки
        print_error (ERROR_FILE_IO, "load_matrix_from_file");
    }

    if (res) {
        mat = create_matrix (rows, cols);
        if (mat.data == NULL) {
            res = 0;   // Ошибка создания матрицы
            print_error (ERROR_MEMORY_ALLOCATION, "load_matrix_from_file");
        }
    }

    if (res) {
        for (int index_row = 0; index_row < rows; index_row++) {
            for (int index_col = 0; index_col < cols; index_col++) {
                mat.data[index_row][index_col] = data[index_row * cols + index_col];
            }
        }
    }

    if (data) free (data);

    if (!res && mat.data != NULL) {
        free_matrix (&mat);
        mat.data = NULL;
        mat.rows = 0;
        mat.cols = 0;
    }

    return mat;
}

/**
 * @brief Выводит матрицу в консоль
 *
 * @param matrix Указатель на матрицу для вывода
 */
void print_matrix (const Matrix* matrix) {
    double* data = NULL;
    int     res  = 1;

    // Проверка входных данных
    if (!matrix || !matrix->data) {
        res = 0;
        print_error (ERROR_NULL_PTR, "print_matrix");
    }

    if (res) {
        data = malloc (matrix->rows * matrix->cols * sizeof (double));

        if (!data) {
            res = 0;
            print_error (ERROR_MEMORY_ALLOCATION, "print_matrix");
        }
    }

    if (res) {
        for (int index_row = 0; index_row < matrix->rows; index_row++) {
            for (int index_col = 0; index_col < matrix->cols; index_col++) {
                data[index_row * matrix->cols + index_col] =
                    matrix->data[index_row][index_col];
            }
        }
        // Вызов функции вывода
        output_print_matrix (matrix->rows, matrix->cols, data);
    }

    if (data) free (data);
}

/**
 * @brief Сохраняет матрицу в файл
 *
 * @param matrix Указатель на сохраняемую матрицу
 * @param filename Имя выходного файла
 *
 * @return Возвращает -1 при ошибке и 0 при успешной отработке функции
 */
int save_matrix_to_file (const Matrix* matrix, const char* filename) {
    double* data   = NULL;
    int     result = -1;
    int     res    = 1;

    // Проверка входных данных
    if (!matrix || !matrix->data) {
        res = 0;
        print_error (ERROR_NULL_PTR, "save_matrix_to_file");
    }

    if (res) {
        data = malloc (matrix->rows * matrix->cols * sizeof (double));

        if (!data) {
            res = 0;
            print_error (ERROR_MEMORY_ALLOCATION, "save_matrix_file");
        }
    }

    if (res) {
        for (int index_row = 0; index_row < matrix->rows; index_row++) {
            for (int index_col = 0; index_col < matrix->cols; index_col++) {
                data[index_row * matrix->cols + index_col] =
                    matrix->data[index_row][index_col];
            }
        }
        result =
            output_save_matrix_to_file (matrix->rows, matrix->cols, data, filename);
    }

    if (data) free (data);

    return result;
}

/**
 * @brief Складывает две матрицы
 *
 * Поэлементно складывает две матрицы одинакового размера.
 * Результат записывается в матрицу result.
 *
 * @param A Указатель на первую матрицу
 * @param B Указатель на вторую матрицу
 * @param result Результирующая матрица
 *
 * @return 0 при успехе, -1 при ошибке
 */
int add_matrices (const Matrix* A, const Matrix* B, Matrix* result) {
    int res        = -1;   // Флаг ошибок
    int rows_match = 0;    // Флаг совпадение числа строк
    int cols_match = 0;   // Флаг совпадения числа столбцов
    int pointers_valid = 0;   // Флаг для указателей

    // Проверка указателей
    pointers_valid = (A != NULL) && (B != NULL) && (result != NULL);

    if (!pointers_valid) print_error (ERROR_NULL_PTR, "add_matrices");
    else {
        // Проверка размеров
        rows_match = (A->rows == B->rows);
        cols_match = (A->cols == B->cols);
        if (!rows_match || !cols_match)
            print_error (ERROR_INVALID_DIMENSIONS, "add_matrices");
        else {
            // Выполнение сложения
            for (int index_row = 0; index_row < A->rows; index_row++) {
                for (int index_col = 0; index_col < A->cols; index_col++) {
                    result->data[index_row][index_col] =
                        A->data[index_row][index_col] +
                        B->data[index_row][index_col];
                }
            }
            res = 0;   // Успешное завершение
        }
    }

    return res;
}

/**
 * @brief Вычитает две матрицы
 *
 * Поэлементно вычитает матрицу В из матрицы матрицы А.
 * Результат записывается в матрицу result.
 *
 * @param A Указатель на первую матрицу
 * @param B Указатель на вторую матрицу
 * @param result Результирующая матрица
 *
 * @return 0 при успехе, -1 при ошибке
 */
int subtract_matrices (const Matrix* A, const Matrix* B, Matrix* result) {
    int res            = -1;   // Флаг ошибок
    int rows_match     = 0;    // Флаг совпадения строк
    int cols_match     = 0;    // Флаг совпадения столбцов
    int pointers_valid = 0;    // Флаг для указателей

    // Проверка указателей
    pointers_valid = (A != NULL) && (B != NULL) && (result != NULL);
    if (!pointers_valid) print_error (ERROR_NULL_PTR, "subtract_matrices");
    else {
        // Проверка размеров
        rows_match = (A->rows == B->rows);
        cols_match = (A->cols == B->cols);
        if (!rows_match || !cols_match)
            print_error (ERROR_INVALID_DIMENSIONS, "subtract_matrices");
        else {
            // Выполнение вычитания
            for (int index_row = 0; index_row < A->rows; index_row++) {
                for (int index_col = 0; index_col < A->cols; index_col++) {
                    result->data[index_row][index_col] =
                        A->data[index_row][index_col] -
                        B->data[index_row][index_col];
                }
            }
            res = 0;   // Успешное завершение
        }
    }

    return res;
}

/**
 * @brief Умножение двух матриц
 *
 * Выполняет матричное умножение A x B
 *
 * @param A Указатель на первую матрицу
 * @param B Указатель на вторую матрицу
 * @param result Результирующая матрица
 *
 * @note Число столбцов матрицы А, должно совпадать с числом строк матрицы В.
 *
 * @return 0 при успехе, 1 при ошибке
 */
int multiply_matrices (const Matrix* A, const Matrix* B, Matrix* result) {
    int res = 1;   // Флаг ошибок
    int pointers_valid = (A != NULL) && (B != NULL) && (result != NULL);
    int size_compatible =
        pointers_valid ? (A->cols == B->rows) : 0;   // Флаг совместимости размеров

    if (!pointers_valid) print_error (ERROR_NULL_PTR, "multiply_matrices");
    else if (!size_compatible) print_error (ERROR_SIZE_MATRIX, "multiply_matrices");
    else {
        for (int index_row = 0; index_row < A->rows; index_row++) {
            for (int index_col = 0; index_col < B->cols; index_col++) {
                MATRIX_TYPE sum = 0;
                for (int k = 0; k < A->cols; k++) {
                    sum += A->data[index_row][k] * B->data[k][index_col];
                }
                result->data[index_row][index_col] = sum;
            }
        }
        res = 0;
    }

return res;
}

/**
 * @brief Транспонирует матрицу
 *
 * Создает новую матрицу - транспонированную версию исходной.
 * Строки становятся столбцами и наоборот.
 *
 * @param matrix Указатель на матрицу
 *
 * @return Транспонированная матрица или нулевая матрица при ошибке
 */
Matrix transpose_matrix (const Matrix* matrix) {
    Matrix res         = {0};
    int    input_valid = 0;

    // Проверка входных данных
    input_valid = (matrix != NULL) && (matrix->rows > 0) && (matrix->cols > 0);

    if (!input_valid) print_error (ERROR_INVALID_DIMENSIONS, "transpose_matrix");
    else {
        res = create_matrix (matrix->cols, matrix->rows);
        if (res.data == NULL)
            print_error (ERROR_MEMORY_ALLOCATION, "transpose_matrix");
        else {
            for (int index_row = 0; index_row < matrix->rows; index_row++) {
                for (int index_col = 0; index_col < matrix->cols; index_col++) {
                    res.data[index_col][index_row] =
                        matrix->data[index_row][index_col];
                }
            }
        }
    }

    return res;
}

/**
 * @brief Вычисляет определитель матрицы
 *
 * Вычисляет определитель квадратной матрицы.
 *
 * @param matrix Указатель на квадратную матрицу
 *
 * @note Используется рекурсивный алгоритм разложения по первой строке
 *
 * @return 0 при ошибке или значение детерминанта
 */
MATRIX_TYPE determinant (const Matrix* matrix) {
    MATRIX_TYPE det = 0;   // Значение квадратной матрицы
    int         is_square = 0;   // Флаг квадратности матрицы

    // Проверка входных данных
    is_square =
        (matrix != NULL) && (matrix->rows == matrix->cols) && (matrix->rows > 0);

    if (!is_square) print_error (ERROR_MATRIX_NOT_SQUARE, "determinant");
    else {
        // Основная логика вычисления
        const int n = matrix->rows;
        if (n == 1) {
            det = matrix->data[0][0];
        } else if (n == 2) {
            det = matrix->data[0][0] * matrix->data[1][1] -
                  matrix->data[0][1] * matrix->data[1][0];
        } else {
            for (int index_col = 0; index_col < n; index_col++) {
                Matrix submat = create_matrix (n - 1, n - 1);
                if (submat.data == NULL)
                    print_error(ERROR_MEMORY_ALLOCATION, "determinant");
                else {
                    // Заполнение подматрицы
                    for (int index_row = 1; index_row < n; index_row++) {
                        int subcol_index = 0;
                        for (int k = 0; k < n; k++) {
                            if (k != index_col) {
                                submat.data[index_row - 1][subcol_index++] =
                                    matrix->data[index_row][k];
                            }
                        }
                    }

                    // Рекурсивный вызов
                    MATRIX_TYPE sub_det = determinant (&submat);
                    det += (index_col % 2 == 0 ? 1 : -1) *
                           matrix->data[0][index_col] * sub_det;
                    free_matrix (&submat);
                }
            }
        }
    }

    return det;
}

