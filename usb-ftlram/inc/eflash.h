/*
********************************************************************************
********************************************************************************

                    Copyright (c) 2017, JETWAY, WuHan
                            All Rights Reserved.

********************************************************************************
********************************************************************************
 */


#ifndef _EFLASH_H
#define _EFLASH_H

#define EFLASH_BASE (0x08000a00)


/* AHBEF32KX32C 寄存器组 */

// 时间参数寄存器 1
#define EFLASH_TIMER1_REG       (*(VINT32U*)(EFLASH_BASE + 0x00))

// 时间参数寄存器 2
#define EFLASH_TIMER2_REG       (*(VINT32U*)(EFLASH_BASE + 0x04))

// 时间参数寄存器 3
#define EFLASH_TIMER3_REG       (*(VINT32U*)(EFLASH_BASE + 0x08))

// 时间参数寄存器 4
#define EFLASH_TIMER4_REG       (*(VINT32U*)(EFLASH_BASE + 0x0c))

// 控制寄存器 1
#define EFLASH_CTRL1_REG        (*(VINT32U*)(EFLASH_BASE + 0x10))

// 控制寄存器 2
#define EFLASH_CTRL2_REG        (*(VINT32U*)(EFLASH_BASE + 0x14))


#define Fsysclk         35

#define Terase          22              //20000us//20ms
#define Txa             35              //35ns          <----------------ns
#define Tnvh            5               //5us
#define Tnvhl           100             //100us
#define Tnvs            5               //5us
#define Tpgs            10              //10us
#define Trcv            1               //1us
#define Tpgh            20              //20ns          <----------------ns
#define Tprog           20              //20us


//Boot from 0x06000000, and switch 0c/0d/0e armba space, but disable jtag.
#define MC_BOOT_FROM_EFLASH     0x55555576
//Boot from 0x06000000, and switch 0c/0d/0e armba space, and enable jtag.
#define MC_BOOT_FROM_EFLASH_DBG 0x55555577

////////////////////////////////////////////////////////////////////////////////
//eflash configuration
#define EFLASH_INFO_BLOCK_START         0x06080000
#define EFLASH_INFO_BLOCK_SIZE          2048

#define EFLASH_BLOCK_SIZE               0x0800
#define EFLASH_REVISE                   0

/* AHBEF32KX32C 时间参数寄存器 1 描述 */
// [30-24] 读等待状态
#define EFLASH_TIMER1_RWS       0 //(0x19<<24)

// [23-16] 设置Terase和Tme
#define EFLASH_TIMER1_MEH       ((250-1)<<16)

// [15-0] 设置Terase和Tme
#define EFLASH_TIMER1_MEL       ((Terase*Fsysclk*4)-1)

/* AHBEF32KX32C 时间参数寄存器 2 描述 */
// [26-16] 设置Tnvh
#define EFLASH_TIMER2_NVH       ((Tnvh*Fsysclk-1)<<16)

// [15-0] 设置Tnvhl
#define EFLASH_TIMER2_NVHL      ((Tnvhl*Fsysclk)-1)

/* AHBEF32KX32C 时间参数寄存器 3 描述 */
// [27-16] 设置Tpgs
#define EFLASH_TIMER3_PGS       (((Tpgs*Fsysclk)-1)<<16)

// [10-0] 设置Tnvs
#define EFLASH_TIMER3_NVS       ((Tnvs*Fsysclk)-1)


/* AHBEF32KX32C 时间参数寄存器 4 描述 */
// [28-20] 设置Trcv
#define EFLASH_TIMER4_RCV       (((Trcv*Fsysclk)-1)<<20)

// [19-16] 设置Tpgh
#define EFLASH_TIMER4_PGH       (0)

// [12-0] 设置Tprog
#define EFLASH_TIMER4_PROG      ((Tprog*Fsysclk)-1)



/* AHBEF32KX32C 控制寄存器 1 描述 */

// [31] 测试模式使能，高电平有效
#define EFLASH_CTRL1_TMEN       (1<<31) 

#define AHBEFM_CTRL1_ERPR_ENA   (1<<29) //Erase and program enable, active high


// [28-24] 测试模式下的时间参数(有效值 >= 1)
#define EFLASH_CTRL1_TMV        (1<<24) 

// [18] 擦除完成后的中断使能，高电平有效
#define EFLASH_CTRL1_INTEN      (1<<18) 

// [17] 调试输出使能，高电平有效
#define EFLASH_CTRL1_DBGOE      (1<<17) 

// [16] 自动扇区擦除使能，高电平有效
#define EFLASH_CTRL1_SERAUTO    (1<<16) 

// 重置
#define EFLASH_CTRL1_RESET      0


/* AHBEF32KX32C 控制寄存器 2 描述 */

// [8] 置“1”将使 FLASH 进入 TMMODE 指定的测试模式.
#define EFLASH_CTRL2_TMENP      

// [4] 块擦除完成后自动置“1”，同时向 CPU 发出中断申请信号。写“0”将清除中断。
#define EFLASH_CTRL2_BERINT     (1<<4)

// [3] 扇区擦除完成后自动置“1”，同时向 CPU 发出中断申请信号。写“0”将清除中断。
#define EFLASH_CTRL2_SERINT     (1<<3)

// [2] 擦除动作完成后置“1“，ER_EN 置“01” 或 “10”后自动清零。 
#define EFLASH_CTRL2_EROVR      (1<<2)

// [1-0] 01：扇区擦除 , 10：块擦除。 其他值不动作。擦除操作完成后自动清零。
#define EFLASH_CTRL2_EREN_SECTOR 1
#define EFLASH_CTRL2_EREN_BULK   2

// 重置
#define EFLASH_CTRL2_RESET       0


void eflash_init(void);

#endif // _EFLASH_H


