/**
 * @file tests_output.c
 *
 * @brief Модуль реализации тестов для output.c
 */

#include "../src/output/output.h"

#include <CUnit/CUnit.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Вспомогательная функция для создания тестового файла
void create_test_file (const char* filename, const char* content) {
    FILE* f = fopen (filename, "w");
    if (f) {
        fprintf (f, "%s", content);
        fclose (f);
    }
}

void test_output_print_matrix (void) {
    // Тест с нормальной матрицей
    double data1[4] = {1.0, 2.0, 3.0, 4.0};
    printf ("Ожидаемый вывод для матрицы 2x2:\n");
    output_print_matrix (2, 2, data1);

    // Тест с NULL данными
    printf ("Ожидаемое сообщение об ошибке:\n");
    output_print_matrix (2, 2, NULL);
}

void test_output_save_matrix_to_file (void) {
    const char* filename = "test_matrix.txt";
    double      data[4]  = {1.1, 2.2, 3.3, 4.4};

    // Успешное сохранение
    CU_ASSERT_EQUAL (output_save_matrix_to_file (2, 2, data, filename), 0);

    // Проверка содержимого файла
    FILE* f = fopen (filename, "r");
    CU_ASSERT_PTR_NOT_NULL (f);
    if (f) {
        int    rows, cols;
        double val1, val2, val3, val4;
        fscanf (f, "%d %d", &rows, &cols);
        CU_ASSERT_EQUAL (rows, 2);
        CU_ASSERT_EQUAL (cols, 2);

        fscanf (f, "%lf %lf", &val1, &val2);
        fscanf (f, "%lf %lf", &val3, &val4);
        CU_ASSERT_DOUBLE_EQUAL (val1, 1.1, 0.001);
        CU_ASSERT_DOUBLE_EQUAL (val4, 4.4, 0.001);
        fclose (f);
    }

    // Ошибка при NULL данных
    CU_ASSERT_EQUAL (output_save_matrix_to_file (2, 2, NULL, filename), -1);

    // Ошибка при неверном имени файла
    CU_ASSERT_EQUAL (
        output_save_matrix_to_file (2, 2, data, "/invalid/path/matrix.txt"), -1);

    remove (filename);
}

void test_output_load_matrix_from_file (void) {
    const char* filename     = "test_load.txt";
    const char* file_content = "2 2\n1.5 2.5\n3.5 4.5\n";
    create_test_file (filename, file_content);

    int     rows, cols;
    double* data = output_load_matrix_from_file (&rows, &cols, filename);

    // Проверка успешной загрузки
    CU_ASSERT_PTR_NOT_NULL (data);
    if (data) {
        CU_ASSERT_EQUAL (rows, 2);
        CU_ASSERT_EQUAL (cols, 2);
        CU_ASSERT_DOUBLE_EQUAL (data[0], 1.5, 0.001);
        CU_ASSERT_DOUBLE_EQUAL (data[3], 4.5, 0.001);
        free (data);
    }

    // Тест с несуществующим файлом
    double* invalid_data =
        output_load_matrix_from_file (&rows, &cols, "nonexistent.txt");
    CU_ASSERT_PTR_NULL (invalid_data);

    // Тест с поврежденным файлом (неправильные размеры)
    const char* bad_content1 = "2 a\n1 2\n3 4\n";
    create_test_file (filename, bad_content1);
    double* bad_data1 = output_load_matrix_from_file (&rows, &cols, filename);
    CU_ASSERT_PTR_NULL (bad_data1);

    // Тест с поврежденным файлом (недостаточно данных)
    const char* bad_content2 = "2 2\n1 2\n3\n";
    create_test_file (filename, bad_content2);
    double* bad_data2 = output_load_matrix_from_file (&rows, &cols, filename);
    CU_ASSERT_PTR_NULL (bad_data2);

    remove (filename);
}

void test_file_operations_integration (void) {
    const char* filename = "test_integration.txt";
    double      data[4]  = {1.0, 2.0, 3.0, 4.0};

    // Сохраняем матрицу
    CU_ASSERT_EQUAL (output_save_matrix_to_file (2, 2, data, filename), 0);

    // Загружаем обратно
    int     rows, cols;
    double* loaded_data = output_load_matrix_from_file (&rows, &cols, filename);
    CU_ASSERT_PTR_NOT_NULL (loaded_data);
    if (loaded_data) {
        CU_ASSERT_EQUAL (rows, 2);
        CU_ASSERT_EQUAL (cols, 2);
        CU_ASSERT_DOUBLE_EQUAL (loaded_data[0], 1.0, 0.001);
        CU_ASSERT_DOUBLE_EQUAL (loaded_data[3], 4.0, 0.001);
        free (loaded_data);
    }

    remove (filename);
}

void register_output_tests (void) {
    CU_pSuite suite = CU_add_suite ("Output Tests", NULL, NULL);
    CU_add_test (suite, "Print Matrix", test_output_print_matrix);
    CU_add_test (suite, "Save Matrix to File", test_output_save_matrix_to_file);
    CU_add_test (suite, "Load Matrix from File", test_output_load_matrix_from_file);
    CU_add_test (suite, "File Operations Integration",
                 test_file_operations_integration);
}
