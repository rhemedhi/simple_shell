#include "custom_shell.h"

char *copy_string(char *destination, char *source) {
    if (destination == source || source == NULL)
        return destination;

    while (*source != '\0') {
        *destination = *source;
        destination++;
        source++;
    }
    *destination = '\0';
    return destination;
}

char *duplicate_string(const char *string) {
    int length = 0;
    char *result;

    if (string == NULL)
        return NULL;

    const char *temp = string;
    while (*temp != '\0') {
        length++;
        temp++;
    }

    result = (char *)malloc(sizeof(char) * (length + 1));
    if (!result)
        return NULL;

    char *copy = result;
    while (*string != '\0') {
        *copy = *string;
        copy++;
        string++;
    }
    *copy = '\0';
    return result;
}

void print_string(char *string) {
    if (string == NULL)
        return;

    while (*string != '\0') {
        custom_putchar(*string);
        string++;
    }
}

int print_character(char character) {
    static int count = 0;
    static char buffer[WRITE_BUFFER_SIZE];

    if (character == BUF_FLUSH || count >= WRITE_BUFFER_SIZE) {
        write(1, buffer, count);
        count = 0;
    }

    if (character != BUF_FLUSH) {
        buffer[count++] = character;
    }

    return 1;
}
