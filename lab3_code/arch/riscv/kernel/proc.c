#include "defs.h"
#include "string.h"
#include "proc.h"
#include "rand.h"
#include "printk.h"
//arch/riscv/kernel/proc.c

extern void __dummy();
extern unsigned long swapper_pg_dir[];
extern char uapp_start[];
extern char uapp_end[];

struct task_struct* idle;           // idle process
struct task_struct* current;        // 指向当前运行线程的 `task_struct`
struct task_struct* task[NR_TASKS]; // 线程数组，所有的线程都保存在此

void task_init() {
    idle=(struct task_struct*)kalloc();
    idle->state=TASK_RUNNING;
    idle->counter=0;
    idle->priority=0;
    idle->pid=0;
    current=idle;
    task[0]=idle;
    int i;
    uint64 va,pa,satp,sstatus;
    pagetable_t pgtbl;
    for(i=1;i<NR_TASKS;i++)
    {
        task[i]=(struct task_struct*)kalloc();
        task[i]->state=TASK_RUNNING;
        task[i]->counter=0;
        task[i]->priority=rand()%5+1;//1~5
        task[i]->pid=i;
        task[i]->thread.ra=(uint64)__dummy;
        task[i]->thread.sp=(uint64)(task[i])+PGSIZE;

        task[i]->thread_info->kernel_sp=(uint64)(task[i])+PGSIZE;
        task[i]->thread_info->user_sp=kalloc();

        pgtbl = (pagetable_t)kalloc();
        memcpy(pgtbl, swapper_pg_dir, PGSIZE);

        va = USER_START;
        pa = (uint64)(uapp_start)-PA2VA_OFFSET;
        create_mapping(pgtbl, va, pa, (uint64)(uapp_end)-(uint64)(uapp_start), 31);
        
        va = USER_END-PGSIZE;
        pa = task[i]->thread_info->user_sp-PA2VA_OFFSET;
        create_mapping(pgtbl, va, pa, PGSIZE, 23);

        satp = csr_read(satp);
        satp = (satp >> 44) << 44;
        satp |= ((unsigned long)pgtbl-PA2VA_OFFSET) >> 12;
        task[i]->pgd = satp;

        task[i]->thread.sepc = USER_START;
        sstatus = csr_read(sstatus);
        sstatus &= ~(1<<8); // set sstatus[SPP] = 0
        sstatus |= 1<<5; // set sstatus[SPIE] = 1
        sstatus |= 1<<18; // set sstatus[SUM] = 1
        task[i]->thread.sstatus = sstatus;
        task[i]->thread.sscratch = USER_END;

    }
    printk("...proc_init done!\n");
}


void do_timer(void)
{
    
    if(current==idle)
    {
        schedule();
    }
    else
    {
        //printk("d");
        current->counter--;
        if(current->counter>0) return;
        else schedule();
    }
}

void schedule(void)
{
    struct task_struct* next = current;
    uint64 max_priority = PRIORITY_MIN-1;
    while (1) {
        for (int i = 1; i < NR_TASKS; i++) {
            if (task[i]->counter == 0) continue;
            if (task[i]->priority > max_priority) {
                max_priority = task[i]->priority;
                next = task[i];
            }
        }

        if (max_priority == PRIORITY_MIN-1) {
            for (int i = 1; i < NR_TASKS; i++) {
                task[i]->counter = rand()%5+1;
                //printk("SET [PID = %d PRIORITY = %d COUNTER = %d]\n", task[i]->pid, task[i]->priority, task[i]->counter);
            }
        }
        else break;
    }

    switch_to(next);
}

extern void __switch_to(struct task_struct* prev, struct task_struct* next);

void switch_to(struct task_struct* next)
{
    if(next==current) return;
    else
    {
    	printk("\nswitch to [PID = %d, PRIORITY = %d, COUNTER = %d]\n",next->pid,next->priority,next->counter);
        //printk("Thread space begin at %lx\n\n", current);
        struct task_struct* prev=current;
        current=next;
        __switch_to(prev,next);
    }
    
}

// arch/riscv/kernel/proc.c

void dummy() {
uint64 MOD = 1000000007;
uint64 auto_inc_local_var = 0;
int last_counter = -1; // 记录上一个counter
int last_last_counter = -1; // 记录上上个counter
while(1) {
    if (last_counter == -1 || current->counter != last_counter) {
        last_last_counter = last_counter;
        last_counter = current->counter;
        auto_inc_local_var = (auto_inc_local_var + 1) % MOD;
        //printk("[PID = %d] is running. auto_inc_local_var = %d\n", current->pid, auto_inc_local_var);
        printk("[PID = %d] is running. ", current->pid);  
        printk("thread space begin at %lx\n", current);
        //printk("Thread space begin at %lx\n\n", current);
        //printk("%d\n", current->counter);
    } else if((last_last_counter == 0 || last_last_counter == -1) && last_counter == 1) { // counter恒为1的情况
        // 这里比较 tricky，不要求理解。
        last_counter = 0; 
        current->counter = 0;
    }
	}
}

