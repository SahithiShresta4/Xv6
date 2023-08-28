#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int
sys_draw(void)
{
  void* buf;
  uint size;

  argptr(0, (void*)&buf, sizeof(buf));
  argptr(1, (void*)&size, sizeof(size));

  char text[] = ".d88b.  d8888b. d88888b d8888b.  .d8b.  d888888b d888888b d8b   db  d888b       .d8888. db    db .d8888. d888888b d88888b .88b  d88. \n
		.8P  Y8. 88   8D 88      88   8D d8   8b  ~~88~~     88    888o  88 88  Y8b      88   YP  8b  d8  88   YP  ~~88~~  88      88 YbdP 88\n 
		88    88 88oodD  88ooooo 88oobY  88ooo88    88       88    88V8o 88 88            8bo.     8bd8    8bo.      88    88ooooo 88  88  88\n 
		88    88 88~~~   88~~~~~ 88`8b   88~~~88    88       88    88 V8o88 88  ooo         Y8b.    88       Y8b.    88    88~~~~~ 88  88  88\n 
		8b  d8   88      88.     88 `88. 88   88    88      .88.   88  V888 88. ~8~      db   8D    88    db   8D    88    88.     88  88  88\n 
 		 Y88P    88      Y88888P 88   YD YP   YP    YP    Y888888P VP   V8P  Y888P       `8888Y'    YP    `8888Y'    YP    Y88888P YP  YP  YP\n";
  if(sizeof(text)>size)
    return -1;
    

  strncpy((char *)buf, text, size);
  return sizeof(text);
}
