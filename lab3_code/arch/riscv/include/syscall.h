#ifndef _SYSCALL_H
#define _SYSCALL_H

#include "types.h"

struct pt_regs {
  unsigned long x[32];
  unsigned long sepc;
  unsigned long sstatus;
};

#endif