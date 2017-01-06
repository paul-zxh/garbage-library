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
#include "onfi_run.h"


volatile NFC *nfcReg;
volatile NFC_CMD_FIFO*  nfcCmd;
volatile NFC_TRS_STATE  nfcTrsState;
volatile INT32U nfcIrqStat; 
volatile INT32U *NFC_FMIOFFRD;
volatile INT32U *NFC_FMICFFRD; 
volatile INT32U *NFC_FTL; 
volatile INT32U onfi_speed ;

INT8U NandFlashInfoString[32];

#define STD_DATA_SIZE_MAX               1024*8
const INT8U stdBuf[STD_DATA_SIZE_MAX] = 
{
        0xac,0x4a,0x52,0x56,0xf8,0x36,0xb3,0x73,0x4a,0x83,0xd4,0x83,0xfd,0x82,0x55,0xc9, 
        0xa7,0x8d,0x0b,0xbf,0x29,0x67,0x4f,0xc4,0xe6,0x17,0xa6,0x2d,0x6e,0x8b,0xb6,0x80, 
        0x8f,0xea,0x99,0x7d,0x9c,0x4d,0x8a,0xb1,0x71,0x4d,0xfa,0x47,0x68,0xaa,0x7f,0xc1, 
        0x57,0x6a,0xb7,0x64,0xc6,0x35,0x96,0x72,0xfe,0xd4,0x1f,0xeb,0x10,0x1a,0x93,0x22, 
        0x73,0x12,0x2c,0xaa,0x7e,0x3a,0xb0,0x8d,0x28,0xbc,0x34,0x3d,0xf2,0x4d,0xb0,0xb0, 
        0x6b,0x32,0x55,0x30,0x11,0x1b,0x38,0x2d,0xa4,0xcc,0x3e,0xb2,0xa1,0x3f,0xf9,0x47, 
        0xe5,0x38,0x40,0x57,0x50,0x03,0xb8,0xee,0xcf,0x4c,0xb4,0x69,0x34,0x42,0x8a,0x59, 
        0xbc,0xfd,0x33,0xcd,0x26,0x60,0xfe,0x30,0x44,0xd9,0x13,0x75,0x66,0x51,0x04,0xc5, 
        0x09,0x98,0xc1,0xe0,0x1d,0xaf,0x27,0x64,0x5f,0xb7,0xe5,0x31,0x1b,0x60,0x9f,0xa4, 
        0xb6,0x2c,0x57,0xcb,0xf6,0xce,0xab,0xdc,0x5d,0x9c,0x60,0x0d,0xf4,0x2c,0x3d,0x97, 
        0x91,0xb4,0x4f,0x88,0xbd,0x4c,0x76,0x9f,0x60,0x81,0x64,0x5f,0xe5,0x08,0xf4,0x1c, 
        0x58,0xde,0x7c,0x1f,0x4b,0xb5,0xf0,0xb5,0x06,0x76,0x1c,0x35,0x39,0x2f,0xa6,0x58, 
        0x49,0x50,0x3e,0x74,0xe2,0xe9,0x94,0xf7,0xf1,0x6e,0x00,0xa2,0xaa,0x18,0x08,0xed, 
        0xb3,0x7c,0x0f,0x9d,0xba,0xe6,0x78,0x62,0xe4,0x10,0xf0,0x10,0xf0,0x3f,0x36,0x48, 
        0x8b,0xf2,0x15,0x2d,0x10,0x1d,0x64,0x65,0xc5,0x07,0x3e,0x8c,0x51,0xf8,0x47,0x6e, 
        0xf1,0x2f,0xb0,0x84,0xb4,0x3a,0x5e,0xb0,0xb6,0xd2,0xbc,0x20,0xae,0xc2,0xd4,0x4e, 
        0xcb,0x69,0x0f,0x22,0x1f,0x7f,0x3b,0x87,0x22,0x18,0xd5,0x95,0x18,0x92,0x93,0x93, 
        0x4e,0xe4,0x38,0xf1,0x7a,0x0a,0xb0,0x0f,0xcb,0x6f,0x14,0xce,0x5a,0xa5,0x5e,0xf1, 
        0x0e,0x43,0x9c,0xa1,0x39,0xab,0xe2,0x1e,0x61,0x34,0x36,0x15,0x0f,0x52,0xc6,0xf8, 
        0x96,0x50,0x2d,0xe9,0xa1,0x34,0xf4,0x90,0x08,0x58,0xbb,0x68,0x2e,0x57,0xa5,0x60, 
        0xec,0xd4,0x60,0x63,0xde,0x44,0x1a,0x91,0x71,0x30,0xfc,0x4c,0x9a,0xaa,0xaa,0xdb, 
        0xad,0xe6,0xcd,0x53,0x14,0x9c,0xa3,0x6f,0x65,0xc4,0xaf,0x9c,0xb7,0x4c,0x6e,0x6a, 
        0x94,0xb6,0x32,0xfe,0xe4,0x6d,0x92,0x6a,0x56,0x24,0x80,0x5b,0xf3,0x12,0xfe,0x21, 
        0x8d,0x62,0x8b,0x79,0x0f,0x28,0x27,0x06,0xed,0xb0,0x1d,0x80,0x5c,0x7c,0xf1,0x05, 
        0x47,0xc4,0x9e,0xf3,0x72,0x4f,0xf1,0xd5,0xa3,0x6e,0x47,0x4a,0x28,0x80,0x75,0x4f, 
        0xbe,0x43,0x0d,0x8c,0x25,0x43,0xe0,0x53,0x43,0xd9,0x66,0x8c,0x50,0xe1,0xdc,0x49, 
        0x58,0x9f,0x63,0xa1,0x03,0x16,0x52,0x27,0x85,0xb0,0x11,0x02,0x97,0xf4,0x36,0x12, 
        0x63,0x48,0x2a,0x1f,0xbb,0xdb,0x24,0x7d,0x19,0xc4,0xa2,0x9c,0x9f,0x7c,0xd1,0xf3, 
        0xb2,0xaa,0xf4,0x4f,0xe5,0xf4,0x42,0x54,0x3a,0x4e,0xca,0xd2,0x76,0xf0,0xda,0xb5, 
        0x2c,0xfa,0x72,0x2a,0x0d,0x64,0xb9,0xcb,0xb8,0x37,0x9c,0x71,0x29,0xd0,0xe3,0xe5, 
        0x53,0x8d,0xfa,0x25,0x41,0x1d,0xc3,0x76,0x12,0x70,0x1c,0x6d,0x4e,0xf7,0xf4,0xae, 
        0x61,0xa3,0xa4,0x86,0xa6,0x53,0x5d,0x29,0x7d,0xbd,0xd2,0x30,0x9e,0xe6,0x9d,0xa3,
        0xac,0x4a,0x52,0x56,0xf8,0x36,0xb3,0x73,0x4a,0x83,0xd4,0x83,0xfd,0x82,0x55,0xc9, 
        0xa7,0x8d,0x0b,0xbf,0x29,0x67,0x4f,0xc4,0xe6,0x17,0xa6,0x2d,0x6e,0x8b,0xb6,0x80, 
        0x8f,0xea,0x99,0x7d,0x9c,0x4d,0x8a,0xb1,0x71,0x4d,0xfa,0x47,0x68,0xaa,0x7f,0xc1, 
        0x57,0x6a,0xb7,0x64,0xc6,0x35,0x96,0x72,0xfe,0xd4,0x1f,0xeb,0x10,0x1a,0x93,0x22, 
        0x73,0x12,0x2c,0xaa,0x7e,0x3a,0xb0,0x8d,0x28,0xbc,0x34,0x3d,0xf2,0x4d,0xb0,0xb0, 
        0x6b,0x32,0x55,0x30,0x11,0x1b,0x38,0x2d,0xa4,0xcc,0x3e,0xb2,0xa1,0x3f,0xf9,0x47, 
        0xe5,0x38,0x40,0x57,0x50,0x03,0xb8,0xee,0xcf,0x4c,0xb4,0x69,0x34,0x42,0x8a,0x59, 
        0xbc,0xfd,0x33,0xcd,0x26,0x60,0xfe,0x30,0x44,0xd9,0x13,0x75,0x66,0x51,0x04,0xc5, 
        0x09,0x98,0xc1,0xe0,0x1d,0xaf,0x27,0x64,0x5f,0xb7,0xe5,0x31,0x1b,0x60,0x9f,0xa4, 
        0xb6,0x2c,0x57,0xcb,0xf6,0xce,0xab,0xdc,0x5d,0x9c,0x60,0x0d,0xf4,0x2c,0x3d,0x97, 
        0x91,0xb4,0x4f,0x88,0xbd,0x4c,0x76,0x9f,0x60,0x81,0x64,0x5f,0xe5,0x08,0xf4,0x1c, 
        0x58,0xde,0x7c,0x1f,0x4b,0xb5,0xf0,0xb5,0x06,0x76,0x1c,0x35,0x39,0x2f,0xa6,0x58, 
        0x49,0x50,0x3e,0x74,0xe2,0xe9,0x94,0xf7,0xf1,0x6e,0x00,0xa2,0xaa,0x18,0x08,0xed, 
        0xb3,0x7c,0x0f,0x9d,0xba,0xe6,0x78,0x62,0xe4,0x10,0xf0,0x10,0xf0,0x3f,0x36,0x48, 
        0x8b,0xf2,0x15,0x2d,0x10,0x1d,0x64,0x65,0xc5,0x07,0x3e,0x8c,0x51,0xf8,0x47,0x6e, 
        0xf1,0x2f,0xb0,0x84,0xb4,0x3a,0x5e,0xb0,0xb6,0xd2,0xbc,0x20,0xae,0xc2,0xd4,0x4e, 
        0xcb,0x69,0x0f,0x22,0x1f,0x7f,0x3b,0x87,0x22,0x18,0xd5,0x95,0x18,0x92,0x93,0x93, 
        0x4e,0xe4,0x38,0xf1,0x7a,0x0a,0xb0,0x0f,0xcb,0x6f,0x14,0xce,0x5a,0xa5,0x5e,0xf1, 
        0x0e,0x43,0x9c,0xa1,0x39,0xab,0xe2,0x1e,0x61,0x34,0x36,0x15,0x0f,0x52,0xc6,0xf8, 
        0x96,0x50,0x2d,0xe9,0xa1,0x34,0xf4,0x90,0x08,0x58,0xbb,0x68,0x2e,0x57,0xa5,0x60, 
        0xec,0xd4,0x60,0x63,0xde,0x44,0x1a,0x91,0x71,0x30,0xfc,0x4c,0x9a,0xaa,0xaa,0xdb, 
        0xad,0xe6,0xcd,0x53,0x14,0x9c,0xa3,0x6f,0x65,0xc4,0xaf,0x9c,0xb7,0x4c,0x6e,0x6a, 
        0x94,0xb6,0x32,0xfe,0xe4,0x6d,0x92,0x6a,0x56,0x24,0x80,0x5b,0xf3,0x12,0xfe,0x21, 
        0x8d,0x62,0x8b,0x79,0x0f,0x28,0x27,0x06,0xed,0xb0,0x1d,0x80,0x5c,0x7c,0xf1,0x05, 
        0x47,0xc4,0x9e,0xf3,0x72,0x4f,0xf1,0xd5,0xa3,0x6e,0x47,0x4a,0x28,0x80,0x75,0x4f, 
        0xbe,0x43,0x0d,0x8c,0x25,0x43,0xe0,0x53,0x43,0xd9,0x66,0x8c,0x50,0xe1,0xdc,0x49, 
        0x58,0x9f,0x63,0xa1,0x03,0x16,0x52,0x27,0x85,0xb0,0x11,0x02,0x97,0xf4,0x36,0x12, 
        0x63,0x48,0x2a,0x1f,0xbb,0xdb,0x24,0x7d,0x19,0xc4,0xa2,0x9c,0x9f,0x7c,0xd1,0xf3, 
        0xb2,0xaa,0xf4,0x4f,0xe5,0xf4,0x42,0x54,0x3a,0x4e,0xca,0xd2,0x76,0xf0,0xda,0xb5, 
        0x2c,0xfa,0x72,0x2a,0x0d,0x64,0xb9,0xcb,0xb8,0x37,0x9c,0x71,0x29,0xd0,0xe3,0xe5, 
        0x53,0x8d,0xfa,0x25,0x41,0x1d,0xc3,0x76,0x12,0x70,0x1c,0x6d,0x4e,0xf7,0xf4,0xae, 
        0x61,0xa3,0xa4,0x86,0xa6,0x53,0x5d,0x29,0x7d,0xbd,0xd2,0x30,0x9e,0xe6,0x9d,0x3a 

};


