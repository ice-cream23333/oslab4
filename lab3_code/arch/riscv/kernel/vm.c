#include "defs.h"

extern char _stext[];
extern char _srodata[];
extern char _sdata[];

unsigned long early_pgtbl[512] __attribute__((__aligned__(0x1000)));

void setup_vm(void)
{
    int i;
    printk("setup_vm\n");
    for(i=0;i<512;i++)//初始化
        early_pgtbl[i]=0;
    int phy_index=(PHY_START >>30) & 0x1ff;//9 bit
    //early_pgtbl[phy_index]= ((PHY_START>>30)&0x1ff)<<10;
    early_pgtbl[phy_index]=((PHY_START>>30)&0x3ffffff)<<28 | 15;
    //early_pgtbl[phy_index]+=15;
    int vm_index=(VM_START>>30)&0x1ff;
    early_pgtbl[vm_index]=((PHY_START>>30)&0x3ffffff)<<28 | 15;//((VM_START>>30)&0x1ff)<<10;
    //early_pgtbl[vm_index]+=15;
}

/* swapper_pg_dir: kernel pagetable 根目录， 在 setup_vm_final 进行映射。 */
unsigned long  swapper_pg_dir[512] __attribute__((__aligned__(0x1000)));

void setup_vm_final(void) {
    memset(swapper_pg_dir, 0x0, PGSIZE);
    printk("setup_vm_final\n");
    unsigned long va = VM_START+OPENSBI_SIZE;
    unsigned long pa = PHY_START+OPENSBI_SIZE;
    unsigned long offset=(unsigned long)_srodata-(unsigned long)_stext;
    // No OpenSBI mapping required
    unsigned long sz;
    // mapping kernel text X|-|R|V
    sz=(unsigned long)_srodata-(unsigned long)_stext;
    create_mapping(swapper_pg_dir,va,pa,sz,11);

    // mapping kernel rodata -|-|R|V
    va+=offset;
    pa+=offset;
    sz=(unsigned long)_sdata-(unsigned long)_srodata;
    create_mapping(swapper_pg_dir,va,pa,sz,3);

    // mapping other memory -|W|R|V
    offset=(unsigned long)_sdata-(unsigned long)_srodata;
    sz=0x8000000-((unsigned long)_sdata-(unsigned long)_stext);
    va+=offset;
    pa+=offset;
    create_mapping(swapper_pg_dir, va, pa,sz,7);

    // set satp with swapper_pg_dir

    //YOUR CODE HERE
    asm volatile (
        "li t0,8\n"
        "slli t0,t0,60\n"
        "mv t1,%[swapper]\n"
        "srli t1,t1,12\n"
        "add t0,t0,t1\n"
        "csrw satp,t0\n"
        :
        :[swapper]"r"((unsigned long)swapper_pg_dir-PA2VA_OFFSET)
        :"memory"
    );
    // flush TLB
    asm volatile("sfence.vma zero, zero");
    return;
}


void create_mapping(unsigned long *pgtbl, unsigned long va, unsigned long pa, unsigned long sz, int perm) {
    unsigned long * p1,*p2,*p3;
    //unsigned long * pgtbl_3;
    unsigned long end = sz + va;
    p1=pgtbl;
    while(va < end){
        if((p1[(va>>30)&0x1ff] & 1) == 0) {
            p2= (unsigned long * )kalloc();
            p1[(va>>30)&0x1ff] = ((((unsigned long)p2 - PA2VA_OFFSET) >> 12) << 10) | 1;
        }
        else {
            p2 = (unsigned long * )((((unsigned long)p1[(va>>30)&0x1ff] >> 10) << 12) + PA2VA_OFFSET);
        }
        if((p2[(va>>21)&0x1ff] & 1) == 0) {
            p3 = (unsigned long * )kalloc();
            p2[(va>>21)&0x1ff] = ((((unsigned long)p3 - PA2VA_OFFSET) >> 12) << 10) | 1;
        }
        else {
            p3 = (unsigned long * )((((unsigned long)p2[(va>>21)&0x1ff] >> 10) << 12) + PA2VA_OFFSET);
        }
        if((p3[(va>>12)&0x1ff] & 1) == 0)
            p3[(va>>12)&0x1ff] = ((pa >> 12) << 10) | perm;

        va += PGSIZE;
        pa += PGSIZE;

    }
    return;
}
