/*
********************************************************************************
********************************************************************************

                    Copyright (c) 2017, JETWAY, WuHan
                            All Rights Reserved.

********************************************************************************
********************************************************************************
 */


#ifndef UPOS_H
#define UPOS_H

////////////////////////////////////////////////////////////////////////////
//usb otg
#define USB_OTG_ADDR_BASE       (0x11100700)

typedef struct usb_otg_reg
{
    INT32U rev;
    INT32U irqStat;
    INT32U irqEnb;
} usb_otg_reg;

//0x11100704
#define USB_OTG_IRQ_STS_ACLRERR     0x1
#define USB_OTG_IRQ_STS_SRPDETECT   0x2
#define USB_OTG_IRQ_STS_HANDOFFINTR 0x4
#define USB_OTG_IRQ_STS_SRPFAIL     0x8
#define USB_OTG_IRQ_STS_DEVICE      0x10
#define USB_OTG_IRQ_STS_HOST        0x20
#define USB_OTG_IRQ_STS_ABUSREQ     0x40
#define USB_OTG_IRQ_STS_ABUSDROP    0x80
#define USB_OTG_IRQ_STS_AHNPEN      0x100
#define USB_OTG_IRQ_STS_BHNPEN      0x200
#define USB_OTG_IRQ_STS_BBUSREQ     0x400
#define USB_OTG_IRQ_STS_RSVD        0x800
#define USB_OTG_IRQ_STS_ADEVICE     0x1000
#define USB_OTG_IRQ_STS_BDEVICE     0x2000
#define USB_OTG_IRQ_STS_SESSIONFAIL 0x4000
#define USB_OTG_IRQ_STS_SUSPENDEN   0x8000
#define USB_OTG_IRQ_STS_RSVDD       0xffff0000

//0x11100708
#define USB_OTG_IRQ_EN_ACLRERR          0x1
#define USB_OTG_IRQ_EN_SRPDET           0x2
#define USB_OTG_IRQ_EN_TRANSINTR        0x4
#define USB_OTG_IRQ_EN_SRPFAIL          0x8
#define USB_OTG_IRQ_EN_SESSIONFAIL      0x10
#define USB_OTG_IRQ_EN_ADEVCHANGE       0x20
#define USB_OTG_IRQ_EN_BDEVCHANGE       0x40
#define USB_OTG_IRQ_EN_DEVCHANGE        0x80
#define USB_OTG_IRQ_EN_HOSTCHANGE       0x100
#define USB_OTG_IRQ_EN_DISCONNECTDETECT 0x200
#define USB_OTG_IRQ_EN_RSVD             0xfffffc00



////////////////////////////////////////////////////////////////////////////
//usb device
#define USB_DEV_ADDR_BASE               (0x11100000)

typedef struct usb_dev_irqm_reg
{
    INT32U irqStatL; // 0x11100000
    INT32U irqStatH;
    INT32U irqEnbL;
    INT32U irqEnbH;
} usb_dev_irqm_reg;

#define USB_DEV_IRQM_IRQ_NULL       0x0000
#define USB_DEV_IRQM_IRQ_USB        0x0001
#define USB_DEV_IRQM_IRQ_CEP        0x0002
#define USB_DEV_IRQM_IRQ_EP1        0x0004
#define USB_DEV_IRQM_IRQ_EP2        0x0008
#define USB_DEV_IRQM_IRQ_EP3        0x0010
#define USB_DEV_IRQM_IRQ_EP4        0x0020
#define USB_DEV_IRQM_IRQ_EP5        0x0040
#define USB_DEV_IRQM_IRQ_EP6        0x0080
#define USB_DEV_IRQM_IRQ_EP7        0x0100
#define USB_DEV_IRQM_IRQ_EP8        0x0200
#define USB_DEV_IRQM_IRQ_EP9        0x0400
#define USB_DEV_IRQM_IRQ_EP10       0x0800
#define USB_DEV_IRQM_IRQ_EPALL      0x0ffc
#define USB_DEV_IRQM_IRQ_ALL        0x0fff
#define USB_DEV_IRQM_IRQ_EP_OFFSET  0x02

typedef struct usb_dev_ctrl_reg
{
    INT32U irqStat; //0x11100010
    INT32U irqEnb;
    INT32U oper;
    INT32U frameCnt;
    INT32U addr;
    INT32U test;
} usb_dev_ctrl_reg;

