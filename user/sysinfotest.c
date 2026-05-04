#include "kernel/types.h"
#include "kernel/riscv.h"
#include "kernel/sysinfo.h"
#include "user/user.h"

void
testmem()
{
  struct sysinfo info;
  
  printf("Testing sysinfo (memory)...\n");
  if (sysinfo(&info) < 0) {
    printf("sysinfotest: sysinfo failed\n");
    exit(1);
  }

  // Kiểm tra bộ nhớ trống phải lớn hơn 0
  if (info.freemem <= 0) {
    printf("sysinfotest: freemem is not positive\n");
    exit(1);
  }
  
  // Sửa từ %d thành %ld để khớp với uint64
  printf("Free memory: %ld bytes\n", info.freemem);
}

void
testproc()
{
  struct sysinfo info;
  
  printf("Testing sysinfo (processes)...\n");
  if (sysinfo(&info) < 0) {
    printf("sysinfotest: sysinfo failed\n");
    exit(1);
  }

  // Tối thiểu phải có tiến trình init và shell
  if (info.nproc < 2) {
    printf("sysinfotest: nproc is too low\n");
    exit(1);
  }
  
  // Sửa từ %d thành %ld để khớp với uint64
  printf("Number of processes: %ld\n", info.nproc);
}

int
main(int argc, char *argv[])
{
  testmem();
  testproc();
  
  printf("sysinfotest: OK\n");
  exit(0);
}