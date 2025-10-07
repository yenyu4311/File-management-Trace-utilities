#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define BUFSIZE 1024

int main(int argc, char *argv[]) {
    int force = 0;   // 是否覆寫
    char *src_file;
    char *dest_file;

    // 解析參數
    if (argc == 4 && strcmp(argv[1], "-f") == 0) {
        force = 1;
        src_file = argv[2];
        dest_file = argv[3];
    } else if (argc == 3) {
        src_file = argv[1];
        dest_file = argv[2];
    } else {
        fprintf(stderr, "Usage: %s [ -f ] source-file dest-file\n", argv[0]);
        return 1;
    }

    // 開啟來源檔案
    int fdin = open(src_file, O_RDONLY);
    if (fdin < 0) {
        perror("open source-file");
        return 1;
    }

    // 開啟目標檔案
    int flags = O_WRONLY | O_CREAT;
    if (force) {
        flags |= O_TRUNC; // 覆寫
    } else {
        flags |= O_EXCL;  // 不覆寫，檔案存在會失敗
    }

    int fdout = open(dest_file, flags, 0644); // 權限 rw-r--r--
    if (fdout < 0) {
        perror("open dest-file");
        close(fdin);
        return 1;
    }

    // 複製檔案內容
    char buf[BUFSIZE];
    ssize_t n;
    while ((n = read(fdin, buf, BUFSIZE)) > 0) {
        if (write(fdout, buf, n) != n) {
            perror("write");
            close(fdin);
            close(fdout);
            return 1;
        }
    }
    if (n < 0) {
        perror("read");
    }

    close(fdin);
    close(fdout);
    return 0;
}
