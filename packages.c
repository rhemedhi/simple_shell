#include "custom_shell.h"

list_t *create_node(const char *str, int num) {
    list_t *new_node = malloc(sizeof(list_t));
    if (new_node == NULL) {
        return NULL;
    }

    new_node->num = num;
    new_node->str = str ? strdup(str) : NULL;
    new_node->next = NULL;

    return new_node;
}

list_t *add_node(list_t **head, const char *str, int num) {
    if (!head) {
        return NULL;
    }

    list_t *new_node = create_node(str, num);
    if (new_node == NULL) {
        return NULL;
    }

    new_node->next = *head;
    *head = new_node;

    return new_node;
}

list_t *add_node_end(list_t **head, const char *str, int num) {
    if (!head) {
        return NULL;
    }

    list_t *new_node = create_node(str, num);
    if (new_node == NULL) {
        return NULL;
    }

    if (*head == NULL) {
        *head = new_node;
    } else {
        list_t *temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_node;
    }

    return new_node;
}

size_t print_list_str(const list_t *head) {
    size_t size = 0;
    while (head != NULL) {
        _puts(head->str ? head->str : "(nil)");
        _puts("\n");
        head = head->next;
        size++;
    }
    return size;
}

int delete_node_at_index(list_t **head, unsigned int index) {
    if (!head || !*head) {
        return 0;
    }

    if (index == 0) {
        list_t *temp = *head;
        *head = (*head)->next;
        free(temp->str);
        free(temp);
        return 1;
    }

    list_t *prev = NULL;
    list_t *current = *head;
    unsigned int i = 0;

    while (current != NULL && i < index) {
        prev = current;
        current = current->next;
        i++;
    }

    if (current == NULL) {
        return 0;
    }

    prev->next = current->next;
    free(current->str);
    free(current);
    return 1;
}

void free_list(list_t **head) {
    if (!head || !*head) {
        return;
    }

    list_t *current = *head;
    list_t *next = NULL;

    while (current != NULL) {
        next = current->next;
        free(current->str);
        free(current);
        current = next;
    }

    *head = NULL;
}