#define USB_DEV_CTRL_IRQ_SOF        0x1
#define USB_DEV_CTRL_IRQ_RSTSTS     0x2
#define USB_DEV_CTRL_IRQ_RESUME     0x4
#define USB_DEV_CTRL_IRQ_SUSPREQ    0x8
#define USB_DEV_CTRL_IRQ_HISPDSET   0x10
#define USB_DEV_CTRL_IRQ_CLCUSEINT  0x40
#define USB_DEV_CTRL_IRQ_DEFAULT    0x4e

#define USB_DEV_CTRL_OPER_GENRSM    0x1
#define USB_DEV_CTRL_OPER_USBHISPD  0x2
#define USB_DEV_CTRL_OPER_USBCURSPD 0x4

#define USB_DEV_CTRL_FRAME_CNT_MICFRAMECNT  0x7
#define USB_DEV_CTRL_FRAME_CNT_FRAMECNG     0x3FF8

#define USB_DEV_CTRL_ADDR_OURADDR   0x7f
#define USB_DEV_CTRL_TEST_TMODE     0x7

typedef struct usb_dev_cep_reg
{
    INT32U dataBuf; //0x11100028
    INT32U ctrlStat;
    INT32U irqEnb;
    INT32U irqStat;
    INT32U inTransfrCnt;
    INT32U outTransfrCnt;
    INT32U rev1;
    INT32U setup1_0;
    INT32U setup3_2;
    INT32U setup5_4;
    INT32U setup7_6;
    INT32U startAddr;
    INT32U endAddr;
    INT32U rev2;
    INT32U rev3;
} usb_dev_cep_reg;

#define USB_DEV_CEP_DATA_BUF_CTRLEPDATABUFA         0xffff

#define USB_DEV_CEP_CTRL_STAT_NAKCLEAR              0x1
#define USB_DEV_CEP_CTRL_STAT_STALL                 0x2
#define USB_DEV_CEP_CTRL_STAT_ZEROLEN               0x4
#define USB_DEV_CEP_CTRL_STAT_FLUSH                 0x8

#define USB_DEV_CEP_IRQ_SETUP_TOKEN                 0x1
#define USB_DEV_CEP_IRQ_SETUP_PACKET                0x2
#define USB_DEV_CEP_IRQ_OUT_TOKEN                   0x4
#define USB_DEV_CEP_IRQ_IN_TOKEN                    0x8
#define USB_DEV_CEP_IRQ_PING_TOKEN                  0x10
#define USB_DEV_CEP_IRQ_DATA_PACKET_TRANS           0x20
#define USB_DEV_CEP_IRQ_DATA_PACKET_RECV            0x40
#define USB_DEV_CEP_IRQ_NAK_SENT                    0x80
#define USB_DEV_CEP_IRQ_STALL_SENT                  0x100
#define USB_DEV_CEP_IRQ_USB_ERROR                   0x200
#define USB_DEV_CEP_IRQ_STS_COMP                    0x400
#define USB_DEV_CEP_IRQ_BUF_FULL                    0x800
#define USB_DEV_CEP_IRQ_BUF_EMP                     0x1000
#define USB_DEV_CEP_IRQ_DEFAULT                     0x060a

#define USB_DEV_CEP_IN_TRANSFR_CNT_INTRANSDATACNT   0xffff
#define USB_DEV_CEP_OUT_TRANSFR_CNT_OUTTRANSDATACNT 0xffff

#define USB_DEV_CEP_SETUP1_0_SETUPBYTE01            0xffff
#define USB_DEV_CEP_SETUP3_2_SETUPBYTE23            0xffff
#define USB_DEV_CEP_SETUP5_4_SETUPBYTE45            0xffff
#define USB_DEV_CEP_SETUP7_6_SETUPBYTE67            0xffff

#define USB_DEV_CEP_START_ADDR_CEPSTARTADDR         0xffff
#define USB_DEV_CEP_END_ADDR_CEPENDADDR             0xffff

//(x>=0, x<10)
typedef struct usb_dev_ep_reg
{
    INT32U dataBuf; //0x11100064 + 0x28*(x)
    INT32U irqStat;
    INT32U irqEnb;
    INT32U avail;
    INT32U rspSc;
    INT32U mps;
    INT32U cnt;
    INT32U cfg;
    INT32U startAddr;
    INT32U endAddr;
} usb_dev_ep_reg;

#define USB_DEV_EP_DATA_BUF_EPDATAREG           0xffff

