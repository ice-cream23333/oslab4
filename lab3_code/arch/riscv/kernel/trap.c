// trap.c 
#include "printk.h"
#include "clock.h"
#include "proc.h"
int flag0=0;

/*void trap_handler(unsigned long scause, unsigned long sepc) {
    unsigned long interrupt;
    unsigned long timer;
    interrupt=scause&(1<<31);
    timer=scause&0x01111111;
    if(interrupt==0x8000000000000000&&timer==1){
        clock_set_next_event();
        do_timer();
    }
}*/
void trap_handler(unsigned long scause, unsigned long sepc, struct pt_regs *regs) {
    if (scause & (1LL << 63)) {
        if (scause ^ (1LL << 63) == 5) {
            // printk("kernel is running!\n");
            // printk("[S] Supervisor Mode Timer Interrupt\n");
            clock_set_next_event();
            do_timer();
        }
        else {}
    }
    else {
        if (scause == 8) {
            syscall(regs);
        }
    }
}