//Unicode 字符串描述符
//P08K-B16-A3-E24
const INT8U NandFlashInfoString_default[32]=
{
    0x20,0x03,
    0x50,0x00,          //P
    0x30,0x00,0x38,0x00,    //08
    0x4B,0x00,          //K
    0x2D,0x00,          //-
    0x42,0x00,          //B
    0x31,0x00,0x36,0x00,    //16
    0x2D,0x00,          //-
    0x41,0x00,          //A
    0x33,0x00,          //3
    0x2D,0x00,          //-
    0x45,0x00,          //E
    0x32,0x00,0x34,0x00 //24
};


#define GPOO    ((volatile INT32U *)0x08000C00)
#define DQSDLYSEL1  ((volatile INT32U *)0x08000c00)
#define DQSDLYSEL2  ((volatile INT32U *)0x08000c04)


void NFC_ReadParameterPage(void);


static inline nfcCmdDly(INT32U val)
{
    while(val --);
}

static inline nfcCmdWr(INT32U val)
{
    nfcCmd->wr[0] =val;
    while((nfcReg->mode>>30)&0x01);
    
}

static inline nfcCmdWPtr(INT32U offset,INT32U val)
{
    nfcCmd->wptr[offset]=val;
    while((nfcReg->mode>>30)&0x01);
    
}

static inline nfcCmdWTbl(INT32U offset,INT32U val,INT32U dly)
{
    nfcCmd->wtbl[offset]=val|(dly<<8);
    while((nfcReg->mode>>30)&0x01);
}
// open and close gpio port 0x08000520
static inline void NFC_FTL_access_off(void)
{
    GPIO_ICS_REG &= (unsigned)(~(0x20));
}

static inline void NFC_FTL_access_on(void)
{
    GPIO_ICS_REG |= 0x20;
}

//init array
void setMem(INT8U *buf, INT32U size, INT8U val)
{
    while(size --)
        buf[size] = val;        
}

//read from nfc
INT16U NFC_FMICFF_read(void)
{
    // do {} while (!(NFC_REG[NFC_FMICFF_SLOT]));
    while(!(nfcReg->fmicff_dslot>>16));
    
    NFC_FMICFFRD[0] = 0;
    return  NFC_FMICFFRD[0];
}


INT32U NFC_FMIOFF_read(void)
{
    while(!(nfcReg->fmioff_dslot>>16));
    NFC_FMIOFFRD[0] = 0;
    return NFC_FMIOFFRD[0];
}


void ONFI_Reset(void)
{   
    while((nfcReg->cmdff_dslot &0xffff)<6); 
    nfcCmdWr(CMD_FORCE(0x0500));    // FORCE 1, CE11, CE12  
    nfcCmdWr(CMD_NOP(tCS[onfi_speed],0));
    nfcCmdWr(CMD_ONFI(0xff));
    nfcCmdWr(CMD_NOP(tWB[onfi_speed],0));
    nfcCmdWr(CMD_NOP(tRST_power_on,0));
    nfcCmdWr(CMD_WAIT(ONFI_WAIT_RDY|0x100)); 
    nfcCmdWr(CMD_FORCE(0x0100));            // // FORCE 0, CE12 
    
}

void ONFI_SynchronousReset(void)
{
    while((nfcReg->cmdff_dslot &0xffff)<6); 
    nfcCmdWr(CMD_FORCE(0x500));                     // FORCE 1, CE11, CE12
    nfcCmdWr(CMD_ONFI(0xfc));
    nfcCmdWr(CMD_NOP(tWB[onfi_speed],0));
    nfcCmdWr(CMD_NOP(tRST_power_on,0));
    nfcCmdWr(CMD_WAIT(ONFI_WAIT_RDY|0x100));
    nfcCmdWr(CMD_FORCE(0x100));                      // FORCE 0, CE12
}


void ONFI_ReadID(INT8U addr)
{
    while((nfcReg->cmdff_dslot &0xffff)<3); 
    nfcCmdWr(CMD_ONFI(0x90));
    nfcCmdWr(CMD_ADDR(addr));
    nfcCmdWr(CMD_NOP(tWHR[onfi_speed],0));
}


void ONFI_ReadParameterPage(INT8U  addr)
{
    while((nfcReg->cmdff_dslot &0xffff)<5);
    nfcCmdWr(CMD_ONFI(0xec));
    nfcCmdWr(CMD_ADDR(addr));
    nfcCmdWr(CMD_NOP(tWB[onfi_speed],0));
    nfcCmdWr(CMD_WAIT(ONFI_WAIT_RDY|0x100));
    nfcCmdWr(CMD_NOP(tRR[onfi_speed],0));
}

void ONFI_ReadUniqueID(INT8U addr)
{
    while((nfcReg->cmdff_dslot &0xffff)<5);
    nfcCmdWr(CMD_ONFI(0xed));
    nfcCmdWr(CMD_ADDR(addr));
    nfcCmdWr(CMD_NOP(tWB[onfi_speed],0));
    nfcCmdWr(CMD_WAIT(ONFI_WAIT_RDY|0x100));
    nfcCmdWr(CMD_NOP(tRR[onfi_speed],0));
}

void ONFI_SetFeatures(INT8U addr, INT32U param)
{
    while((nfcReg->cmdff_dslot &0xffff)<11);
    nfcCmdWr(CMD_ONFI(0xef));
    nfcCmdWr(CMD_ADDR(addr));
    nfcCmdWr(CMD_NOP(tADL[onfi_speed],0));  
    nfcCmdWr(CMD_WAIT(ONFI_WAIT_RDY|0x100));    //mhy       
    nfcCmdWr(CMD_NOP(tWB[onfi_speed],0));       //mhy       
    nfcCmdWr(CMD_PUT(param&0xff));
    nfcCmdWr(CMD_PUT((param>>8)&0xff));
    nfcCmdWr(CMD_PUT((param>>16)&0xff));
    nfcCmdWr(CMD_PUT((param>>24)&0xff));
    nfcCmdWr(CMD_NOP(tWB[onfi_speed],ONFI_TO_FLASH));
    nfcCmdWr(CMD_WAIT(ONFI_WAIT_RDY|0x100));
}


void ONFI_GetFeatures(INT32U addr)
{
    while((nfcReg->cmdff_dslot &0xffff)<5);
    nfcCmdWr(CMD_ONFI(0xEE));
    nfcCmdWr(CMD_ADDR(addr));
    nfcCmdWr(CMD_NOP(tWB[onfi_speed],0));
    nfcCmdWr(CMD_WAIT(ONFI_WAIT_RDY|0x100));
    nfcCmdWr(CMD_NOP(tRR[onfi_speed],0));
}

INT16U ONFI_ReadStatus(void)
{
    while((nfcReg->cmdff_dslot &0xffff)<3);

    nfcCmdWr(CMD_ONFI(0x70));
    nfcCmdWr(CMD_NOP(tWHR[onfi_speed],0));
    
    if (nfcReg->mode & NFC_MODE_SYNC) {
        while((nfcReg->cmdff_dslot &0xffff)<6);
        nfcCmdWr(CMD_FORCE(0x19a00));           //FORCE 1, RE
        nfcCmdWr(CMD_NOP(tDQSD,0));             //NOP ,tDQSD
        nfcCmdWr(CMD_GET(1));
        nfcCmdWr(CMD_NOP(tDQSD,0));             //NOP ,tDQSD
        nfcCmdWr(CMD_FORCE(0x18a00));       
        nfcCmdWr(CMD_NOP(tRHW[onfi_speed],0));
    }
    else {
        nfcCmdWr(CMD_GET(1-1));
    }
    
    return(NFC_FMICFF_read());
}

//read status enhanced
INT16U ONFI_SelectLunWithStatus(INT32U addr)
{
    while((nfcReg->cmdff_dslot &0xffff)<6);
    nfcCmdWr(CMD_ONFI(0x78));
    nfcCmdWr(CMD_ADDR(addr&0xff));              
    nfcCmdWr(CMD_ADDR((addr>>8)&0xff));
    nfcCmdWr(CMD_ADDR((addr>>16)&0xff));
    nfcCmdWr(CMD_NOP(tWHR[onfi_speed],0));

    if (nfcReg->mode & NFC_MODE_SYNC) {
        while((nfcReg->cmdff_dslot &0xffff)<5); 
        nfcCmdWr(CMD_FORCE(0x19a00));           //FORCE 1, RE
        nfcCmdWr(CMD_NOP(tDQSD,0));             //NOP ,tDQSD
        nfcCmdWr(CMD_GET(1));
        nfcCmdWr(CMD_NOP(tDQSD,0));             //NOP ,tDQSD
        nfcCmdWr(CMD_FORCE(0x18a00));       
        nfcCmdWr(CMD_NOP(tRHW[onfi_speed],0));
    }
    else {
        nfcCmdWr(CMD_GET(1-1));
    }
     return(NFC_FMICFF_read());
}


void ONFI_ChangeReadColumn(INT16U addr)
{   
    while((nfcReg->cmdff_dslot &0xffff)<5);
    nfcCmdWr(CMD_ONFI(0x05));
    nfcCmdWr(CMD_ADDR(addr&0xff));              
    nfcCmdWr(CMD_ADDR((addr>>8)&0xff));
    nfcCmdWr(CMD_ONFI(0xE0));  
    nfcCmdWr(CMD_NOP(tCCS[onfi_speed],0));
}

//change read column enhanced definition
void ONFI_SelectCacheRegister(INT32U rowaddr, INT16U coladdr)
{
    while((nfcReg->cmdff_dslot &0xffff)<9);
    nfcCmdWr(CMD_ONFI(0x06));
    nfcCmdWr(CMD_NOP(tRHW[onfi_speed],0));
    
    nfcCmdWr(CMD_ADDR((coladdr&0xff)));             
    nfcCmdWr(CMD_ADDR((coladdr>>8)&0xff));    
    
    nfcCmdWr(CMD_ADDR(rowaddr&0xff));               
    nfcCmdWr(CMD_ADDR((rowaddr>>8)&0xff));
    nfcCmdWr(CMD_ADDR((rowaddr>>16)&0xff));   
    
    nfcCmdWr(CMD_ONFI(0xE0));
    nfcCmdWr(CMD_NOP(tCCS[onfi_speed],0));
 }

void ONFI_ChangeWriteColumn(INT16U addr)
{
    while((nfcReg->cmdff_dslot &0xffff)<4);
    nfcCmdWr(CMD_ONFI(0x85));
    nfcCmdWr(CMD_ADDR((addr&0xff)));                
    nfcCmdWr(CMD_ADDR((addr>>8)&0xff));  
    nfcCmdWr(CMD_NOP(tCCS[onfi_speed],ONFI_TO_FLASH));
 }


void ONFI_ChangeRowAddress(INT32U rowaddr, INT16U coladdr)
{
    while((nfcReg->cmdff_dslot &0xffff)<7);
    nfcCmdWr(CMD_ONFI(0x85));
    nfcCmdWr(CMD_ADDR((coladdr&0xff)));             
    nfcCmdWr(CMD_ADDR((coladdr>>8)&0xff));      
    nfcCmdWr(CMD_ADDR(rowaddr&0xff));               
    nfcCmdWr(CMD_ADDR((rowaddr>>8)&0xff));
    nfcCmdWr(CMD_ADDR((rowaddr>>16)&0xff)); 
    nfcCmdWr(CMD_NOP(tCCS[onfi_speed],0));
}



/* READ Operations */
void ONFI_ReadMode(void)
{
    nfcCmdWr(CMD_ONFI(0x00));
}

void ONFI_ReadPage(INT32U rowaddr, INT16U coladdr)
{
    while((nfcReg->cmdff_dslot &0xffff)<11);
    nfcCmdWr(CMD_ONFI(0x00));
    nfcCmdWr(CMD_ADDR((coladdr&0xff)));             
    nfcCmdWr(CMD_ADDR((coladdr>>8)&0xff));   
    
    nfcCmdWr(CMD_ADDR(rowaddr&0xff));               
    nfcCmdWr(CMD_ADDR((rowaddr>>8)&0xff));
    nfcCmdWr(CMD_ADDR((rowaddr>>16)&0xff));     
        
    nfcCmdWr(CMD_ONFI(0x30));
    nfcCmdWr(CMD_NOP(tWB[onfi_speed],0));
    nfcCmdWr(CMD_WAIT(ONFI_WAIT_SRRDY | ONFI_WAIT_SRARDY));

    
    nfcCmdWr(CMD_NOP(tRR[onfi_speed],0));
    
    //nfcCmdWr(CMD_ONFI(0x00));
}