#define USB_DEV_EP_IRQ_BUF_FULL                 0x1
#define USB_DEV_EP_IRQ_BUF_EMP                  0x2
#define USB_DEV_EP_IRQ_SHORT_PACKET_TRANS       0x4
#define USB_DEV_EP_IRQ_DATA_PACKET_TRANS        0x8
#define USB_DEV_EP_IRQ_DATA_PACKET_RECV         0x10
#define USB_DEV_EP_IRQ_DATA_OUT_TOKEN           0x20
#define USB_DEV_EP_IRQ_DATA_IN_TOKEN            0x40
#define USB_DEV_EP_IRQ_PING_TOKEN               0x80
#define USB_DEV_EP_IRQ_USB_NAK_SENT             0x100
#define USB_DEV_EP_IRQ_USB_STALL_SENT           0x200
#define USB_DEV_EP_IRQ_NYET_SENT                0x400
#define USB_DEV_EP_IRQ_ERR_SENT                 0x800

#define USB_DEV_EP_IRQ_OUT_DEFAULT              (USB_DEV_EP_IRQ_ERR_SENT)
#define USB_DEV_EP_IRQ_IN_DEFAULT               (USB_DEV_EP_IRQ_DATA_IN_TOKEN|USB_DEV_EP_IRQ_USB_STALL_SENT)

#define USB_DEV_EP_AVAIL_CNT                    0xfff

#define USB_DEV_EP_RSP_SC_BUFFLUSH              0x1
#define USB_DEV_EP_RSP_SC_MODE                  0x6
#define USB_DEV_EP_RSP_SC_EPTOGGLE              0x8
#define USB_DEV_EP_RSP_SC_EPHALT                0x10
#define USB_DEV_EP_RSP_SC_ZEROLENIN             0x20
#define USB_DEV_EP_RSP_SC_PACKETEND             0x40
#define USB_DEV_EP_RSP_SC_NAKSEND               0x80

#define USB_DEV_EP_MPS_MAXPACKETSIZE            0xfff

#define USB_DEV_EP_CNT_EPTRANSCNT               0xffff

#define USB_DEV_EP_CFG_EPVALID                  0x1
#define USB_DEV_EP_CFG_EPTYPE                   0x6
#define USB_DEV_EP_CFG_EPDIR                    0x8
#define USB_DEV_EP_CFG_EPNUM                    0xf0
#define USB_DEV_EP_CFG_MULTFIELD                0x300

#define USB_DEV_EP_START_ADDR_EPSTARTADDR       0xffff
#define USB_DEV_EP_END_ADDR_EPENDADDR           0xffff

#define USB_DEV_EP1      0
#define USB_DEV_EP2      1
#define USB_DEV_EP3      2
#define USB_DEV_EP4      3
#define USB_DEV_EP5      4
#define USB_DEV_EP6      5
#define USB_DEV_EP7      6
#define USB_DEV_EP8      7
#define USB_DEV_EP9      8
#define USB_DEV_EP10     9

#define USB_DEV_EP_MIN   0
#define USB_DEV_EP_MAX   9
#define USB_DEV_EP_COUNT 10

//#define USB_DEV_EP_MIN    0
//#define USB_DEV_EP_MAX    1
//#define USB_DEV_EP_COUNT  2


#define USB_DEV_EP_OUT  0
#define USB_DEV_EP_IN   1

#define USB_DEV_EP_MAX_PACKET_SIZE  512
#define USB_DEV_CEP_MAX_PACKET_SIZE 64

typedef struct usb_dev_reg
{
    usb_dev_irqm_reg irqm;
    usb_dev_ctrl_reg ctrl;
    usb_dev_cep_reg  cep;
    usb_dev_ep_reg   ep1;
    usb_dev_ep_reg   ep2;
    usb_dev_ep_reg   ep3;
    usb_dev_ep_reg   ep4;
    usb_dev_ep_reg   ep5;
    usb_dev_ep_reg   ep6;
    usb_dev_ep_reg   ep7;
    usb_dev_ep_reg   ep8;
    usb_dev_ep_reg   ep9;
    usb_dev_ep_reg   ep10;
} usb_dev_reg;



////////////////////////////////////////////////////////////////////////////
//usb crypto
#define USB_CPT_ADDR_BASE   0x20010000

