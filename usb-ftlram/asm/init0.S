#define  _ASSEMBLE_FILE_INIT0_S_

#include "leon.h"

#define CONSBUF     0x0800053c

    .global     start, main, Isr_Handler
    .section    ".flashstart"
/*
================================================================================
    The entry of shared svt-trap.
--------------------------------------------------------------------------------

COMMENTS : enable svt, not care about relocatable vector trap.

*/
start:
	flush
    mov %psr, %l0 			! Tips : l0 = psr, l1 = pc, l2 = npc */
    mov %tbr, %l3
    and %l3, TT_MASK, %l3
    srl %l3, TT_SHL,  %l3   ! %l3 is %tbr.tt

    set trap_table, %l4     ! %l4 = p = trap_table
    mov %l3, %l5        ! %l5 = tt
    ld  [%l4], %l6      ! %l6 = p->strat
ENTER_LOOP:
    cmp %l6, %l5
ENTER_LOOP_C:
    bg,a    DETERMINE_END   
    add %l4, 12, %l4
    
    ld  [%l4 + 4], %l6
    cmp %l6, %l5
    bge,a   TRAP_ENTRY
    ld  [%l4 + 8], %l6

    add %l4, 12, %l4
DETERMINE_END:
    ld  [%l4], %l6
    cmp %l6, 0
    bne,a   ENTER_LOOP_C
    cmp %l6, %l5

    ld  [%l4 + 4], %l6
    cmp %l6, 0
    bne,a   ENTER_LOOP_C
    cmp %l6, %l5

    ld  [%l4 + 8], %l6
    cmp %l6,    0
    bne,a   ENTER_LOOP_C
    cmp %l6, %l5

    !not in trap_table
    BAD_TRAP


TRAP_ENTRY:
    jmp %l6         ! %l6 = p->func
    nop

    /*while entering a trap handler, locals as follows:
        L0 = psr
        L1 = pc
        L2 = npc
        L5 = psr.tt */

/*
================================================================================
    Reset trap handler.
--------------------------------------------------------------------------------
*/
_hardreset_svt:
    ! set svt
    mov %asr17, %g5
    set 1<<13, %g1
    or  %g5, %g1, %g5
    mov %g5, %asr17
    nop; nop; nop

    set 0x00e0, %g1 ! EC:0,EF:0,PIL:0000,S:1,PS:1,ET:1,CWP:00000
    mov %g1, %psr
    mov 2, %wim     ! current window is 0, so invalid windows is 1.
    nop; nop; nop
    
    set start, %g1
    mov %g1, %tbr
    mov 0, %y
    set RAM_END, %fp
    set RAM_END, %sp
    sub %sp, 64, %sp

    // goto main directly
    call main
    nop
    set CONSBUF, %g1
    st %o0, [ %g1 ]

    // return from main and halt
_halt:  
    ba _halt
    ta 1
    nop

/*
================================================================================
    Window overflow trap handler.
--------------------------------------------------------------------------------
*/
_window_overflow_svt:
    mov %wim, %l3
    mov %g1, %l7    ! backup g1

    // Calculate next WIM
    srl %l3, 1, %g1
    sll %l3, _nwindows_min1, %l4
    or  %l4, %g1, %g1

    save
    mov %g1, %wim
    nop; nop; nop
    
    std %l0, [%sp + 0]
    std %l2, [%sp + 8]
    std %l4, [%sp + 16]
    std %l6, [%sp + 24]
    std %i0, [%sp + 32]
    std %i2, [%sp + 40]
    std %i4, [%sp + 48]
    std %i6, [%sp + 56]

    restore     ! Go back to trap window.
    mov %l7, %g1

    jmp %l1 ! Re-excute save.
    rett    %l2
    nop

