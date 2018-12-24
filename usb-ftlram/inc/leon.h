/*
********************************************************************************
********************************************************************************

                    Copyright (c) 2017, JETWAY, WuHan
                            All Rights Reserved.

********************************************************************************
********************************************************************************
 */


#ifndef _LEON_
#define _LEON_

#if 0
#ifndef __ASSEMBLER__
  
/* process trap regs */
struct leonbare_pt_regs {
    unsigned long psr;
    unsigned long pc;
    unsigned long npc;
};
#define PT_REGS_SZ   sizeof(struct leonbare_pt_regs)

/* A Sparc stack frame */
struct sparc_stackframe_regs {
    unsigned long sf_locals[8];
    unsigned long sf_ins[6];
    struct sparc_stackframe_regs *sf_fp;
    unsigned long sf_callers_pc;
    char *sf_structptr;
    unsigned long sf_xargs[6];
    unsigned long sf_xxargs[1];
};  
#define SF_REGS_SZ sizeof(struct sparc_stackframe_regs)

/* A register window */
struct sparc_regwindow_regs {
    unsigned long locals[8];
    unsigned long ins[8];
};
#define RW_REGS_SZ sizeof(struct sparc_regwindow_regs)

/* A fpu window */
struct sparc_fpuwindow_regs {
    unsigned long locals[32];
    unsigned long fsr;
    unsigned long lastctx;
};
#define FW_REGS_SZ sizeof(struct sparc_fpuwindow_regs)

#else
#define PT_REGS_SZ     0x50 
#define SF_REGS_SZ     0x60 /* 24*4 */
#define RW_REGS_SZ     0x20 /* 16*4 */
#define FW_REGS_SZ     0x88 /* 34*4 */

#endif /* !ASM */
#endif


#if 1
/* Registers to not touch at all. */
#define t_psr        l0 /* Set by caller */
#define t_pc         l1 /* Set by caller */
#define t_npc        l2 /* Set by caller */
#define t_wim        l3 /* Set by caller */
#define t_twinmask   l4 /* Set at beginning of this entry routine. */
#define t_kstack     l5 /* Set right before pt_regs frame is built */
#define t_retpc      l6 /* If you change this, change winmacro.h header file */
#define t_systable   l7 /* Never touch this, could be the syscall table ptr. */
#define curptr       g6 /* Set after pt_regs frame is built */

#define twin_tmp1   l4
#define glob_tmp    g4

#define TT_MASK         0xFF0       // trap type mask from tbr
#define TT_SHL          4       // shift to get a tbr value

#define _nwindows           16
#define _nwindows_min1      15
#define _nwindows_min2      14

#define BAD_TRAP        ta 0; nop; nop; nop;

#define SF_REGS_SZ      64
#define PT_REGS_SZ      80
#define ARGUMENTS_SZ    32


/* These are for pt_regs. */
#define PT_PSR    0x0
#define PT_PC     0x4
#define PT_NPC    0x8
#define PT_Y      0xc
#define PT_G0     0x10
#define PT_WIM    PT_G0
#define PT_G1     0x14
#define PT_G2     0x18
#define PT_G3     0x1c
#define PT_G4     0x20
#define PT_G5     0x24
#define PT_G6     0x28
#define PT_G7     0x2c
#define PT_I0     0x30
#define PT_I1     0x34
#define PT_I2     0x38
#define PT_I3     0x3c
#define PT_I4     0x40
#define PT_I5     0x44
#define PT_I6     0x48
#define PT_FP     PT_I6
#define PT_I7     0x4c

/* Stack_frame offsets */
#define SF_L0     0x00
#define SF_L1     0x04
#define SF_L2     0x08
#define SF_L3     0x0c
#define SF_L4     0x10
#define SF_L5     0x14
#define SF_L6     0x18
#define SF_L7     0x1c
#define SF_I0     0x20
#define SF_I1     0x24
#define SF_I2     0x28
#define SF_I3     0x2c
#define SF_I4     0x30
#define SF_I5     0x34
#define SF_FP     0x38
#define SF_PC     0x3c
#define SF_RETP   0x40
#define SF_XARG0  0x44
#define SF_XARG1  0x48
#define SF_XARG2  0x4c
#define SF_XARG3  0x50
#define SF_XARG4  0x54
#define SF_XARG5  0x58
#define SF_XXARG  0x5c

