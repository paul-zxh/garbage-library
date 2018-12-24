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
#include "onfi_run.h"


//TODO:这里需要根据实际情况做调整，flash文件系统读写最小单位需为512字节为最佳

//一次中断最多读写一页的大小数据
void nandFlashWrite(INT32U sectorAddr,INT32U sectorNum)
{
    INT32U sectors;
    nfcTrsState.sectorAdr = sectorAddr;
    nfcTrsState.sectorTransLen =  sectorNum;

    switchCryptoMode(OPDMA);
    NFC_IRQ_ENABLE();

    sectors = MIN(nfcTrsState.sectorTransLen,SECTER_SIZE_PER_IRQ);

    nfcTrsState.sectorTransLen -= sectors;
    nfcTrsState.transferLastFlag = (nfcTrsState.sectorTransLen>0 )?LAST_TRANS_FAIL:LAST_TRANS_TRUE; 
    nfcTrsState.Ping_Pong = 0x00;
    nfcTrsState.transferFirstFlag = FIRST_TRANS_TRUE;
    Disk_Program(nfcTrsState.sectorAdr,sectors);    
    nfcTrsState.sectorAdr +=sectors ;
}

void nandFlashRead(INT32U sectorAddr,INT32U sectorNum)
{

    INT32U sectors;
    nfcTrsState.sectorAdr = sectorAddr;
    nfcTrsState.sectorTransLen =  sectorNum;

    switchCryptoMode(OPDMA);    
    NFC_IRQ_ENABLE();

    sectors = SECTER_SIZE_PER_IRQ - (sectorAddr % SECTER_SIZE_PER_IRQ);
    if(sectors == 0) {
        sectors = MIN(nfcTrsState.sectorTransLen,SECTER_SIZE_PER_IRQ);
    }
    else {
        sectors = MIN(sectorNum, sectors);
    }
    
    nfcTrsState.sectorTransLen -= sectors;
    nfcTrsState.transferLastFlag = (nfcTrsState.sectorTransLen>0 )?LAST_TRANS_FAIL:LAST_TRANS_TRUE;
    nfcTrsState.Ping_Pong=0x00;
    nfcTrsState.transferFirstFlag=FIRST_TRANS_TRUE;
    Disk_Read(nfcTrsState.sectorAdr,sectors);
    nfcTrsState.sectorAdr +=sectors ;
}


