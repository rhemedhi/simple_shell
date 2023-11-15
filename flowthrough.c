#include "custom_shell.h"

int custom_shell_loop(info_t *info, char **arguments) {
    ssize_t read_status = 0;
    int builtin_return = 0;

    while (read_status != -1 && builtin_return != -2) {
        clear_information(info);

        if (is_interactive_mode(info))
            custom_puts("$ ");

        custom_error_putchar(BUF_FLUSH);

        read_status = get_input(info);

        if (read_status != -1) {
            set_information(info, arguments);
            builtin_return = find_custom_builtin(info);

            if (builtin_return == -1)
                find_custom_command(info);
        } else if (is_interactive_mode(info))
            custom_putchar('\n');

        free_information(info, 0);
    }

    write_command_history(info);
    free_information(info, 1);

    if (!is_interactive_mode(info) && info->status)
        exit(info->status);

    if (builtin_return == -2) {
        if (info->error_number == -1)
            exit(info->status);
        exit(info->error_number);
    }

    return (builtin_return);
}

int find_custom_builtin(info_t *info) {
    int i, builtin_ret = -1;
    builtin_command_table custom_builtin_tbl[] = {
        {"exit", custom_exit},
        {"env", custom_environment},
        {"help", custom_help},
        {"history", custom_history},
        {"setenv", custom_set_environment},
        {"unsetenv", custom_unset_environment},
        {"cd", custom_change_directory},
        {"alias", custom_alias},
        {NULL, NULL}
    };

    for (i = 0; custom_builtin_tbl[i].command_type; i++) {
        if (custom_string_compare(info->argv[0], custom_builtin_tbl[i].command_type) == 0) {
            info->line_count++;
            builtin_ret = custom_builtin_tbl[i].function(info);
            break;
        }
    }

    return (builtin_ret);
}

void find_custom_command(info_t *info) {
    char *command_path = NULL;
    int i, count_non_delimiters = 0;

    info->path = info->argv[0];

    if (info->line_count_flag == 1) {
        info->line_count++;
        info->line_count_flag = 0;
    }

    for (i = 0; info->argument[i]; i++) {
        if (!is_delimiter_character(info->argument[i], " \t\n")) {
            count_non_delimiters++;
        }
    }

    if (!count_non_delimiters)
        return;

    command_path = find_path(info, _get_custom_environment(info, "PATH="), info->argv[0]);

    if (command_path) {
        info->path = command_path;
        fork_custom_command(info);
    } else {
        if ((is_interactive_mode(info) || _get_custom_environment(info, "PATH=") || info->argv[0][0] == '/') && is_command(info, info->argv[0]))
            fork_custom_command(info);
        else if (*(info->argument) != '\n') {
            info->status = 127;
            print_error_info(info, "not found\n");
        }
    }
}

void fork_custom_command(info_t *info) {
    pid_t child_pid = fork();

    if (child_pid == -1) {
        perror("Error:");
        return;
    }

    if (child_pid == 0) {
        if (execve(info->path, info->argv, get_custom_environment(info)) == -1) {
            free_information(info, 1);
            if (errno == EACCES)
                exit(126);
            exit(1);
        }
    } else {
        wait(&(info->status));

        if (WIFEXITED(info->status)) {
            info->status = WEXITSTATUS(info->status);
            if (info->status == 126)
                print_error_info(info, "Permission denied\n");
        }
    }
}

