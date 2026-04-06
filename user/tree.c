#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"


char* fmtname(char* path) {
  char* p;
  for (p = path + strlen(path); p >= path && *p != '/'; p--);
  p++;
  return p;
}

void tree(char* path, int level, int maxDepth, int onlyDir) {
  char buf[512], * p;
  int fd;
  struct dirent de;
  struct stat st;

  if ((fd = open(path, 0)) < 0) {
    fprintf(2, "tree: cannot open %s\n", path);
    return;
  }

  if (fstat(fd, &st) < 0) {
    fprintf(2, "tree: cannot stat %s\n", path);
    close(fd);
    return;
  }

  
  if (onlyDir && st.type == T_FILE) {
    close(fd);
    return;
  }

  
  for (int i = 0; i < level; i++) {
    printf("  ");
  }
  if (level > 0) printf("|-- ");
  printf("%s\n", fmtname(path));

 
  if (st.type == T_FILE || (maxDepth != -1 && level >= maxDepth)) {
    close(fd);
    return;
  }

  
  if (st.type == T_DIR) {
    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
      printf("tree: path too long\n");
    }
    else {
      strcpy(buf, path);
      p = buf + strlen(buf);
      *p++ = '/';
      while (read(fd, &de, sizeof(de)) == sizeof(de)) {
        if (de.inum == 0) continue;
        
        if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) continue;

        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        tree(buf, level + 1, maxDepth, onlyDir);
      }
    }
  }
  close(fd);
}

int main(int argc, char* argv[]) {
  char* path = ".";
  int maxDepth = -1; 
  int onlyDir = 0;

  
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-d") == 0) {
      onlyDir = 1;
    }
    else if (strcmp(argv[i], "-L") == 0 && i + 1 < argc) {
      maxDepth = atoi(argv[++i]);
    }
    else {
      path = argv[i];
    }
  }

  tree(path, 0, maxDepth, onlyDir);
  exit(0);
}