void ONFI_ReadCacheSequential(void)
{
    while((nfcReg->cmdff_dslot &0xffff)<4);
    nfcCmdWr(CMD_ONFI(0x31));
    nfcCmdWr(CMD_NOP(tWB[onfi_speed],0));
    //nfcCmdWr(CMD_WAIT(ONFI_WAIT_SRRDY | ONFI_WAIT_SRARDY));
    nfcCmdWr(CMD_WAIT(ONFI_WAIT_SRRDY));
    nfcCmdWr(CMD_NOP(tRR[onfi_speed],0));
}


void ONFI_ReadPageCacheRandom(INT32U rowaddr, INT16U coladdr)
{
    while((nfcReg->cmdff_dslot &0xffff)<10);
    nfcCmdWr(CMD_ONFI(0x00));
    nfcCmdWr(CMD_ADDR((coladdr&0xff)));             
    nfcCmdWr(CMD_ADDR((coladdr>>8)&0xff));    
    
    nfcCmdWr(CMD_ADDR(rowaddr&0xff));               
    nfcCmdWr(CMD_ADDR((rowaddr>>8)&0xff));
    nfcCmdWr(CMD_ADDR((rowaddr>>16)&0xff)); 
    nfcCmdWr(CMD_ONFI(0x31));
    nfcCmdWr(CMD_NOP(tWB[onfi_speed],0));
    //nfcCmdWr(CMD_WAIT(ONFI_WAIT_SRRDY | ONFI_WAIT_SRARDY));
    nfcCmdWr(CMD_WAIT(ONFI_WAIT_SRRDY));
    nfcCmdWr(CMD_NOP(tRR[onfi_speed],0));
}
//read cache end 3F
void ONFI_ReadCacheLast(void)
{
    while((nfcReg->cmdff_dslot &0xffff)<4);
    nfcCmdWr(CMD_ONFI(0x3F));
    nfcCmdWr(CMD_NOP(tWB[onfi_speed],0));
    nfcCmdWr(CMD_WAIT(ONFI_WAIT_SRRDY | ONFI_WAIT_SRARDY));
    nfcCmdWr(CMD_NOP(tRR[onfi_speed],0));
}

void ONFI_ReadPageMultiPlane(INT32U rowaddr, INT16U coladdr)
{
    while((nfcReg->cmdff_dslot &0xffff)<10);
    nfcCmdWr(CMD_ONFI(0x00));
    nfcCmdWr(CMD_ADDR((coladdr&0xff)));             
    nfcCmdWr(CMD_ADDR((coladdr>>8)&0xff));    
    
    nfcCmdWr(CMD_ADDR(rowaddr&0xff));               
    nfcCmdWr(CMD_ADDR((rowaddr>>8)&0xff));
    nfcCmdWr(CMD_ADDR((rowaddr>>16)&0xff)); 
    nfcCmdWr(CMD_ONFI(0x32));
    nfcCmdWr(CMD_NOP(tWB[onfi_speed],0));
//  nfcCmdWr(CMD_WAIT(ONFI_WAIT_SRRDY | ONFI_WAIT_SRARDY));
    nfcCmdWr(CMD_WAIT(ONFI_WAIT_SRRDY));
    //nfcCmdWr(CMD_NOP(tRR[onfi_speed],0));
}



void ONFI_ProgramPage(INT32U rowaddr, INT16U coladdr)
{
    while((nfcReg->cmdff_dslot &0xffff)<7);

    nfcCmdWr(CMD_ONFI(0x80));
    nfcCmdWr(CMD_ADDR((coladdr&0xff)));             
    nfcCmdWr(CMD_ADDR((coladdr>>8)&0xff));    
    
    nfcCmdWr(CMD_ADDR(rowaddr&0xff));               
    nfcCmdWr(CMD_ADDR((rowaddr>>8)&0xff));
    nfcCmdWr(CMD_ADDR((rowaddr>>16)&0xff)); 
    nfcCmdWr(CMD_NOP(tADL[onfi_speed],0));
}


void ONFI_ProgramPage_end(void)
{
    while((nfcReg->cmdff_dslot &0xffff)<4);

    nfcCmdWr(CMD_ONFI(0x10));
    nfcCmdWr(CMD_NOP(tWB[onfi_speed],0));
    nfcCmdWr(CMD_WAIT(ONFI_WAIT_SRRDY | ONFI_WAIT_SRARDY));
    nfcCmdWr(CMD_NOP(tWB[onfi_speed],0));
}

void ONFI_ProgramPageCache_end(void)
{
    while((nfcReg->cmdff_dslot &0xffff)<4);

    nfcCmdWr(CMD_ONFI(0x15));
    //nfcCmdWr(CMD_ONFI(0x11));
    nfcCmdWr(CMD_NOP(tWB[onfi_speed],0));
    nfcCmdWr(CMD_WAIT(ONFI_WAIT_SRRDY ));
    nfcCmdWr(CMD_NOP(tWB[onfi_speed],0));
}


void ONFI_ProgramPageMultiPlane_end(void)
{
    while((nfcReg->cmdff_dslot &0xffff)<4);

    nfcCmdWr(CMD_ONFI(0x11));
    nfcCmdWr(CMD_NOP(tWB[onfi_speed],0));
    nfcCmdWr(CMD_WAIT(ONFI_WAIT_SRRDY ));
    nfcCmdWr(CMD_NOP(tWB[onfi_speed],0));
}




void ONFI_EraseBlock(INT32U rowaddr)
{
    while((nfcReg->cmdff_dslot &0xffff)<7);

    nfcCmdWr(CMD_ONFI(0x60));
    nfcCmdWr(CMD_ADDR(rowaddr&0xff));               
    nfcCmdWr(CMD_ADDR((rowaddr>>8)&0xff));
    nfcCmdWr(CMD_ADDR((rowaddr>>16)&0xff)); 
    nfcCmdWr(CMD_ONFI(0xD0));
    nfcCmdWr(CMD_NOP(tWB[onfi_speed],0));
    //nfcCmdWr(CMD_WAIT(ONFI_WAIT_RDY | 0x100);
    nfcCmdWr(CMD_WAIT(ONFI_WAIT_SRRDY | 0x100));
}

void ONFI_EraseBlockMultiPlane(INT32U addr)
{
   while((nfcReg->cmdff_dslot &0xffff)<7);

    nfcCmdWr(CMD_ONFI(0x60));
    nfcCmdWr(CMD_ADDR(addr&0xff));              
    nfcCmdWr(CMD_ADDR((addr>>8)&0xff));
    nfcCmdWr(CMD_ADDR((addr>>16)&0xff)); 
    nfcCmdWr(CMD_ONFI(0xD1));
    nfcCmdWr(CMD_NOP(tWB[onfi_speed],0));
    nfcCmdWr(CMD_WAIT(ONFI_WAIT_RDY | 0x100));
}

void ONFI_EraseBlockMultiLun(INT32U addr)
{
    
}


void ONFI_ProgramPageMultiLun_end(INT32U rowaddr)
{
    while((nfcReg->cmdff_dslot &0xffff)<4);

    nfcCmdWr(CMD_ONFI(0x10));//use where ?
    //switch lun
    nfcCmdWr(CMD_ONFI(0x78));
    nfcCmdWr(CMD_ADDR(rowaddr&0xff));               
    nfcCmdWr(CMD_ADDR((rowaddr>>8)&0xff));
    nfcCmdWr(CMD_ADDR((rowaddr>>16)&0xff)); 
    nfcCmdWr(CMD_NOP(tWHR[onfi_speed],0));
  // nfcCmdWr(CMD_ONFI(0x11));
    nfcCmdWr(CMD_NOP(tWB[onfi_speed],0));
    nfcCmdWr(CMD_WAIT(ONFI_WAIT_SRRDY ));
    nfcCmdWr(CMD_NOP(tWB[onfi_speed],0));
}


/* COPYBACK Operations */

void ONFI_CopybackRead_start(INT32U rowaddr, INT16U coladdr)
{
    while((nfcReg->cmdff_dslot &0xffff)<6);

    nfcCmdWr(CMD_ONFI(0x00));
    nfcCmdWr(CMD_ADDR((coladdr&0xff)));             
    nfcCmdWr(CMD_ADDR((coladdr>>8)&0xff));    
    
    nfcCmdWr(CMD_ADDR(rowaddr&0xff));               
    nfcCmdWr(CMD_ADDR((rowaddr>>8)&0xff));
    nfcCmdWr(CMD_ADDR((rowaddr>>16)&0xff)); 

/*
    if (nfcReg->mode& NFC_MODE_SYNC) {
        while((nfcReg->cmdff_dslot &0xffff)<6);;    // check 5 more

        nfcCmdWr(CMD_FORCE(0x19a00));     // FORCE 1, RE
        nfcCmdWr(CMD_NOP(tDQSD,0));      // NOP, tDQSD
        nfcCmdWr(CMD_GET(cnt));
        nfcCmdWr(CMD_NOP(tDQSD,0));      // NOP, tDQSD
        nfcCmdWr(CMD_FORCE(0x18a00));     // FORCE 0, RE
        nfcCmdWr(CMD_NOP(tRHW[onfi_speed],0));
    }
    else {
        nfcCmdWr(CMD_GET(cnt - 1));         // minus 1
    }
*/
}
void ONFI_CopybackRead_end(void)
{
    while((nfcReg->cmdff_dslot &0xffff)<4);
    nfcCmdWr(CMD_ONFI(0x35));

    nfcCmdWr(CMD_NOP(tWB[onfi_speed],0));
    nfcCmdWr(CMD_WAIT(ONFI_WAIT_SRRDY | ONFI_WAIT_SRARDY));
    nfcCmdWr(CMD_NOP(tRR[onfi_speed],0));
}

void ONFI_CopybackRead(INT32U rowaddr, INT16U coladdr, INT32U cnt)
{
    while((nfcReg->cmdff_dslot &0xffff)<10);

    nfcCmdWr(CMD_ONFI(0x00));
    nfcCmdWr(CMD_ADDR((coladdr&0xff)));             
    nfcCmdWr(CMD_ADDR((coladdr>>8)&0xff));    
    
    nfcCmdWr(CMD_ADDR(rowaddr&0xff));               
    nfcCmdWr(CMD_ADDR((rowaddr>>8)&0xff));
    nfcCmdWr(CMD_ADDR((rowaddr>>16)&0xff)); 


    nfcCmdWr(CMD_ONFI(0x35));
    
    nfcCmdWr(CMD_NOP(tWB[onfi_speed],0));
    nfcCmdWr(CMD_WAIT(ONFI_WAIT_SRRDY | ONFI_WAIT_SRARDY));
    nfcCmdWr(CMD_NOP(tRR[onfi_speed],0));


    if (nfcReg->mode& NFC_MODE_SYNC) {
        while((nfcReg->cmdff_dslot &0xffff)<6);;    // check 5 more

        nfcCmdWr(CMD_FORCE(0x19a00));       // FORCE 1, RE
        nfcCmdWr(CMD_NOP(tDQSD,0));         // NOP, tDQSD
        nfcCmdWr(CMD_GET(cnt));
        nfcCmdWr(CMD_NOP(tDQSD,0));         // NOP, tDQSD
        nfcCmdWr(CMD_FORCE(0x18a00));       // FORCE 0, RE
        nfcCmdWr(CMD_NOP(tRHW[onfi_speed],0));
    }
    else {
        nfcCmdWr(CMD_GET(cnt - 1));         // minus 1
    }

}

