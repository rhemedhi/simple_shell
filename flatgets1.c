#include "custom_shell.h"

char **get_environment(info_t *info) {
    if (!info->environ || info->env_changed) {
        info->environ = convert_list_to_strings(info->env);
        info->env_changed = 0;
    }
    return info->environ;
}

int unset_environment_variable(info_t *info, char *variable) {
    list_t *current_node = info->env;
    size_t index = 0;
    char *match;

    if (!current_node || !variable)
        return 0;

    while (current_node) {
        match = starts_with_string(current_node->str, variable, '=');
        if (match && *match == '=') {
            info->env_changed = delete_node_at_index_from_list(&(info->env), index);
            index = 0;
            current_node = info->env;
            continue;
        }
        current_node = current_node->next;
        index++;
    }
    return info->env_changed;
}

int set_environment_variable(info_t *info, char *variable, char *value) {
    char *buffer = NULL;
    list_t *current_node;
    char *match;

    if (!variable || !value)
        return 0;

    buffer = malloc(strlen(variable) + strlen(value) + 2);
    if (!buffer)
        return 1;

    strcpy(buffer, variable);
    strcat(buffer, "=");
    strcat(buffer, value);

    current_node = info->env;
    while (current_node) {
        match = starts_with_string(current_node->str, variable, '=');
        if (match && *match == '=') {
            free(current_node->str);
            current_node->str = buffer;
            info->env_changed = 1;
            return 0;
        }
        current_node = current_node->next;
    }

    add_node_to_list_end(&(info->env), buffer, 0);
    free(buffer);
    info->env_changed = 1;
    return 0;
}

