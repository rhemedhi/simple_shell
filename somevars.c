#include "custom_shell.h"

int check_chain_delimiter(info_t *info, char *buffer, size_t *position) {
    size_t current_position = *position;

    if (buffer[current_position] == '|' && buffer[current_position + 1] == '|') {
        buffer[current_position] = 0;
        current_position++;
        info->cmd_buf_type = CMD_OR;
    } else if (buffer[current_position] == '&' && buffer[current_position + 1] == '&') {
        buffer[current_position] = 0;
        current_position++;
        info->cmd_buf_type = CMD_AND;
    } else if (buffer[current_position] == ';') {
        buffer[current_position] = 0;
        info->cmd_buf_type = CMD_CHAIN;
    } else {
        return 0;
    }

    *position = current_position;
    return 1;
}

void check_command_chain(info_t *info, char *buffer, size_t *position, size_t start_position, size_t length) {
    size_t current_position = *position;

    if (info->cmd_buf_type == CMD_AND && info->status) {
        buffer[start_position] = 0;
        current_position = length;
    } else if (info->cmd_buf_type == CMD_OR && !info->status) {
        buffer[start_position] = 0;
        current_position = length;
    }

    *position = current_position;
}

int replace_alias_commands(info_t *info) {
    int i;
    list_t *node;
    char *temp;

    for (i = 0; i < 10; i++) {
        node = node_starts_with(info->alias, info->argv[0], '=');
        if (!node)
            return 0;
        free(info->argv[0]);
        temp = _strchr(node->str, '=');
        if (!temp)
            return 0;
        temp = _strdup(temp + 1);
        if (!temp)
            return 0;
        info->argv[0] = temp;
    }
    return 1;
}

int replace_environment_variables(info_t *info) {
    int i = 0;
    list_t *node;

    for (i = 0; info->argv[i]; i++) {
        if (info->argv[i][0] != '$' || !info->argv[i][1])
            continue;

        if (!_strcmp(info->argv[i], "$?")) {
            replace_string(&(info->argv[i]), _strdup(convert_number_to_string(info->status, 10, 0)));
            continue;
        }

        if (!_strcmp(info->argv[i], "$$")) {
            replace_string(&(info->argv[i]), _strdup(convert_number_to_string(getpid(), 10, 0)));
            continue;
        }

        node = node_starts_with(info->env, &info->argv[i][1], '=');
        if (node) {
            replace_string(&(info->argv[i]), _strdup(_strchr(node->str, '=') + 1));
            continue;
        }
        replace_string(&info->argv[i], _strdup(""));
    }
    return 0;
}

int replace_string_value(char **old_string, char *new_string) {
    free(*old_string);
    *old_string = new_string;
    return 1;
}
