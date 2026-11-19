#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "calc.h"

/* Read an int; reject letters and values outside the int range. */
static int readInt(const char *text, int *number)
{
    char *end;
    errno = 0;
    const long value = strtol(text, &end, 10);
    if ((errno != 0) || (end == text) || (*end != '\0') ||
        (value < INT_MIN) || (value > INT_MAX)) {
        return 0;
    }
    *number = (int)value;
    return 1;
}

int main(int argc, char *argv[])
{
    /* Program name + triples of arguments + "-k" + key. */
    if ((argc < 6) || ((argc - 3) % 3 != 0) ||
        (strcmp(argv[argc - 2], "-k") != 0)) {
        fprintf(stderr, "Usage: cmdcalc A OP B ... -k KEY\n");
        return 1;
    }

    int key;
    if (!readInt(argv[argc - 1], &key)) {
        fprintf(stderr, "Error: invalid key\n");
        return 1;
    }

    const int count = (argc - 3) / 3;
    long long *results = malloc((size_t)count * sizeof(*results));
    if (results == NULL) {
        fprintf(stderr, "Error: not enough memory\n");
        return 1;
    }

    for (int i = 0; i < count; ++i) {
        int left;
        int right;
        const int position = 1 + 3 * i;
        if (!readInt(argv[position], &left) ||
            !readInt(argv[position + 2], &right) ||
            (strlen(argv[position + 1]) != 1) ||
            !calculate(left, argv[position + 1][0], right, &results[i])) {
            fprintf(stderr, "Error in expression %d\n", i + 1);
            free(results);
            return 1;
        }
    }

    char *text = decode(results, count, key);
    if (text == NULL) {
        fprintf(stderr, "Error: invalid ASCII code or not enough memory\n");
        free(results);
        return 1;
    }

    for (int i = 0; i < count; ++i) {
        printf("Ответ №%d: %lld\n", i + 1, results[i]);
    }
    printf("Результирующая строка: ");
    /* Write count bytes so even ASCII code 0 does not cut off the output. */
    fwrite(text, sizeof(*text), (size_t)count, stdout);
    printf("\n");

    free(text);
    free(results);
    return 0;
}
