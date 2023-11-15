#include "custom_shell.h"

char **split_string_to_words(char *string, char *delimiter) {
    int i, j, k, m, num_words = 0;
    char **words;

    if (string == NULL || string[0] == '\0')
        return NULL;

    if (!delimiter)
        delimiter = " ";

    for (i = 0; string[i] != '\0'; i++) {
        if (!is_delimiter_character(string[i], delimiter) && 
           (is_delimiter_character(string[i + 1], delimiter) || string[i + 1] == '\0'))
            num_words++;
    }

    if (num_words == 0)
        return NULL;

    words = malloc((num_words + 1) * sizeof(char *));
    if (!words)
        return NULL;

    for (i = 0, j = 0; j < num_words; j++) {
        while (is_delimiter_character(string[i], delimiter))
            i++;

        k = 0;
        while (!is_delimiter_character(string[i + k], delimiter) && string[i + k] != '\0')
            k++;

        words[j] = malloc((k + 1) * sizeof(char));
        if (!words[j]) {
            for (k = 0; k < j; k++)
                free(words[k]);
            free(words);
            return NULL;
        }

        for (m = 0; m < k; m++)
            words[j][m] = string[i++];
        words[j][m] = '\0';
    }

    words[j] = NULL;
    return words;
}
