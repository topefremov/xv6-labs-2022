#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "kernel/sysinfo.h"

int
main(int argc, char *argv[])
{
  char *a;
  int fd, n;

  a = sbrk(4096);
  fd = open("sbrk", O_CREATE|O_WRONLY);
  unlink("sbrk");
  if(fd < 0)  {
    printf("open sbrk failed\n");
    exit(1);
  }
  struct sysinfo info;
  printf("let's write from %p\n", &a);
  sysinfo(&info);
  info.freemem = 0;
  if ((n = write(fd, a, 4096)) < 0) {
    printf("write sbrk failed\n");
    exit(1);
  }
  close(fd);
  return 0;
}
