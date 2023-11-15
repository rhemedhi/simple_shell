#ifndef _CUSTOM_SHELL_H_
#define _CUSTOM_SHELL_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

#define READ_BUFFER_SIZE 1024
#define WRITE_BUFFER_SIZE 1024
#define BUFFER_FLUSH -1

#define COMMAND_NORMAL	0
#define COMMAND_OR	1
#define COMMAND_AND	2
#define COMMAND_CHAIN	3

#define USE_GETLINE 0
#define USE_STRTOK  0

#define HISTORY_FILE	".custom_shell_history"
#define HISTORY_MAX	4096

extern char **custom_environ;
typedef struct str_node
{
	int number;
	char *string;
	struct str_node *next;
} str_list;
typedef struct arg_info
{
	char *argument;
	char **arguments_array;
	char *path_string;
	int argument_count;
	unsigned int line_count;
	int error_number;
	int line_count_flag;
	char *file_name;
	str_list *environment_list;
	str_list *command_history;
	str_list *alias_history;
	char **custom_environ;
	int environment_changed;
	int status;

	char **command_buffer;
	int command_buffer_type;
	int read_file_descriptor;
	int history_count;
} info;

#define INFO_INITIALIZATION \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
	0, 0, 0}

typedef struct builtin_command
{
	char *command_type;
	int (*function)(info *);
} builtin_command_table;

int custom_shell(info *);
int find_custom_builtin(info *);
void find_custom_command(info *);
void fork_custom_command(info *);

int custom_loop_shell(char **);

void custom_error_puts(char *);
int custom_error_putchar(char);
int custom_put_fd(char, int);
int custom_puts_fd(char *, int);

int custom_string_length(char *);
int custom_string_compare(char *, char *);
char *starts_with_string(const char *, const char *);
char *string_concat(char *, char *);

char *string_copy(char *, char *);
char *string_duplicate(const char *);
void custom_puts(char *);
int custom_putchar(char);

char *string_copy_n(char *, char *, int);
char *string_concat_n(char *, char *, int);
char *string_character(char *, char);

char **custom_string_to_words(char *, char *);
char **custom_string_to_words_v2(char *, char);

char *memory_set(char *, char, unsigned int);
void free_memory(char **);
void *memory_realloc(void *, unsigned int, unsigned int);

int buffer_free(void **);

int convert_atoi(char *);
int is_delimiter(char, char *);
int is_alphabetic(int);
int custom_atoi(char *);

int error_atoi(char *);
void print_error_info(info *, char *);
int print_decimal(int, int);
char *convert_number_to_string(long int, int, int);
void remove_commented_lines(char *);

int custom_exit(info *);
int custom_change_directory(info *);
int custom_help(info *);

int custom_history(info *);
int custom_alias(info *);

ssize_t get_input_line(info *);
int get_input(info *, char **, size_t *);
void handle_interrupt_signal(int);

void clear_information(info *);
void set_information(info *, char **);
void free_information(info *, int);

char *_get_custom_environment(info *, const char *);
int custom_environment(info *);
int custom_set_environment(info *);
int custom_unset_environment(info *);
int populate_custom_environment_list(info *);

char **get_custom_environment(info *);
int unset_custom_environment(info *, char *);
int set_custom_environment(info *, char *, char *);

char *get_history_file_name(info *);
int write_command_history(info *);
int read_command_history(info *);
int build_command_history_list(info *, char *, int);
int renumber_command_history(info *);

str_list *add_node_to_list(str_list **, const char *, int);
str_list *add_node_to_list_end(str_list **, const char *, int);
size_t print_string_list(const str_list *);
int delete_node_at_index_from_list(str_list **, unsigned int);
void free_string_list(str_list **);

size_t list_length(const str_list *);
char **list_to_string_array(str_list *);
size_t print_integer_list(const str_list *);
str_list *find_node_starts_with(str_list *, char *, char);
ssize_t get_node_index_in_list(str_list *, str_list *);

int is_command_chaining(info *, char *, size_t *);
void check_command_chaining(info *, char *, size_t *, size_t, size_t);
int replace_alias_commands(info *);
int replace_environment_variables(info *);
int replace_string_value(char **, char *);

#endif
