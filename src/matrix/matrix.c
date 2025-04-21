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
    char res = 1;   // Флаг успешности выполнения

    // Проверка корректности размеров
    if (rows <= 0 || cols <= 0) res = 0;
    else {
        mat.rows = rows;
        mat.cols = cols;
        mat.data = (MATRIX_TYPE**) malloc (rows * sizeof (MATRIX_TYPE*));

        if (mat.data == NULL) res = 0;   // Ошибка выделения
        else {
            // Выделение памяти под каждую строку
            for (int row = 0; row < rows && res; row++) {
                mat.data[row] = (MATRIX_TYPE*) malloc (cols * sizeof (MATRIX_TYPE));
                if (mat.data[row] == NULL) res = 0;   // Ошибка выделения памяти
            }
        }
    }

    if (!res) {   // Освобождаем память в случае ошибки
        if (mat.data != NULL) {
            for (int row = 0; row < mat.rows; row++) {
                if (mat.data[row] != NULL) free (mat.data[row]);
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
    char res = 1;   // Флаг успешности выполнения

    // Загрузка данных из файла через функцию из output.c
    data = output_load_matrix_from_file (&rows, &cols, filename);
    if (!data) res = 0;   // Ошибка загрузки
    if (res) {
        mat = create_matrix (rows, cols);
        if (mat.data == NULL) res = 0;   // Ошибка создания матрицы
    }

    if (res) {
        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < cols; col++) {
                mat.data[row][col] = data[row * cols + col];
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
    char    res  = 1;

    // Проверка входных данных
    if (!matrix || !matrix->data) res = 0;

    if (res) {
        data = malloc (matrix->rows * matrix->cols * sizeof (double));
        if (!data) res = 0;
    }

    if (res) {
        for (int row = 0; row < matrix->rows; row++) {
            for (int col = 0; col < matrix->cols; col++) {
                data[row * matrix->cols + col] = matrix->data[row][col];
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
    char    res    = 1;

    // Проверка входных данных
    if (!matrix || !matrix->data) res = 0;

    if (res) {
        data = malloc (matrix->rows * matrix->cols * sizeof (double));
        if (!data) res = 0;
    }

    if (res) {
        for (int row = 0; row < matrix->rows; row++) {
            for (int col = 0; col < matrix->cols; col++) {
                data[row * matrix->cols + col] = matrix->data[row][col];
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
    char res        = -1;   // Флаг ошибок
    char rows_match = 0;    // Флаг совпадение числа строк
    char cols_match = 0;   // Флаг совпадения числа столбцов
    char pointers_valid = 0;   // Флаг для указателей

    // Проверка указателей
    pointers_valid = (A != NULL) && (B != NULL) && (result != NULL);

    if (!pointers_valid) res = -1;
    else {
        // Проверка размеров
        rows_match = (A->rows == B->rows);
        cols_match = (A->cols == B->cols);
        if (!rows_match || !cols_match) res = -1;
        else {
            // Выполнение сложения
            for (int row = 0; row < A->rows; row++) {
                for (int col = 0; col < A->cols; col++) {
                    result->data[row][col] = A->data[row][col] + B->data[row][col];
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
    char res            = -1;   // Флаг ошибок
    char rows_match     = 0;    // Флаг совпадения строк
    char cols_match     = 0;    // Флаг совпадения столбцов
    char pointers_valid = 0;    // Флаг для указателей

    // Проверка указателей
    pointers_valid = (A != NULL) && (B != NULL) && (result != NULL);
    if (!pointers_valid) res = -1;
    else {
        // Проверка размеров
        rows_match = (A->rows == B->rows);
        cols_match = (A->cols == B->cols);
        if (!rows_match || !cols_match) res = -1;
        else {
            // Выполнение вычитания
            for (int row = 0; row < A->rows; row++) {
                for (int col = 0; col < A->cols; col++) {
                    result->data[row][col] = A->data[row][col] - B->data[row][col];
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
    char res            = 1;   // Флаг ошибок
    char pointers_valid = (A != NULL) && (B != NULL) && (result != NULL);
    char size_compatible =
        pointers_valid ? (A->cols == B->rows) : 0;   // Флаг совместимости размеров

    if (!pointers_valid || !size_compatible) res = 1;
    else {
        for (int row = 0; row < A->rows; row++) {
            for (int col = 0; col < B->cols; col++) {
                MATRIX_TYPE sum = 0;
                for (int k = 0; k < A->cols; k++) {
                    sum += A->data[row][k] * B->data[k][col];
                }
                result->data[row][col] = sum;
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

    if (input_valid) {
        res = create_matrix (matrix->cols, matrix->rows);
        if (res.data != NULL) {
            for (int row = 0; row < matrix->rows; row++) {
                for (int col = 0; col < matrix->cols; col++) {
                    res.data[col][row] = matrix->data[row][col];
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
    char        is_square = 0;   // Флаг квадратности матрицы

    // Проверка входных данных
    is_square =
        (matrix != NULL) && (matrix->rows == matrix->cols) && (matrix->rows > 0);

    if (is_square) {
        // Основная логика вычисления
        const int n = matrix->rows;
        if (n == 1) det = matrix->data[0][0];
        else if (n == 2)
            det = matrix->data[0][0] * matrix->data[1][1] -
                  matrix->data[0][1] * matrix->data[1][0];
        else {
            for (int col = 0; col < n; col++) {
                Matrix submat = create_matrix (n - 1, n - 1);
                if (submat.data != NULL) {
                    // Заполнение подматрицы
                    for (int row = 1; row < n; row++) {
                        int subcol_index = 0;
                        for (int k = 0; k < n; k++) {
                            if (k != col) {
                                submat.data[row - 1][subcol_index++] =
                                    matrix->data[row][k];
                            }
                        }
                    }

                    // Рекурсивный вызов
                    MATRIX_TYPE sub_det = determinant (&submat);
                    det += (col % 2 == 0 ? 1 : -1) * matrix->data[0][col] * sub_det;
                    free_matrix (&submat);
                }
            }
        }
    }

    return det;
}
