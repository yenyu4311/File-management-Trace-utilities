#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <libgen.h> // for basename()

#define BUFSIZE 1024

void copy_file(const char *src, const char *dest, int force) {
    int fdin, fdout;
    char buf[BUFSIZE];
    ssize_t n;

    fdin = open(src, O_RDONLY);
    if (fdin < 0) {
        perror(src);
        return;
    }

    int flags = O_WRONLY | O_CREAT;
    if (force)
        flags |= O_TRUNC;
    else
        flags |= O_EXCL;

    fdout = open(dest, flags, 0644);
    if (fdout < 0) {
        perror(dest);
        close(fdin);
        return;
    }

    while ((n = read(fdin, buf, BUFSIZE)) > 0) {
        if (write(fdout, buf, n) != n) {
            perror("write");
            break;
        }
    }
    if (n < 0)
        perror("read");

    close(fdin);
    close(fdout);
}

int main(int argc, char *argv[]) {
    int force = 0;
    int start = 1;

    // parse -f
    if (argc > 1 && strcmp(argv[1], "-f") == 0) {
        force = 1;
        start = 2;
    }

    if (argc - start < 2) {
        fprintf(stderr, "Usage: %s [ -f ] source-file [source2 ...] dest-file\n", argv[0]);
        exit(1);
    }

    char *dest = argv[argc - 1];
    struct stat st;

    // 檢查 dest 是否為目錄
    if (stat(dest, &st) == 0 && S_ISDIR(st.st_mode)) {
        // 多檔複製模式
        for (int i = start; i < argc - 1; i++) {
            char newpath[PATH_MAX];
            snprintf(newpath, sizeof(newpath), "%s/%s", dest, basename(argv[i]));
            copy_file(argv[i], newpath, force);
        }
    } else {
        // 單檔複製模式
        if (argc - start != 2) {
            fprintf(stderr, "Error: destination must be a directory when copying multiple files.\n");
            exit(1);
        }
        copy_file(argv[start], dest, force);
    }

    return 0;
}
