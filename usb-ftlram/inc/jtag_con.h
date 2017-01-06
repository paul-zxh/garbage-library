/*
********************************************************************************
********************************************************************************

                    Copyright (c) 2017, JETWAY, WuHan
                            All Rights Reserved.

********************************************************************************
********************************************************************************
 */


#ifndef _JTAG_CONSOLE_
#define _JTAG_CONSOLE

/*
 * jtag console register definition
 */
#define CON_DATA_REG    0x08000530
#define CON_STS_REG     0x08000534
#define CON_REQ_REG     0x08000538


#define JTAG_DATA       (*((volatile unsigned int *)CON_DATA_REG))
#define JTAG_STAT       (*((volatile unsigned int *)CON_STS_REG))
#define JTAG_REQ        (*((volatile unsigned int *)CON_REQ_REG))


/*
 *  file channel number
 */
#define log0        0
#define log1        1
#define log2        2
#define log3        3

/*
 *  message type definition
 */
#define MSG_STRING      0
#define MSG_BINARY      1

/*
 * printf api for 8 bits
 * out_msg : pointer to buffer
 * count : base in 8bits
 */
int printfbfun(unsigned char *out_msg, int count);
int printfb(unsigned char *out_msg, int count);
int fprintfb(unsigned char *out_msg, int count);

/*
 * printf api for 32 bits
 * out_msg : pointer to buffer
 * counts : base in 32bits
 */
int printfwfun(unsigned int *out_msg, int count);
int printfw(unsigned int *out_msg, int count);
int fprintfw(unsigned int *out_msg, int count);

/*
 * printf api for string
 */
int printfsfun(char *out_msg, int count);
int printfs(char *out_msg, int count);
int fprintfs(char *out_msg, int count);



/*
 * jtag console buffer flush
 */
int flush_console();

/*
 * printf crlf key
 */
int fcrlf(int channel);

/*
 * jtag console delay function
 */
void jtag_delay(int time);


/*
 *  channel : file channel number
 *  file name  must be 0~63
 */
int ffopen(int filename);
int ffclose(int filename);


/*
 *use like printf,fprintf
 */
void jtag_fprintf(int filename,char *fmt,...);
void jtag_printf(char *fmt,...);


#endif


