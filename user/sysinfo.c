#include "kernel/types.h"
#include "kernel/riscv.h"
#include "kernel/sysinfo.h"
#include "user/user.h"


void
sinfo(struct sysinfo *info) {
  if (sysinfo(info) < 0) {
    printf("FAIL: sysinfo failed");
    exit(1);
  }
}

int
main(int argc, char *argv[])
{
  struct sysinfo info;
  sinfo(&info);
  printf("Free mem: %d\n", info.freemem);    
  return 0;
}