/*
================================================================================
    Window underflow trap handler.
--------------------------------------------------------------------------------
*/
_window_underflow_svt:
    mov %wim, %l3
    sll %l3, 1, %l4

    srl %l3, _nwindows_min1, %l5
    or  %l5, %l4, %l5
    mov %l5, %wim
    nop; nop; nop

    restore     ! Two restores to get into the
    restore     ! window to restore
    ldd [%sp + 0], %l0
    ldd [%sp + 8], %l2
    ldd [%sp + 16], %l4
    ldd [%sp + 24], %l6
    ldd [%sp + 32], %i0
    ldd [%sp + 40], %i2
    ldd [%sp + 48], %i4
    ldd [%sp + 56], %i6
    save
    save        ! get back to the trap window

    jmp %l1 ! Re-excute restore
    rett    %l2
    nop

/*
================================================================================
    Interrupt request trap handler.
--------------------------------------------------------------------------------
COMMENTS : ARGUMENTS_SZ is very necessary. this svt irq handler need to
       call subroutine(handler_irq), so some stack space should be 
       retained to pass arguments to subroutine. 

*/
_leonbare_irq_entry_svt:
    sub %l5, 0x10, %l7  ! interrupt level
    mov %wim, %l3
    ! here, locals as follows:
    ! l0 = psr
    ! l1 = pc
    ! l2 = npc
    ! l3 = wim
    ! l7 = interrupt level

    sub %fp, (SF_REGS_SZ + ARGUMENTS_SZ + PT_REGS_SZ), %t_kstack
    PT_STORE_ALL(t_kstack, t_psr, t_pc, t_npc, g2)

    mov 1, %l4
    sll %l4, %l0, %l4
    andcc   %l4, %l3, %g0
    be  CURRENT_WINDOW_IS_VALID
    nop

CURRENT_WINDOW_IS_INVALID:
    srl %l3, 1, %g2 ! Compute new wim
    sll %l3, _nwindows_min1, %l3
    or  %l3, %g2, %g2   ! here, g2 = new wim

    save
    mov %g2, %wim
    RW_STORE(sp)
    restore

CURRENT_WINDOW_IS_VALID:
    mov %t_kstack, %sp  ! here, storing sp is safe!

    or  %l0, SPARC_PSR_PIL_MASK, %o0    ! no nested irqs
    wr  %o0, SPARC_PSR_ET_MASK, %psr    ! set ET = 1
    nop; nop; nop

    ! In handler_irq, no nested irq, but allow trap nested
    mov %l7, %o0    ! irq level

    call    Isr_Handler
    add %sp, (SF_REGS_SZ + ARGUMENTS_SZ), %o1   ! pt_regs ptr

    ! Notice that this instruction will never become a Slot Instruction.
    mov %l0, %psr   ! set ET = 0

    mov 2, %g1
    sll %g1, %l0, %g1
    srl %g1, _nwindows, %g2
    or  %g1, %g2, %g1
    mov %wim, %g2
    andcc   %g2, %g1, %g0
    be  WINDOW_IS_NOT_UNDERFLOW
    nop

WINDOW_IS_UNDERFLOW:
    sll %g2, 1, %g1
    srl %g2, _nwindows_min1, %g2
    or  %g1, %g2, %g1
    mov %g1, %wim

    PT_LOAD_ALL(sp, t_psr, t_pc, t_npc, g1)
    
    restore
    RW_LOAD(sp)
    ba  IRQ_RETURN
    save
    

WINDOW_IS_NOT_UNDERFLOW:        ! g1 is used to swap
    PT_LOAD_ALL(sp, t_psr, t_pc, t_npc, g1)

IRQ_RETURN:
    mov %t_psr, %psr
    nop; nop; nop
    jmp %t_pc
    rett    %t_npc

trap_table:
    .long   0x00,   0x00,   _hardreset_svt
    .long   0x05,   0x05,   _window_overflow_svt
    .long   0x06,   0x06,   _window_underflow_svt
    .long   0x11,   0x1f,   _leonbare_irq_entry_svt
    .long   0x00,   0x00,   0x00