#define USB_CPT_ADDR_EP1    (USB_CPT_ADDR_BASE + 0x00)
#define USB_CPT_ADDR_EP2    (USB_CPT_ADDR_BASE + 0x40)
#define USB_CPT_ADDR_EP10   (USB_CPT_ADDR_BASE + 0x80)
#define USB_CPT_ADDR_RSEM1  (USB_CPT_ADDR_BASE + 0xC0)
#define USB_CPT_ADDR_RSDM1  (USB_CPT_ADDR_BASE + 0x100)
#define USB_CPT_ADDR_RSDM2  (USB_CPT_ADDR_BASE + 0x140)
#define USB_CPT_ADDR_RSEM2  (USB_CPT_ADDR_BASE + 0x180)
#define USB_CPT_ADDR_RHM    (USB_CPT_ADDR_BASE + 0x1C0)

#define USB_CPT_CTRL_REG    (*(VINT32U*)(USB_CPT_ADDR_BASE + 0x200))
#define USB_CPT_PORT_REG    (*(VINT32U*)(USB_CPT_ADDR_BASE + 0x204))
#define USB_CPT_DATA0_REG   (*(VINT32U*)(USB_CPT_ADDR_BASE + 0x208))
#define USB_CPT_DATA1_REG   (*(VINT32U*)(USB_CPT_ADDR_BASE + 0x20c))
#define USB_CPT_DATA2_REG   (*(VINT32U*)(USB_CPT_ADDR_BASE + 0x210))
#define USB_CPT_DATA3_REG   (*(VINT32U*)(USB_CPT_ADDR_BASE + 0x214))
#define USB_CPT_STAT_REG    (*(VINT32U*)(USB_CPT_ADDR_BASE + 0x218))

typedef struct usb_cpt_epx_reg
{
    INT32U ctrl;
    INT32U sts;
    INT32U dtIn[4];
    INT32U size;
    INT32U hashDtOut[8];
    INT32U dtPktCnt;
} usb_cpt_epx_reg;

//crpto epx ctrl bit define
//      bit 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16
//ep1   bit 0,        4,           9,                   14
//ep2   bit 0,     3,                 10,              14
//ep10  bit 0,     3,                 10,              14
//rsem1 bit 0,1,2,3,4,5,   7,8,9,                  14,15,16
//rsdm1 bit 0,1,2,3,4,5,   7,8,   10,             14,15,16
//rsdm2 bit 0,1,2,3,4,      7,   9,10,             14,15,16
//rsem2 bit 0,1,2,3,4,      7,   9,10,11,12,    14,15,16
//rhm   bit 0,1,2,3,4,            9,         12,    14,16

// 命令/数据模式控制位
#define USB_CPT_EPX_CTRL_DMA_MODE                       (0x00010000)

// 子密钥扩展操作启动位
#define USB_CPT_EPX_CTRL_GEN_KEY_START                  (0x00008000)

// 软复位启动位
#define USB_CPT_EPX_CTRL_SOFT_RESET                     (0x00004000)

// Hash 长度值下载使能
#define USB_CPT_EPX_CTRL_HASH_LEN_LOAD_ENABLE           (0x00001000)

// 哈希操作使能位
#define USB_CPT_EPX_CTRL_HASH_ENABLE                    (0x00000800)

// 响应通道清除使能位
#define USB_CPT_EPX_CTRL_RSP_CHANNEL_FLUSH_ENABLE       (0x00000400)

// 命令通道清除使能位
#define USB_CPT_EPX_CTRL_CMD_CHANNEL_FLUSH_ENABLE       (0x00000200)

#define USB_CPT_EPX_CTRL_DMAR32_SPEED_TEST_ENABLE       (0x00000100)

// 加密模式选择
#define USB_CPT_EPX_CTRL_CRYPTO_MODE_DATA_TX_NOT_ENCODE (0x00000080)

#define USB_CPT_EPX_CTRL_DMAW64_SPEED_TEST_ENABLE       (0x00000020)

// 数据输入源选择
#define USB_CPT_EPX_CTRL_SRCSEL                         (0x00000010)

// 数据输出目的地选择
#define USB_CPT_EPX_CTRL_DIRSEL                         (0x00000008)

// SMS4 操作模式选择(cryptomod 开启时只能选择数据输入)
#define USB_CPT_EPX_CTRL_SMS4_MODE_DECODE_DATA          (0x00000006)
#define USB_CPT_EPX_CTRL_SMS4_MODE_ENCODE_DATA          (0x00000004)
#define USB_CPT_EPX_CTRL_SMS4_MODE_DECODE_KEY           (0x00000002)
#define USB_CPT_EPX_CTRL_SMS4_MODE_ENCODE_KEY           (0x00000000)

// DMA 模块及SMS4 模块使能
#define USB_CPT_EPX_CTRL_DMA_SMS4_ENABLE                (0x00000001)

