#include "custom_shell.h"

void print_to_stderr(char *str) {
    if (str == NULL) {
        return;
    }

    while (*str != '\0') {
        write_char_to_stderr(*str++);
    }
}

int write_char_to_stderr(char c) {
    static int buffer_index = 0;
    static char buffer[WRITE_BUFFER_SIZE];

    if (c == BUF_FLUSH || buffer_index >= WRITE_BUFFER_SIZE) {
        write(STDERR_FILENO, buffer, buffer_index);
        buffer_index = 0;
    }

    if (c != BUF_FLUSH) {
        buffer[buffer_index++] = c;
    }

    return 1;
}

int write_char_to_fd(char c, int fd) {
    static int buffer_index = 0;
    static char buffer[WRITE_BUFFER_SIZE];

    if (c == BUF_FLUSH || buffer_index >= WRITE_BUFFER_SIZE) {
        write(fd, buffer, buffer_index);
        buffer_index = 0;
    }

    if (c != BUF_FLUSH) {
        buffer[buffer_index++] = c;
    }

    return 1;
}

int print_to_fd(char *str, int fd) {
    int chars_put = 0;

    if (str == NULL) {
        return 0;
    }

    while (*str != '\0') {
        chars_put += write_char_to_fd(*str++, fd);
    }

    return chars_put;
}

