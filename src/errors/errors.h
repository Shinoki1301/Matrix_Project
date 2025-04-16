/**
 * @file errors.h
 * @brief Функции для вывода ошибок
 */

#ifndef ERRORS_H
#define ERRORS_H

typedef enum {
    ERROR_OK                 = 0,
    ERROR_NULL_PTR           = 1,
    ERROR_SIZE_MATRIX        = 2,
    ERROR_MEMORY_ALLOCATION  = 3,
    ERROR_FILE_IO            = 4,
    ERROR_MATRIX_NOT_SQUARE  = 5,
    ERROR_INVALID_DIMENSIONS = 6,
    ERROR_UNKNOWN            = 7
} error_code;

/**
 * @brief Пишет текстовое описание ошибки по её коду
 *
 * @param code Код ошибки
 *
 * @return Текстовое описание ошибки
 */
const char* error_message (error_code);

/**
 * @brief Печатает сообщение об ошибке с кодом
 *
 * @param code Код ошибки
 * @param function Имя функции, где произошла ошибка
 */
void print_error (error_code code, const char* function);

#endif

