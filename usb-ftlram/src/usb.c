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
#include "usb.h"
#include "nflash.h"
//#include "ahb_usbnfc.h"
#include "onfi_run.h"




volatile usb_otg_reg *pUsbOtgReg;
volatile usb_dev_reg *pUsbDevReg;
volatile usb_cpt_reg *pUsbCptReg;
volatile INT32U usb_irq_stat;
volatile INT32U usb_irq_flag;

INT8U is_usb_enb;
usb_cep_pkt cep;
usb_bk_pkt bk;
INT8U usb_cmd_buf[2048+128];
volatile INT32U testSel;
SCSI_READ10 *pRead10;
SCSI_WRITE10 *pWrite10;
volatile INT32U epInMask;
volatile INT32U epOutMask;


const INT8U usb_data_desc_device[18] =
{
    0x12, 
    0x01, 
    0x00, 0x02,
    0x00, 0x00, 0x00,
    0x40,
    0xff, 0xff,
    0xee, 0xee,
    0x00, 0x01,
    0x01, 0x02, 0x03,
    0x01
};

const INT8U usb_data_desc_config[32] =
{
    0x09, 0x02, 0x20, 0x00, 0x01, 0x01, 0x00, 0xc0, 0x32,
    0x09, 0x04, 0x00, 0x00, 0x02, 0x08, 0x06, 0x50, 0x00,
    0x07, 0x05, 0x03, 0x02, 0x00, 0x02, 0x00, 
    0x07, 0x05, 0x84, 0x02, 0x00, 0x02, 0x00
};

const INT8U usb_data_desc_string0[4] = 
{
    0x04, 0x03, 0x09, 0x04
};

const INT8U usb_data_desc_string1[32] = 
{
    0x20, 0x03, 0x41, 0x00, 0x42, 0x00, 0x43, 0x00, 
    0x44, 0x00, 0x45, 0x00, 0x46, 0x00, 0x47, 0x00, 
    0x48, 0x00, 0x49, 0x00, 0x50, 0x00, 0x51, 0x00, 
    0x52, 0x00, 0x53, 0x00, 0x54, 0x00, 0x55, 0x00
};

const INT8U usb_data_desc_string2[32] = 
{
    0x20, 0x03, 0x41, 0x00, 0x42, 0x00, 0x43, 0x00, 
    0x44, 0x00, 0x45, 0x00, 0x46, 0x00, 0x47, 0x00, 
    0x48, 0x00, 0x49, 0x00, 0x50, 0x00, 0x51, 0x00, 
    0x52, 0x00, 0x53, 0x00, 0x54, 0x00, 0x55, 0x00
};

const INT8U usb_data_desc_string3[32] = 
{
    0x20, 0x03, 0x41, 0x00, 0x42, 0x00, 0x43, 0x00, 
    0x44, 0x00, 0x45, 0x00, 0x46, 0x00, 0x47, 0x00, 
    0x48, 0x00, 0x49, 0x00, 0x50, 0x00, 0x51, 0x00, 
    0x52, 0x00, 0x53, 0x00, 0x54, 0x00, 0x55, 0x00
};

const INT8U usb_data_desc_qualifier[10] =
{
    0x0a, 0x06, 0x00, 0x2, 0x00, 0x00, 0x00, 0x40, 0x01, 0x00
};

void usb_otg_init(void)
{

    INT32U times = 0x10000;
    GPIO_GCC_REG = GPIO_GCC_REG & 0xffffefff;
    while (times--);
    GPIO_GCC_REG = GPIO_GCC_REG | 0x00001000;

    while(!(GPIO_inData_REG & 0x10000));
    GPIO_pullUp_REG = 0x0000dfff;
    pUsbOtgReg->irqEnb = 0;
    pUsbOtgReg->irqStat = USB_OTG_IRQ_STS_SUSPENDEN + USB_OTG_IRQ_STS_BBUSREQ;
}

void usb_init()
{
    pUsbOtgReg = (usb_otg_reg*)(USB_OTG_ADDR_BASE);
    pUsbDevReg = (usb_dev_reg*)(USB_DEV_ADDR_BASE);
    pUsbCptReg = (usb_cpt_reg*)(USB_CPT_ADDR_BASE);

    memset8((BYTE*)(&cep), 0, sizeof(usb_cep_pkt));
    cep.desc_buf = NULL;
    cep.state = USB_CEP_STATE_DEFAULT;
    cep.state_change = USB_CEP_STATE_DEFAULT;

    memset8((BYTE*)(&bk), 0, sizeof(usb_bk_pkt));
    bk.rx_buf = usb_cmd_buf;
    bk.tx_buf = usb_cmd_buf;

    is_usb_enb = 0;

    usb_irq_stat = 0;
    usb_irq_flag = 0;
    epOutMask = 1;
    epInMask = 1;

    usb_otg_init();
    usb_dev_init();

}

