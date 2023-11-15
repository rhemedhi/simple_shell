#include "custom_shell.h"

int free_and_null_pointer(void **pointer_address) {
    if (pointer_address && *pointer_address) {
        free(*pointer_address);
        *pointer_address = NULL;
        return 1;
    }
    return 0;
}
