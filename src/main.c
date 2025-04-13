/**
 * @file main.c
 * @brief Главный модуль, демонстрирующий матричные операции и выполнение
 * индивидуального задания
 *
 * @details
 * Программа вычисляет выражение выражение A × B^T − C + D.
 * Где:
 * - A, B, C, D - матрицы, загружаеммые из файлов
 * - B^T - транспонированная матрица В
 * - Все операции выполняются с проверкой ошибок
 *
 * Алгоритм работы:
 * 1. Загрузка матриц A, B, C, D из файлов
 * 2. Транспонирование матрицы B
 * 3. Умножение A на B^T
 * 4. Вычитание матрицы C
 * 5. Сложение с матрицей D
 * 6. Сохранение результата
 *
 * @return 1 при успешном выполнении, 0 при ошибке
 *
 * @note Для работы требуются файлы в папке data/
 *
 * @see matrix.h output.h
 */

#include "matrix/matrix.h"
#include "output/output.h"

#include <stdio.h>
#include <stdlib.h>

int main () {
    int res = 1;   // Общий флаг успеха операций

    // 1. Загрузка матриц
    Matrix A = load_matrix_from_file ("data/data_main/matrix_a.txt");
    Matrix B = load_matrix_from_file ("data/data_main/matrix_b.txt");
    Matrix C = load_matrix_from_file ("data/data_main/matrix_c.txt");
    Matrix D = load_matrix_from_file ("data/data_main/matrix_d.txt");

    if (!A.data || !B.data || !C.data || !D.data) {
        res = 0;
        fprintf (stderr, "Ошибка загрузки матриц.\n");
    }

    // 2. Транспонирование B
    Matrix B_transpose = {0};
    if (res) {
        B_transpose = transpose_matrix (&B);
        if (!B_transpose.data) {
            res = 0;
            fprintf (stderr, "Ошибка транспонирования B.\n");
        }
    }

    // 3. Умножение A × B^T
    Matrix AB = {0};
    if (res) {
        AB = create_matrix (A.rows, B_transpose.cols);
        if (!AB.data) {
            res = 0;
            fprintf (stderr, "Ошибка создания матрицы AB.\n");
        }
    }

    if (res) {
        if (multiply_matrices (&A, &B_transpose, &AB) != 0) {
            res = 0;
            fprintf (stderr, "Ошибка умножения матриц.\n");
        }
    }

    // 4. Вычитание C (AB - C)
    Matrix AB_minus_C = {0};
    if (res) {
        AB_minus_C = create_matrix (AB.rows, AB.cols);
        if (!AB_minus_C.data) {
            res = 0;
            fprintf (stderr, "Ошибка создания матрицы AB_minus_C.\n");
        }
    }

    if (res) {
        if (subtract_matrices (&AB, &C, &AB_minus_C) != 0) {
            res = 0;
            fprintf (stderr, "Ошибка вычитания матриц.\n");
        }
    }

    // 5. Сложение с D (AB_minus_C + D)
    Matrix result = {0};
    if (res) {
        result = create_matrix (AB_minus_C.rows, AB_minus_C.cols);
        if (!result.data) {
            res = 0;
            fprintf (stderr, "Ошибка создания результирующей матрицы.\n");
        }
    }

    if (res) {
        if (add_matrices (&AB_minus_C, &D, &result) != 0) {
            res = 0;
            fprintf (stderr, "Ошибка сложения матриц.\n");
        }
    }

    // 6. Вывод и сохранение результата
    if (res) {
        printf ("Результат выражения A×B^T−C+D:\n");
        print_matrix (&result);

        if (save_matrix_to_file (&result, "data/output/result.txt") != 0) {
            res = 0;
            fprintf (stderr, "Ошибка сохранения результата.\n");
        } else {
            printf ("Результат сохранен в data/output/result.txt\n");
        }
    }

    // Освобождение памяти
    free_matrix (&A);
    free_matrix (&B);
    free_matrix (&C);
    free_matrix (&D);
    free_matrix (&B_transpose);
    free_matrix (&AB);
    free_matrix (&AB_minus_C);
    free_matrix (&result);

    return res ? 0 : 1;
}