void usb_dev_dsr(void)
{
    VINT32U irq_stat;
    VINT32U irq_enb;

    if(usb_irq_stat & USB_DEV_IRQM_IRQ_USB) {
        irq_stat = pUsbDevReg->ctrl.irqStat;
        irq_enb = pUsbDevReg->ctrl.irqEnb;
        pUsbDevReg->ctrl.irqStat = irq_stat;

        if((irq_stat & USB_DEV_CTRL_IRQ_CLCUSEINT) && (is_usb_enb == 0)) {
            usb_dev_init();
            is_usb_enb = 1;
        }
        else if(irq_stat & USB_DEV_CTRL_IRQ_RSTSTS) {
            pUsbDevReg->ctrl.addr = 0;
            usb_bk_reset_out();
            usb_bk_reset_in();
        }
        else {
            //error
        }
    }
    else if (usb_irq_stat & USB_DEV_IRQM_IRQ_CEP) {
        irq_stat = pUsbDevReg->cep.irqStat;
        irq_enb = pUsbDevReg->cep.irqEnb;
        pUsbDevReg->cep.irqStat = irq_stat;

        if (irq_stat & USB_DEV_CEP_IRQ_USB_ERROR) {
            pUsbDevReg->cep.ctrlStat = USB_DEV_CEP_CTRL_STAT_STALL;
        }
        else if (irq_stat & USB_DEV_CEP_IRQ_SETUP_PACKET ) {
            usb_cep_setup();
        }
        else if((irq_stat & USB_DEV_CEP_IRQ_IN_TOKEN) && (!(irq_stat & USB_DEV_CEP_IRQ_STS_COMP))){
            if (cep.reqError == 1) {
                pUsbDevReg->cep.ctrlStat = USB_DEV_CEP_CTRL_STAT_STALL;
            }

            usb_cep_sent();
        }
        else if(irq_stat & USB_DEV_CEP_IRQ_STS_COMP) {
            usb_cep_update();
        }
        else {
            //error
        }
    }
    else if (usb_irq_stat & USB_DEV_IRQM_IRQ_EP3) {
        irq_stat = pUsbDevReg->ep3.irqStat;
        pUsbDevReg->ep3.irqStat = irq_stat;
        if(irq_stat & USB_DEV_EP_IRQ_DATA_PACKET_RECV) {
            GPIO_outData_REG = 0x4000;
            while(1);
        }
    }
    else if ((usb_irq_stat & USB_DEV_IRQM_IRQ_EP4) && (epInMask == 1)) {
        irq_stat = pUsbDevReg->ep4.irqStat;
        pUsbDevReg->ep4.irqStat = irq_stat;

        if((irq_stat & USB_DEV_EP_IRQ_USB_STALL_SENT) || (irq_stat & USB_DEV_EP_IRQ_DATA_IN_TOKEN)) {
            usb_bk_in();
        }
    }

    else {

    }
}

void usb_dev_init(void)
{
    INT32U delay_times;
    pUsbDevReg->irqm.irqEnbL = USB_DEV_IRQM_IRQ_USB | 
                               USB_DEV_IRQM_IRQ_CEP | 
                               USB_DEV_IRQM_IRQ_EP3 | 
                               USB_DEV_IRQM_IRQ_EP4;
    
    pUsbDevReg->irqm.irqEnbH = 0;
    pUsbDevReg->ctrl.irqEnb = USB_DEV_CTRL_IRQ_DEFAULT;
    pUsbDevReg->ctrl.oper = USB_DEV_CTRL_OPER_USBHISPD | USB_DEV_CTRL_OPER_USBCURSPD;
    pUsbDevReg->ctrl.addr = 0;

    pUsbDevReg->cep.irqEnb = USB_DEV_CEP_IRQ_DEFAULT;
    pUsbDevReg->cep.startAddr = 0;
    pUsbDevReg->cep.endAddr = USB_DEV_CEP_MAX_PACKET_SIZE/2-1;
    pUsbDevReg->cep.irqStat |= pUsbDevReg->cep.irqStat;

    pUsbDevReg->ep3.irqEnb = 0;//USB_DEV_EP_IRQ_OUT_DEFAULT;
    pUsbDevReg->ep3.rspSc = 0;
    pUsbDevReg->ep3.mps = USB_DEV_EP_MAX_PACKET_SIZE;
    pUsbDevReg->ep3.cfg = 0x33;
    pUsbDevReg->ep3.startAddr = 0x100;
    pUsbDevReg->ep3.endAddr = 0x1ff;

    pUsbDevReg->ep4.irqEnb = USB_DEV_EP_IRQ_IN_DEFAULT;
    pUsbDevReg->ep4.rspSc = 2;
    pUsbDevReg->ep4.mps = USB_DEV_EP_MAX_PACKET_SIZE;
    pUsbDevReg->ep4.cfg = 0x4b;
    pUsbDevReg->ep4.startAddr = 0x100;
    pUsbDevReg->ep4.endAddr = 0x1ff;

    // crypto init
    pUsbCptReg->irqCtl = USB_CPT_IRQ_EP3;
    
    delay_times = 10;
    pUsbCptReg->rsem1.ctrl = USB_CPT_EPX_CTRL_SOFT_RESET;
    while(delay_times--);

    delay_times = 10;
    pUsbCptReg->rsdm1.ctrl = USB_CPT_EPX_CTRL_SOFT_RESET;
    while(delay_times--);

    pUsbCptReg->rsem1.ctrl = USB_CPT_EPX_CTRL_DATASRC_DEFAULT;
    pUsbCptReg->rsdm1.ctrl = USB_CPT_EPX_CTRL_DATADIR_DEFAULT;

    pUsbCptReg->irqCtl |= USB_CPT_IRQ_NXT_CMD;

    is_usb_enb = 0;
    usb_irq_stat = 0;

    usb_bk_reset_out();
    usb_bk_reset_in();
}

