#include "custom_shell.h"

int custom_exit(info_t *info) {
    int exit_status = 0;

    if (info->argv[1]) {
        exit_status = atoi(info->argv[1]);
        if (exit_status == 0 && info->argv[1][0] != '0') {
            info->status = 2;
            print_error(info, "Illegal number: ");
            _eputs(info->argv[1]);
            _eputchar('\n');
            return 1;
        }
        info->err_num = exit_status;
        return -2;
    }

    info->err_num = -1;
    return -2;
}

int custom_change_directory(info_t *info) {
    char *current_directory, *new_directory;
    int chdir_result;

    current_directory = getcwd(NULL, 0);
    if (!current_directory)
        _puts("Failed to get current directory\n");

    if (!info->argv[1]) {
        new_directory = _getenv(info, "HOME");
        if (!new_directory)
            chdir_result = chdir((new_directory = _getenv(info, "PWD")) ? new_directory : "/");
        else
            chdir_result = chdir(new_directory);
    } else if (_strcmp(info->argv[1], "-") == 0) {
        if (!_getenv(info, "OLDPWD")) {
            _puts(current_directory);
            _putchar('\n');
            return 1;
        }
        _puts(_getenv(info, "OLDPWD"));
        _putchar('\n');
        chdir_result = chdir((new_directory = _getenv(info, "OLDPWD")) ? new_directory : "/");
    } else {
        chdir_result = chdir(info->argv[1]);
    }

    if (chdir_result == -1) {
        print_error(info, "can't change directory to ");
        _eputs(info->argv[1]);
        _eputchar('\n');
    } else {
        _setenv(info, "OLDPWD", _getenv(info, "PWD"));
        _setenv(info, "PWD", getcwd(NULL, 0));
    }

    free(current_directory);
    return 0;
}

int custom_help(info_t *info) {
    _puts("Help function is not yet implemented\n");
    return 0;
}

