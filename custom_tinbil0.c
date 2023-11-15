#include "custom_shell.h"

int display_history(info_t *info) {
    list_t *current = info->history;
    while (current) {
        printf("%d %s\n", current->num, current->str);
        current = current->next;
    }
    return 0;
}

int remove_alias(info_t *info, char *str) {
    char *equal_sign_position;
    int ret;

    equal_sign_position = strchr(str, '=');
    if (!equal_sign_position)
        return 1;

    *equal_sign_position = '\0';
    ret = delete_node_at_index(&(info->alias), get_node_index(info->alias, node_starts_with(info->alias, str, -1)));
    *equal_sign_position = '=';
    
    return ret;
}

int set_new_alias(info_t *info, char *str) {
    char *equal_sign_position;

    equal_sign_position = strchr(str, '=');
    if (!equal_sign_position)
        return 1;

    if (!*++equal_sign_position)
        return remove_alias(info, str);

    remove_alias(info, str);
    return (add_node_end(&(info->alias), str, 0) == NULL);
}

int print_single_alias(list_t *node) {
    char *equal_sign_position, *alias;

    if (node) {
        equal_sign_position = strchr(node->str, '=');
        for (alias = node->str; alias <= equal_sign_position; alias++)
            putchar(*alias);
        putchar('\'');
        printf("%s'\n", equal_sign_position + 1);
        return 0;
    }
    return 1;
}

int manage_alias(info_t *info) {
    int i;
    char *equal_sign_position;

    if (info->argc == 1) {
        list_t *current_alias = info->alias;
        while (current_alias) {
            print_single_alias(current_alias);
            current_alias = current_alias->next;
        }
        return 0;
    }

    for (i = 1; info->argv[i]; i++) {
        equal_sign_position = strchr(info->argv[i], '=');
        if (equal_sign_position)
            set_new_alias(info, info->argv[i]);
        else
            print_single_alias(node_starts_with(info->alias, info->argv[i], '='));
    }

    return 0;
}