// read form host
INT32U usb_dev_ep3_rx(INT8U *buf, INT32U len)
{
    INT32U i =0;
    INT32U dsize = 0;
    INT32U tsize = 0;
    INT32U k =0;
    INT32U tmp = 0;
    INT32U *pBuf = (INT32U*)buf;
    
    tsize = USB_CPT_GET_DATA_PACKET_COUNT(pUsbCptReg->rsem1.sts);
    pUsbCptReg->rsem1.ctrl = USB_CPT_EPX_CTRL_DATASRC_DEFAULT;
    
    len = tsize;
    while(tsize > 0) {
        dsize = (tsize >= 16) ? 16 : tsize;
        while(!(pUsbCptReg->rsem1.sts & USB_CPT_EPX_STAT_FIFO_NO_EMPTY));

        pUsbCptReg->outPortSel = USB_CPT_PORT_RSEM1;

        for(i=0; i<4; i++) {
            tmp = pUsbCptReg->dtOut[i];
            pBuf[k++] = ILTOBENDIAN(tmp);
        }

        tsize = tsize - dsize;
    }

    return len;
}

// write to host
void usb_dev_ep4_tx(INT8U *buf, INT32U len)
{
    INT32U i =0;
    INT32U tsize = 0;
    INT32U dsize = 0;
    INT32U k =0;
    INT32U tmp = 0;
    INT32U *pBuf = (INT32U*)buf;
    
    pUsbCptReg->rsdm1.ctrl = USB_CPT_EPX_CTRL_DATADIR_DEFAULT;

    tsize = len;
    while(tsize > 0) {
        dsize = (tsize>=16) ? 16 : tsize;
        while(!(pUsbCptReg->rsdm1.sts & USB_CPT_EPX_STAT_FIFO_NO_FULL));

        for(i=0; i<4; i++) {
            tmp = pBuf[k++];
            pUsbCptReg->rsdm1.dtIn[i] = ILTOBENDIAN(tmp);
        }

        pUsbCptReg->rsdm1.size = dsize;
        tsize = tsize - dsize;
    }

    pUsbDevReg->ep4.cnt = len;
    while(!(pUsbCptReg->irqStat & (1<<USB_DEV_EP4)));

    pUsbCptReg->irqStat = (1<< USB_DEV_EP4);
    pUsbDevReg->ep4.irqStat |= pUsbDevReg->ep4.irqStat; //clear usb bulkin ep irq state register

    pUsbCptReg->rsem1.ctrl = USB_CPT_EPX_CTRL_DATASRC_DEFAULT;
}

