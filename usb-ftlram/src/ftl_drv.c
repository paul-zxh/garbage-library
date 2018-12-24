/*
********************************************************************************
********************************************************************************

                    Copyright (c) 2017, JETWAY, WuHan
                            All Rights Reserved.

********************************************************************************
********************************************************************************
 */

#include "J32xx.h"
#include "ftl_nand.h"
#include "ftl_drv.h"
#include "usb.h"
#include "nflash.h"
//#include "ahb_usbnfc.h"
#include "pt/pt.h"
#include "onfi_run.h"


extern volatile NFC *nfcReg;
extern volatile usb_cpt_reg *pUsbCptReg;


struct cryptodma_t 
{
    INT32U *data;   
    INT32U length;   
};

struct device_extension_s
{
    struct cryptodma_t  rsem1;
    struct cryptodma_t  rsdm1;
};

struct device_extension_s dx;


static inline INT32U is_NFC_DMAW_done()
{
    return (nfcReg->intr_src_clr & NFC_INTR_SRC_DMAW_DONE);
}

static inline INT32U is_NFC_DMAR_done()
{
    return (nfcReg->intr_src_clr & NFC_INTR_SRC_DMAR_DONE);
}

static inline void crypto_wait_reset(void)
{
    volatile INT32U i = 0;

    for (i = 0; i < 10; i++) {
        asm("nop");
    }
}

static inline void crypto_rsem1_init()
{
     pUsbCptReg->rsem1.ctrl     = USB_CPT_EPX_CTRL_SOFT_RESET;
     crypto_wait_reset();
}

static inline void crypto_rsem1_request(struct cryptodma_t *dma, INT32U* data, INT32U length, INT32U mode)
{
    dma->data  = data;
    dma->length  = length;
    pUsbCptReg->rsem1.ctrl  = mode | USB_CPT_EPX_CTRL_DMA_SMS4_ENABLE;
}

static inline void crypto_rsem1_disable()
{
    pUsbCptReg->rsem1.ctrl &= (~USB_CPT_EPX_CTRL_DMA_SMS4_ENABLE);
}

static inline INT32U is_rsem1_infifonofull()
{
    return (pUsbCptReg->rsem1.sts & USB_CPT_EPX_STAT_FIFO_NO_FULL);
}

static inline void crypto_rsdm1_disable()
{
    pUsbCptReg->rsdm1.ctrl &= (~USB_CPT_EPX_CTRL_DMA_SMS4_ENABLE);
}

static inline void crypto_rsdm1_init()
{
    pUsbCptReg->rsdm1.ctrl = USB_CPT_EPX_CTRL_SOFT_RESET;
    crypto_wait_reset();
}

static inline void crypto_rsdm1_request(struct cryptodma_t *dma, INT32U* data, 
                                        INT32U length,INT32U mode)
{
    dma->data  = data;
    dma->length  = length;
    pUsbCptReg->rsdm1.ctrl= mode | USB_CPT_EPX_CTRL_DMA_SMS4_ENABLE;
}

static inline INT32U is_rsdm1_outfifonoempty()
{
    return (pUsbCptReg->rsdm1.sts & USB_CPT_EPX_STAT_FIFO_NO_EMPTY);
}

static INT32U rsem1_nfc(struct pt *pt)
{
    PT_BEGIN(pt);

    while (dx.rsem1.length) {
        PT_WAIT_UNTIL(pt, is_rsem1_infifonofull());
        
        pUsbCptReg->rsem1.dtIn[0]= *(dx.rsem1.data ++);
        pUsbCptReg->rsem1.dtIn[1]= *(dx.rsem1.data ++);
        pUsbCptReg->rsem1.dtIn[2]= *(dx.rsem1.data ++);
        pUsbCptReg->rsem1.dtIn[3]= *(dx.rsem1.data ++);
        pUsbCptReg->rsem1.size= (dx.rsem1.length >= 16) ? 16 : dx.rsem1.length;
        dx.rsem1.length = (dx.rsem1.length >= 16) ? (dx.rsem1.length - 16) : 0;
    }
    pUsbCptReg->rsem1.size = 0;
    PT_WAIT_WHILE(pt, 1);

    PT_END(pt);
}

static INT32U nfc_rsdm1(struct pt *pt)
{
    PT_BEGIN(pt);

    while (dx.rsdm1.length) {
        PT_WAIT_UNTIL(pt, is_rsdm1_outfifonoempty());
        pUsbCptReg->outPortSel = 0x5a000004;
        *(dx.rsdm1.data ++) = pUsbCptReg->dtOut[0];
        *(dx.rsdm1.data ++) = pUsbCptReg->dtOut[1];
        *(dx.rsdm1.data ++) = pUsbCptReg->dtOut[2];
        *(dx.rsdm1.data ++) = pUsbCptReg->dtOut[3];

        dx.rsdm1.length = (dx.rsdm1.length >= 16) ? (dx.rsdm1.length - 16) : 0;
    }
    
    PT_WAIT_WHILE(pt, 1);

    PT_END(pt);
}

