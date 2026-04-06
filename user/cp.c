#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

int
main(int argc, char *argv[])
{
  char buf[512]; // Bộ nhớ đệm để copy theo khối (buffer)
  int fd_src, fd_dst;
  int n;

  // 1. Kiểm tra thiếu đối số: báo lỗi nếu không đủ src và dst
  if(argc < 3){
    fprintf(2, "usage: cp src dst\n");
    exit(1);
  }

  // 2. Mở file nguồn (chế độ read-only)
  if((fd_src = open(argv[1], O_RDONLY)) < 0){
    fprintf(2, "cp: cannot open %s\n", argv[1]);
    exit(1);
  }

  // 3. Mở/Tạo file đích (chế độ ghi, tạo mới/ghi đè)
  // O_CREATE: tạo file nếu chưa có, O_WRONLY: chỉ ghi, O_TRUNC: xóa nội dung cũ
  if((fd_dst = open(argv[2], O_CREATE | O_WRONLY | O_TRUNC)) < 0){
    fprintf(2, "cp: cannot open/create %s\n", argv[2]);
    close(fd_src);
    exit(1);
  }

  // 4. Thực hiện copy theo buffer (512 bytes mỗi lần)
  while((n = read(fd_src, buf, sizeof(buf))) > 0){
    if(write(fd_dst, buf, n) != n){
      fprintf(2, "cp: write error\n");
      close(fd_src);
      close(fd_dst);
      exit(1);
    }
  }

  // Báo lỗi nếu quá trình đọc bị thất bại
  if(n < 0){
    fprintf(2, "cp: read error\n");
  }

  // Đóng các file sau khi hoàn tất để giải phóng tài nguyên
  close(fd_src);
  close(fd_dst);
  exit(0);
}