void switchCryptoMode(INT32U mode)
{
    switch(mode) {
        case OPCMD:
            pUsbCptReg->rsem1.ctrl = (USB_CPT_EPX_CTRL_DMA_SMS4_ENABLE | 
                                      USB_CPT_EPX_CTRL_SMS4_MODE_ENCODE_DATA |
                                      USB_CPT_EPX_CTRL_CRYPTO_MODE_DATA_TX_NOT_ENCODE |
                                      USB_CPT_EPX_CTRL_SRCSEL);
            pUsbCptReg->rsdm1.ctrl = (USB_CPT_EPX_CTRL_DMA_SMS4_ENABLE | 
                                      USB_CPT_EPX_CTRL_SMS4_MODE_ENCODE_DATA |
                                      USB_CPT_EPX_CTRL_CRYPTO_MODE_DATA_TX_NOT_ENCODE |
                                      USB_CPT_EPX_CTRL_DIRSEL);
            
            pUsbDevReg->ep4.irqStat |= pUsbDevReg->ep4.irqStat;
            pUsbDevReg->irqm.irqEnbL |= USB_DEV_IRQM_IRQ_EP4;
            pUsbCptReg->irqCtl |= USB_CPT_IRQ_EP3;
            
            epInMask = 1;
            epOutMask = 1;
            break;

        case OPDMA:
            pUsbCptReg->rsem1.ctrl = (USB_CPT_EPX_CTRL_DMA_SMS4_ENABLE | 
                                      USB_CPT_EPX_CTRL_SMS4_MODE_ENCODE_DATA |
                                      USB_CPT_EPX_CTRL_CRYPTO_MODE_DATA_TX_NOT_ENCODE |
                                      USB_CPT_EPX_CTRL_SRCSEL |
                                      USB_CPT_EPX_CTRL_DIRSEL |
                                      USB_CPT_EPX_CTRL_DMA_MODE );
            
            pUsbCptReg->rsdm1.ctrl = (USB_CPT_EPX_CTRL_DMA_SMS4_ENABLE | 
                                      USB_CPT_EPX_CTRL_SMS4_MODE_DECODE_DATA |
                                      USB_CPT_EPX_CTRL_CRYPTO_MODE_DATA_TX_NOT_ENCODE |
                                      USB_CPT_EPX_CTRL_SRCSEL |
                                      USB_CPT_EPX_CTRL_DIRSEL |
                                      USB_CPT_EPX_CTRL_DMA_MODE );
            
            pUsbDevReg->irqm.irqEnbL &= ~USB_DEV_IRQM_IRQ_EP4;
            pUsbCptReg->irqCtl &= ~USB_CPT_IRQ_EP3;
            
            epInMask = 0;
            epOutMask = 0;
            break;

        default:
            break;
    }
}



void usb_cep_setup(void)
{
    INT32U i;
    INT16U tmpVal = 0;
    INT32U *preg = (INT32U *)(&pUsbDevReg->cep.setup1_0);

    for (i = 0; i < 8;) {
        tmpVal = (INT16U)(preg[i / 2] & 0x0000ffff);
        cep.ctrl_buf[i++] = (INT8U)(tmpVal & 0x00ff);
        cep.ctrl_buf[i++] = (INT8U)((tmpVal & 0xff00) >> 8);
    }

    cep.desc_buf = cep.ctrl_buf;
    cep.desc_size = 0;

    switch (cep.ctrl_buf[1]) {
        case USB_CEP_DEVREQ_GET_DESCRIPTOR:
            usb_cep_get_descriptor();
            break;
        case USB_CEP_DEVREQ_SET_ADDRESS:
            usb_cep_set_address();
            break;
        case USB_CEP_DEVREQ_GET_STATUS:
            usb_cep_get_status();
            break;
        case USB_CEP_DEVREQ_CLEAR_FEATURE:
            usb_cep_clear_feature();
            break;
        case USB_CEP_DEVREQ_SET_FEATURE:
            usb_cep_set_feature();
            break;
        case USB_CEP_DEVREQ_SET_DESCRIPTOR:
            usb_cep_set_descriptor();
            break;
        case USB_CEP_DEVREQ_GET_CONFIGURATION:
            usb_cep_get_config();
            break;
        case USB_CEP_DEVREQ_SET_CONFIGURATION:
            usb_cep_set_config();
            break;
        case USB_CEP_DEVREQ_GET_INTERFACE:
            usb_cep_get_interface();
            break;
        case USB_CEP_DEVREQ_SET_INTERFACE:
            usb_cep_set_interface();
            break;
        case USB_CEP_DEVREQ_SYNCH_FRAME:
            usb_cep_synch_frame();
            break;
        case USB_CEP_DEVREQ_GET_MAX_LUN:
            usb_cep_get_max_lun();
            break;
        case USB_CEP_DEVREQ_MASS_STORAGE_RESET:
            usb_cep_mass_storage_reset();
            break;
        default:
            break;
    }

    pUsbDevReg->cep.ctrlStat = 0;
}

void usb_cep_update(void)
{
    usb_cep_devreq *req = (usb_cep_devreq *)cep.ctrl_buf;

    switch (req->request) {
        case USB_CEP_DEVREQ_SET_ADDRESS:
            //pUsbDevReg->cep.irqEnb = USB_DEV_CEP_IRQ_DEFAULT;
            pUsbDevReg->ctrl.addr = cep.ctrl_buf[2];
            cep.state = cep.state_change;
            break;
        case USB_CEP_DEVREQ_SET_CONFIGURATION:
            break;
        case USB_CEP_DEVREQ_SET_INTERFACE:
            break;

        case USB_CEP_DEVREQ_SET_FEATURE:        
            pUsbDevReg->ctrl.test=testSel;
                // jprintf("updata feature\n");
            break;
        case USB_CEP_DEVREQ_MASS_STORAGE_RESET:
            break;
        default:
            break;
    }
}