void ONFI_CopybackProgramInterleaved(void)
{

}
/* ========================2016 07 25 begin================================== */
void ONFI_CopybackProgram(INT32U rowaddr, INT16U coladdr)
{
    while((nfcReg->cmdff_dslot &0xffff)<10);

    nfcCmdWr(CMD_ONFI(0x85));
    nfcCmdWr(CMD_ADDR((coladdr&0xff)));             
    nfcCmdWr(CMD_ADDR((coladdr>>8)&0xff));
    
    nfcCmdWr(CMD_ADDR(rowaddr&0xff));               
    nfcCmdWr(CMD_ADDR((rowaddr>>8)&0xff));
    nfcCmdWr(CMD_ADDR((rowaddr>>16)&0xff)); 
    
    nfcCmdWr(CMD_ONFI(0x10));
    
    nfcCmdWr(CMD_NOP(tWB[onfi_speed],0));   
    nfcCmdWr(CMD_WAIT(ONFI_WAIT_SRRDY | ONFI_WAIT_SRARDY));
    nfcCmdWr(CMD_NOP(tWB[onfi_speed],0));
}
void ONFI_CopybackProgram_start(INT32U rowaddr, INT16U coladdr)
{
    while((nfcReg->cmdff_dslot &0xffff)<10);

    nfcCmdWr(CMD_ONFI(0x85));
    nfcCmdWr(CMD_ADDR((coladdr&0xff)));             
    nfcCmdWr(CMD_ADDR((coladdr>>8)&0xff));
    
    nfcCmdWr(CMD_ADDR(rowaddr&0xff));               
    nfcCmdWr(CMD_ADDR((rowaddr>>8)&0xff));
    nfcCmdWr(CMD_ADDR((rowaddr>>16)&0xff)); 
    nfcCmdWr(CMD_NOP(tADL[onfi_speed],0));
}

void ONFI_CopybackProgram_end(void)
{
    while((nfcReg->cmdff_dslot &0xffff)<4); 
    nfcCmdWr(CMD_ONFI(0x10));
    
    nfcCmdWr(CMD_NOP(tWB[onfi_speed],0));   
    nfcCmdWr(CMD_WAIT(ONFI_WAIT_SRRDY | ONFI_WAIT_SRARDY));
    nfcCmdWr(CMD_NOP(tWB[onfi_speed],0));
}

/* ========================2016 07 25 end================================== */

/* ================================================================== */

/* One-Time Programmable (OTP) Operations */

/* ================================================================== */





void ONFI_receive8(INT8U *buff, INT32U cnt)
{
     if (nfcReg->mode& NFC_MODE_SYNC) {
       while((nfcReg->cmdff_dslot &0xffff)<5);

        nfcCmdWr(CMD_FORCE(0x19a00));     // FORCE 1, RE
        nfcCmdWr(CMD_NOP(tDQSD,0));       // NOP, tDQSD
        nfcCmdWr(CMD_GET(cnt));
        nfcCmdWr(CMD_NOP(tDQSD,0));       // NOP, tDQSD
        nfcCmdWr(CMD_FORCE(0x18a00));     // FORCE 0, RE
    }
    else {
        while((nfcReg->cmdff_dslot &0xffff)<1);;

        nfcCmdWr(CMD_GET(cnt - 1));       // minus 1
    }

    while (cnt--)
        *(buff++) = (INT8U)NFC_FMICFF_read();
}

void ONFI_receive16(INT16U *buff, INT32U cnt)
{
    if (nfcReg->mode& NFC_MODE_SYNC) {
        while((nfcReg->cmdff_dslot &0xffff)<5);

        nfcCmdWr(CMD_FORCE(0x19a00));     // FORCE 1, RE
        nfcCmdWr(CMD_NOP(tDQSD,0));       // NOP, tDQSD
        nfcCmdWr(CMD_GET(cnt));
        nfcCmdWr(CMD_NOP(tDQSD,0));       // NOP, tDQSD
        nfcCmdWr(CMD_FORCE(0x18a00));     // FORCE 0, RE
    }
    else {
        while((nfcReg->cmdff_dslot &0xffff)<1);

        nfcCmdWr(CMD_GET(cnt - 1));       // minus 1
    }

    while (cnt--)
        *(buff++) = NFC_FMICFF_read();
}


void ONFI_dma_read(INT32U cnt, INT32U flag,INT8U swBus)
{
    // adjust cnt according to NFC_MODE
    //jprintf("mode %8.8x\n",nfcReg->mode);
    //jprintf("write mode %8.8x\n",nfcReg->mode);
    //NFC_Set_Mode(NFC_MODE_ASYNC);//mode = 8bit
    switch (nfcReg->mode& (NFC_MODE_D16 | NFC_MODE_SYNC)) {
        case NFC_MODE_SYNC:
            cnt = ((cnt + 3) >> 2) << 1;
            break;
        case (NFC_MODE_D16 | NFC_MODE_SYNC):
            cnt = (cnt + 3) >> 2;
            break;
        case NFC_MODE_D16:
            cnt = (((cnt + 3) >> 2) << 1) - 1;
            break;
        default:
            cnt = (((cnt + 3) >> 2) << 2) - 1;
    }
    if(swBus ==0x01)    
        nfcCmdWr(CMD_ONFI(0x00));
    if (nfcReg->mode& NFC_MODE_SYNC) {
        while((nfcReg->cmdff_dslot &0xffff)<7);

        nfcCmdWr(CMD_FORCE(0x19a00));             // FORCE 1, RE
        nfcCmdWr(CMD_NOP(tDQSD,0));               // NOP, tDQSD
        nfcCmdWr(CMD_NOP(tWHR[onfi_speed],0));    // NOP, tWHR
        nfcCmdWr(CMD_DMA_GET(cnt,flag));
        nfcCmdWr(CMD_NOP(tDQSD,ONFI_USE_DMA));    // NOP, tDQSD
        nfcCmdWr(CMD_FORCE(0x18a10));             // FORCE 0, RE
        nfcCmdWr(CMD_NOP(tRHW[onfi_speed],ONFI_USE_DMA));
    }
    else {
         while((nfcReg->cmdff_dslot &0xffff)<3);
        
        //nfcCmdWr(CMD_NOP(tWHR[onfi_speed],0));  // NOP, tWHR
        nfcCmdWr(CMD_DMA_GET(cnt,flag));
        //nfcCmdWr(CMD_NOP(tRHW[onfi_speed],ONFI_USE_DMA));
        nfcCmdWr(CMD_NOP(tWHR[onfi_speed],(ONFI_USE_DMA | flag)));
     
    }
}

void ONFI_dma_write(INT32U cnt, INT32U flag)
{
    while((nfcReg->cmdff_dslot &0xffff)<2);

    // adjust cnt according to NFC_MODE
    //jprintf("write mode %8.8x\n",nfcReg->mode);
    //NFC_Set_Mode(NFC_MODE_D16);//mode =16bit
    switch(nfcReg->mode& (NFC_MODE_D16 | NFC_MODE_SYNC)) {
        case NFC_MODE_SYNC:
            cnt = ((cnt + 3) >> 2) << 1;
            break;
        case (NFC_MODE_D16 | NFC_MODE_SYNC):
            cnt = (cnt + 3) >> 2;
            break;
        case NFC_MODE_D16:
            cnt = (((cnt + 3) >> 2) << 1) - 1;
            break;
        default:
            cnt = (((cnt + 3) >> 2) << 2) - 1;
    }

    nfcCmdWr(CMD_DMA_PUT(cnt, flag));
    nfcCmdWr(CMD_NOP(tWHR[onfi_speed],ONFI_USE_DMA | ONFI_TO_FLASH | flag));
}

/* ================================================================== */


// open and grow the simulated target files if necessary.
// ONFI Modes
// mode 0: >=50ns
// mode 1: >=30ns < 50ns
// mode 2: >=20ns < 30ns
// mode 3: >=15ns < 20ns
// mode 4: >=12ns < 15ns
// mode 5: >=10ns <=12ns

// Select clocks for different ONFI modes
// Actual PLL clock is greater than 400MHz
// mode 0: 400MHz/20 = 20MHz (50ns) ==> suitable for ONFI mode 1
// mode 1: 400MHz/12 = 33.33MHz (30ns)  ==> suitable for ONFI mode 2
// mode 2: 400MHz/8  = 50MHz (20ns) ==> suitable for ONFI mode 3
// mode 3: 400MHz/6  = 66.66MHz (15ns) ==> suitable for ONFI mode 4
// mode 4: 400MHz/4  = 100 MHz (10ns) ==> suitable for ONFI mode 5
void NFC_Init(INT8U onficlk_mode)
{
    switch (NFC_CLK_DIV) {
        case 12:
            GPOO[2] |= (0x00000008+1);
            break;
        case 8:
            GPOO[2] |= (0x00000008+2);
            break;
        case 6:
            GPOO[2] |= (0x00000008+3);
            break;
        case 4:
            GPOO[2] |= (0x00000008+4);
            break;
        default :
            GPOO[2] |= (0x00000008+1);
            break;
    }
    GPOO[1] = 0x00000000;
    GPOO[1] = 0x00000000;
    GPOO[1] = 0x00000000;
    GPOO[1] = 0x00000000;

    //----------------------------------------------------------------
    // This section of code must be present to enable clocks to NFC
    GPIO_GCC_REG |= 0x80000;

//----------------------------------------------------------------
// This section of code is to set DQS delay
// The dqs delay model for rtlsim can provide delay in odd number, 
// i.e. 1,3,5,7,9 .... So the value for DQSDLYSEL=2^[(delay-1)/2].
// For 11ns delay, DQSDLYSEL=2^[(11-1)/2]=2^5=0x20.
// For 13ns delay, DQSDLYSEL=2^[(13-1)/2]=2^6=0x40.
// For 19ns delay, DQSDLYSEL=2^[(19-1)/2]=2^9=0x200.
    switch (onficlk_mode) {
        case 0:
            DQSDLYSEL1[0] = 0x00000040; // 13ns
            DQSDLYSEL2[0] = 0x00000000;
            break;
        case 1:
            DQSDLYSEL1[0] = 0x00000008; // 7ns
            //DQSDLYSEL1[0] = 0x00000010; // 9ns
            DQSDLYSEL2[0] = 0x00000000;
            break;
        case 2:
            DQSDLYSEL1[0] = 0x00000004; // 5ns
            DQSDLYSEL2[0] = 0x00000000;
            break;
        case 3:
            DQSDLYSEL1[0] = 0x00000004; // 5ns
            DQSDLYSEL2[0] = 0x00000000;
            break;
        case 5:
            DQSDLYSEL1[0] = 0x00000002; // 3ns
            DQSDLYSEL2[0] = 0x00000000;
        break;
        default:
            DQSDLYSEL1[0] = 0x00000040; // 13ns
            DQSDLYSEL2[0] = 0x00000000;
        break;
            
    }
    
    nfcReg->reset = 0x00;
}


void NFC_Set_Mode(INT8U face_mode)
{
    // wait for CMDFF is empty
    while((nfcReg->cmdff_dslot &0xffff)<cmdFifoSize);
    
    // "0x1f" selects the page size of 4k per page and numbers of sector per page
    nfcReg->mode = (nfcReg->mode & 0xffff0000) | 0x1f00 | face_mode;

    if (face_mode & NFC_MODE_SYNC) {
        // Prepare HW Read Status RG_RS0 - RG_RS9
        // for sync speed mode 2
        //nfcReg->enable |= 0xc0;           //BT_RS_ENB, BT_FMC_ENB
        nfcReg->rs_cmd = 0x00007020;        // CMD, 70h       
        nfcReg->rs_force1 = 0x00019a06;     // FORCE 1, RE
        nfcReg->rs_whr_nop = 0x00000084;    // NOP, tDQSD
        nfcReg->rs_get = 0x00000182;        // GET, 1 cycles
        nfcReg->rs_clk_nop = 0x00000084;    // NOP, 20/tCLK+1
        nfcReg->rs_force0 = 0x00018a06;     // FORCE 0, RE
        nfcReg->rs_rhw_nop = 0x01000384;    // NOP, tRHW, rs_last        
    } 
    else {
        //----------------------------------------------------------------
        // Prepare HW Read Status RG_RS0 - RG_RS3
        // for async speed mode 0
        nfcReg->rs_cmd = 0x00007020;        // CMD, 70h
        nfcReg->rs_force1 = 0x00000384;     // NOP, tWHR=120
        nfcReg->rs_whr_nop = 0x00000082;    // GET, 1 cycles
        nfcReg->rs_get = 0x01000684;        // NOP, tWHR=200, rs_last
    }
}