#define USB_CPT_EPX_CTRL_DATASRC_DEFAULT                (0x00000095)
#define USB_CPT_EPX_CTRL_DATADIR_DEFAULT                (0x0000008D)

//crpto epx stat bit define
//ep1   bit 3,4,6-10,16-26
//ep2   bit 3,4,6-10
//ep10  bit 3,4,6-10
//rsem1 bit 2,3,4,6-10,16-26
//rsdm1 bit 2,3,4,6-10
//rsdm2 bit 2,3,4,6-10,16-26
//rsem2 bit 2,3,4,6-10,11,16-26
//rhm   bit 2,3,6-10,11,16-26

// 当前传输的数据包字节数（1~512B）
#define USB_CPT_EPX_STAT_PKT_DATA_COUNT_MASK (0x07ff0000) // (26:16)

//Hash 运算结果有效位：
//1：hash 运算结果有效
//0：hash 运算结果无效
#define USB_CPT_EPX_STAT_HASH_RESULT_VALUD   (1<<11)

// 当前burst 传输数据输出字节长度
#define USB_CPT_EPX_STAT_DATA_NUMBER_MASK    (0x000007c0) // (10:6)

// 模块fifo 非满信号
// 1：fifo  非满
// 0：fifo  满
#define USB_CPT_EPX_STAT_FIFO_NO_FULL        (0x00000010)

// 模块fifo 非空信号
// 1：fifo  非空
// 0：fifo 空
#define USB_CPT_EPX_STAT_FIFO_NO_EMPTY       (0x00000008)

// 模块忙标志，可被系统复位或软复位清零。
// 1：忙。
// 0：空闲。
#define USB_CPT_EPX_STAT_BUSY                (0x00000004)

#define USB_CPT_GET_DATA_PACKET_COUNT(x)     ((x>>16)&0x7ff)

typedef struct usb_cpt_reg
{
    usb_cpt_epx_reg ep1;
    usb_cpt_epx_reg ep2;
    usb_cpt_epx_reg ep10;
    usb_cpt_epx_reg rsem1;
    usb_cpt_epx_reg rsdm1;
    usb_cpt_epx_reg rsdm2;
    usb_cpt_epx_reg rsem2;
    usb_cpt_epx_reg rhm;
    INT32U irqCtl;
    INT32U outPortSel;
    INT32U dtOut[4];
    INT32U irqStat;
} usb_cpt_reg;

#define USB_CPT_IRQ_EP1         0x00000001  // DATA_AVL
#define USB_CPT_IRQ_EP2         0x00000002  // XMIT_DONE
#define USB_CPT_IRQ_EP3         0x00000004  // DATA_AVL
#define USB_CPT_IRQ_EP4         0x00000008  // XMIT_DONE
#define USB_CPT_IRQ_EP5         0x00000010  // DATA_AVL
#define USB_CPT_IRQ_EP6         0x00000020  // XMIT_DONE
#define USB_CPT_IRQ_EP7         0x00000040  // DATA_AVL
#define USB_CPT_IRQ_EP8         0x00000080  // XMIT_DONE
#define USB_CPT_IRQ_EP9         0x00000100  // DATA_AVL
#define USB_CPT_IRQ_EP10        0x00000200  // XMIT_DONE
#define USB_CPT_IRQ_RSEM2_DONE  0x00000400  // TRANS_DONE
#define USB_CPT_IRQ_RSDM2_DONE  0x00000800  // TRANS_DONE

#define USB_CPT_IRQ_ALL_OUT 0x155
#define USB_CPT_IRQ_ALL_IN  0x2aa
#define USB_CPT_IRQ_ALL     0xfff;

#define USB_CPT_IRQ_MIN     0
#define USB_CPT_IRQ_MAX     11
#define USB_CPT_IRQ_COUNT   12

//before set this bit, must make sure this bit is zero
#define USB_CPT_IRQ_NXT_CMD 0x00010000

#define USB_CPT_EP1     0
#define USB_CPT_EP2     1
#define USB_CPT_EP3     2
#define USB_CPT_EP4     3
#define USB_CPT_EP5     4
#define USB_CPT_EP6     5
#define USB_CPT_EP7     6
#define USB_CPT_EP8     7
#define USB_CPT_EP9     8
#define USB_CPT_EP10    9
#define USB_CPT_RSEM1   USB_CPT_EP3
#define USB_CPT_RSDM1   USB_CPT_EP4
#define USB_CPT_RSDM2   USB_CPT_EP5
#define USB_CPT_RSEM2   USB_CPT_EP7
#define USB_CPT_RHM     USB_CPT_EP9