void usb_cep_sent(void)
{
    INT32U i;
    INT16U tmpVal = 0;
    INT8U *pbuf = (INT8U *)(cep.desc_buf);
    INT32U size = cep.desc_size;

    if (size == 0) {
        pUsbDevReg->cep.ctrlStat = USB_DEV_CEP_CTRL_STAT_ZEROLEN;
        return;
    }


    if (size > USB_DEV_CEP_MAX_PACKET_SIZE) {
        size = USB_DEV_CEP_MAX_PACKET_SIZE;
    }

    pUsbDevReg->cep.ctrlStat = USB_DEV_CEP_CTRL_STAT_FLUSH;
    while (pUsbDevReg->cep.ctrlStat & USB_DEV_CEP_CTRL_STAT_FLUSH);

    for (i = 0; i < ((size + 1) >> 1); i++) {
        tmpVal = (0x00ff & pbuf[0]) | ((0x00ff & pbuf[1]) << 8);
        pbuf += 2;
        pUsbDevReg->cep.dataBuf = tmpVal;
    }

    pUsbDevReg->cep.inTransfrCnt = size;

    if (cep.sent_state != 1) {
        while ((pUsbDevReg->cep.irqStat & (USB_DEV_CEP_IRQ_DATA_PACKET_TRANS | USB_DEV_CEP_IRQ_STS_COMP)) != (USB_DEV_CEP_IRQ_DATA_PACKET_TRANS | USB_DEV_CEP_IRQ_STS_COMP));
    }
    else {
        while ((pUsbDevReg->cep.irqStat & USB_DEV_CEP_IRQ_DATA_PACKET_TRANS) != USB_DEV_CEP_IRQ_DATA_PACKET_TRANS);
    }

    if (cep.desc_size <= USB_DEV_CEP_MAX_PACKET_SIZE) {
        cep.desc_size = 0;
        cep.desc_buf = NULL;
    }
    else {
        cep.desc_size -= USB_DEV_CEP_MAX_PACKET_SIZE;
        cep.desc_buf += USB_DEV_CEP_MAX_PACKET_SIZE;
        cep.sent_state = (cep.desc_size > USB_DEV_CEP_MAX_PACKET_SIZE) ? 1 : 0;
    }
}

void usb_cep_get_status(void)
{
    INT16U value = ((0x00ff & cep.ctrl_buf[3]) << 8) | cep.ctrl_buf[2];
    INT16U length = ((0x00ff & cep.ctrl_buf[7]) << 8) | cep.ctrl_buf[6];
    INT8U bRecipient = cep.ctrl_buf[0] & USB_CEP_DEVREQ_RECIPIENT_MASK;

    if ((value != 0) || (length != 2)) {
        cep.req_error = 1;
        return;
    }

    cep.ctrl_buf[0] = 0;
    cep.ctrl_buf[1] = 0;

    if (bRecipient == USB_CEP_DEVREQ_RECIPIENT_DEVICE) {
        if (cep.remote_wakeup == 1) {
            cep.ctrl_buf[0] = 3;
        }
        else {
            cep.ctrl_buf[0] = 1;
        }
    }
    else if (bRecipient == USB_CEP_DEVREQ_RECIPIENT_ENDPOINT) {
        // 如果指定端点当前被halted了，则返回1
        INT8U endp = cep.ctrl_buf[4];
        //TODO
        //........
        //get endpoint stat
        cep.ctrl_buf[0] = 1;

    }
    else if (bRecipient == USB_CEP_DEVREQ_RECIPIENT_INTERFACE) {
        //TODO
    }
    else {
        //TODO
    }

    cep.desc_size = 2;
}

void usb_cep_clear_feature(void)
{
    INT8U endp;
    INT16U value = ((0x00ff & cep.ctrl_buf[3]) << 8) | cep.ctrl_buf[2];
    INT8U bRecipient = cep.ctrl_buf[0] & USB_CEP_DEVREQ_RECIPIENT_MASK;

    if ((bRecipient == USB_CEP_DEVREQ_RECIPIENT_DEVICE) && (value == 1)) {//DEVICE_REMOTE_WAKEUP
        cep.remote_wakeup = 0;
    }
    else if ((bRecipient == USB_CEP_DEVREQ_RECIPIENT_ENDPOINT) && (value == 0)) {
        endp = cep.ctrl_buf[4];
        //......
        //set endpoint stat
    }
    else if (bRecipient == USB_CEP_DEVREQ_RECIPIENT_INTERFACE) {
        //TODO
    }
    else {
        //cep.reqError = 1;
    }

    cep.desc_size = 0;
}

void usb_cep_set_feature(void)
{

    INT16U index = ((0x00ff & cep.ctrl_buf[5]) << 8) | cep.ctrl_buf[4];
    testSel = (index>>8)&0x0f;
    cep.desc_size = 0;
    pUsbDevReg->cep.ctrlStat = 0;
/*
    INT8U endp;
    INT16U value = ((0x00ff & cep.ctrl_buf[3]) << 8) | cep.ctrl_buf[2];
    INT8U bRecipient = cep.ctrl_buf[0] & USB_CEP_DEVREQ_RECIPIENT_MASK;

    if ((bRecipient == USB_CEP_DEVREQ_RECIPIENT_DEVICE) && (value == 1)) {
        cep.remote_wakeup = 1;
    }
    else if ((bRecipient == USB_CEP_DEVREQ_RECIPIENT_ENDPOINT) && (value == 0)) {
        endp = cep.ctrl_buf[4];
        //......
        //set endpoint stat
    }
    else if (bRecipient == USB_CEP_DEVREQ_RECIPIENT_INTERFACE) {
        //TODO
    }
    else {
        //cep.reqError = 1;
    }

    cep.desc_size = 0;
    */
}