void ONFI_set_speed(INT8U speed)
{

    while((nfcReg->cmdff_dslot &0xffff)<cmdFifoSize);

    // !!! do not change CMDTBL unless SET_FEATURE is completed !!!
    // !!! watch out for the latency of CMDFF commands execution !!!

    //OP=CMD 0~2   
    nfcCmdWTbl(0,0x00000049,0);    //OP=CMD 
    nfcCmdWTbl(1,0x00000048,tCLS[speed]);    //tCLS=50ns command setup time
    nfcCmdWTbl(2,0x000000c1,tCLH[speed]);    //tCLH=20ns command hold time
    //OP=ADR 3~5  
    nfcCmdWTbl(3,0x0000004a,0);    //OP=ADDR
    nfcCmdWTbl(4,0x00000048,tALS[speed]);    //tALS=50ns address setup time
    nfcCmdWTbl(5,0x000000c2,tALH[speed]);    //tALH=20ns address hold time
    //OP=GET  6~7
    nfcCmdWTbl(6,0x00000104,0);    //OP=GET
    nfcCmdWTbl(7,0x00000084,tRP[speed]);    //tRP=50ns RE# pulse width
    //OP=PUT 8~9
    nfcCmdWTbl(8,0x00000148,0);    //OP=PUT
    nfcCmdWTbl(9,0x000000c8,tWP[speed]);    //tWP=50ns  WE#pulse width
    
    onfi_speed = speed;
}



int ONFI_init(INT8U onficlk_mode,INT8U face_mode)
{
    
    
    NFC_Init(onficlk_mode);
    
    NFC_Set_Mode(face_mode);
    //Enable FMC(flash memroy controller ) and auto ReadStatus
    nfcReg->enable |=(NFC_ENABLE_FMC | NFC_ENABLE_RS);
    
    onfi_speed = 0;
    //----------------------------------------------------------------
    // Assume clock freq. of 33.33MHz (30ns clock)
    // Prepare CMDPTR and CMDTBL for async mode 0 and sync mode 1
    //----------------------------------------------------------------
    // CMDPTR    
    nfcCmdWPtr(0,0x00000200);   //OP=CMD
    nfcCmdWPtr(1,0x00000263);   //OP=ADDR
    nfcCmdWPtr(2,0x000002c6);   //OP=GET
    nfcCmdWPtr(3,0x000002e8);   //OP=PUT
    nfcCmdWPtr(4,0x000001ce);   //OP=NOP
    nfcCmdWPtr(6,0x000001ef);   //OP=FORCE
        
    
    // CMDTBL
    //OP=CMD 0~2   
    nfcCmdWTbl(0,0x00000049,0);          //OP=CMD 
    nfcCmdWTbl(1,0x00000048,tCLS[0]);    //tCLS=50ns command setup time
    nfcCmdWTbl(2,0x000000c1,tCLH[0]);    //tCLH=20ns command hold time
    //OP=ADR 3~5  
    nfcCmdWTbl(3,0x0000004a,0);          //OP=ADDR
    nfcCmdWTbl(4,0x00000048,tALS[0]);    //tALS=50ns address setup time
    nfcCmdWTbl(5,0x000000c2,tALH[0]);    //tALH=20ns address hold time
    //OP=GET  6~7
    nfcCmdWTbl(6,0x00000104,0);          //OP=GET
    nfcCmdWTbl(7,0x00000084,tRP[0]);     //tRP=50ns RE# pulse width
    //OP=PUT 8~9
    nfcCmdWTbl(8,0x00000148,0);          //OP=PUT
    nfcCmdWTbl(9,0x000000c8,tWP[0]);     //tWP=50ns  WE#pulse width

        
    nfcCmdWTbl(14,0x00000080,0);    //OP=NOP
    nfcCmdWTbl(15,0x00000090,0);    //OP=FORCE
                             
    nfcCmdWTbl(16,0x00000041,0);    //OP=CMD
    nfcCmdWTbl(17,0x00000041,0);    //tCK
    nfcCmdWTbl(18,0x000000c0,0);    //OP=CMD_3 , tCAD
                            
    nfcCmdWTbl(19,0x00000042,0) ;   //OP=ADDR
    nfcCmdWTbl(20,0x00000042,0);    //tCK
    nfcCmdWTbl(21,0x000000c0,0);    //tCAD
                             
    nfcCmdWTbl(22,0x000000a3,0);    //OP=GET
    nfcCmdWTbl(23,0x000000e3,0);    //OP=PUT
    
    
    //----------------------------------------------------------------
    // Set up BCHE Main Data, parameters from config.h
    // For sync. mode N must be DINT16U aligned 
    
    nfcReg->kn_data =  ((kDATA_NLEN - 1) << 16) | (kDATA_KLEN - 1);
    nfcReg->ts_data =  (kDATA_SEED << 16) | kDATA_T;
        
    // Set up BCHE Aux Data, parameters from config.h
    nfcReg->kn_aux = ((kAUX_NLEN - 1) << 16) | (kAUX_KLEN - 1);
    nfcReg->ts_aux = (kAUX_SEED << 16) | kAUX_T;
    
    return 0;
}

void NFC_Flash_Init(INT32U mode,INT32U speedMode)
{

    nfcReg = (NFC*)NFC_BASS_ADDRESS; 
    nfcCmd = (NFC_CMD_FIFO*)(NFC_BASS_ADDRESS+0x4000);
    //cmdFifoSize = 32;
    NFC_FMIOFFRD = (INT32U *)0x20035800;
    NFC_FMICFFRD = (INT32U *)0x20035C00;
    NFC_FTL = (INT32U *)0x2003C000;

    memset8((BYTE*)(&nfcTrsState), 0, sizeof(NFC_TRS_STATE));
    
    switch (mode) {
        case ASYNC16_MODE:
            ONFI_init(speedMode,NFC_MODE_D16);
            ONFI_Reset();
            ONFI_SetFeatures(1,speedMode); //(1, 0x05);
            ONFI_set_speed(speedMode);  
            break;
        case SYNC16_MODE:   
            ONFI_init(speedMode,NFC_MODE_D16);
            ONFI_Reset();
            ONFI_SetFeatures(1,0x10|speedMode); //(1, 0x11);
            NFC_Set_Mode(NFC_MODE_SYNC | NFC_MODE_D16);
            ONFI_SynchronousReset();
            ONFI_set_speed(speedMode);
            break;
        case ASYNC8_MODE:
            ONFI_init(speedMode,0);
            ONFI_Reset();
            ONFI_SetFeatures(1,speedMode); //(1, 0x05);
            ONFI_set_speed(speedMode);
            break;
        case SYNC8_MODE:
            ONFI_init(speedMode,0);
            ONFI_Reset();
            ONFI_SetFeatures(1,0x10|speedMode); //(1, 0x11);
            NFC_Set_Mode(NFC_MODE_SYNC);
            ONFI_SynchronousReset();
            ONFI_set_speed(speedMode);
            break;
        default:
            break;
    }
    
}

void NFC_Reset()
{
    NFC_Flash_Init(NFC_FACE_MODE, NFC_SPEED_MODE);
}

//ftl_len bit[23:12]
//ftl_eol:bit[31]
//ftl_sar:bit[11:0]
//aux data to Flash
void setup_NFC_AUX(void)
{
    INT32U tmp=0;
    NFC_FTL_access_on();
    nfcReg->sar = 0;    
    INT32U *pStdBuf = (INT32U *)stdBuf;
    INT8U i=0;
    for (i = 0; i< (kAUX_BYTES/4);i++) {   
#if NFC_STD_DATA_USE    
        NFC_FTL[i] = *(pStdBuf++);  
#else
        NFC_FTL[NFC_AUX_DAT_ADR+i]=tmp;
        if(tmp == 0xffffffff)
            tmp = 0x00000000;
        else
            tmp += 0x01010101;      
#endif      
    }
}

/*==============================2016 07 26 ===================*/
void setup_NFC_FTL(INT16U size,INT32U data)
{
    INT32U tp_rdat, tp_addr;
    INT32U i,tmp=0;
    INT32U *pStdBuf = (INT32U *)stdBuf;
    INT16U stdBufSizeMax = STD_DATA_SIZE_MAX/4;
    NFC_FTL_access_on();
        
    //size=(size+7)/8*8;            // 64-bit aligned
            
    for (i = 0; i < (size/4); i++) {
#if NFC_STD_DATA_USE        
        //NFC_FTL[NFC_FTL_DAT_ADR+i] = *(pStdBuf+i%stdBufSizeMax);
        NFC_FTL[NFC_FTL_DAT_ADR+i] = data;
#else
        NFC_FTL[NFC_FTL_DAT_ADR+i]=tmp;

        if(tmp == 0xffffffff)
            tmp = 0x00000000;
        else
            tmp += 0x01010101;
#endif
    }
            
    nfcReg->lsar = NFC_FTL_CMD_ADR;
    tp_rdat = nfcReg->lsar;
        
    NFC_FTL[tp_rdat] = (0x80000000)|NFC_FTL_DAT_ADR|((size/4-1)<<12);
    
}


/*==============================2016 07 26 ===================*/
void get_NFC_FTL(INT16U size)
{
    
    INT32U tp_rdat,i;   
    NFC_FTL_access_on();
    nfcReg->lsar = NFC_FTL_CMD_ADR;
    tp_rdat = nfcReg->lsar;

    NFC_FTL[tp_rdat] = (0x80000000)|NFC_FTL_DAT_ADR|((size/4-1)<<12);
    /*
    size=(size+7)/8*8;          // 64-bit aligned

    INT16U times = (size+kDATA_BYTES-1)/kDATA_BYTES;
    INT16U revSize = size;
    INT16U curSendSize=0;
    INT16U havSendSize=0;
    for(i=0;i<times;i++)    
    {
        curSendSize = (revSize>kDATA_BYTES)?kDATA_BYTES:revSize;
        
        if(i!=(times-1))
        {
            NFC_FTL[tp_rdat + i] = 0x00000200|((curSendSize/4-1)<<12)+havSendSize/4;//0x8007f200;       
        }else
        {   
            NFC_FTL[tp_rdat + i] = 0x80000200|((curSendSize/4-1)<<12)+havSendSize/4;//0x8007f200;   
        }   
        havSendSize +=curSendSize;
    }   
    */
}
/*==============================2016 07 26 ===================*/




//addr switch
//blockAdr = curSectorAdr/SECTOR_PER_BLOCK;
//pageAdr = (curSectorAdr%SECTOR_PER_BLOCK)/SECTOR_PER_PAGE;
//
//pageAdr %2==0 ==> pageAdr = pageAdr  blockAdr=blockAdr  
//pageAdr %2!=0 ==> pageAdr = pageAdr -1   blockAdr = blockAdr +1

// secorAdr = SECTOR_PER_BLOCK*blockAdr + pageAdr*SECTOR_PER_PAGE
// blockAdr = curSectorAdr/SECTOR_PER_BLOCK;
// pageAdr = (curSectorAdr%SECTOR_PER_BLOCK)/SECTOR_PER_PAGE;
// jprintf(" W %d %d \n",blockAdr,pageAdr);

