/*
********************************************************************************
********************************************************************************

                    Copyright (c) 2017, JETWAY, WuHan
                            All Rights Reserved.

********************************************************************************
********************************************************************************
 */


#include "j32xx.h"
#include "usb.h"
#include "nflash.h"
//#include "ahb_usbnfc.h"
#include "ftl.h"
#include "ftl_drv.h"
#include "ftl_nand.h"
#include "onfi_run.h"



//2016-08-02 add
//#include "onfi_run.h"
//#define NUM_BLOCKS  8   //2048 blocks per plane, J3223 has 2 plane


void Isr_Handler(INT32U irq_level, INT32U reserve)
{
    IRQMP_ICLEAR_REG = (1 << irq_level);
}


static void system_init(void)
{
    DSU_CTRL_REG = 0x1c6;
    GPIO_ICS_REG = 0x387;
    GPIO_GCC_REG = 0xffff;

    volatile int *ahbcswc_ctr_reg = (volatile unsigned int *)0x12000000;
    ahbcswc_ctr_reg[0]=(46<<25)|0x402ff;

}

static void hardware_init(void)
{
    eflash_init();
    NFC_Flash_Init(ASYNC8_MODE, NFC_SPEED_MODE);
    //NFC_Flash_Init(SYNC8_MODE, NFC_SPEED_MODE); //test 16bit translate:
                                                 //it prompts :>Ftl->Flash: Check FTL_DONE Fail!
    usb_init();

    jprintf("\n");
}

static void BLOCK_INIT(void)
{
    INT32U E_BLOCK=0;
    INT32U time=0;
    for (E_BLOCK=0; E_BLOCK<=256; E_BLOCK++) {
        //time = 100000;
        EraseBlock(E_BLOCK*256);
        //while(time--);
        
    }

}


extern INT32U page_buff[BYTES_PER_PAGE >> 2];
extern volatile NFC *nfcReg;

extern volatile INT32U *NFC_FTL;

extern int NFC_ProgramFtl_T(INT32U secNum,INT32U rowAdr,INT16U columnAdr,INT32U data);
extern int NFC_ReadFtl_T(INT32U secNum,INT32U rowAdr,INT16U columnAdr);

extern INT32U file_start;
extern INT32U file_end;

extern INT32U FLIEVPN;



int xxx = 0x22223333;
int main(void)
{

    system_init();
    hardware_init();

    jprintf("xxx: %x\n", xxx);

    //Nfc_getFlashInfo();


//================2016-08-02======================//

    

//================2016-08-02======================//


    //EraseBlock(PAGES_PER_PHYBLK); 
    jprintf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
    
    //setup_page_buff(page_buff, 0x12131415);
    //WritePage(1*PAGES_PER_PHYBLK, 0, 1, page_buff);

    //setup_page_buff(page_buff, 0x00000000);
    //ReadPage(1*PAGES_PER_PHYBLK, 0, 1, page_buff);
    //jprintf("%x,%x\n", page_buff[0], page_buff[1]);

    //NFC_ProgramFtl_T(lpn, secoffset, secnum);
    //NFC_ReadFtl_T(lpn, secoffset, secnum);

//================2016-11-29======================//
/*
    INT16U file_counter=0;
    ftl_open();
    for (file_counter=0; file_counter<500; file_counter++) {
        set_format_mark(file_counter);
        get_format_mark(file_counter);  
        jprintf("FLIEVPN = 0x%8.8x\n",FLIEVPN);

    }
*/
//================2016-11-29======================//


//$$$=============2016-12-29======================//

    UINT16 block_number=0;
    UINT16 page_number=0;
    INT32U i=0;
    INT32U j=0;
    

    //for (i=0; i<2; i++) {

        EraseBlock(10*256);

        //for (j=0; j<2; j++) {
            NFC_ProgramFtl_T(1, 10*256+4,0,0x12345678);
            //jprintf("ftlram write finished %d \n",j);
        //}
        //jprintf("ftlram write finished %d \n",i);
    //}   
    

    //for (i=0; i<2; i++) {
        //for (j=0; j<2; j++) {
            NFC_ReadFtl_T(1, 10*256+4,0);
            //for (j=0; j<30; j++)
            //    ;
            //NFC_ReadFtl_T(1, 10*256+4,180);
            //NFC_ReadFtl_T(1, 10*256+4,1024);
            //NFC_ReadFtl_T(1, 10*256+4,1536);
        //}
    //    jprintf("ftlram read finished %d \n",i);
    //}

//$$$=============2016-12-29======================//



    //NFC_ProgramFtl_T(1 ,15, 1,0x11112222);

    //NFC_ProgramFtl_T(1 ,14, 1,0x200);

    //NFC_ProgramFtl_T(1 ,15, 1);

    
    //NFC_ReadFtl_T(1 ,12, 1);

    //NFC_ReadFtl_T(1 ,14, 1);

    //NFC_ReadFtl_T(1 ,15, 1);
//================2016-11-29======================//


    //jprintf("ftlram has finished\n");




    return 0x55aaaa55;

}




//================by zxh=========//





/*
    jprintf("xxx: %x\n", xxx);
    EraseBlock(1*PAGES_PER_PHYBLK);	

    setup_page_buff(page_buff, 0x12131415);
    WritePage(1*PAGES_PER_PHYBLK, 0, 1, page_buff);

    setup_page_buff(page_buff, 0x00000000);
    ReadPage(1*PAGES_PER_PHYBLK, 0, 1, page_buff);
    jprintf("%x, %x\n", page_buff[0], page_buff[1]);
*/