/* Reg_window offsets */
#define RW_L0     0x00
#define RW_L1     0x04
#define RW_L2     0x08
#define RW_L3     0x0c
#define RW_L4     0x10
#define RW_L5     0x14
#define RW_L6     0x18
#define RW_L7     0x1c
#define RW_I0     0x20
#define RW_I1     0x24
#define RW_I2     0x28
#define RW_I3     0x2c
#define RW_I4     0x30
#define RW_I5     0x34
#define RW_I6     0x38
#define RW_I7     0x3c

/* Fpu_window offsets */
#define FW_F0     0x00
#define FW_F2     0x08
#define FW_F4     0x10
#define FW_F6     0x18
#define FW_F8     0x20
#define FW_F10    0x28
#define FW_F12    0x30
#define FW_F14    0x38
#define FW_F16    0x40
#define FW_F18    0x48
#define FW_F20    0x50
#define FW_F22    0x58
#define FW_F24    0x60
#define FW_F26    0x68
#define FW_F28    0x70
#define FW_F30    0x78
#define FW_FSR    0x80


#define SAVE_ALL_HEAD \
        sethi   %hi(leonbare_trapsetup), %l4; \
        jmpl    %l4 + %lo(leonbare_trapsetup), %l6;

#define SAVE_ALL \
        SAVE_ALL_HEAD \
        nop;

#define RESTORE_ALL b leonbare_trapreturn; clr %l6;

#define WRITE_PAUSE nop; nop; nop; 


/* psr defines */
#define SPARC_PSR_WIN_MASK  0x0000001f   /* bit   0-4 */
#define SPARC_PSR_ET_MASK   0x00000020   /* bit   5 */
#define SPARC_PSR_PS_MASK   0x00000040   /* bit   6 */
#define SPARC_PSR_S_MASK    0x00000080   /* bit   7 */
#define SPARC_PSR_PIL_MASK  0x00000F00   /* bits  8 - 11 */
#define SPARC_PSR_EF_MASK   0x00001000   /* bit  12 */
#define SPARC_PSR_EC_MASK   0x00002000   /* bit  13 */
#define SPARC_PSR_ICC_MASK  0x00F00000   /* bits 20 - 23 */
#define SPARC_PSR_VER_MASK  0x0F000000   /* bits 24 - 27 */
#define SPARC_PSR_IMPL_MASK 0xF0000000   /* bits 28 - 31 */
#define SPARC_PSR_PIL_SHIFT 8

/* Loading and storing struct pt_reg trap frames. */
#define PT_STORE_ALL(base_reg, reg_psr, reg_pc, reg_npc, g_scratch) \
    PT_STORE_PRIV(base_reg, reg_psr, reg_pc, reg_npc) \
    PT_STORE_GLOBALS(base_reg) \
    PT_STORE_YREG(base_reg, g_scratch) \
    PT_STORE_INS(base_reg)

#define PT_STORE_PRIV(base_reg, pt_psr, pt_pc, pt_npc) \
    st  %pt_psr, [%base_reg + SF_REGS_SZ + ARGUMENTS_SZ + PT_PSR]; \
    st  %pt_pc, [%base_reg + SF_REGS_SZ + ARGUMENTS_SZ + PT_PC]; \
    st  %pt_npc, [%base_reg + SF_REGS_SZ + ARGUMENTS_SZ + PT_NPC];

#define PT_STORE_GLOBALS(base_reg) \
    st  %g1, [%base_reg + SF_REGS_SZ + ARGUMENTS_SZ + PT_G1]; \
    std %g2, [%base_reg + SF_REGS_SZ + ARGUMENTS_SZ + PT_G2]; \
    std %g4, [%base_reg + SF_REGS_SZ + ARGUMENTS_SZ + PT_G4]; \
    std %g6, [%base_reg + SF_REGS_SZ + ARGUMENTS_SZ + PT_G6];

