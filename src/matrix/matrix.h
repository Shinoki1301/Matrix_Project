/**
 * @file matrix.h
 * @brief Основные операции с матрицами
 *
 * Этот файл содержит объявление структур и функций для работы с матрицами:
 * - Создание/удаление матрицы
 * - Базовые операции (сложение, вычитание, умножение, деление)
 * - Транспонирование и вычисление детерминанта матрицы
 * - Освобождение памяти
 * - Вывод матрицы в консоль
 * - Чтение, запись и копирование матрицы из файла
 * - Копирование матрицы
 *
 * @author Shinoki1301
 * @date 29.03.2025
 * @version 1.0
 */

#ifndef MATRIX_H
#define MATRIX_H

#include "../../include/config.h"

#include <stdio.h>
#include <stdlib.h>

/**
 * @struct Matrix
 * @brief Структура, представляющая матрицы
 */
typedef struct {
    int           rows;   ///< Количество строк
    int           cols;   ///< Количество столбцов
    MATRIX_TYPE** data;   ///< Двумерный массив данных
} Matrix;

/**
 * @brief Создает новую матрицу с заданными размерами
 * @param rows Количество строк
 * @param cols Количество столбцов
 * @return Структура Matrix при успехе или нулевая матрица при ошибке
 */
Matrix create_matrix (int rows, int cols);

/**
 * @brief Освобождает память, выделенную под матрицу
 * @param matrix Указатель на матрицу
 */
void free_matrix (Matrix* matrix);

/**
 * @brief Загружает матрицу из текстового файла
 * @param filename Имя файла
 * @return Загруженную матрицу или нулевую матицу в случае ошибки
 */
Matrix load_matrix_from_file (const char* filename);

/**
 * @brief Выводит матрицу в консоль
 * @param matrix Указатель на матрицу для вывода
 */
void print_matrix (const Matrix* matrix);

/**
 * @brief Сохраняет матрицу в текстовый файл
 * @param matrix Указатель на матрицу
 * @param filename Имя файла
 * @return 0 в случае успеха, -1 в случае ошибки
 */
int save_matrix_to_file (const Matrix* matrix, const char* filename);

/**
 * @brief Складывает две матрицы
 * @param A Указатель на первую матрицу
 * @param B Указатель на вторую матрицу
 * @param result Результирующая матрица
 * @return 0 при успехе, -1 при ошибке
 */
int add_matrices (const Matrix* A, const Matrix* B, Matrix* result);

/**
 * @brief Вычитает две матрицы
 * @param A Указатель на первую матрицу
 * @param B Указатель на вторую матрицу
 * @param result Результирующая матрица
 * @return 0 при успехе, -1 при ошибке
 */
int subtract_matrices (const Matrix* A, const Matrix* B, Matrix* result);

/**
 * @brief Умножает две матрицы
 * @param A Указатель на первую матрицу
 * @param B Указатель на вторую матрицу
 * @param result Выводная матрица
 * @return 0 при успехе, 1 при ошибке
 */
int multiply_matrices (const Matrix* A, const Matrix* B, Matrix* result);

/**
 * @brief Транспонирует матрицу
 * @param matrix Указатель на матрицу
 * @return Транспонированная матрица или нулевая матрица при ошибке
 */
Matrix transpose_matrix (const Matrix* matrix);

/**
 * @brief Вычисляет детерминант квадратной матрицы
 * @param matrix Указатель на квадратную матрицу
 * @note Использует рекурсивный алгоритм
 * @return Значение детерминанта матрицы или 0 при ошибке
 */
MATRIX_TYPE determinant (const Matrix* matrix);

#endif   // MATRIX_H
