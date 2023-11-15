#include "custom_shell.h"

size_t count_nodes(const list_t *head) {
    size_t count = 0;
    const list_t *current = head;

    while (current != NULL) {
        count++;
        current = current->next;
    }

    return count;
}

char **convert_list_to_strings(list_t *head) {
    size_t num_nodes = count_nodes(head);
    char **strings_array = NULL;
    size_t i = 0;

    if (head == NULL || num_nodes == 0)
        return NULL;

    strings_array = malloc(sizeof(char *) * (num_nodes + 1));
    if (strings_array == NULL)
        return NULL;

    while (head != NULL && i < num_nodes) {
        strings_array[i] = strdup(head->str);
        if (strings_array[i] == NULL) {
            for (size_t j = 0; j < i; j++)
                free(strings_array[j]);
            free(strings_array);
            return NULL;
        }
        head = head->next;
        i++;
    }
    strings_array[i] = NULL;

    return strings_array;
}

size_t print_linked_list(const list_t *head) {
    size_t count = 0;
    const list_t *current = head;

    while (current != NULL) {
        _puts(convert_number(current->num, 10, 0));
        _putchar(':');
        _putchar(' ');
        _puts(current->str ? current->str : "(nil)");
        _puts("\n");
        current = current->next;
        count++;
    }

    return count;
}

list_t *find_node_starting_with(list_t *head, char *prefix, char c) {
    while (head != NULL) {
        char *start = starts_with_string(head->str, prefix);
        if (start != NULL && ((c == -1) || (*start == c)))
            return head;
        head = head->next;
    }
    return NULL;
}

ssize_t get_node_index_from_list(list_t *head, list_t *node) {
    ssize_t index = 0;

    while (head != NULL) {
        if (head == node)
            return index;
        head = head->next;
        index++;
    }

    return -1;
}

