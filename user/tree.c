#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

// Hàm in thụt lề dựa trên độ sâu (level)
// Cải tiến hàm in thụt lề để có đường kẻ nối tiếp
void print_indent(int level, int is_last) {
    for (int i = 0; i < level - 1; i++) {
        printf("|   "); 
    }
    if (level > 0) {
        printf("|-- "); // Dùng |-- để nối vào tên file/thư mục
    }
}

void tree(char *path, int level, int max_depth, int only_dir) {
    int fd;
    struct dirent de;
    struct stat st;
    char buf[512], *p;

    if ((fd = open(path, 0)) < 0) return;
    if (fstat(fd, &st) < 0 || st.type != T_DIR) {
        close(fd);
        return;
    }

    if (max_depth >= 0 && level >= max_depth) {
        close(fd);
        return;
    }

    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
        if (de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
            continue;

        // Tạo path mới
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;

        struct stat st_inner;
        if (stat(buf, &st_inner) < 0) continue;
        if (only_dir && st_inner.type != T_DIR) continue;

        // IN RA THEO ĐÚNG FORMAT CÂY
        for (int i = 0; i < level; i++) printf("|   ");
        printf("|-- %s\n", de.name);

        // Đệ quy
        if (st_inner.type == T_DIR) {
            tree(buf, level + 1, max_depth, only_dir);
        }
    }
    close(fd);
}

int main(int argc, char *argv[]) {
    char *path = ".";
    int max_depth = -1; // Mặc định không giới hạn
    int only_dir = 0;

    // Xử lý các flag (đơn giản)
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0) {
            only_dir = 1;
        } else if (strcmp(argv[i], "-L") == 0 && i + 1 < argc) {
            max_depth = atoi(argv[++i]);
        } else {
            path = argv[i];
        }
    }

    printf("%s\n", path);
    tree(path, 0, max_depth, only_dir);
    
    exit(0);
}