#define USB_CPT_EP_MIN   0
#define USB_CPT_EP_MAX   9
#define USB_CPT_EP_COUNT 10

#define USB_CPT_PORT_EP1            0x5A000001
#define USB_CPT_PORT_EP2            0x5A000002
#define USB_CPT_PORT_RSEM1          0x5A000003
#define USB_CPT_PORT_RSDM1          0x5A000004
#define USB_CPT_PORT_RSDM2          0x5A000005
#define USB_CPT_PORT_RSEM2_HASH     0x5A000006
#define USB_CPT_PORT_RSEM2          0x5A000007
#define USB_CPT_PORT_RHM_CNT        0x5A000008
#define USB_CPT_PORT_RHM            0x5A000009
#define USB_CPT_PORT_EP10           0x5A00000A


typedef struct
{
    INT32U ctrl;
    INT32U sts;
    INT32U rcvlen;
    INT32U sndlen;
    INT32U error[2];
} USB_CPT_NFC_REG;

typedef struct
{
    INT32U ep3ctrl;
    INT32U ep3sts;
    INT32U ep3sndlen;
    INT32U ep4ctrl;
    INT32U ep4sts;
    INT32U ep4rcvlen;
    INT32U ep5ctrl;
    INT32U ep5sts;
    INT32U ep5sndlen;
    INT32U ep6ctrl;
    INT32U ep6sts;
    INT32U ep6rcvlen;
    INT32U ep7ctrl;
    INT32U ep7sts;
    INT32U ep7sndlen;
    INT32U ep8ctrl;
    INT32U ep8sts;
    INT32U ep8rcvlen;
    INT32U ep9ctrl;
    INT32U ep9sts;
    INT32U ep9sndlen;
} USB_CPT_USB_REG;


#define OPTRANS                     0x10                        
#define OPCRYPTO                    0x20
#define OPDT                        0x30
#define OPDMA                       0x40

#define OPCPU                       0x35
#define OPCMD                       0x36



////////////////////////////////////////////////////////////////////////////
// usb cep
typedef struct usb_cep_devreq
{
    INT8U       type;
    INT8U       request;
    INT8U       value_lo;
    INT8U       value_hi;
    INT8U       index_lo;
    INT8U       index_hi;
    INT8U       length_lo;
    INT8U       length_hi;
} usb_cep_devreq; // USB device requests, the setup packet.

// Encoding of the request_type
#define USB_CEP_DEVREQ_DIRECTION_OUT        0
#define USB_CEP_DEVREQ_DIRECTION_IN         (1 << 7)
#define USB_CEP_DEVREQ_DIRECTION_MASK       (1 << 7)

#define USB_CEP_DEVREQ_TYPE_STANDARD        0
#define USB_CEP_DEVREQ_TYPE_CLASS           (0x1 << 5)
#define USB_CEP_DEVREQ_TYPE_VENDOR          (0x2 << 5)
#define USB_CEP_DEVREQ_TYPE_RESERVED        (0x3 << 5)
#define USB_CEP_DEVREQ_TYPE_MASK            (0x3 << 5)

#define USB_CEP_DEVREQ_RECIPIENT_DEVICE     0x00
#define USB_CEP_DEVREQ_RECIPIENT_INTERFACE  0x01
#define USB_CEP_DEVREQ_RECIPIENT_ENDPOINT   0x02
#define USB_CEP_DEVREQ_RECIPIENT_OTHER      0x03
#define USB_CEP_DEVREQ_RECIPIENT_MASK       0x1F

// The standard request codes.
#define USB_CEP_DEVREQ_GET_STATUS            0
#define USB_CEP_DEVREQ_CLEAR_FEATURE         1
#define USB_CEP_DEVREQ_SET_FEATURE           3
#define USB_CEP_DEVREQ_SET_ADDRESS           5
#define USB_CEP_DEVREQ_GET_DESCRIPTOR        6
#define USB_CEP_DEVREQ_SET_DESCRIPTOR        7
#define USB_CEP_DEVREQ_GET_CONFIGURATION     8
#define USB_CEP_DEVREQ_SET_CONFIGURATION     9
#define USB_CEP_DEVREQ_GET_INTERFACE        10
#define USB_CEP_DEVREQ_SET_INTERFACE        11
#define USB_CEP_DEVREQ_SYNCH_FRAME          12

