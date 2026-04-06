#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int readline(int fd, char* buf, int max) {
  int n = 0;
  char c;

  while (n < max - 1) {
    if (read(fd, &c, 1) < 1) {
      if (n == 0) return 0;
      break;
    }
    if (c == '\n') break;
    buf[n++] = c;
  }
  buf[n] = '\0';
  return n + 1;
}

void diff(char* file1, char* file2, int quiet) {
  int fd1, fd2;
  char buf1[512], buf2[512];
  int n1, n2;
  int line = 1;

  if ((fd1 = open(file1, 0)) < 0) exit(1);
  if ((fd2 = open(file2, 0)) < 0) {
    close(fd1);
    exit(1);
  }

  while (1) {
    n1 = readline(fd1, buf1, sizeof(buf1));
    n2 = readline(fd2, buf2, sizeof(buf2));

    if (n1 == 0 && n2 == 0) break;

    char* s1 = (n1 == 0) ? "EOF" : buf1;
    char* s2 = (n2 == 0) ? "EOF" : buf2;

    if (strcmp(s1, s2) != 0) {
      if (quiet) {
        printf("diff: files differ\n");
        close(fd1);
        close(fd2);
        return;
      }
      printf("%s:%d: < %s\n", file1, line, s1);
      printf("%s:%d: > %s\n", file2, line, s2);
    }
    line++;
  }

  close(fd1);
  close(fd2);
}

int main(int argc, char* argv[]) {
  int quiet = 0;
  char* f1 = 0, * f2 = 0;

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-q") == 0) {
      quiet = 1;
    }
    else if (f1 == 0) {
      f1 = argv[i];
    }
    else if (f2 == 0) {
      f2 = argv[i];
    }
  }

  if (f1 && f2) {
    diff(f1, f2, quiet);
  }
  exit(0);
}
