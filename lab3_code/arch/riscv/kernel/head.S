.extern start_kernel
.extern _traps
.extern mm_init
.extern task_init
.extern setup_vm
.extern setup_vm_final

.section .text.init
.globl _start,_stack
_start:
  la sp,_stack
  call setup_vm
  call relocate
  call mm_init
  call task_init
  call setup_vm_final
 
  la t0, _traps
  csrw stvec, t0
  csrr t0, sie
  ori t0, t0, 1<<5
  csrw sie, t0
    
  addi a6, x0, 0
  addi a7, x0, 0
  rdtime a0
  li a1, 10000000
  add a0, a0, a1
  addi a1, x0, 0
  addi a2, x0, 0
  addi a3, x0, 0
  addi a4, x0, 0
  addi a5, x0, 0
	ecall

  csrr t0, sstatus
  ori t0, t0, 2
  csrw sstatus, t0
    
    
  j start_kernel


relocate:
    # set ra = ra + PA2VA_OFFSET
    # set sp = sp + PA2VA_OFFSET (If you have set the sp before)
   
    ###################### 
    #   YOUR CODE HERE   #
    ######################
    #li t0, PA2VA_OFFSET
    li t0, 0xffffffdf80000000
    add ra, ra, t0
    add sp, sp, t0

    # set satp with early_pgtbl
    
    ###################### 
    #   YOUR CODE HERE   #
    ######################
    
    li t0, 8
    slli t0, t0, 60
    la t1, early_pgtbl
    srli t1, t1, 12
    add t0, t0, t1
    csrw satp, t0
    
    # flush tlb
    sfence.vma zero, zero
    ret
    
#.section .bss.stack
#.space 4096
#.globl stacktop
#_stack:



