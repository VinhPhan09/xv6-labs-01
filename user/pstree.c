#include "kernel/types.h"
#include "kernel/ptree.h"
#include "user/user.h"

#define MAXP 64

void
print_tree(struct ptreeinfo* buf, int n, int ppid, int depth)
{
  for (int i = 0; i < n; i++) {
    if (buf[i].ppid == ppid) {
      for (int d = 0; d < depth; d++)
        printf("  ");
      printf("%d %s state=%d mem=%d\n",
        buf[i].pid,
        buf[i].name,
        buf[i].state,
        buf[i].memsize);
      print_tree(buf, n, buf[i].pid, depth + 1);
    }
  }
}

int
main(void)
{
  struct ptreeinfo buf[MAXP];
  int n = ptree(buf, MAXP);

  if (n < 0) {
    printf("pstree: ptree failed\n");
    exit(1);
  }

  print_tree(buf, n, 0, 0);
  exit(0);
}