//TODO:注意!!这里读写操作被我改过了的,实际运用中并非如此
void Disk_Program(INT32U sectorAdr,INT32U sectorNum)
{
    INT32U data_bytes = kDATA_BYTES*sectorNum;
    INT32U data_pack;  //= kDATA_PACK*sectorNum;        
    INT32U blockAdr;   //= sectorAdr/SECTOR_PER_BLOCK;;
    INT32U pageAdr;    //= (sectorAdr%SECTOR_PER_BLOCK)/SECTOR_PER_PAGE;
    INT32U rowAdr ;    //= sectorAdr/SECTOR_PER_PAGE;
    INT16U colAdr;
//  jprintf("W0 %d %d \n",blockAdr,pageAdr);
    nfcReg->dmaw_endcnt = data_bytes/8 -1;
    nfcReg->h2f_endcnt = data_bytes/8-1;
    
#if (NFC_FTL_WRITE_EN==OPEN)
    setup_NFC_AUX();
    nfcReg->aux_endcnt = ((kAUX_BYTES+7)/8) -1;
    nfcReg->enable |=NFC_ENABLE_DMAW | NFC_ENABLE_BCHE | NFC_ENABLE_H2F|NFC_ENABLE_AUX;
#elif (NFC_FTL_WRITE_EN==CLOSE)
    nfcReg->enable |=NFC_ENABLE_DMAW | NFC_ENABLE_BCHE | NFC_ENABLE_H2F;
#endif

    INT32U resSectorNum = sectorNum;
    INT32U curSectorNum = 0;
    INT32U curSectorAdr = sectorAdr;

    
#if  NFC_MULTI_PLANE_EN
    INT32U swCurSectorAdr = 0x00;
    INT32U rowAdr2=0;
    while (resSectorNum > 0) {
        curSectorNum = (resSectorNum>SECTOR_PER_PAGE)?SECTOR_PER_PAGE:resSectorNum;
        data_pack = curSectorNum *  kDATA_PACK;
        swCurSectorAdr=addrMap(curSectorAdr);       
        rowAdr = swCurSectorAdr /SECTOR_PER_PAGE;
        colAdr = 0;//curSectorAdr%rowAdr;
        blockAdr = swCurSectorAdr/SECTOR_PER_BLOCK;
        pageAdr = (swCurSectorAdr%SECTOR_PER_BLOCK)/SECTOR_PER_PAGE;        
        if((swCurSectorAdr%SECTOR_PER_BLOCK)==0)
            if (blockAdr%2 == 0) {
                rowAdr2 = rowAdr+SECTOR_PER_BLOCK/SECTOR_PER_PAGE;          
                ONFI_EraseBlockMultiPlane(rowAdr);
                ONFI_EraseBlock(rowAdr2);
            }       
        ONFI_ProgramPage(rowAdr, colAdr);    
        ONFI_dma_write(data_pack, 0);   
        curSectorAdr +=curSectorNum;
        resSectorNum -=curSectorNum;
        
    #if (NFC_FTL_WRITE_EN==OPEN)
        #if (NFLASHWITH==16)    
            ONFI_ChangeWriteColumn(data_pack>>1);
        #elif (NFLASHWITH ==8)
            ONFI_ChangeWriteColumn(data_pack);
        #endif  
            ONFI_dma_write(kAUX_PACK,ONFI_AUX_DATA);
    #endif      

        if((resSectorNum>0) || (nfcTrsState.transferLastFlag==LAST_TRANS_FAIL)) {
            if(nfcTrsState.Ping_Pong==1)
                ONFI_ProgramPageCache_end();
            else
                ONFI_ProgramPageMultiPlane_end();
        }
        else {
            ONFI_ProgramPage_end();
        }
        nfcTrsState.Ping_Pong = (nfcTrsState.Ping_Pong==0)?1:0;
    }

#else
    while (resSectorNum > 0) {
        curSectorNum = (resSectorNum>SECTOR_PER_PAGE)?SECTOR_PER_PAGE:resSectorNum;
        data_pack = curSectorNum *  kDATA_PACK;
        rowAdr = curSectorAdr / SECTOR_PER_PAGE;
        colAdr = 0;//((curSectorAdr % SECTOR_PER_PAGE)*kDATA_PACK)>>1;
        if ((curSectorAdr % SECTOR_PER_BLOCK) == 0) {
            ONFI_EraseBlock(rowAdr);
        }
        
        ONFI_ProgramPage(rowAdr, colAdr);    
        ONFI_dma_write(data_pack, 0);   
        curSectorAdr +=curSectorNum;
        resSectorNum -=curSectorNum;

        #if (NFC_FTL_WRITE_EN==OPEN)
            #if     (NFLASHWITH==16)    
                ONFI_ChangeWriteColumn(data_pack>>1);
            #elif (NFLASHWITH ==8)
                ONFI_ChangeWriteColumn(data_pack);
            #endif  
            ONFI_dma_write(kAUX_PACK,ONFI_AUX_DATA);
        #endif

        #if  NFC_CACHE_EN       
            if((resSectorNum>0)||(nfcTrsState.transferLastFlag==LAST_TRANS_FAIL))
                ONFI_ProgramPageCache_end();
            else            
                ONFI_ProgramPage_end();
        #else
            ONFI_ProgramPage_end();
        #endif      
     }
    
#endif  
}


//  blockAdr = curSectorAdr/SECTOR_PER_BLOCK;
//  pageAdr = (curSectorAdr%SECTOR_PER_BLOCK)/SECTOR_PER_PAGE;
//  jprintf("R %d %d \n",blockAdr,pageAdr); 

//TODO:注意!!这里读写操作被我改过了的,实际运用中并非如此
void Disk_Read(INT32U sectorAdr,INT32U sectorNum)
{
    INT32U data_bytes = kDATA_BYTES*sectorNum;
    INT32U data_pack ;// = kDATA_PACK*sectorNum;    
    INT32U blockAdr ;//= sectorAdr/SECTOR_PER_BLOCK;
    INT32U pageAdr ;//=  (sectorAdr%SECTOR_PER_BLOCK)/SECTOR_PER_PAGE;  
    INT32U rowAdr ;//= sectorAdr/SECTOR_PER_PAGE;
    INT16U colAdr;// = 0x00;
    
#if (NFC_FTL_READ_EN == OPEN)
    get_NFC_FTL(kAUX_BYTES);
#endif

    nfcReg->dmar_endcnt= (data_bytes / 8) - 1; 
    nfcReg->enable |= NFC_ENABLE_DMAR | NFC_ENABLE_F2H | NFC_ENABLE_BCHD;   
    INT32U resSectorNum = sectorNum;
    INT32U curSectorNum = 0;
    INT32U curSectorAdr = sectorAdr;    
    
#if  NFC_MULTI_PLANE_EN
    INT32U swCurSectorAdr = 0x00;
    swCurSectorAdr=addrMap(curSectorAdr);
    rowAdr = swCurSectorAdr /SECTOR_PER_PAGE;
    colAdr = 0;
    
    if(nfcTrsState.transferFirstFlag==FIRST_TRANS_TRUE) {
        ONFI_ReadPage(rowAdr, colAdr);
    }

    while(resSectorNum>0) {   
        curSectorNum = (resSectorNum>SECTOR_PER_PAGE)?SECTOR_PER_PAGE:resSectorNum;
        data_pack = curSectorNum *  kDATA_PACK;
        
        curSectorAdr +=curSectorNum;
        resSectorNum -=curSectorNum;
        swCurSectorAdr=addrMap(curSectorAdr);
        rowAdr = swCurSectorAdr /SECTOR_PER_PAGE;

        colAdr = curSectorAdr%SECTOR_PER_PAGE;
        if((resSectorNum>0)||(nfcTrsState.transferLastFlag==LAST_TRANS_FAIL)) {
            ONFI_ReadPageCacheRandom(rowAdr, colAdr);
        }
        else {   
            ONFI_ReadCacheLast();
        }

        colAdr = sectorAdr%SECTOR_PER_PAGE;
        if(colAdr != 0) {
            ONFI_ChangeReadColumn((colAdr*kDATA_PACK)>>1);
        }

        ONFI_dma_read(data_pack, 0,1);      
        
    }   
    
#else 
    #if NFC_CACHE_EN
        rowAdr = curSectorAdr /SECTOR_PER_PAGE;
        colAdr = 0; 
        if(nfcTrsState.transferFirstFlag==FIRST_TRANS_TRUE) 
            ONFI_ReadPage(rowAdr, colAdr);
        
        while(resSectorNum>0) {   
            curSectorNum = (resSectorNum>SECTOR_PER_PAGE)?SECTOR_PER_PAGE:resSectorNum;
            data_pack = curSectorNum *  kDATA_PACK;
            rowAdr = curSectorAdr /SECTOR_PER_PAGE;
            colAdr = 0;//curSectorAdr%rowAdr;
            
            if((resSectorNum>0)||(nfcTrsState.transferLastFlag==LAST_TRANS_FAIL))   
                ONFI_ReadCacheSequential();
            else            
                ONFI_ReadCacheLast();
            ONFI_dma_read(data_pack, 0,1);
            curSectorAdr +=curSectorNum;
            resSectorNum -=curSectorNum;        
         }
    #else
        while(resSectorNum>0) {
            curSectorNum = (resSectorNum>SECTOR_PER_PAGE)?SECTOR_PER_PAGE:resSectorNum;
            data_pack = curSectorNum *  kDATA_PACK;
            rowAdr = curSectorAdr /SECTOR_PER_PAGE;
            colAdr = 0;

            ONFI_ReadPage(rowAdr, colAdr);
            ONFI_dma_read(data_pack, 0,1);
            curSectorAdr +=curSectorNum;
            resSectorNum -=curSectorNum;
        #if (NFC_FTL_READ_EN==OPEN)
            #if(NFLASHWITH==16) 
                ONFI_ChangeReadColumn(data_pack>>1);
            #elif(NFLASHWITH ==8)
                ONFI_ChangeReadColumn(data_pack);
            #endif
        
            ONFI_dma_read(kAUX_PACK,ONFI_AUX_DATA,0);
        #endif 

         }  
    #endif
#endif
}

void NFC_Loop()
{
    return ;
    
    INT32U sectorNum = 4;
    unsigned const int data_bytes = kDATA_BYTES*sectorNum;
    unsigned const int data_pack  = kDATA_PACK*sectorNum;       
    switchCryptoMode(OPDMA);
    nfcReg->intr_src_clr |=nfcReg->intr_src_clr;
    nfcReg->intr_enb = NFC_INTR_SRC_DMAR_DONE;  
    
    nfcReg->dmaw_endcnt = data_bytes/8 -1;
    nfcReg->h2f_endcnt = data_bytes/8-1;
    nfcReg->dmar_endcnt= (data_bytes / 8) - 1; 
    //enable Paths
    nfcReg->enable |=NFC_ENABLE_DMAW | NFC_ENABLE_BCHE | NFC_ENABLE_H2F|0x0000a500;
    
}

void GPIO_ERROR_FLG(char witdh)
{
    GPIO_outData_REG = 0x4000;//(0 << 14);
    while(witdh>0) witdh--;
    GPIO_outData_REG = 0;//(0 << 14);
}

BOOL  NFC_FTL_Verify(INT16U size)
{
#if  NFC_AUX_VERIFY_EN
    INT32U *pStdBuf = (INT32U *)stdBuf;
    INT16U i=0;
    INT32U tmp=0;
    INT16U stdBufSizeMax = STD_DATA_SIZE_MAX/4;
    
    for (i=0; i<(size/4); i++) {
        #if NFC_STD_DATA_USE
            //if(*(pStdBuf+i%stdBufSizeMax)!=NFC_FTL[NFC_FTL_DAT_ADR+i])    
            //if (NFC_FTL[NFC_FTL_DAT_ADR+i] != 0x12345678) {
                //jprintf("Ftl  verify Fail. wr[%d]=0x%8.8x rd[%d]=0x%8.8x\n",i,*(pStdBuf+i%stdBufSizeMax),i,NFC_FTL[NFC_FTL_DAT_ADR+i]);
                jprintf("Ftl  verify result. rd[%d]=0x%8.8x\n",i,NFC_FTL[NFC_FTL_DAT_ADR+i]);
            //    GPIO_ERROR_FLG(0);
            //    return FALSE;
            //}
            //jprintf("Ftl  verify success. rd[%d]=0x%8.8x\n",i,NFC_FTL[NFC_FTL_DAT_ADR+i]);
        #else
            if (tmp != NFC_FTL[NFC_FTL_DAT_ADR+i]) {
                jprintf("Ftl  verify Fail. wr[%d]=0x%8.8x rd[%d]=0x%8.8x\n",i,tmp,i,NFC_FTL[NFC_FTL_DAT_ADR+i]);
                GPIO_ERROR_FLG(0);
                return FALSE;
            }   
            if (tmp == 0xffffffff)
                tmp = 0x00000000;
            else
                tmp += 0x01010101;
        #endif  
    }
    return TRUE;
#else
    return FALSE;
#endif  
}


    //ftl_len bit[23:12]
    //ftl_eol:bit[31]
    //ftl_sar:bit[11:0]
    
void setup_NFC_LISTFTL(INT32U size)
{

    INT32U tp_rdat, tp_addr;
    INT32U i;
    INT32U *pStdBuf = (INT32U *)stdBuf;
    INT16U stdBufSizeMax = STD_DATA_SIZE_MAX/4;
    NFC_FTL_access_on();

    tp_rdat = 0x200;
    for (i = 0; i < (size/4); i++) {
        tp_addr = tp_rdat + i;
        NFC_FTL[tp_addr] = *(pStdBuf+i%stdBufSizeMax);
    }
    
    nfcReg->lsar = 0x00000100;
        tp_rdat = nfcReg->lsar;
    NFC_FTL[tp_rdat + 0] = 0x80000200|((size/4-1)<<12);//0x8007f200;

}