void usb_cep_set_address(void)
{
    //pUsbDevReg->cep.irqEnb = USB_DEV_CEP_IRQ_STS_COMP;

    if ((cep.ctrl_buf[3] != 0) ||
        (cep.ctrl_buf[2] > 0x7f) ||
        (cep.state == USB_CEP_STATE_CONFIGURED)) {
        cep.reqError = 1;
        return;
    }

    if (cep.state == USB_CEP_STATE_ADDRESSED) {
        if ((cep.ctrl_buf[2] == 0) && (cep.ctrl_buf[3] == 0)) {
            cep.state_change = USB_CEP_STATE_DEFAULT;
        }
    }
    else if (cep.state == USB_CEP_STATE_DEFAULT) {
        cep.state_change = (cep.ctrl_buf[2] == 0) ? USB_CEP_STATE_DEFAULT : 
                                                    USB_CEP_STATE_ADDRESSED;
    }

    cep.desc_size = 0;
    pUsbDevReg->cep.ctrlStat = 0;
}

void usb_cep_get_descriptor(void)
{
    cep.sent_state = 0;
    switch (cep.ctrl_buf[3]) {
        case USB_CEP_DEVREQ_DESC_TYPE_DEVICE:
            cep.desc_buf = (INT8U *)usb_data_desc_device;
            cep.desc_size = (cep.ctrl_buf[6] > 18) ? 18 : cep.ctrl_buf[6];
            break;

        case USB_CEP_DEVREQ_DESC_TYPE_CONFIGURATION:
            cep.desc_buf = (INT8U *)usb_data_desc_config;
            cep.desc_size = (cep.ctrl_buf[6] > 32) ? 32 : cep.ctrl_buf[6];
            break;

        case USB_CEP_DEVREQ_DESC_TYPE_STRING:
            switch(cep.ctrl_buf[2]) {
                case 0:
                    cep.desc_buf = (INT8U *)usb_data_desc_string0;
                    cep.desc_size = MIN(4, cep.ctrl_buf[6]);
                    break;

                case 1:
                    cep.desc_buf = (INT8U *)usb_data_desc_string1;
                    cep.desc_size = MIN(32, cep.ctrl_buf[6]);
                    break;

                case 2:
                    cep.desc_buf = (INT8U *)usb_data_desc_string2;
                    cep.desc_size = MIN(32, cep.ctrl_buf[6]);
                    break;

                case 3:
                    cep.desc_buf = (INT8U *)usb_data_desc_string3;
                    cep.desc_size = MIN(32, cep.ctrl_buf[6]);
                    break;

                default:
                    break;
            }
            
            break;

        case 0x06:
            cep.desc_buf = (INT8U *)(&usb_data_desc_qualifier);
            cep.desc_size = (cep.ctrl_buf[6] > 10) ? 10 : cep.ctrl_buf[6];
            break;

        case 0x0a:
            cep.desc_size = 0;
            break;

        default:
            break;
    }
}

void usb_cep_set_descriptor(void)
{
    //none
}

void usb_cep_get_config(void)
{
    if (cep.state == USB_CEP_STATE_DEFAULT) {
        cep.req_error = 1;
        return;
    }

    if (cep.state == USB_CEP_STATE_ADDRESSED) {
        cep.ctrl_buf[0] = 0;
        cep.desc_size = 1;
    }
    else if (cep.state == USB_CEP_STATE_CONFIGURED) {
        cep.ctrl_buf[0] = cep.configuration;
        cep.desc_size = 1;
    }
}

void usb_cep_set_config(void)
{
    if (cep.state == USB_CEP_STATE_DEFAULT) {
        cep.req_error = 1;
        return;
    }

    if (cep.ctrl_buf[2] == 0) {
        cep.state_change = USB_CEP_STATE_ADDRESSED;
        cep.configuration = 0;
    }
    else {
        cep.state_change = USB_CEP_STATE_CONFIGURED;
        cep.configuration = 1;
    }

    cep.desc_size = 0;
    pUsbDevReg->cep.irqStat = 0;
}

void usb_cep_get_interface(void)
{
    if ((cep.state == USB_CEP_STATE_DEFAULT) || (cep.state == USB_CEP_STATE_ADDRESSED)) {
        cep.req_error = 1;
        return;
    }

    if (cep.state == USB_CEP_STATE_CONFIGURED) {
        cep.ctrl_buf[0] = 0xff;
        cep.desc_size = 1;
    }

    // set_idle
    if(cep.ctrl_buf[0] == 0x21) {
        cep.ctrl_buf[0] = 0;
        cep.desc_size = 0;
    }
}