#define USB_CEP_DEVREQ_GET_MAX_LUN          0xfe
#define USB_CEP_DEVREQ_MASS_STORAGE_RESET   0xff


// Descriptor types. These are placed in value_hi for the
// GET_DESCRIPTOR and SET_DESCRIPTOR requests, with an index
// in value_lo. They also go into the type fields of the
// various descriptor structures.
#define USB_CEP_DEVREQ_DESC_TYPE_DEVICE               1
#define USB_CEP_DEVREQ_DESC_TYPE_CONFIGURATION        2
#define USB_CEP_DEVREQ_DESC_TYPE_STRING               3
#define USB_CEP_DEVREQ_DESC_TYPE_INTERFACE            4
#define USB_CEP_DEVREQ_DESC_TYPE_ENDPOINT             5

// Feature selectors. These go into value_lo for the CLEAR_FEATURE and
// SET_FEATURE requests, and in the first response byte for
// GET_STATUS.
#define USB_CEP_DEVREQ_FEATURE_DEVICE_REMOTE_WAKEUP         1
#define USB_CEP_DEVREQ_FEATURE_ENDPOINT_HALT                0

// Index decoding. When the CLEAR_FEATURE, SET_FEATURE and GET_STATUS
// requests is applied to an endpoint (as per the recipient field in
// the type field) index_lo identifies the endpoint.
#define USB_CEP_DEVREQ_INDEX_DIRECTION_OUT                  0
#define USB_CEP_DEVREQ_INDEX_DIRECTION_IN                   (1 << 7)
#define USB_CEP_DEVREQ_INDEX_DIRECTION_MASK                 (1 << 7)
#define USB_CEP_DEVREQ_INDEX_ENDPOINT_MASK                  0x0F


#define USB_CEP_STATE_DETACHED             0x01
#define USB_CEP_STATE_ATTACHED             0x02
#define USB_CEP_STATE_POWERED              0x03
#define USB_CEP_STATE_DEFAULT              0x04
#define USB_CEP_STATE_ADDRESSED            0x05
#define USB_CEP_STATE_CONFIGURED           0x06
#define USB_CEP_STATE_MASK                 0x7F
#define USB_CEP_STATE_SUSPENDED            (1 << 7)

// State changes. Application code or higher-level packages should
// install an appropriate state change function which will get
// invoked with details of the state change.
typedef enum
{
    USB_CEP_STATE_CHANGE_DETACHED          = 1,
    USB_CEP_STATE_CHANGE_ATTACHED          = 2,
    USB_CEP_STATE_CHANGE_POWERED           = 3,
    USB_CEP_STATE_CHANGE_RESET             = 4,
    USB_CEP_STATE_CHANGE_ADDRESSED         = 5,
    USB_CEP_STATE_CHANGE_CONFIGURED        = 6,
    USB_CEP_STATE_CHANGE_DECONFIGURED      = 7,
    USB_CEP_STATE_CHANGE_SUSPENDED         = 8,
    USB_CEP_STATE_CHANGE_RESUMED           = 9
} usb_cep_state_change;


////////////////////////////////////////////////////////////////////////////
// usb bulk
#define USB_BULK_CBW_SIGNATURE          0x55534243
#define USB_BULK_CSW_SIGNATURE          0x55534253

#define USB_BULK_MAX_CMD_LEN            0x10

#define BULK_CSW_GOOD                   0x00
#define BULK_CSW_FAIL                   0x01
#define BULK_CSW_PHASE_ERROR            0x02

typedef enum
{
    BULK_IDLE = 0,
    BULK_OUT_DATA,
    BULK_OUT_END,
    BULK_IN_DATA,
    BULK_IN_END,
    BULK_ACK,
    BULK_STALL,
    BULK_GOOD_END,
    BULK_FAIL_END,
    BULK_STOP
}usb_bulk_status;


////////////////////////////////////////////////////////////////////////////
//usb scsi
#define SCSI_DEVICE_TYPE_DACCESS            0x00
#define SCSI_DEVICE_TYPE_CDROM              0x05

#define RBC_CMD_FORMT                       0x04
#define RBC_CMD_READ10                      0x28
#define RBC_CMD_READCAPACITY                0x25
#define RBC_CMD_STARTSTOPUNIT               0x1B
#define RBC_CMD_SYNCACHE                    0x35
#define RBC_CMD_VERIFY10                    0x2F
#define RBC_CMD_WRITE10                     0x2A

