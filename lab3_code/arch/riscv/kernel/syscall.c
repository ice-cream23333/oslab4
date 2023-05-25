#include "syscall.h"
#include "proc.h"
#define SYS_WRITE   64
#define SYS_GETPID  172
extern struct task_struct* current;
void syscall(struct pt_regs* regs) {
    if (regs->x[17] == SYS_WRITE) {
        if (regs->x[10] == 1) {
            ((char*)(regs->x[11]))[regs->x[12]] = '\0';
            regs->x[10] = printk((char *)(regs->x[11]));
        }
    }
    else if (regs->x[17] == SYS_GETPID) {
        regs->x[10] = current->pid;
    }
    else {}
}