void usb_cep_set_interface(void)
{
    if (cep.state != USB_CEP_STATE_CONFIGURED) {
        cep.req_error = 1;
    }
    else {
        //TODO
    }

    cep.desc_size = 0;
    pUsbDevReg->cep.irqStat = 0;
}

void usb_cep_synch_frame(void)
{
    //;
}

void usb_cep_get_max_lun(void)
{
    cep.ctrl_buf[0] = 0;
    cep.desc_size = 1;
}

void usb_cep_mass_storage_reset(void)
{
    cep.desc_size = 0;
    pUsbDevReg->cep.irqStat = 0;
}

void usb_bk_reset_out()
{
    bk.rx_len = 0;
    bk.need_rx_len = 0;
    memset8(bk.rx_buf, 0, 2048);
}

void usb_bk_reset_in()
{
    bk.tx_len = 0;
    bk.need_tx_len = 0;
    memset8(bk.tx_buf, 0, 2048);
}

void usb_bk_error(INT8U stat, INT32U size)
{
    bk.csw.status = stat;
    bk.csw.residue_len = ILTOBENDIAN(bk.cbw.len - size);
}

void usb_bk_out_data()
{
    if(bk.need_rx_len > 512) {
        usb_dev_ep3_rx(bk.rx_buf + bk.rx_len, 512);
        bk.rx_len += 512;
        bk.need_rx_len -= 512;
        bk.status = BULK_OUT_DATA;
    }
    else {
        usb_dev_ep3_rx(bk.rx_buf + bk.rx_len, bk.need_rx_len);
        bk.rx_len += bk.need_rx_len;
        bk.need_rx_len = 0;
        bk.status = BULK_OUT_END;
    }
}

void usb_bk_in_data()
{
    if(bk.need_tx_len > 512) {
        usb_dev_ep4_tx(bk.tx_buf + bk.tx_len, 512);
        bk.tx_len += 512;
        bk.need_tx_len -= 512;
        bk.status = BULK_IN_DATA;
    }
    else {
        usb_dev_ep4_tx(bk.tx_buf + bk.tx_len, bk.need_tx_len);
        bk.tx_len += bk.need_tx_len;
        bk.need_tx_len = 0;
        bk.status = BULK_IN_END;
    }
}

void usb_bk_cbw()
{
    usb_dev_ep3_rx((INT8U*)(&bk.cbw), sizeof(usb_bk_cbw_pkt));
    bk.csw.sign = USB_BULK_CSW_SIGNATURE;
    bk.csw.tag  = bk.cbw.tag;
    bk.cbw.len  = ILTOBENDIAN(bk.cbw.len);

    if((bk.cbw.sign != USB_BULK_CBW_SIGNATURE) || (bk.cbw.cmd_len > USB_BULK_MAX_CMD_LEN)) {
        return;
    }

    usb_scsi();
}

void usb_bk_csw()
{
    usb_dev_ep4_tx((INT8U*)(&bk.csw), sizeof(usb_bk_csw_pkt));
    bk.status = BULK_IDLE;
}

void usb_bk_out()
{
    switch(bk.status) {
        case BULK_IDLE:
            usb_bk_cbw();
            break;

        case BULK_OUT_DATA:
            usb_bk_out_data();
            break;

        default:
            break;
    }
    
    while(pUsbCptReg->irqCtl & USB_CPT_IRQ_NXT_CMD);    // wait this bit 0
    pUsbCptReg->irqCtl |= USB_CPT_IRQ_NXT_CMD;          // use for receive next bulkout by cpu interface
}

void usb_bk_in()
{
    switch(bk.status) {
        case BULK_ACK:
            bk.need_tx_len = MIN(bk.need_tx_len, bk.cbw.len);
            usb_bk_error(BULK_CSW_GOOD, bk.need_tx_len);
            usb_bk_in_data();
            break;

        case BULK_IN_DATA:
            usb_bk_in_data();
            break;

        case BULK_STALL:
            bk.status= BULK_IN_END;
            pUsbDevReg->ep4.rspSc = USB_DEV_EP_RSP_SC_EPHALT;
            break;

        case BULK_GOOD_END:
            usb_bk_error(BULK_CSW_GOOD, bk.cbw.len);
            usb_bk_csw();
            break;

        case BULK_FAIL_END:
            usb_bk_error(BULK_CSW_FAIL, 0);
            usb_bk_csw();
            break;

        case BULK_IN_END:
            usb_bk_csw();
            break;

        case BULK_OUT_END:
            usb_bk_csw();
            break;

        case BULK_STOP:
            usb_bk_error(BULK_CSW_GOOD, 0);
            usb_bk_csw();
            break;

        default:
            break;
    }
}

