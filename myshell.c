#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>

#define MAX_COMMANDS 100
#define MAX_COMMAND_LENGTH 100
#define MAX_HISTORY 100

char *history[MAX_HISTORY];
int history_count = 0;

void add_to_history(const char *command) {
    if (history_count < MAX_HISTORY) {
        history[history_count++] = strdup(command);
    } else {
        free(history[0]);
        memmove(history, history + 1, (MAX_HISTORY - 1) * sizeof(char *));
        history[MAX_HISTORY - 1] = strdup(command);
    }
}

void show_history() {
    for (int i = 0; i < history_count; i++) {
        printf("%s\n", history[i]);
    }
}

void convert_windows_to_wsl_path(char *path) {
    if (path[1] == ':' && path[2] == '\\') {
        path[0] = '/';
        path[1] = 'm';
        path[2] = 'n';
        path[3] = 't';
        path[4] = '/';
        path[5] = tolower(path[0]);
        for (int i = 6; path[i-1] != '\0'; i++) {
            if (path[i] == '\\') {
                path[i] = '/';
            } else {
                path[i] = tolower(path[i]);
            }
        }
    }
}

void change_directory(char *path) {
    char wsl_path[MAX_COMMAND_LENGTH];
    strcpy(wsl_path, path);
    convert_windows_to_wsl_path(wsl_path);
    if (chdir(wsl_path) == -1) {
        perror("chdir failed");
    }
}


void print_working_directory() {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
    } else {
        perror("getcwd failed");
    }
}

void execute_command(char **args) {
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        if (execvp(args[0], args) == -1) {
            perror("exec failed");
            exit(EXIT_FAILURE);
        }
    } else {
        wait(NULL);
    }
}

void parse_command(char *input, char **args) {
    int i = 0;
    char *token;
    int in_quote = 0;
    int arg_start = 0;
    char buffer[MAX_COMMAND_LENGTH];
    int buffer_pos = 0;

    for (int j = 0; input[j] != '\0'; j++) {
        if (input[j] == '\"') {
            in_quote = !in_quote;
            continue;
        }

        if (input[j] == ' ' && !in_quote) {
            buffer[buffer_pos] = '\0';
            args[i++] = strdup(buffer);
            buffer_pos = 0;
            if (i == 1 && strcmp(args[0], "cd") == 0) {
                args[i++] = strdup(input + j + 1);
                break;
            }
        } else {
            buffer[buffer_pos++] = input[j];
        }
    }

    if (buffer_pos > 0) {
        buffer[buffer_pos] = '\0';
        args[i++] = strdup(buffer);
    }

    args[i] = NULL;
}


void shell_loop(char **directories, int dir_count) {
    char input[MAX_COMMAND_LENGTH];
    char *args[MAX_COMMANDS];

    while (1) {
        printf("$ ");
        fflush(stdout);
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = '\0';
        if (strlen(input) == 0) {
            continue;
        }

        add_to_history(input);

        parse_command(input, args);

        if (strcmp(args[0], "exit") == 0) {
            break;
        } else if (strcmp(args[0], "cd") == 0) {
            if (args[1] == NULL) {
                fprintf(stderr, "cd: expected argument\n");
            } else {
                change_directory(args[1]);
            }
        } else if (strcmp(args[0], "pwd") == 0) {
            print_working_directory();
        } else if (strcmp(args[0], "history") == 0) {
            show_history();
        } else {
            execute_command(args);
        }
    }
}

int main(int argc, char *argv[]) {
    shell_loop(argv + 1, argc - 1);
    return 0;
}