void NFC_ProgramFtl_T(INT32U secNum,INT32U rowAdr,INT16U columnAdr,INT32U data)
{
    INT16U overTime;
    INT32U data_pack = kDATA_PACK * secNum; //534*2 * 1
    INT32U data_size = kDATA_BYTES* secNum; //512*2 * 1
    //jprintf("xxxxxxxx \n");
    setup_NFC_FTL(data_size,data);
    //setup_NFC_FTL(2048,data);
    nfcReg->enable  |= NFC_ENABLE_BCHE | NFC_ENABLE_H2F | NFC_ENABLE_FTLRD;

    //ONFI_EraseBlock(rowAdr);
    ONFI_ProgramPage(rowAdr, columnAdr);
    
    ONFI_dma_write(data_pack, 0);
    //ONFI_dma_write(2136, 0);//datapack should be same datasize
    
    //ONFI_ProgramPage_end();
    
    ONFI_ProgramPage_end();
    //for (overTime=10; overTime>0; overTime--);
    //ONFI_ChangeWriteColumn(columnAdr+1100);
    //ONFI_dma_write(data_pack, 0);    
    //Check for FTL_DONE
    
    overTime = NFC_OVERTIME;
    while(!(nfcReg->intr_src_clr&NFC_INTR_SRC_FTL_DONE)) {
        if((overTime --)==0) {
            jprintf(">Ftl->Flash: Check FTL_DONE Fail!\n");
            return ;
        }
    }       
    overTime = NFC_OVERTIME;
    while( !(nfcReg->intr_src_clr &NFC_INTR_SRC_BCHE_DONE)) {
        if((overTime --)==0) {
            jprintf(">Ftl->Flash: Check BCHE_DONE Fail!\n");
            return ;
        }
    }

    overTime = NFC_OVERTIME;    
    while(nfcReg->cmdff_dslot != cmdFifoSize) {
        if((overTime --)==0) {
            jprintf(">ReadPage: NFC_ProgramFtl_T Check CmdFifo Fail! 0x%8.8x\n",
                    nfcReg->cmdff_dslot ); 
            return ;
        }
    }   
    nfcReg->intr_src_clr |= nfcReg->intr_src_clr;
    nfcReg->enable &= ~(NFC_ENABLE_BCHE | NFC_ENABLE_H2F | NFC_ENABLE_FTLRD);
    //jprintf("ftl program down .\n");
}

INT32U NFC_ReadFtl_T(INT32U secNum,INT32U rowAdr,INT16U columnAdr)
{
    INT32U retVal ;
    INT32U data_pack  = kDATA_PACK * secNum; //534*2 * 1
    INT32U data_size  = kDATA_BYTES* secNum; //512*2 * 1
    INT16U stdBufSizeMax = STD_DATA_SIZE_MAX/4; 
    get_NFC_FTL(data_size);
    //get_NFC_FTL(2048);
    int i=0;
    for ( i = 0; i < (data_size/2); i ++)
        NFC_FTL[NFC_FTL_DAT_ADR + i] = 0xffffffff;
    
    nfcReg->enable |= NFC_ENABLE_F2H | NFC_ENABLE_BCHD|NFC_ENABLE_FTLWR;


    ONFI_ReadPage(rowAdr, columnAdr);   
    ONFI_dma_read(data_pack, 0,1);
    //ONFI_dma_read(2136, 0,1);


    while(!(nfcReg->intr_src_clr&NFC_INTR_SRC_FTL_DONE));
    nfcReg->intr_src_clr |= nfcReg->intr_src_clr;

    retVal |=(NFC_FTL_Verify(data_size))<<16; 

    nfcReg->enable&= ~(NFC_ENABLE_F2H | NFC_ENABLE_BCHD|NFC_ENABLE_FTLWR);

    //jprintf("ftl read down . \n");
    return retVal ;
}


void NFC_IRQ_ENABLE()
{
    nfcReg->intr_src_clr |= nfcReg->intr_src_clr;
    nfcReg->intr_enb = NFC_INTR_SRC_H2F_DONE | NFC_INTR_SRC_DMAR_DONE;  
}

void NFC_IRQ_DISABLE()
{
    nfcReg->intr_enb = 0;
    nfcReg->intr_src_clr |= nfcReg->intr_src_clr;
}

void NFC_handler(void)
{   
    NFC_IRQ_DISABLE();
    INT32U sectors=0;

    //jtag_printf("nfcState:%08x\n", nfcIrqStat);

    // nfc write
    if(nfcIrqStat & NFC_INTR_SRC_DMAW_DONE) {
        //jtag_printf("nfcwState:%08x,%02x\n", nfcIrqStat, nfcTrsState.transferLastFlag);
        
        if(nfcTrsState.transferLastFlag == LAST_TRANS_TRUE) {
            while(nfcReg->cmdff_dslot != cmdFifoSize);      
            nfcReg->enable &= ~(NFC_ENABLE_DMAW | NFC_ENABLE_AUX | NFC_ENABLE_BCHE | NFC_ENABLE_H2F);
            switchCryptoMode(OPCMD);    
        }
        else {           
            nfcReg->enable &= ~(NFC_ENABLE_DMAW | NFC_ENABLE_H2F| NFC_ENABLE_AUX);  
            NFC_IRQ_ENABLE();
            sectors = MIN(nfcTrsState.sectorTransLen, SECTER_SIZE_PER_IRQ);             
            nfcTrsState.sectorTransLen -= sectors;
            nfcTrsState.transferLastFlag = (nfcTrsState.sectorTransLen>0) ? LAST_TRANS_FAIL: LAST_TRANS_TRUE;
            nfcTrsState.transferFirstFlag = FIRST_TRANS_FAIL;
            Disk_Program(nfcTrsState.sectorAdr,sectors);                
            nfcTrsState.sectorAdr += sectors;
            
        }
    }

    // nfc read
    if(nfcIrqStat & NFC_INTR_SRC_DMAR_DONE) {
        //jtag_printf("nfcrState:%08x,%02x\n", nfcIrqStat, nfcTrsState.transferLastFlag);
        
        if(nfcTrsState.transferLastFlag == LAST_TRANS_TRUE) {
            #if (NFC_FTL_READ_EN == OPEN)
                nfcReg->enable &= ~NFC_ENABLE_DMAR;
                nfcReg->enable |= NFC_ENABLE_FTLWR;         
                //while((nfcReg->state1>>24) != 1);     //wait for ftl done 
                /while(!(nfcReg->intr_src_clr & NFC_INTR_SRC_FTL_DONE));
                nfcReg->enable &= ~NFC_ENABLE_FTLWR;
            #endif


        
            while(nfcReg->cmdff_dslot != cmdFifoSize);

            //led_ctrl(1,1,1);
            //while(pUsbCptReg->rsdm1.sts &0xC004);
            
            //while((*((volatile INT32U *)0x20010104)) &0x0000c004);    //while((((RSEMDM*)RSDM1_ADR)->sts) &0xC004);


         
            nfcReg->enable &=~(NFC_ENABLE_DMAR | NFC_ENABLE_F2H | NFC_ENABLE_BCHD); 
            switchCryptoMode(OPCMD);
        }
        else {

            #if (NFC_FTL_READ_EN==OPEN)
                    nfcReg->enable &=~NFC_ENABLE_DMAR;
                    nfcReg->enable |=NFC_ENABLE_FTLWR;          
                    while ((nfcReg->state1>>24)!=1);        //wait for ftl done 
                    nfcReg->enable &=~NFC_ENABLE_FTLWR;
            #elif(NFC_FTL_READ_EN==CLOSE)
                    nfcReg->enable &=~(NFC_ENABLE_DMAR | NFC_ENABLE_F2H );
            #endif  
            NFC_IRQ_ENABLE();
            sectors = MIN(nfcTrsState.sectorTransLen,SECTER_SIZE_PER_IRQ);      
            nfcTrsState.sectorTransLen  -=sectors;
            nfcTrsState.transferLastFlag = (nfcTrsState.sectorTransLen>0)?LAST_TRANS_FAIL:LAST_TRANS_TRUE;  
            nfcTrsState.transferFirstFlag = FIRST_TRANS_FAIL;
            Disk_Read(nfcTrsState.sectorAdr,sectors);               
            nfcTrsState.sectorAdr += sectors;                           
        }       
    }   

    
}

void Nfc_getFlashInfo_8()
{
    jprintf("ECC Length:%d\n",kDATA_T);
    jprintf("NFC system Clock Freq=%dMhz\n",NFC_SYSTEM_CLK );

    INT8U buff[8];  
    ONFI_ReadID(0x00);  ONFI_receive8(buff, 5);     
    if(buff[0]==0x2C)
        jprintf("Manufacture Id         :Micron\n");
    if(buff[1]&0x08)
        jprintf("Vcc                        :3.3V\n");
    if(buff[1]&0x80)
        jprintf("Density per CE#        :64G\n");   
    if(buff[2]&0x01)
        jprintf("LUNs per CE#           :2\n");
    else
        jprintf("LUNs per CE#          :1\n");
    if(buff[2]&0x40)
        jprintf("Cell type              :MLC\n");
    if(buff[3]&0x10)
        jprintf("Multi-LUN operations   :support\n");
    else
        jprintf("Multi-LUN operations            :not support\n");
    if(buff[4]&0x01)
        jprintf("Planes per LUN         :2\n");
    if(buff[4]&0x08)
        jprintf("Blocks per LUN         :4096\n");
    if(buff[4]&0x80)
        jprintf("Timing mode            :4(25ns)\n"); 
    setMem(buff,5,0);
    //Read Id Parameters For Address 20h: Byte 0~4  ,4 undefined    
    ONFI_ReadID(0x20);  ONFI_receive8(buff, 4);         
    jprintf("InterFace Info         :%s\n",buff);
    setMem(buff,5,0);
    jprintf("InterFace Info         :%s\n",buff);

}


void Nfc_getFlashInfo_16()
{
    jprintf("ECC Length:%d\n",kDATA_T);
    jprintf("NFC system Clock Freq=%dMhz\n",NFC_SYSTEM_CLK );

    INT16U buff[5]; 
    ONFI_ReadID(0x20);  ONFI_receive16(buff, 4);        
    jprintf("InterFace Info         :%s\n",buff);
    
    INT16U buff2[8];
    ONFI_ReadParameterPage(0);
    ONFI_receive16(buff2,4);
    jprintf("InterFace Info :%s %4.4x,%4.4x,%4.4x,%4.4x,\n",
            buff2,buff2[0],buff2[1],buff2[2],buff2[3]);
}


void Nfc_getFlashInfo()
{
#if (NFLASHWITH ==16)
    Nfc_getFlashInfo_16();
    NFC_ReadParameterPage();
#elif (NFLASHWITH ==8)
    Nfc_getFlashInfo_8();
    NFC_ReadParameterPage();
#endif      
}


void NFC_Eraser_Cpu (INT32U secAdr)
{
        INT32U rowAdr = secAdr /SECTOR_PER_PAGE;        
        ONFI_EraseBlock(rowAdr);
}

