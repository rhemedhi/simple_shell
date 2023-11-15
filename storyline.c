#include "custom_shell.h"

char *get_custom_history_file(info_t *info) {
    char *home_dir = _getenv(info, "HOME=");
    if (!home_dir)
        return NULL;

    size_t history_file_length = strlen(home_dir) + strlen(HISTORY_FILE) + 2;
    char *history_file = malloc(sizeof(char) * history_file_length);
    if (!history_file) {
        free(home_dir);
        return NULL;
    }

    history_file[0] = '\0';
    strcpy(history_file, home_dir);
    strcat(history_file, "/");
    strcat(history_file, HISTORY_FILE);

    free(home_dir);
    return history_file;
}

int write_custom_history(info_t *info) {
    char *filename = get_custom_history_file(info);
    if (!filename)
        return -1;

    ssize_t file_descriptor = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
    free(filename);

    if (file_descriptor == -1)
        return -1;

    list_t *node = info->history;
    while (node) {
        _putsfd(node->str, file_descriptor);
        _putfd('\n', file_descriptor);
        node = node->next;
    }
    _putfd(BUF_FLUSH, file_descriptor);
    close(file_descriptor);
    return 1;
}

int read_custom_history(info_t *info) {
    char *filename = get_custom_history_file(info);
    if (!filename)
        return 0;

    ssize_t file_descriptor = open(filename, O_RDONLY);
    free(filename);

    if (file_descriptor == -1)
        return 0;

    struct stat st;
    if (fstat(file_descriptor, &st) != 0 || st.st_size < 2) {
        close(file_descriptor);
        return 0;
    }

    char *buffer = malloc(sizeof(char) * (st.st_size + 1));
    if (!buffer) {
        close(file_descriptor);
        return 0;
    }

    ssize_t rdlen = read(file_descriptor, buffer, st.st_size);
    buffer[st.st_size] = '\0';
    close(file_descriptor);

    if (rdlen <= 0) {
        free(buffer);
        return 0;
    }

    int line_count = 0;
    int last = 0;
    for (int i = 0; i < st.st_size; i++) {
        if (buffer[i] == '\n') {
            buffer[i] = '\0';
            build_history_list(info, buffer + last, line_count++);
            last = i + 1;
        }
    }

    if (last != st.st_size)
        build_history_list(info, buffer + last, line_count++);

    free(buffer);
    info->histcount = line_count;

    while (info->histcount-- >= HISTORY_MAX)
        delete_node_at_index(&(info->history), 0);

    renumber_history(info);
    return info->histcount;
}

int build_custom_history_list(info_t *info, char *buf, int linecount) {
    list_t *node = info->history;
    if (info->history)
        node = info->history;
    add_node_end(&node, buf, linecount);
    if (!info->history)
        info->history = node;
    return 0;
}

int renumber_custom_history(info_t *info) {
    list_t *node = info->history;
    int i = 0;

    while (node) {
        node->num = i++;
        node = node->next;
    }

    return (info->histcount = i);
}

