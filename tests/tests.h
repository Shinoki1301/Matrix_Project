/**
 * @file tests.h
 *
 * @brief Модуль содержащий прототипы тестов
 */

#ifndef TESTS_H
#define TESTS_H

#include <CUnit/CUnit.h>
#include <sys/stat.h>

// Прототипы тестовых функций
void test_create_and_free_matrix (void);
void test_matrix_addition (void);
void test_matrix_multiplication (void);
void test_determinant (void);
void test_invalid_operations (void);
void test_file_operations (void);
void test_integration (void);
void test_file_operations_integration (void);

// Функции регистрации тестов
void register_matrix_tests (void);
void register_output_tests (void);

#endif
