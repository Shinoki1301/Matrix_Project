/**
 * @file errors.c
 *
 * @brief Содержит реализацию функций для вывода ошибок
 */

#include "errors.h"

#include <stdio.h>

/**
 * @brief Пишет текстовое сообщение с названием ошибки
 *
 * @param code Код ошибки
 *
 * @return Текстовое описание ошибки
 */
const char* error_message (error_code code) {
    switch (code) {
    case ERROR_OK:
        return "Нет ошибки";
    case ERROR_NULL_PTR:
        return "Недопустимый указатель(NULL)";
    case ERROR_SIZE_MATRIX:
        return "Разный размер матриц";
    case ERROR_MEMORY_ALLOCATION:
        return "Ошибка выделения памяти";
    case ERROR_FILE_IO:
        return "Ошибка ввода/вывода файла";
    case ERROR_MATRIX_NOT_SQUARE:
        return "Матрица не является квадратной";
    case ERROR_INVALID_DIMENSIONS:
        return "Недопустимый размер матрицы";
    default:
        return "Неизвестная ошибка";
    }
}

/**
 * @brief Печатает сообщение об ошибки, вместе с кодом ошибки
 *
 * @param code Код ошибки
 * @param function Имя функции, где произошла ошибка
 */
void print_error (error_code code, const char* function) {
    fprintf (stderr, "Ошибка в функции %s. Код ошибки %d: %s\n", function, code,
             error_message (code));
}
