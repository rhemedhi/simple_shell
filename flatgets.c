#include "custom_shell.h"

ssize_t buffer_input(info_t *info, char **buf, size_t *len) {
    ssize_t bytes_read = 0;
    size_t len_p = 0;

    if (!*len) {
        free(*buf);
        *buf = NULL;
        signal(SIGINT, sigintHandler);

        
        printf("$ ");
        fflush(stdout);
        char input[MAX_INPUT_LENGTH];
        if (fgets(input, MAX_INPUT_LENGTH, stdin) != NULL) {
            bytes_read = strlen(input);
            if (bytes_read > 0 && input[bytes_read - 1] == '\n') {
                input[bytes_read - 1] = '\0';
                bytes_read--;
            }
            info->linecount_flag = 1;
            remove_comments(input);
            build_command_history_list(info, input, info->histcount++);

            *len = bytes_read;
            *buf = strdup(input);
        }
    }

    return bytes_read;
}

ssize_t get_input(info_t *info) {
    static char *buf;
    static size_t i = 0, j = 0, len = 0;
    ssize_t r = 0;
    char **buf_p = &(info->arg), *p;

    r = buffer_input(info, &buf, &len);
    if (r == -1) {
        return -1;
    }

    if (len) {
        j = i;
        p = buf + i;

        
        
        while (j < len) {
            if (buf[j] == ';') {
                break;
            }
            j++;
        }

        i = j + 1;
        if (i >= len) {
            i = len = 0;
            info->cmd_buf_type = CMD_NORM;
        }

        *buf_p = p;
        return strlen(p);
    }

    *buf_p = buf;
    return r;
}

ssize_t read_buffer(info_t *info, char *buf, size_t *i) {
    ssize_t r = 0;

    if (*i) {
        return 0;
    }

    printf("Reading from file descriptor...\n");
    return r;
}

int custom_getline(info_t *info, char **ptr, size_t *length) {
    static char buf[READ_BUFFER_SIZE];
    static size_t i = 0, len = 0;
    size_t k;
    ssize_t r = 0, s = 0;
    char *p = NULL, *new_p = NULL, *c;

    p = *ptr;
    if (p && length) {
        s = *length;
    }

    if (i == len) {
        i = len = 0;
    }

    r = read_buffer(info, buf, &len);
    if (r == -1 || (r == 0 && len == 0)) {
        return -1;
    }

    c = strchr(buf + i, '\n');
    k = c ? 1 + (size_t)(c - buf) : len;
    new_p = realloc(p, s ? s + k : k + 1);
    if (!new_p) {
        return -1;
    }

    if (s) {
        strncat(new_p, buf + i, k - i);
    } else {
        strncpy(new_p, buf + i, k - i + 1);
    }

    s += k - i;
    i = k;
    p = new_p;

    if (length) {
        *length = s;
    }
    *ptr = p;
    return s;
}

void sigintHandler(int sig_num) {
    printf("\n");
    printf("$ ");
    fflush(stdout);
}

