#include "custom_shell.h"

char *copy_string(char *destination, char *source, int length) {
    char *result = destination;
    int i;

    for (i = 0; i < length - 1 && source[i] != '\0'; i++) {
        destination[i] = source[i];
    }

    while (i < length) {
        destination[i++] = '\0';
    }

    return result;
}

char *concatenate_strings(char *destination, char *source, int n) {
    char *result = destination;
    int i = 0;

    while (*destination != '\0') {
        destination++;
    }

    while (*source != '\0' && i < n) {
        *destination = *source;
        destination++;
        source++;
        i++;
    }

    *destination = '\0';
    return result;
}

char *find_character(char *string, char character) {
    while (*string != '\0') {
        if (*string == character) {
            return string;
        }
        string++;
    }

    return NULL;
}

