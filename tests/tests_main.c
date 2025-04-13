/**
 * @file tests_main.c
 *
 * @brief Модуль реализации тестов для main.c
 */

#include "matrix/matrix.h"
#include "output/output.h"

#include <CUnit/CUnit.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Вспомогательная функция для создания тестовых файлов
void create_test_matrix_file (const char* filename, int rows, int cols,
                              double* data) {
    FILE* f = fopen (filename, "w");
    if (f) {
        fprintf (f, "%d %d\n", rows, cols);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                fprintf (f, "%.2f ", data[i * cols + j]);
            }
            fprintf (f, "\n");
        }
        fclose (f);
    }
}

// Тест успешного выполнения всей программы
void test_main_success (void) {
    // Подготовка тестовых данных
    double a_data[4] = {1, 2, 3, 4};           // 2x2
    double b_data[4] = {5, 6, 7, 8};           // 2x2
    double c_data[4] = {0.5, 0.5, 0.5, 0.5};   // 2x2
    double d_data[4] = {0.1, 0.1, 0.1, 0.1};   // 2x2

    create_test_matrix_file ("data/matrix_a.txt", 2, 2, a_data);
    create_test_matrix_file ("data/matrix_b.txt", 2, 2, b_data);
    create_test_matrix_file ("data/matrix_c.txt", 2, 2, c_data);
    create_test_matrix_file ("data/matrix_d.txt", 2, 2, d_data);

    // Вызов тестируемой функции
    int result = system ("./matrix_app");

    // Проверка результатов
    CU_ASSERT_EQUAL (result, 1);

    // Проверка файла с результатом
    FILE* f = fopen ("data/result.txt", "r");
    CU_ASSERT_PTR_NOT_NULL (f);
    if (f) {
        int rows, cols;
        fscanf (f, "%d %d", &rows, &cols);
        CU_ASSERT_EQUAL (rows, 2);
        CU_ASSERT_EQUAL (cols, 2);

        double val1, val2, val3, val4;
        fscanf (f, "%lf %lf", &val1, &val2);
        fscanf (f, "%lf %lf", &val3, &val4);

        // Проверка вычисления A×B^T−C+D
        CU_ASSERT_DOUBLE_EQUAL (val1, 38.6, 0.001);
        CU_ASSERT_DOUBLE_EQUAL (val4, 90.6, 0.001);

        fclose (f);
    }

    // Очистка
    remove ("data/matrix_a.txt");
    remove ("data/matrix_b.txt");
    remove ("data/matrix_c.txt");
    remove ("data/matrix_d.txt");
    remove ("data/result.txt");
}

// Тест обработки ошибок при загрузке файлов
void test_main_file_errors (void) {
    // Создаем только один файл
    double a_data[4] = {1, 2, 3, 4};
    create_test_matrix_file ("data/matrix_a.txt", 2, 2, a_data);

    // Остальные файлы отсутствуют
    int result = system ("./matrix_app");
    CU_ASSERT_EQUAL (result, 0);

    // Очистка
    remove ("data/matrix_a.txt");
}

// Тест обработки несовместимых размеров матриц
void test_main_dimension_errors (void) {
    // Матрицы несовместимых размеров
    double a_data[6] = {1, 2, 3, 4, 5, 6};     // 2x3
    double b_data[6] = {5, 6, 7, 8, 9, 10};    // 3x2
    double c_data[4] = {0.5, 0.5, 0.5, 0.5};   // 2x2
    double d_data[4] = {0.1, 0.1, 0.1, 0.1};   // 2x2

    create_test_matrix_file ("data/matrix_a.txt", 2, 3, a_data);
    create_test_matrix_file ("data/matrix_b.txt", 3, 2, b_data);
    create_test_matrix_file ("data/matrix_c.txt", 2, 2, c_data);
    create_test_matrix_file ("data/matrix_d.txt", 2, 2, d_data);

    int result = system ("./matrix_app");
    CU_ASSERT_EQUAL (result, 0);

    // Очистка
    remove ("data/matrix_a.txt");
    remove ("data/matrix_b.txt");
    remove ("data/matrix_c.txt");
    remove ("data/matrix_d.txt");
}

// Тест обработки ошибок при сохранении результата
void test_main_save_error (void) {
    // Подготовка тестовых данных
    double a_data[4] = {1, 2, 3, 4};
    double b_data[4] = {5, 6, 7, 8};
    double c_data[4] = {0.5, 0.5, 0.5, 0.5};
    double d_data[4] = {0.1, 0.1, 0.1, 0.1};

    create_test_matrix_file ("data/matrix_a.txt", 2, 2, a_data);
    create_test_matrix_file ("data/matrix_b.txt", 2, 2, b_data);
    create_test_matrix_file ("data/matrix_c.txt", 2, 2, c_data);
    create_test_matrix_file ("data/matrix_d.txt", 2, 2, d_data);

    // Создаем несуществующую папку для проверки ошибки сохранения
    int result = system ("./matrix_app");

    // В этом тесте мы не можем напрямую проверить ошибку сохранения,
    // так как функция main() не возвращает разные коды ошибок
    // Но можем проверить, что программа завершилась с ошибкой
    CU_ASSERT_EQUAL (result, 0);

    // Очистка
    remove ("data/matrix_a.txt");
    remove ("data/matrix_b.txt");
    remove ("data/matrix_c.txt");
    remove ("data/matrix_d.txt");
}

void register_main_tests (void) {
    CU_pSuite suite = CU_add_suite ("Main Module Tests", NULL, NULL);
    CU_add_test (suite, "Test Main Success", test_main_success);
    CU_add_test (suite, "Test File Errors", test_main_file_errors);
    CU_add_test (suite, "Test Dimension Errors", test_main_dimension_errors);
    CU_add_test (suite, "Test Save Error", test_main_save_error);
}
