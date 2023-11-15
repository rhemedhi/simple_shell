#include "custom_shell.h"

void initialize_info(info_t *info) {
    info->arg = NULL;
    info->argv = NULL;
    info->path = NULL;
    info->argc = 0;
}

void configure_info(info_t *info, char **argument_vector) {
    int i = 0;

    info->fname = argument_vector[0];
    if (info->arg != NULL) {
        info->argv = custom_string_to_words(info->arg, " \t");
        if (info->argv == NULL) {
            info->argv = (char **)malloc(sizeof(char *) * 2);
            if (info->argv != NULL) {
                info->argv[0] = string_duplicate(info->arg);
                info->argv[1] = NULL;
            }
        }

        for (i = 0; info->argv && info->argv[i] != NULL; i++);
        info->argc = i;

        replace_alias_commands(info);
        replace_environment_variables(info);
    }
}

void release_info(info_t *info, int free_all_fields) {
    free_memory(info->argv);
    info->argv = NULL;
    info->path = NULL;

    if (free_all_fields) {
        if (info->cmd_buf == NULL)
            free_memory(info->arg);

        if (info->env != NULL)
            free_string_list(&(info->env));

        if (info->history != NULL)
            free_string_list(&(info->history));

        if (info->alias != NULL)
            free_string_list(&(info->alias));

        free_memory((char **)info->environ);
        info->environ = NULL;

        buffer_free((void **)info->cmd_buf);

        if (info->read_file_descriptor > 2)
            close(info->read_file_descriptor);

        custom_putchar(BUFFER_FLUSH);
    }
}

