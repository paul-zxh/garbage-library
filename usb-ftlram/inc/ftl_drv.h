/*
********************************************************************************
********************************************************************************

                    Copyright (c) 2017, JETWAY, WuHan
                            All Rights Reserved.

********************************************************************************
********************************************************************************
 */

#ifndef __FTL_DRV__
#define __FTL_DRV__
#include "j32xx.h"


void setup_page_buff(INT32U *buff, INT32U data);
void dump_page_buff(INT32U *buff, INT32U len);



void WritePage(INT32U const  lpn, INT32U const secoffset, INT32U const secnum, INT32U *data);
void ReadPage(INT32U const  lpn, INT32U const secoffset, INT32U const secnum, INT32U *data);

void EraseBlock(INT32U const rawaddr);

void nand_write_from_host(INT32U const  lpn, INT32U const secoffset, INT32U const secnum);
void nand_read_to_host(INT32U const lpn, INT32U const secoffset, INT32U const secnum);


#endif




