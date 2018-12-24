/*
********************************************************************************
********************************************************************************

                    Copyright (c) 2017, JETWAY, WuHan
                            All Rights Reserved.

********************************************************************************
********************************************************************************
 */

#ifndef __FTL__
#define __FTL__




void ftl_open(void);
void ftl_read(INT32U const lba, INT32U const secnum);
void ftl_write(INT32U const lba, INT32U const secnum);
void ftl_flush(void);
void ftl_isr(void);
void set_format_mark(INT32U filename);
void get_format_mark(INT32U filename);




#endif