#define PT_STORE_YREG(base_reg, scratch) \
    mov %y, %scratch; \
    st  %scratch, [%base_reg + SF_REGS_SZ + ARGUMENTS_SZ + PT_Y];

#define PT_STORE_INS(base_reg) \
    std %i0, [%base_reg + SF_REGS_SZ + ARGUMENTS_SZ + PT_I0]; \
    std %i2, [%base_reg + SF_REGS_SZ + ARGUMENTS_SZ + PT_I2]; \
    std %i4, [%base_reg + SF_REGS_SZ + ARGUMENTS_SZ + PT_I4]; \
    std %i6, [%base_reg + SF_REGS_SZ + ARGUMENTS_SZ + PT_I6]; 

#define PT_LOAD_ALL(base_reg, pt_psr, pt_pc, pt_npc, scratch) \
    PT_LOAD_YREG(base_reg, scratch) \
    PT_LOAD_INS(base_reg) \
    PT_LOAD_GLOBALS(base_reg) \
    PT_LOAD_PRIV(base_reg, pt_psr, pt_pc, pt_npc)

#define PT_LOAD_YREG(base_reg, scratch) \
        ld      [%base_reg + SF_REGS_SZ + ARGUMENTS_SZ + PT_Y], %scratch; \
        mov %scratch, %y;

#define PT_LOAD_INS(base_reg) \
        ldd     [%base_reg + SF_REGS_SZ + ARGUMENTS_SZ + PT_I0], %i0; \
        ldd     [%base_reg + SF_REGS_SZ + ARGUMENTS_SZ + PT_I2], %i2; \
        ldd     [%base_reg + SF_REGS_SZ + ARGUMENTS_SZ + PT_I4], %i4; \
        ldd     [%base_reg + SF_REGS_SZ + ARGUMENTS_SZ + PT_I6], %i6;

#define PT_LOAD_GLOBALS(base_reg) \
        ld      [%base_reg + SF_REGS_SZ + ARGUMENTS_SZ + PT_G1], %g1; \
        ldd     [%base_reg + SF_REGS_SZ + ARGUMENTS_SZ + PT_G2], %g2; \
        ldd     [%base_reg + SF_REGS_SZ + ARGUMENTS_SZ + PT_G4], %g4; \
        ldd     [%base_reg + SF_REGS_SZ + ARGUMENTS_SZ + PT_G6], %g6;

#define PT_LOAD_PRIV(base_reg, pt_psr, pt_pc, pt_npc) \
        ld      [%base_reg + SF_REGS_SZ + ARGUMENTS_SZ + PT_PSR], %pt_psr; \
        ld      [%base_reg + SF_REGS_SZ + ARGUMENTS_SZ + PT_PC], %pt_pc; \
        ld      [%base_reg + SF_REGS_SZ + ARGUMENTS_SZ + PT_NPC], %pt_npc;


/* Store the register window onto the 8-byte aligned area starting
 * at %reg. It might be %sp, it might not, we don't care.
 */
#define RW_STORE(reg) \
    std %l0, [%reg + RW_L0]; \
    std %l2, [%reg + RW_L2]; \
    std %l4, [%reg + RW_L4]; \
    std %l6, [%reg + RW_L6]; \
    std %i0, [%reg + RW_I0]; \
    std %i2, [%reg + RW_I2]; \
    std %i4, [%reg + RW_I4]; \
    std %i6, [%reg + RW_I6];

/* Load a register window from the area beginning at %reg. */
#define RW_LOAD(reg) \
    ldd [%reg + RW_L0], %l0; \
    ldd [%reg + RW_L2], %l2; \
    ldd [%reg + RW_L4], %l4; \
    ldd [%reg + RW_L6], %l6; \
    ldd [%reg + RW_I0], %i0; \
    ldd [%reg + RW_I2], %i2; \
    ldd [%reg + RW_I4], %i4; \
    ldd [%reg + RW_I6], %i6;

#endif

#endif


