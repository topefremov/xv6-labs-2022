#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int *p;
  p = malloc(sizeof(int));
  *p = 10;
  printf("Val: %d\n", *p);
  printf("Address: %p\n", p);
  exit(0);
}