void NFC_ReadParameterPage(void)
{   

    INT16U i;
    char buf[256];
    ONFI_ReadParameterPage(0);  
    ONFI_receive8(buf, 256);  
    
    char *pDeviceModel = &buf[44];
    jprintf("[Nand Flash]: %s\n",pDeviceModel);
    
    INT32U *pBytesPerPage=(INT32U*)&buf[80];    
    INT16U *pSBytesPerPage=(INT16U*)&buf[84];
    jprintf("[Page Size]: %d + %d\n",ILTOBENDIAN(pBytesPerPage[0]),WLTOBENDIAN(pSBytesPerPage[0]));
    INT32U *pPagePerBlock=(INT32U*)&buf[92];
    jprintf("[Page Num]: %d/Block\n",ILTOBENDIAN(pPagePerBlock[0]));

    if(NAND_FLASH_PAGE_SIZE != ILTOBENDIAN(pBytesPerPage[0]))
        jprintf("Error Please Reset Nand Flash Size Per Page\n");

    jprintf("Supports 16-bit data bus width");  
    if(buf[6]&0x01)
        jprintf("   YES\n");
    else
        jprintf("   NO\n");

    jprintf("Supports multiple LUN operations");  
    if((buf[6]>>1)&0x01)
        jprintf("   YES\n");
    else
        jprintf("   NO\n");

    jprintf("Supports non-sequential page programming");  
    if((buf[6]>>2)&0x01)
        jprintf("   YES\n");
    else
        jprintf("   NO\n");

    jprintf("Supports interleaved (multi-plane) programand erase operations");  
    if((buf[6]>>3)&0x01)
        jprintf("   YES\n");
    else
        jprintf("   NO\n");

    jprintf("Supports odd-to-even page copyback");  
    if((buf[6]>>4)&0x01)
        jprintf("   YES\n");
    else
        jprintf("   NO\n");

    jprintf("Supports synchronous interface");  
    if((buf[6]>>5)&0x01)
        jprintf("   YES\n");
    else
        jprintf("   NO\n");

    jprintf("Supports interleaved (multi-plane) read operations");  
    if((buf[6]>>6)&0x01)
        jprintf("   YES\n");
    else
        jprintf("   NO\n");

    jprintf("Supports extended parameter page");  
    if((buf[6]>>7)&0x01)
        jprintf("   YES\n");
    else
        jprintf("   NO\n");

    jprintf("Supports program page register clear enhancement");  
    if((buf[7]>>0)&0x01)
        jprintf("   YES\n");
    else
        jprintf("   NO\n");

    if((NFC_FACE_MODE==SYNC16_MODE)||(NFC_FACE_MODE==SYNC8_MODE))
        if((buf[6]>>5)&0x01)
            jprintf("ERROR : This Nand Flash not support SYNC mode,Please reset nandFlashCfg.h or change nand Flash \n");
}


void flashInfoStringInit()
{
    memcpy(NandFlashInfoString, NandFlashInfoString_default, 32);
//flash Info update
//PAGE
    NandFlashInfoString[6] = NAND_FLASH_PAGE_SIZE/1024+0x30; 
//WIDTH 
    NandFlashInfoString[14]=NFLASHWITH/10+0x30;
    NandFlashInfoString[16]=NFLASHWITH%10+0x30;
//MODE

    NandFlashInfoString[20]=((NFC_FACE_MODE==ASYNC16_MODE)||(NFC_FACE_MODE==ASYNC8_MODE))?'A':'S';
    NandFlashInfoString[22]=NFC_SPEED_MODE+0x30;
//ECC
    NandFlashInfoString[28]=NFC_ECC_LEN/10+0x30;
    NandFlashInfoString[30]=NFC_ECC_LEN%10+0x30;
}


/*
void NFC_Program_Cpu(INT32U secNum,INT32U secAdr,INT32U *buf)
{

    unsigned const int data_bytes = kDATA_BYTES*secNum;
        unsigned const int data_pack  = kDATA_PACK*secNum;
    INT32U    rowAdr = secAdr /SECTOR_PER_PAGE; 
    INT16U  colAdr = secAdr%SECTOR_PER_PAGE;
    
    //INT32U    blockAdr = secAdr/SECTOR_PER_BLOCK;
    //INT32U    pageAdr = (secAdr%SECTOR_PER_BLOCK)/SECTOR_PER_PAGE;    
    
    nfcReg->intr_src_clr |= nfcReg->intr_src_clr;
        
    nfcReg->dmaw_endcnt = data_bytes/8 -1;
    nfcReg->h2f_endcnt = data_bytes/8-1;
    nfcReg->enable |=NFC_ENABLE_DMAW | NFC_ENABLE_BCHE | NFC_ENABLE_H2F;
    
        
    if((secAdr%SECTOR_PER_BLOCK)==0)
    {
            
        ONFI_EraseBlock(rowAdr);
    }
            
    ONFI_ProgramPage(secAdr, colAdr);
        ONFI_dma_write(data_pack, 0);
        ONFI_ProgramPage_end();
    usbcpt_write_nfc(data_bytes,buf);   

    while( !(nfcReg->intr_src_clr &NFC_INTR_SRC_H2F_DONE));
    nfcReg->intr_src_clr |= nfcReg->intr_src_clr;
    while(nfcReg->cmdff_dslot != cmdFifoSize);  
    nfcReg->enable &=~(NFC_ENABLE_DMAW | NFC_ENABLE_BCHE | NFC_ENABLE_H2F); 
}
*/

/*
INT32U  addrMap(INT32U secAddr)
{
    INT32U page,Xpage,block,Xblock,XsecAdr;
    block =secAddr/SECTOR_PER_BLOCK;
    page = (secAddr%SECTOR_PER_BLOCK)/SECTOR_PER_PAGE;

    if(block%2==0) {
        if(page%2==0) {           
            Xblock=block;
        }
        else {
            Xblock=block+1;
        }
        Xpage = page/2;
    }
    else {
        if(page%2==0) {           
            Xblock = block-1;
        }
        else {           
            Xblock=block;
        }
        Xpage=page/2+PAGE_PER_BLOCK/2;
    }
    XsecAdr = Xblock*SECTOR_PER_BLOCK+Xpage*SECTOR_PER_PAGE;
    //jprintf("----%d %d %d\n",Xblock,Xpage,XsecAdr);
    return XsecAdr;
}
*/

/*
INT32U addrMapMultiLun(INT32U secAdr)
{
    INT32U page,Xpage,block,Xblock,XsecAdr;
    
    block =secAdr/SECTOR_PER_BLOCK;
    page = (secAdr%SECTOR_PER_BLOCK)/SECTOR_PER_PAGE;

    if(block%2==0) {
        if(page%2==0) {           
            Xblock=block;
        }
        else {
            Xblock=block+BLOCK_NUM_PER_FLASH/2;
        }
        Xpage = page/2;
    }
    else {
        if(page%2==0) {           
            Xblock = block-1;
        }
        else {           
            Xblock=block-1+BLOCK_NUM_PER_FLASH/2;
        }
        Xpage=page/2+PAGE_PER_BLOCK/2;
    }
    XsecAdr = Xblock*SECTOR_PER_BLOCK+Xpage*SECTOR_PER_PAGE;
    return XsecAdr;
    //jprintf("MAP:%d %d %d %d %d \n",block,page,Xblock,Xpage,XsecAdr);
}
*/


/*==================2016 07 26=======================*/
/*
void NFC_Copy_Back_Ftl(INT32U srcAdr ,INT32U dstAdr,INT16U secNum)
{
    INT16U overTime;
    INT16U i;
    INT32U  data_pack  = kDATA_PACK ;
    INT32U data_size= kDATA_BYTES;  
    INT16U columnAdr ;
    for (i=0;i<secNum;i++) {
        columnAdr= (i*data_pack)>>1;
        //get_NFC_FTL(data_size);   
        get_NFC_FTL(data_size,0x200);
        nfcReg->enable |= NFC_ENABLE_F2H | NFC_ENABLE_BCHD|NFC_ENABLE_FTLWR;
         
        ONFI_ReadPage(srcAdr,columnAdr);
*/
/*#if   (NFLASHWITH==16)            
        ONFI_ChangeReadColumn((i*data_pack)>>1);
#elif (NFLASHWITH ==8)
        ONFI_ChangeReadColumn(i*data_pack);
#endif      */
/*==================2016 07 26=======================*/
/*
        ONFI_dma_read(data_pack, 0,1);
        while(!(nfcReg->intr_src_clr&NFC_INTR_SRC_FTL_DONE));
        nfcReg->intr_src_clr |= nfcReg->intr_src_clr;
        nfcReg->enable&= ~(NFC_ENABLE_F2H | NFC_ENABLE_BCHD|NFC_ENABLE_FTLWR);  


    //write to flash
        get_NFC_FTL(data_size,0x200);
        nfcReg->enable  |= NFC_ENABLE_BCHE | NFC_ENABLE_H2F | NFC_ENABLE_FTLRD;
        ONFI_ProgramPage(dstAdr, columnAdr);
*/

/*#if   (NFLASHWITH==16)            
        ONFI_ChangeWriteColumn((i*data_pack)>>1);
#elif (NFLASHWITH ==8)
        ONFI_ChangeWriteColumn(i*data_pack);
#endif  */  
/*==================2016 07 26=======================*/

/*
        ONFI_dma_write(data_pack, 0);
        ONFI_ProgramPage_end();
        overTime = NFC_OVERTIME;
        while (!(nfcReg->intr_src_clr&NFC_INTR_SRC_FTL_DONE)) {
            if((overTime --)==0)
            {   jprintf(">Ftl->Flash: Check FTL_DONE Fail!\n"); return ; }
        }   
        overTime = NFC_OVERTIME;
        while( !(nfcReg->intr_src_clr &NFC_INTR_SRC_BCHE_DONE))
        {
            if((overTime --)==0)
            {   jprintf(">Ftl->Flash: Check BCHE_DONE Fail!\n"); return ; }
        }
        overTime = NFC_OVERTIME;    
        while(nfcReg->cmdff_dslot != cmdFifoSize)
        {
            if((overTime --)==0)
            {   jprintf(">ReadPage: Check CmdFifo Fail! 0x%2.2x\n",nfcReg->cmdff_dslot ); return ;}
        }   
        nfcReg->intr_src_clr |= nfcReg->intr_src_clr;
        nfcReg->enable &= ~(NFC_ENABLE_BCHE | NFC_ENABLE_H2F | NFC_ENABLE_FTLRD);
    }


    jprintf("NFC_Copy_Back_Ftl Done\n");
}
*/

/*
//ftl data from Flash
void get_NFC_FTL(INT16U size)
{
    
    INT32U tp_rdat,i;   
    NFC_FTL_access_on();
    nfcReg->lsar = NFC_FTL_CMD_ADR;
        tp_rdat = nfcReg->lsar;

    NFC_FTL[tp_rdat] = 0x80000200|((size/4-1)<<12);
    */
    /*
    size=(size+7)/8*8;          // 64-bit aligned

    INT16U times = (size+kDATA_BYTES-1)/kDATA_BYTES;
    INT16U revSize = size;
    INT16U curSendSize=0;
    INT16U havSendSize=0;
    for(i=0;i<times;i++)    
    {
        curSendSize = (revSize>kDATA_BYTES)?kDATA_BYTES:revSize;
        
        if(i!=(times-1))
        {
            NFC_FTL[tp_rdat + i] = 0x00000200|((curSendSize/4-1)<<12)+havSendSize/4;//0x8007f200;       
        }else
        {   
            NFC_FTL[tp_rdat + i] = 0x80000200|((curSendSize/4-1)<<12)+havSendSize/4;//0x8007f200;   
        }   
        havSendSize +=curSendSize;
    }   
    */
//}


/*
void setup_NFC_FTL(INT16U size)
{
        INT32U tp_rdat, tp_addr;
        INT32U i,tmp=0;
        INT32U *pStdBuf = (INT32U *)stdBuf;
        INT16U stdBufSizeMax = STD_DATA_SIZE_MAX/4;
        NFC_FTL_access_on();
    
        size=(size+7)/8*8;          // 64-bit aligned
        
        for (i = 0; i < (size/4); i++)
        {
#if NFC_STD_DATA_USE        
            NFC_FTL[NFC_FTL_DAT_ADR+i] = *(pStdBuf+i%stdBufSizeMax);
#else
            NFC_FTL[NFC_FTL_DAT_ADR+i]=tmp;
            if(tmp == 0xffffffff)
                tmp = 0x00000000;
            else
                tmp += 0x01010101;
#endif
        }
        
        nfcReg->lsar = NFC_FTL_CMD_ADR;
            tp_rdat = nfcReg->lsar;
    
        NFC_FTL[tp_rdat ] = 0x80000200|((size/4-1)<<12);
*/
    /*
        INT16U times = (size+kDATA_BYTES-1)/kDATA_BYTES;
        INT16U revSize = size;
        INT16U curSendSize=0;
        INT16U havSendSize=0;
        for(i=0;i<times;i++)    
        {
            curSendSize = (revSize>kDATA_BYTES)?kDATA_BYTES:revSize;
            
            if(i!=(times-1))
            {
                NFC_FTL[tp_rdat + i] = 0x00000200|((curSendSize/4-1)<<12)+havSendSize/4;//0x8007f200;       
            }else
            {   
                NFC_FTL[tp_rdat + i] = 0x80000200|((curSendSize/4-1)<<12)+havSendSize/4;//0x8007f200;   
            }   
            havSendSize +=curSendSize;
        }   
        */

//}










