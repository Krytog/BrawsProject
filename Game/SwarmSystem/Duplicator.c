#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUF_SIZE 1024

int main(int argc, char* argv[]) {
    int pipefd1[2], pipefd2[2];
    pid_t pid1, pid2;
    char buf[BUF_SIZE];
    int status;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s path1 path2n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (pipe(pipefd1) == -1 || pipe(pipefd2) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid1 = fork();
    if (pid1 == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid1 == 0) {
        close(pipefd1[0]);
        close(pipefd2[1]);
        if (dup2(pipefd1[1], STDOUT_FILENO) == -1 || dup2(pipefd2[0], STDIN_FILENO) == -1) {
            perror("dup2");
            exit(EXIT_FAILURE);
        }
        execl(argv[1], argv[1], NULL);
        perror("execl");
        exit(EXIT_FAILURE);
    }

    pid2 = fork();
    if (pid2 == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid2 == 0) {
        close(pipefd1[1]);
        close(pipefd2[0]);
        if (dup2(pipefd1[0], STDIN_FILENO) == -1 || dup2(pipefd2[1], STDOUT_FILENO) == -1) {
            perror("dup2");
            exit(EXIT_FAILURE);
        }
        execl(argv[2], argv[2], NULL);
        perror("execl");
        exit(EXIT_FAILURE);
    }

    close(pipefd1[0]);
    close(pipefd1[1]);

    waitpid(pid1, &status, 0);
    waitpid(pid2, &status, 0);
    return 0;
}