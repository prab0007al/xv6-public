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

//returns numver of virtual pages
int 
sys_numvp(void){
	struct proc *currproc = myproc();
	uint sz = currproc->sz;
	uint pages;

	pages = PGROUNDUP(sz)/PGSIZE;

	pages += 1;

	return pages;

}

//returns number of physical pages
int
sys_numpp(void){
	struct proc *curproc = myproc();
	pde_t *pgdir = curproc->pgdir;
	uint count = 0;
	//uint i;
	pte_t *pte;


	for(int i = 0;i<curproc->sz;i+=PGSIZE){
		pte = walkpgdir(pgdir, (void *)i, 0);

		if(pte && (*pte & PTE_P)){
			count++;
		}
	}
	count++;
	//pte = walkpgdir(pgdir, (void*)(KERNBASE-PGSIZE), 0);
	//if(pte && (*pte & PTE_P)){
	  //     count++;
	//}

	return count;
}

//returns size of page table : user + kernel
int
sys_getptsize(void){
	struct proc* p = myproc();
	uint size = 0;

	pde_t *pgdir = p->pgdir;
	for(int i =0;i<NPDENTRIES;i++){
		if(pgdir[i] & PTE_P){
			size++;
		}
	}
	return size;
}


// Get USER page table size only
int
sys_getuptsize(void)
{
  struct proc *curproc = myproc();
  pde_t *pgdir = curproc->pgdir;
  uint count = 1;  // 1 for page directory (shared between user and kernel)
  uint i;
  
  // Only count page tables in USER address space (0 to KERNBASE)
  // PDX(KERNBASE) = 512, so we check first 512 PDE entries
  for(i = 0; i < PDX(KERNBASE); i++){
    if(pgdir[i] & PTE_P){
      count++;
    }
  }
  
  return count;
}

// Get KERNEL page table size only
int
sys_getkptsize(void)
{
  struct proc *curproc = myproc();
  pde_t *pgdir = curproc->pgdir;
  uint count = 0;  // Don't count page directory here (already counted in user)
  uint i;

  // Only count page tables in KERNEL address space (KERNBASE to end)
  // Start from PDX(KERNBASE) = 512 and go to NPDENTRIES = 1024
  for(i = PDX(KERNBASE); i < NPDENTRIES; i++){
    if(pgdir[i] & PTE_P){
      count++;
    }
  }

  return count;
}

int
sys_mmap(void)
{
  int n;  // Number of bytes to allocate
  struct proc *curproc = myproc();
  uint oldsz, newsz;

  // Get the argument (number of bytes)
  if(argint(0, &n) < 0)
    return 0;  // Invalid input

  // Validate input: must be positive and page-aligned
  if(n <= 0 || n % PGSIZE != 0)
    return 0;  // Invalid input

  oldsz = curproc->sz;
  newsz = oldsz + n;

  // Check if new size exceeds KERNBASE
  if(newsz >= KERNBASE)
    return 0;  // Would overflow into kernel space

  // Only increase virtual address space, don't allocate physical memory
  // We'll allocate physical pages on demand during page fault
  curproc->sz = newsz;

  // Return the starting virtual address of the mapped region
  return oldsz;
}


















