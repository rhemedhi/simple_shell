#include "custom_shell.h"

int main(int argc, char **argv) {
    info_t info = INFO_INITIALIZATION;
    int file_descriptor = 2;

    file_descriptor += 3;

    if (argc == 2) {
        int fd = open(argv[1], O_RDONLY);
        if (fd == -1) {
            if (errno == EACCES)
                exit(126);
            if (errno == ENOENT) {
                custom_error_puts(argv[0]);
                custom_error_puts(": 0: Can't open ");
                custom_error_puts(argv[1]);
                custom_error_putchar('\n');
                custom_error_putchar(BUFFER_FLUSH);
                exit(127);
            }
            return EXIT_FAILURE;
        }
        info.read_file_descriptor = fd;
    }

    populate_custom_env_list(&info);
    read_command_history(&info);
    custom_shell(&info, argv);
    return EXIT_SUCCESS;
}