#define SPC_CMD_INQUIRY                     0x12
#define SPC_CMD_MODESELECT6                 0x15
#define SPC_CMD_MODESENSE6                  0x1A
#define SPC_CMD_MODESENSE10                 0x5A
#define SPC_CMD_PERSISTANTRESERVIN          0x5E
#define SPC_CMD_PERSISTANTRESERVOUT         0x5F
#define SPC_CMD_PRVENTALLOWMEDIUMREMOVAL    0x1E
#define SPC_CMD_RELEASE6                    0x17
#define SPC_CMD_REQUESTSENSE                0x03
#define SPC_CMD_RESERVE6                    0x16
#define SPC_CMD_TESTUNITREADY               0x00
#define SPC_CMD_WRITEBUFFER                 0x3B
#define SPC_CMD_FORMAT_CAPACITY             0x23
#define SPC_CMD_MEDIUM_REMOVAL              0x1E


#define MMC_CMD_TOC                         0x43
#define MMC_CMD_GET_CONFIGURATION           0x46
#define MMC_CMD_GET_EVENT                   0x4a


typedef struct
{
    INT8U   cmdid;
    INT8U   rsv0;
    INT32U  logicalBlockAddress;
    INT8U   rsv1;
    INT16U  transferLength;
    INT8U   control;
    INT8U   rsv2[6];
} __attribute__((packed)) SCSI_READ10;

typedef struct
{
    INT8U   cmdid;
    INT8U   rsv0;
    INT32U  logicalBlockAddress;
    INT8U   rsv1;
    INT16U  transferLength;
    INT8U   control;
} __attribute__((packed)) SCSI_WRITE10;

typedef struct usb_cep_pkt
{
    INT8U ctrl_buf[8];
    INT8U *desc_buf;
    INT32U desc_size;

    INT32U state;
    INT32U state_change;

    INT8U reqError;
    INT8U max_lun;
    INT8U req_error;
    INT8U remote_wakeup;
    INT8U self_powered;
    INT8U configuration;
    INT8U sent_state;
    INT8U rsv2;
} __attribute__((packed)) usb_cep_pkt;

typedef struct usb_bk_cbw_pkt
{
    INT32U sign;
    INT32U tag;
    INT32U len;
    INT8U  flag;
    INT8U  lun;
    INT8U  cmd_len;
    INT8U  cmd[16];
} __attribute__((packed)) usb_bk_cbw_pkt;

typedef struct usb_bk_csw_pkt
{
    INT32U sign;
    INT32U tag;
    INT32U residue_len;
    INT8U  status;
} __attribute__((packed)) usb_bk_csw_pkt;

typedef struct usb_bk_pkt
{
    usb_bk_cbw_pkt cbw;
    INT8U status;

    usb_bk_csw_pkt csw;
    INT8U rsv[3];

    INT32U need_rx_len;
    INT32U rx_len;
    INT8U *rx_buf;

    INT32U need_tx_len;
    INT32U tx_len;
    INT8U *tx_buf;
} __attribute__((packed)) usb_bk_pkt;

void usb_otg_init(void);
void usb_dev_dsr(void);
void usb_dev_init(void);
INT32U usb_dev_ep1_rx(INT8U *buf, INT32U len);
void usb_dev_ep2_tx(INT8U *buf, INT32U len);
void usb_cep_setup(void);
void usb_cep_update(void);
void usb_cep_sent(void);
void usb_cep_get_status(void);
void usb_cep_set_feature(void);
void usb_cep_clear_feature(void);
void usb_cep_set_address(void);
void usb_cep_get_descriptor(void);
void usb_cep_set_descriptor(void);
void usb_cep_get_config(void);
void usb_cep_set_config(void);
void usb_cep_get_interface(void);
void usb_cep_set_interface(void);
void usb_cep_synch_frame(void);
void usb_cep_get_max_lun(void);
void usb_cep_mass_storage_reset(void);

void switchCryptoMode(INT32U mode);

void usb_bk_reset_out();
void usb_bk_reset_in();
void usb_bk_csw();
void usb_bk_cbw();
void usb_bk_error(INT8U stat, INT32U size);
void usb_bk_out_data();
void usb_bk_in_data();
void usb_bk_out();
void usb_bk_in();
void usb_scsi();

void usb_init (void);

extern volatile usb_otg_reg *pUsbOtgReg;
extern volatile usb_dev_reg *pUsbDevReg;
extern volatile usb_cpt_reg *pUsbCptReg;
extern volatile INT32U usb_irq_stat;
extern volatile INT32U usb_irq_flag;
extern volatile INT32U epInMask;
extern volatile INT32U epOutMask;


#endif