void transmit( INT32U *data, INT32U length)
{
    static struct pt pt1;

    crypto_rsem1_init();
    crypto_rsem1_request(&dx.rsem1, data, length, 0x1008d);

    PT_INIT(&pt1);
    do {
        rsem1_nfc(&pt1);
    } while (dx.rsem1.length || (!is_NFC_DMAW_done()));

    crypto_rsem1_disable();
}

void receive( INT32U *data, INT32U length)
{
    static struct pt pt1;

    crypto_rsdm1_init();
    crypto_rsdm1_request(&dx.rsdm1, data, length, 0x10097);

    PT_INIT(&pt1);
    do {
        nfc_rsdm1(&pt1);
    } while ((dx.rsdm1.length) || (!is_NFC_DMAR_done()));

    crypto_rsdm1_disable();
}


void setup_page_buff(INT32U *buff, INT32U data)
{
    INT32U i;

    for (i = 0; i < (BYTES_PER_PAGE >> 2); i++) {
        buff[i] = data;
    }
}

void dump_page_buff(INT32U *buff, INT32U len)
{
    INT32U i;

    ASSERT(len <= BYTES_PER_PAGE);
    
    for (i = 0; i <(len >> 2); i++) {
        if ((i % 8) == 0) {
            jprintf("\n%04d - ", i);
        }
        jprintf("%x ", buff[i]);
    }
    jprintf("\n");
}

static void ReadPage_via_RSDM1(INT32U const rawaddr, INT32U const coladdr, INT32U const secnum,  INT32U *data)
{
    INT32U data_bytes = kDATA_BYTES*secnum;
    INT32U data_pack  = kDATA_PACK*secnum;

    ASSERT(data_bytes <=  BYTES_PER_PAGE);

    nfcReg->dmar_endcnt= (data_bytes / 8) - 1;
    nfcReg->enable |= NFC_ENABLE_DMAR | NFC_ENABLE_F2H | NFC_ENABLE_BCHD;
    
    ONFI_ReadPage(rawaddr, coladdr);
    ONFI_dma_read(data_pack, 0, 1);

    receive(data, data_bytes);

    nfcReg->enable &= ~(NFC_ENABLE_DMAR | NFC_ENABLE_F2H | NFC_ENABLE_BCHD);
}

static void WritePage_via_RSEM1(INT32U const rawaddr, INT32U const coladdr, INT32U const secnum,  INT32U *data)
{
    INT32U data_bytes = kDATA_BYTES*secnum;
    INT32U data_pack  = kDATA_PACK*secnum;

    ASSERT(data_bytes <= BYTES_PER_PAGE);

    nfcReg->dmaw_endcnt= (data_bytes / 8) - 1;
    nfcReg->h2f_endcnt = (data_bytes / 8 ) -1;
    nfcReg->enable |= NFC_ENABLE_DMAW | NFC_ENABLE_H2F | NFC_ENABLE_BCHE ;

    ONFI_ProgramPage(rawaddr, coladdr);
    ONFI_dma_write(data_pack, 0);
    transmit(data, data_bytes);
    ONFI_ProgramPage_end();

    nfcReg->enable &= ~(NFC_ENABLE_DMAW | NFC_ENABLE_H2F |NFC_ENABLE_BCHE);
}

void WritePage(INT32U const  lpn, INT32U const secoffset, INT32U const secnum, INT32U *data)
{
    WritePage_via_RSEM1(lpn, secoffset, secnum, data);
    //NFC_ProgramFtl_T(lpn, secoffset, secnum);
}

void ReadPage(INT32U const lpn, INT32U const secoffset, INT32U const secnum, INT32U *data)
{
    ReadPage_via_RSDM1(lpn, secoffset, secnum, data);
    //NFC_ReadFtl_T(lpn, secoffset, secnum);
}

void nand_write_from_host(INT32U const  lpn, INT32U const secoffset, INT32U const secnum)
{
    // Real write path USB->NFC->NANDFLASH
    // Put your real code here 
    
}

void nand_read_to_host(INT32U const lpn, INT32U const secoffset, INT32U const secnum)
{
    // Real read path NANDFLASH->NFC->USB
    // put your real code here 

}


void EraseBlock(INT32U const  rawaddr)
{
    ONFI_EraseBlock(rawaddr);
}


