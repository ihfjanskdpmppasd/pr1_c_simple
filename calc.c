#include <stdlib.h>
#include "calc.h"

/* Расширяем тип до вычисления, чтобы результат поместился. */
int calculate(int left, char operation, int right, long long *result)
{
    switch (operation) {
        case '+':
            *result = (long long)left + right;
            break;
        case '-':
            *result = (long long)left - right;
            break;
        case '*':
            *result = (long long)left * right;
            break;
        case '%':
            if (right == 0) {
                return 0;
            }
            *result = (long long)left % right;
            break;
        default:
            return 0;
    }
    return 1;
}

/* Вычитаем ключ и записываем символы с полученными ASCII-кодами. */
char *decode(const long long *results, int count, int key)
{
    char *text = malloc((size_t)count + 1);
    if (text == NULL) {
        return NULL;
    }
    for (int i = 0; i < count; ++i) {
        const long long code = results[i] - key;
        if ((code < 0) || (code > 127)) {
            free(text);
            return NULL;
        }
        text[i] = (char)code;
    }
    text[count] = '\0';
    return text;
}
