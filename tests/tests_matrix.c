/**
 * @file tests_matrix.c
 *
 * @brief Модуль реализации тестов для matrix.c
 */
#include "matrix/matrix.h"

#include <CUnit/Basic.h>
#include <stdio.h>
#include <stdlib.h>

void test_matrix_creation (void) {
    Matrix m = create_matrix (2, 3);
    CU_ASSERT_PTR_NOT_NULL (m.data);
    CU_ASSERT_EQUAL (m.rows, 2);
    CU_ASSERT_EQUAL (m.cols, 3);
    free_matrix (&m);

    Matrix invalid = create_matrix (-1, 0);
    CU_ASSERT_PTR_NULL (invalid.data);
    CU_ASSERT_EQUAL (invalid.rows, 0);
    CU_ASSERT_EQUAL (invalid.cols, 0);
}

void test_matrix_addition (void) {
    Matrix a = create_matrix (2, 2);
    Matrix b = create_matrix (2, 2);

    // Заполняем матрицы тестовыми данными
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            a.data[i][j] = i + j;
            b.data[i][j] = (i + j) * 2;
        }
    }

    Matrix result     = create_matrix (2, 2);
    int    add_result = add_matrices (&a, &b, &result);

    CU_ASSERT_EQUAL (add_result, 0);
    CU_ASSERT_DOUBLE_EQUAL (result.data[0][0], 0.0, 0.001);
    CU_ASSERT_DOUBLE_EQUAL (result.data[1][1], 6.0, 0.001);

    free_matrix (&a);
    free_matrix (&b);
    free_matrix (&result);

    // Тест с несовместимыми размерами
    Matrix c              = create_matrix (3, 2);
    Matrix invalid_add    = create_matrix (2, 2);
    int    invalid_result = add_matrices (&a, &c, &invalid_add);
    CU_ASSERT_NOT_EQUAL (invalid_result, 0);

    free_matrix (&c);
    free_matrix (&invalid_add);
}

void test_matrix_multiplication (void) {
    Matrix a = create_matrix (2, 3);
    Matrix b = create_matrix (3, 2);

    // Заполняем матрицы тестовыми данными
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j++) {
            a.data[i][j] = i + j;
        }
    }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 2; j++) {
            b.data[i][j] = i * j;
        }
    }

    Matrix result     = create_matrix (3, 3);
    int    mul_result = multiply_matrices (&a, &b, &result);

    CU_ASSERT_EQUAL (mul_result, 0);
    CU_ASSERT_DOUBLE_EQUAL (result.data[0][0], 0.0, 0.001);
    CU_ASSERT_DOUBLE_EQUAL (result.data[1][1], 8.0, 0.001);

    free_matrix (&a);
    free_matrix (&b);
    free_matrix (&result);

    // Тест с несовместимыми размерами
    Matrix c              = create_matrix (2, 2);
    Matrix invalid_mul    = create_matrix (2, 2);
    int    invalid_result = multiply_matrices (&a, &c, &invalid_mul);
    CU_ASSERT_NOT_EQUAL (invalid_result, 0);

    free_matrix (&c);
    free_matrix (&invalid_mul);
}

void test_matrix_transpose (void) {
    Matrix m = create_matrix (2, 3);

    // Заполняем матрицу тестовыми данными
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j++) {
            m.data[i][j] = i * 3 + j;
        }
    }

    Matrix transposed = transpose_matrix (&m);

    CU_ASSERT_EQUAL (transposed.rows, 3);
    CU_ASSERT_EQUAL (transposed.cols, 2);
    CU_ASSERT_DOUBLE_EQUAL (transposed.data[0][0], 0.0, 0.001);
    CU_ASSERT_DOUBLE_EQUAL (transposed.data[1][1], 4.0, 0.001);

    free_matrix (&m);
    free_matrix (&transposed);
}

void test_determinant (void) {
    Matrix m = create_matrix (2, 2);

    // Заполняем матрицу тестовыми данными
    m.data[0][0] = 1.0;
    m.data[0][1] = 2.0;
    m.data[1][0] = 3.0;
    m.data[1][1] = 4.0;

    double det = determinant (&m);
    CU_ASSERT_DOUBLE_EQUAL (det, -2.0, 0.001);

    free_matrix (&m);

    // Тест с неквадратной матрицей
    Matrix non_square  = create_matrix (2, 3);
    double invalid_det = determinant (&non_square);
    CU_ASSERT_DOUBLE_EQUAL (invalid_det, 0.0, 0.001);

    free_matrix (&non_square);
}

void test_null_safety (void) {
    // Проверка обработки NULL указателей
    Matrix result = create_matrix (1, 1);

    CU_ASSERT_NOT_EQUAL (add_matrices (NULL, NULL, &result), 0);
    CU_ASSERT_NOT_EQUAL (subtract_matrices (NULL, NULL, &result), 0);
    CU_ASSERT_NOT_EQUAL (multiply_matrices (NULL, NULL, &result), 0);

    Matrix transposed = transpose_matrix (NULL);
    CU_ASSERT_PTR_NULL (transposed.data);

    free_matrix (&result);

    // Проверка с частично валидными аргументами
    Matrix m = create_matrix (1, 1);
    CU_ASSERT_NOT_EQUAL (add_matrices (&m, NULL, &result), 0);
    CU_ASSERT_NOT_EQUAL (multiply_matrices (NULL, &m, &result), 0);

    free_matrix (&m);
}

void test_file_operations (void) {
    const char* filename = "test_matrix.txt";

    // Создаем тестовый файл
    FILE* f = fopen (filename, "w");
    fprintf (f, "2 2\n1.0 2.0\n3.0 4.0");
    fclose (f);

    // Тестируем загрузку
    Matrix loaded = load_matrix_from_file (filename);
    CU_ASSERT_PTR_NOT_NULL (loaded.data);
    CU_ASSERT_EQUAL (loaded.rows, 2);
    CU_ASSERT_EQUAL (loaded.cols, 2);
    CU_ASSERT_DOUBLE_EQUAL (loaded.data[0][0], 1.0, 0.001);
    CU_ASSERT_DOUBLE_EQUAL (loaded.data[1][1], 4.0, 0.001);

    // Тестируем сохранение
    int save_result = save_matrix_to_file (&loaded, "test_save.txt");
    CU_ASSERT_EQUAL (save_result, 0);

    free_matrix (&loaded);
    remove (filename);
    remove ("test_save.txt");
}

void test_file_errors (void) {
    // Тест с несуществующим файлом
    Matrix loaded = load_matrix_from_file ("nonexistent.txt");
    CU_ASSERT_PTR_NULL (loaded.data);

    // Тест с некорректными данными
    FILE* f = fopen ("bad_matrix.txt", "w");
    fprintf (f, "2 a\n1 2\n3 4");
    fclose (f);

    loaded = load_matrix_from_file ("bad_matrix.txt");
    CU_ASSERT_PTR_NULL (loaded.data);
    remove ("bad_matrix.txt");
}

void register_matrix_tests (void) {
    CU_pSuite suite = CU_add_suite ("Matrix Tests", NULL, NULL);
    CU_add_test (suite, "Matrix Creation", test_matrix_creation);
    CU_add_test (suite, "Matrix Addition", test_matrix_addition);
    CU_add_test (suite, "Matrix Multiplication", test_matrix_multiplication);
    CU_add_test (suite, "Matrix Transpose", test_matrix_transpose);
    CU_add_test (suite, "Matrix Determinant", test_determinant);
    CU_add_test (suite, "NULL Safety", test_null_safety);
    CU_add_test (suite, "File Operations", test_file_operations);
}
