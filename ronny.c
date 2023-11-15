#include "custom_shell.h"

int print_environment(info_t *info) {
    print_string_list(info->environment_list);
    return 0;
}

char *get_environment_variable(info_t *info, const char *name) {
    str_list *current_node = info->environment_list;
    char *variable_value;

    while (current_node) {
        variable_value = starts_with_string(current_node->string, name);
        if (variable_value && *variable_value)
            return variable_value;
        current_node = current_node->next;
    }
    return NULL;
}

int set_environment_variable(info_t *info) {
    if (info->argument_count != 3) {
        custom_error_puts("Incorrect number of arguments\n");
        return 1;
    }
    if (set_custom_environment(info, info->arguments_array[1], info->arguments_array[2]))
        return 0;
    return 1;
}

int unset_environment_variable(info_t *info) {
    int i;

    if (info->argument_count == 1) {
        custom_error_puts("Too few arguments.\n");
        return 1;
    }

    for (i = 1; i <= info->argument_count; i++)
        unset_custom_environment(info, info->arguments_array[i]);

    return 0;
}

int populate_environment_list(info_t *info) {
    str_list *node = NULL;
    size_t i;

    for (i = 0; custom_environ[i]; i++)
        add_node_to_list_end(&node, custom_environ[i], 0);

    info->environment_list = node;
    return 0;
}

