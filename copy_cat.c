#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int filecopy(int infd, int outfd, int bufsize) {
    char *buffer = (char *)malloc(bufsize);
    if (buffer == NULL) {
        perror("malloc");
        return -1;
    }

    ssize_t n;
    while ((n = read(infd, buffer, bufsize)) > 0) {
        if (write(outfd, buffer, n) != n) {
            perror("write");
            free(buffer);
            return -1;
        }
    }

    if (n < 0) {
        perror("read");
        free(buffer);
        return -1;
    }

    free(buffer);
    return 0;
}

int main(int argc, char *argv[]) {
    int fd, ret;

    if (argc == 1) {
        // 無參數：stdin → stdout
        ret = filecopy(STDIN_FILENO, STDOUT_FILENO, 1024);
    } else if (argc == 2) {
        // 一個檔案參數：開檔讀取
        fd = open(argv[1], O_RDONLY);
        if (fd < 0) {
            perror(argv[1]);
            return 1;
        }
        ret = filecopy(fd, STDOUT_FILENO, 1024);
        close(fd);
    } else {
        fprintf(stderr, "Usage: %s [file]\n", argv[0]);
        return 1;
    }

    if (ret < 0) {
        fprintf(stderr, "Copy failed.\n");
        return 1;
    }

    return 0;
}
