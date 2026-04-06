#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

int show_all = 0;
int summary_only = 0;


long du(char* path) {
  char buf[512], * p;
  int fd;
  struct dirent de;
  struct stat st;
  long total_size = 0;

  if ((fd = open(path, 0)) < 0) {
    fprintf(2, "du: cannot open %s\n", path);
    return 0;
  }

  if (fstat(fd, &st) < 0) {
    fprintf(2, "du: cannot stat %s\n", path);
    close(fd);
    return 0;
  }

  
  if (st.type == T_FILE) {
    total_size = st.size;
    if (show_all && !summary_only) {
      printf("%l\t%s\n", total_size, path);
    }
    close(fd);
    return total_size;
  }

  
  if (st.type == T_DIR) {
    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
      printf("du: path too long\n");
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

        /
        total_size += du(buf);
      }
    }
  }

  
  if (!summary_only) {
    printf("%l\t%s\n", total_size, path);
  }

  close(fd);
  return total_size;
}

int main(int argc, char* argv[]) {
  char* path = ".";
  int i;

  
  for (i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-a") == 0) {
      show_all = 1;
    }
    else if (strcmp(argv[i], "-s") == 0) {
      summary_only = 1;
    }
    else {
      path = argv[i];
    }
  }

  long total = du(path);

  
  if (summary_only) {
    printf("%l\t%s\n", total, path);
  }

  exit(0);
}
