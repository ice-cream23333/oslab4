#include "printk.h"
#include "stddef.h"
#include "types.h"
#include "defs.h"
#include "sbi.h"

extern char _stext[];
extern char _srodata[];
extern void schedule();
extern void test();
extern int printk(const char *, ...);
int start_kernel() {
    
    printk("Hello RISC-V\n");
    printk("idle process is running!\n");
    printk("[S-MODE] Hello RISC-V\n");
    /**_stext=555;
    *_srodata=222;
    printk("stext=%d\n",*_stext);
    printk("srodata=%d\n",*_srodata);*/
    schedule();
    test(); // DO NOT DELETE !!!

	return 0;
}
