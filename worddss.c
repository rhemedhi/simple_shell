#include "custom_shell.h"

int string_length(char *str) {
    int length = 0;

    if (!str)
        return 0;

    while (*str++) {
        length++;
    }

    return length;
}

int string_compare(char *str1, char *str2) {
    while (*str1 && *str2) {
        if (*str1 != *str2)
            return (*str1 - *str2);
        str1++;
        str2++;
    }

    if (*str1 == *str2)
        return 0;
    else
        return (*str1 < *str2 ? -1 : 1);
}

char *find_substring_start(const char *haystack, const char *needle) {
    while (*needle) {
        if (*needle++ != *haystack++)
            return NULL;
    }

    return (char *)haystack;
}

char *concatenate_strings(char *destination, char *source) {
    char *result = destination;

    while (*destination)
        destination++;

    while (*source)
        *destination++ = *source++;

    *destination = *source;
    return result;
}
