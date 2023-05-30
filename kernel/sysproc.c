#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "sysinfo.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  // don't grow above PLIC
  if ((addr + n) >= PLIC)
    return -1;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;


  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}


#ifdef LAB_PGTBL
int
sys_pgaccess(void)
{
  // starting virtual address
  uint64 va;
  // number of pages to check
  int len;
  // buffer to store the results into a bitmask
  uint32 buf = 0;
  argaddr(0, &va);
  argint(1, &len);
  if(len < 0 && len > 32)
    return -1;

  pte_t *pte;
  pagetable_t pagetable = myproc()->pagetable;
  for(int i = 0; i < len; i++){
    if((pte = walk(pagetable, va, 0)) != 0 && (*pte & PTE_A)){
      buf |= 1 << i;
      // clear access bit
      *pte &= ~PTE_A;
    }
    va += PGSIZE;
  }

  // address of to a user buffer
  uint64 bufa;
  argaddr(2, &bufa);

  if(copyout(pagetable, bufa, (char*)&buf, sizeof(buf)) < 0)
    return -1;

  return 0;
}
#endif

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64
sys_sysinfo(void)
{
  struct proc *p = myproc();
  struct sysinfo info;
  info.freemem = collect_freemem();
  info.nproc = collect_unused_procs();
  uint64 addr; // user pointer to struct sysinfo
  argaddr(0, &addr);
  if(copyout(p->pagetable, addr, (char *)&info, sizeof(info)) < 0)
    return -1;
  return 0;
}
