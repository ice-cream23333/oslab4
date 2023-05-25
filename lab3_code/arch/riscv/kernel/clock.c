// clock.c
#include "clock.h"

unsigned long TIMECLOCK = 10000000;

unsigned long get_cycles() {
    //#error "Still have unfilled code!"
    unsigned long time;
    __asm__ volatile (
        "rdtime %[time]"
        :[time] "=r" (time)
    );
    return time;
}

void clock_set_next_event() {
    //#error "Still have unfilled code!"
    unsigned long next = get_cycles() + TIMECLOCK;
    sbi_ecall(0,0,next,0,0,0,0,0);
} 