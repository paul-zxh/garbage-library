/*
********************************************************************************
********************************************************************************

                    Copyright (c) 2017, JETWAY, WuHan
                            All Rights Reserved.

********************************************************************************
********************************************************************************
 */



#include "j32xx.h"
#include "eflash.h"

void eflash_init(void)
{   
    EFLASH_TIMER1_REG = EFLASH_TIMER1_MEL | EFLASH_TIMER1_MEH |EFLASH_TIMER1_RWS;
    EFLASH_TIMER2_REG = EFLASH_TIMER2_NVH | EFLASH_TIMER2_NVHL;
    EFLASH_TIMER3_REG = EFLASH_TIMER3_PGS | EFLASH_TIMER3_NVS;
    EFLASH_TIMER4_REG = EFLASH_TIMER4_RCV | EFLASH_TIMER4_PGH | EFLASH_TIMER4_PROG;
}

