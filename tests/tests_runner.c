/**
 * @file tests_runner.c
 *
 * @brief Модуль запуска всех тестов
 */

#include <CUnit/Basic.h>
#include <stdio.h>

// Объявления тестовых функций
void register_matrix_tests (void);
void register_output_tests (void);
void test_file_operations (void);
void test_file_operations_integration (void);

int main (void) {
    // Инициализация реестра тестов
    if (CU_initialize_registry () != CUE_SUCCESS) {
        return CU_get_error ();
    }

    // Регистрация всех тестовых сьют
    register_matrix_tests ();
    register_output_tests ();

    // Сьют для файловых операций
    CU_pSuite fileSuite = CU_add_suite ("File Operations", NULL, NULL);
    if (fileSuite == NULL) {
        CU_cleanup_registry ();
        return CU_get_error ();
    }
    CU_add_test (fileSuite, "File Read/Write", test_file_operations);

    // Сьют для интеграционных тестов
    CU_pSuite integrationSuite = CU_add_suite ("Integration", NULL, NULL);
    if (integrationSuite == NULL) {
        CU_cleanup_registry ();
        return CU_get_error ();
    }
    CU_add_test (integrationSuite, "Output Integration",
                 test_file_operations_integration);

    // Запуск тестов
    CU_basic_set_mode (CU_BRM_VERBOSE);
    CU_basic_run_tests ();
    CU_cleanup_registry ();

    return CU_get_error ();
}
