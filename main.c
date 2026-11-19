#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "calc.h"

int readInt(const char *text, int *number)
{
    int i = 0;
    int sign = 1;
    long long value = 0;

    if (text[i] == '-') {
        sign = -1;
        ++i;
    }
    else if (text[i] == '+') {
        ++i;
    }

    if (text[i] == '\0') {
        return 0;
    }

    while (text[i] != '\0') {
        if ((text[i] < '0') || (text[i] > '9')) {
            return 0;
        }
        value = value * 10 + (text[i] - '0');
        if ((sign * value < INT_MIN) || (sign * value > INT_MAX)) {
            return 0;
        }
        ++i;
    }

    *number = (int)(sign * value);
    return 1;
}

int main(int argc, char *argv[])
{
    if ((argc < 6) || ((argc - 3) % 3 != 0)) {
        printf("Аргументы потерялись! Пример: cmdcalc 41 + 33 -k 2\n");
        return 1;
    }

    const char *flag = argv[argc - 2];
    if ((flag[0] != '-') || (flag[1] != 'k') || (flag[2] != '\0')) {
        printf("Ключ без бирки! Перед ним должен стоять флаг -k.\n");
        return 1;
    }

    int key;
    if (!readInt(argv[argc - 1], &key)) {
        printf("Такой ключ не подходит: нужно целое число допустимого размера.\n");
        return 1;
    }

    const int count = (argc - 3) / 3;
    long long *results = malloc((size_t)count * sizeof(long long));
    if (results == NULL) {
        printf("Память закончилась. Даже калькулятору нужно личное пространство!\n");
        return 1;
    }

    for (int i = 0; i < count; ++i) {
        int left;
        int right;
        const int position = 1 + 3 * i;
        const char *operation = argv[position + 1];
        if (!readInt(argv[position], &left) ||
            !readInt(argv[position + 2], &right) ||
            (operation[0] == '\0') || (operation[1] != '\0') ||
            !calculate(left, operation[0], right, &results[i])) {
            printf("В выражении №%d что-то не сходится. Проверь числа и знак!\n", i + 1);
            free(results);
            return 1;
        }
    }

    char *text = decode(results, count, key);
    if (text == NULL) {
        printf("Шифр не сдался: код символа вне диапазона 0–127 или не хватает памяти.\n");
        free(results);
        return 1;
    }

    for (int i = 0; i < count; ++i) {
        printf("Ответ №%d: %lld\n", i + 1, results[i]);
    }
    printf("Результирующая строка: ");
    for (int i = 0; i < count; ++i) {
        printf("%c", text[i]);
    }
    printf("\n");

    free(text);
    free(results);
    return 0;
}
