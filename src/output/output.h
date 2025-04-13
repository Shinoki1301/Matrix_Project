/**
 * @file output.h
 * @brief Функции ввода и вывода для матриц
 *
 * @details
 * Модуль  отвечает за все операции ввода и вывода матрицы:
 * - Вывод матрицы в консоль
 * - Сохранение матрицы в файл
 * - Загрузка матрицы из текстового файла
 *
 * Формат файла:
 * Первые два числа - размеры матрицы (rows cols)
 * Затем идут элементы построчно
 *
 * @note Все функции проверяют корректность входных данных
 *
 * @see matrix.h
 */

#ifndef OUTPUT_H
#define OUTPUT_H

/**
 * @brief Выводит матрицу в консоль
 * @param rows Количество строк
 * @param cols Количество столбцов
 * @param data Указатель на массив
 */
void output_print_matrix (int rows, int cols, const double* data);

/**
 * @brief Сохраняет матрицу в файл
 * @param rows Количество строк
 * @param cols Количество столбцов
 * @param data Указатель на массив
 * @param filename Указатель на файл для сохранения матрицы
 * @return 0 при успехе, -1 при ошибке
 */
int output_save_matrix_to_file (int rows, int cols, const double* data,
                                const char* filename);

/**
 * @brief Загружает матрицу из файла
 * @param rows Количество строк
 * @param cols Количество столбцов
 * @param filename Указатель на файл для чтения матрицы
 * @return Указатель на созданную матрицу или NULL в случае ошибки
 */
double* output_load_matrix_from_file (int* rows, int* cols, const char* filename);

#endif   // OUTPUT_H
