#include "custom_shell.h"

char *custom_memory_set(char *destination, char value, unsigned int size) {
    unsigned int i;

    for (i = 0; i < size; i++)
        destination[i] = value;

    return destination;
}

void custom_free_strings(char **string_array) {
    char **temp = string_array;

    if (!string_array)
        return;

    while (*string_array)
        free(*string_array++);

    free(temp);
}

void *custom_reallocate_memory(void *pointer, unsigned int old_size, unsigned int new_size) {
    char *new_pointer;

    if (!pointer)
        return malloc(new_size);

    if (!new_size) {
        free(pointer);
        return NULL;
    }

    if (new_size == old_size)
        return pointer;

    new_pointer = malloc(new_size);
    if (!new_pointer)
        return NULL;

    old_size = (old_size < new_size) ? old_size : new_size;
    while (old_size--)
        new_pointer[old_size] = ((char *)pointer)[old_size];

    free(pointer);
    return new_pointer;
}
