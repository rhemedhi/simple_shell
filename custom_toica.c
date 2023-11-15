#include "custom_shell.h"

int is_interactive_mode(info_t *info) {
    return (isatty(STDIN_FILENO) && info->read_file_descriptor <= 2);
}

int is_delimiter_character(char c, char *delimiter) {
    while (*delimiter) {
        if (*delimiter++ == c)
            return 1;
    }
    return 0;
}

int is_alphabetic_character(int c) {
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) ? 1 : 0;
}

int convert_to_integer(char *string) {
    int i, sign = 1, flag = 0, output;
    unsigned int result = 0;

    for (i = 0; string[i] != '\0' && flag != 2; i++) {
        if (string[i] == '-')
            sign *= -1;

        if (string[i] >= '0' && string[i] <= '9') {
            flag = 1;
            result *= 10;
            result += (string[i] - '0');
        } else if (flag == 1) {
            flag = 2;
        }
    }

    if (sign == -1)
        output = -result;
    else
        output = result;

    return output;
}

