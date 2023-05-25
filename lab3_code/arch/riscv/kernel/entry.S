    .section .text.entry
    .align 2
	.macro SWAP_U_S_STACK reg
    csrr \reg, sscratch
    csrw sscratch, sp
    mv sp, \reg
	.endm
    .globl _traps 
_traps:
	csrr t0, sscratch
    beq t0, x0, _traps_switch
    SWAP_U_S_STACK t0
_traps_switch:
    addi sp, sp, -33*8
	#保存寄存器的值
	sd x0, 0*8(sp)
	sd x1, 1*8(sp)
	sd x2, 2*8(sp)
	sd x3, 3*8(sp)
	sd x4, 4*8(sp)
	sd x5, 5*8(sp)
	sd x6, 6*8(sp)
	sd x7, 7*8(sp)
	sd x8, 8*8(sp)
	sd x9, 9*8(sp)
	sd x10, 10*8(sp)
	sd x11, 11*8(sp)
	sd x12, 12*8(sp)
	sd x13, 13*8(sp)
	sd x14, 14*8(sp)
	sd x15, 15*8(sp)
	sd x16, 16*8(sp)
	sd x17, 17*8(sp)
	sd x18, 18*8(sp)
	sd x19, 19*8(sp)
	sd x20, 20*8(sp)
	sd x21, 21*8(sp)
	sd x22, 22*8(sp)
	sd x23, 23*8(sp)
	sd x24, 24*8(sp)
	sd x25, 25*8(sp)
	sd x26, 26*8(sp)
	sd x27, 27*8(sp)
	sd x28, 28*8(sp)
	sd x29, 29*8(sp)
	sd x30, 30*8(sp)
	sd x31, 31*8(sp)

	csrr t0, sepc
	sd t0, 32*8(sp)




    csrr a0, scause
	csrr a1, sepc
	call trap_handler
	#进行异常处理


	ld x0, 0*8(sp)
	ld x1, 1*8(sp)
	ld x3, 3*8(sp)
	ld x4, 4*8(sp)
	ld x5, 5*8(sp)
	ld x6, 6*8(sp)
	ld x7, 7*8(sp)
	ld x8, 8*8(sp)
	ld x9, 9*8(sp)
	ld x10, 10*8(sp)
	ld x11, 11*8(sp)
	ld x12, 12*8(sp)
	ld x13, 13*8(sp)
	ld x14, 14*8(sp)
	ld x15, 15*8(sp)
	ld x16, 16*8(sp)
	ld x17, 17*8(sp)
	ld x18, 18*8(sp)
	ld x19, 19*8(sp)
	ld x20, 20*8(sp)
	ld x21, 21*8(sp)
	ld x22, 22*8(sp)
	ld x23, 23*8(sp)
	ld x24, 24*8(sp)
	ld x25, 25*8(sp)
	ld x26, 26*8(sp)
	ld x27, 27*8(sp)
	ld x28, 28*8(sp)
	ld x29, 29*8(sp)
	ld x30, 30*8(sp)
	ld x31, 31*8(sp)
	ld t0, 32*8(sp)
	addi t0,t0,4
	csrw sepc, t0
	ld x2, 16(sp)
	#恢复寄存器的值

	addi sp, sp, 33*8

	csrr t0, sscratch
    beq t0, x0, _traps_end
    SWAP_U_S_STACK t0
_traps_end:
	sret
	
.globl __dummy
__dummy:
	la t0,dummy
	csrw sepc,t0
	sret
    

    
.globl __switch_to
__switch_to:
	addi t0,a0,40
	sd ra,0(t0)
	sd sp,8(t0)

	addi t0,a0,56
	sd s0,0*8(t0)
	sd s1,1*8(t0)
	sd s2,2*8(t0)
	sd s3,3*8(t0)
	sd s4,4*8(t0)
	sd s5,5*8(t0)
	sd s6,6*8(t0)
	sd s7,7*8(t0)
	sd s8,8*8(t0)
	sd s9,9*8(t0)
	sd s10,10*8(t0)
	sd s11,11*8(t0)
	
	addi t0,a1,40
	ld ra,0(t0)
	ld sp,8(t0)

	addi t0,a1,56
	ld s0,0*8(t0)
	ld s1,1*8(t0)
	ld s2,2*8(t0)
	ld s3,3*8(t0)
	ld s4,4*8(t0)
	ld s5,5*8(t0)
	ld s6,6*8(t0)
	ld s7,7*8(t0)
	ld s8,8*8(t0)
	ld s9,9*8(t0)
	ld s10,10*8(t0)
	ld s11,11*8(t0)

	addi t0, a0, 152
	csrr t1, sepc
	sd t1, 0(t0)
	csrr t1, sstatus
	sd t1, 8(t0)
	csrr t1, sscratch
	sd t1, 16(t0)
	csrr t1, satp
	sd t1, 24(t0)

	addi t0, a1, 152
	ld t1, 0(t0)
	csrw sepc, t1
	ld t1, 8(t0)
	csrw sstatus, t1
	ld t1, 16(t0)
	csrw sscratch, t1
	ld t1, 24(t0)
	csrw satp, t1

	# flush tlb
	sfence.vma zero, zero

	ret