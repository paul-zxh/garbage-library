/*
********************************************************************************
********************************************************************************

                    Copyright (c) 2017, JETWAY, WuHan
                            All Rights Reserved.

********************************************************************************
********************************************************************************
 */


#include "j32xx.h"
#ifdef JTAG_PRINT_SUPPORT

#include "jtag_con.h"
#include <stdarg.h>
#include "vsprintf.h"

#define JTAG_DATA_REG  (*(VINT32U*)(CON_DATA_REG))
#define JTAG_STS_REG   (*(VINT32U*)(CON_STS_REG ))
#define JTAG_REQ_REG   (*(VINT32U*)(CON_REQ_REG ))

int ffopen(int filename)
{
    int m_sts = 0;

    // open a filename 
    do {
        m_sts = JTAG_STS_REG;
    }
    while (!(m_sts & 0x80000000));

    JTAG_STS_REG = 0x81000000 |(0x01<<8)|(filename << 16);            //2'b01: inform jtag to open a new file for receiving data
       
    while (1) {
        m_sts = JTAG_STS_REG;
        if (m_sts & 0x01000000) {
            JTAG_DATA_REG = 0;
            break;
        }
    }

    do {
        m_sts = JTAG_STS_REG;
    }
    while (!(m_sts & 0x01000000));
    
    return 1;
}

int ffclose(int filename)
{
    int m_sts = 0;
    // close a filename 
    do {
        m_sts = JTAG_STS_REG;
    }
    while (!(m_sts & 0x80000000));  
    JTAG_STS_REG = 0x81000000 |(0x03<<8)|(filename << 16);                    //2'b11: inform jtag to close the file
    
    // send packet command/type

    while (1) {
        m_sts = JTAG_STS_REG;
        if (m_sts & 0x01000000) {
            JTAG_DATA_REG = 0;
            break;
        }
    }

    do {
        m_sts = JTAG_STS_REG;
    }
    while (!(m_sts & 0x01000000));

    return 1;
}

void Jtag_SendByte(char pt)
{

    int m_sts = 0;
    int i;
    int count = 1;
    // connect ?
    
    //JTAG_STS_REG = 0;
    JTAG_STS_REG = 0x81000000;
    do {
        m_sts = JTAG_STS_REG;
    }
    while (!(m_sts & 0x80000000));

    // send packet command/type
    m_sts = JTAG_STS_REG;
    // type tag = 1'b1 for binary
    // packet command = 2'b00 for inform jtag console there is data and write data
    m_sts = (m_sts & 0xfffffbf0) | 0x001;
    JTAG_STS_REG = m_sts;

    // send data
    for (i = 0; i < count; i++) {
        while (1) {
            m_sts = JTAG_STS_REG;
            if (m_sts & 0x01000000) {  
                JTAG_DATA_REG = pt;
            
                break;
            }
            
        }
    }
    do {
        m_sts = JTAG_STS_REG;
    }
    while (!(m_sts & 0x01000000));
    
        
}

void Jtag_SendString(char *pt)
{
    while(*pt)
        Jtag_SendByte(*pt++);
}

void jtag_printf(char *fmt,...)
{
    va_list ap;
    char string[256];

    va_start(ap,fmt);
    vsprintf(string,fmt,ap);
    Jtag_SendString(string);
    va_end(ap);
}

void Jtag_FSendByte(int filename,char pt)
{

    int m_sts = 0;
    int i;
    int count = 1;
    // connect ?
    
    //JTAG_STS_REG = 0;

    do {
        m_sts = JTAG_STS_REG;
    }
    while (!(m_sts & 0x80000000));
    
    JTAG_STS_REG = 0x81000000 |(0x01)|(0x02<<8)|(filename << 16);         //2'b10: inform jtag to append the existing for receiving data

    // send data
    for (i = 0; i < count; i++) {
        while (1) {
            m_sts = JTAG_STS_REG;
            if (m_sts & 0x01000000) {  
                JTAG_DATA_REG = pt;
                break;
            }
            
        }
    }
    do {
        m_sts = JTAG_STS_REG;
    }
    while (!(m_sts & 0x01000000));
    
        
}

void Jtag_FSendString(int filename,char *pt)
{
    while(*pt)
        Jtag_FSendByte(filename,*pt++);
}

void jtag_fprintf(int filename,char *fmt,...)
{
    va_list ap;
    char string[256];

    va_start(ap,fmt);
    vsprintf(string,fmt,ap);
    Jtag_FSendString(filename,string);
    va_end(ap);
}

#endif// JTAG_PRINT_SUPPORT

