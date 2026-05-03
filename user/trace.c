#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h" // Cần thiết cho MAXARG
#include "user/user.h"    // Cần thiết cho trace(), atoi(), printf()

int
main(int argc, char *argv[])
{
  int i;
  char *nargv[MAXARG];

  if(argc < 3 || (argv[1][0] < '0' || argv[1][0] > '9')){
    fprintf(2, "Usage: %s mask command [args...]\n", argv[0]);
    exit(1);
  }

  // Gọi system call trace
  if (trace(atoi(argv[1])) < 0) {
    fprintf(2, "%s: trace failed\n", argv[0]);
    exit(1);
  }

  // Chuẩn bị danh sách đối số để gọi exec
  for(i = 2; i < argc && i < MAXARG; i++){
    nargv[i-2] = argv[i];
  }
  nargv[i-2] = 0; // Kết thúc mảng bằng null theo quy định của exec

  exec(nargv[0], nargv);
  exit(0);
}   
