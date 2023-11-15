#include "custom_shell.h"

int is_executable(info_t *info, char *path) {
    struct stat st;

    if (!path || stat(path, &st) != 0) {
        return 0;
    }

    if (S_ISREG(st.st_mode)) {
        return 1;
    }
    
    return 0;
}

char *copy_characters(char *path_string, int start, int stop) {
    int length = stop - start;
    char *buffer = (char *)malloc((length + 1) * sizeof(char));

    if (!buffer) {
        return NULL;
    }

    int k = 0;
    for (int i = start; i < stop; i++) {
        if (path_string[i] != ':') {
            buffer[k++] = path_string[i];
        }
    }
    buffer[k] = '\0';
    
    return buffer;
}

char *find_command_path(info_t *info, char *path_string, char *command) {
    if (!path_string) {
        return NULL;
    }

    if (strlen(command) > 2 && starts_with_string(command, "./")) {
        if (is_executable(info, command)) {
            return command;
        }
    }

    int i = 0, current_pos = 0;
    char *path = NULL;

    while (path_string[i] != '\0') {
        if (path_string[i] == ':') {
            path = copy_characters(path_string, current_pos, i);
            if (!path) {
                return NULL;
            }

            if (*path != '\0') {
                strcat(path, "/");
                strcat(path, command);
            } else {
                strcpy(path, command);
            }

            if (is_executable(info, path)) {
                return path;
            }

            current_pos = i + 1;
        }
        i++;
    }

    return NULL;
}