const INT8U usb_data_udisk_inquiry[36]=
{
    0x00, //磁道驱动器
    0x80, //可移动介质
    0x00, //SCSI版本
    0x01, //数据格式
    0x1F, //附加数据长度
    0x00, 0x00, 0x00,
    'X','X','X','X','X','X','X',' ',    
    'U','s','b',' ', 'M','a','s','s', ' ','S','t','o','r','a','g','e',
    '1','.','0','0'
};

const INT8U usb_data_udisk_mode_sense6[4]=
{
    0x03, 0x00, 0x00, 0x00
};

const INT8U usb_data_udisk_mode_sense10[4]=
{
    0x03, 0x00, 0x00, 0x00
};

const INT8U usb_data_udisk_request_sense[20]=
{
    0xf0, //响应码
    0x00, //段号
    0x06, //LUN复位
    0x00, 0x00, 0x00, 0x00, //信息
    0x0A, //附加数据长度
    0x00, 0x00, 0x00, 0x00, //信息
    0x28, 0x00, //检测码
    0x00, 
    0x00,
    0x00 ,0x00, 
    0x00, 0x00
};

//TODO:这里需要根据实际情况做调整,即块数乘上每块大小就是U盘容量,块大小一般是512字节，这里方便测试改为1024字节
//注意:实际运用中这里应该为512字节
const INT8U usb_data_udisk_read_capacity[8]=
{
    0x00, 0x01, 0x00, 0x00, // 块数
    0x00, 0x00, 0x04, 0x00  // 每块大小为1024字节
};

//TODO:同上
const INT8U usb_data_udisk_read_formate_capacity[12]=
{
    0x00, 0x00, 0x00, 0x08, //列表长度
    0x00, 0x01, 0x00, 0x00, //块数
    0x04, 0x00, 0x04, 0x00  //每块大小为1024字节
};

void usb_scsi()
{
    bk.need_rx_len = 0;
    bk.rx_len = 0;
    bk.need_tx_len = 0;
    bk.tx_len = 0;
    bk.status = BULK_ACK;

    switch(bk.cbw.cmd[0]) {
        case SPC_CMD_INQUIRY:
            bk.tx_buf = (INT8U*)usb_data_udisk_inquiry;
            bk.need_tx_len = 36;
            break;

        case SPC_CMD_MODESENSE6:
            bk.tx_buf = (INT8U*)usb_data_udisk_mode_sense6;
            bk.need_tx_len = 4;
            break;

        case SPC_CMD_MODESENSE10:
            bk.tx_buf = (INT8U*)usb_data_udisk_mode_sense10;
            bk.need_tx_len = 4;
            break;

        case SPC_CMD_REQUESTSENSE:
            bk.tx_buf = (INT8U*)usb_data_udisk_request_sense;
            bk.need_tx_len = 18;
            break;

        case RBC_CMD_READ10:
            //TODO:这里需要根据实际情况做调整,这里nandFlashRead里参数sector以1KB为单位，
            //对应read_capacity里块大小也为1KB，
            //实际使用中，应该为512最佳
            pRead10 = (SCSI_READ10*)(bk.cbw.cmd);
            nandFlashRead(pRead10->logicalBlockAddress*0x1000 + 0x00300000, 
                          pRead10->transferLength);

            bk.status= BULK_GOOD_END;
            break;

        case RBC_CMD_WRITE10:
            //TODO:这里需要根据实际情况做调整，同上
            pWrite10 = (SCSI_WRITE10*)(bk.cbw.cmd);
            nandFlashWrite(pWrite10->logicalBlockAddress*0x1000 + 0x00300000, 
                           pWrite10->transferLength);

            bk.status= BULK_GOOD_END;
            break;

        case RBC_CMD_READCAPACITY:
            bk.tx_buf = (INT8U*)usb_data_udisk_read_capacity;
            bk.need_tx_len = 8;
            break;

        case SPC_CMD_FORMAT_CAPACITY:
            bk.tx_buf = (INT8U*)usb_data_udisk_read_formate_capacity;
            bk.need_tx_len = 12;
            break;

        case RBC_CMD_VERIFY10:
            bk.status = BULK_GOOD_END;
            break;

        case RBC_CMD_STARTSTOPUNIT:
            bk.status = BULK_GOOD_END;
            break;

        case SPC_CMD_MEDIUM_REMOVAL:
            bk.status = BULK_FAIL_END;
            break;

        case SPC_CMD_TESTUNITREADY:
            bk.status = BULK_GOOD_END;
            break;

        default:
            if(bk.cbw.len > 0) {
                memset8(usb_cmd_buf, 0, 2048);
                bk.tx_buf = usb_cmd_buf;
                bk.need_tx_len = MIN(bk.cbw.len, 2048);
            }
            else {
                bk.status = BULK_GOOD_END;
            }
            break;
    }
}



