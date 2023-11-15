#include "custom_shell.h"

int custom_string_to_integer(char *string) {
    int i = 0;
    unsigned long int result = 0;
    int sign = 1;

    if (*string == '+')
        string++;

    for (i = 0; string[i] != '\0'; i++) {
        if (string[i] >= '0' && string[i] <= '9') {
            result *= 10;
            result += (string[i] - '0');
            if (result > INT_MAX)
                return -1;
        } else {
            return -1;
        }
    }
    return (sign * result);
}

void print_custom_error(info_t *info, char *error_string) {
    _eputs(info->file_name);
    _eputs(": ");
    print_integer(info->line_count, STDERR_FILENO);
    _eputs(": ");
    _eputs(info->custom_argv[0]);
    _eputs(": ");
    _eputs(error_string);
}

int print_integer(int input, int fd) {
    int (*custom_putchar)(char) = custom_putchar;
    int i, count = 0;
    unsigned int abs_value, current;

    if (fd == STDERR_FILENO)
        custom_putchar = custom_error_putchar;

    if (input < 0) {
        abs_value = -input;
        custom_putchar('-');
        count++;
    } else {
        abs_value = input;
    }

    current = abs_value;
    for (i = 1000000000; i > 1; i /= 10) {
        if (abs_value / i) {
            custom_putchar('0' + current / i);
            count++;
        }
        current %= i;
    }
    custom_putchar('0' + current);
    count++;

    return count;
}

char *convert_to_string(long int number, int base, int flags) {
    static char *character_array;
    static char buffer[50];
    char sign = 0;
    char *ptr;
    unsigned long n = number;

    if (!(flags & CONVERT_UNSIGNED) && number < 0) {
        n = -number;
        sign = '-';
    }

    character_array = flags & CONVERT_LOWERCASE ? "0123456789abcdef" : "0123456789ABCDEF";
    ptr = &buffer[49];
    *ptr = '\0';

    do {
        *--ptr = character_array[n % base];
        n /= base;
    } while (n != 0);

    if (sign)
        *--ptr = sign;
    return ptr;
}

void remove_comment_symbol(char *buffer) {
    int i;

    for (i = 0; buffer[i] != '\0'; i++) {
        if (buffer[i] == '#' && (!i || buffer[i - 1] == ' ')) {
            buffer[i] = '\0';
            break;
        }
    }
}

