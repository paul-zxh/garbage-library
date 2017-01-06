/*
********************************************************************************
********************************************************************************

                    Copyright (c) 2017, JETWAY, WuHan
                            All Rights Reserved.

********************************************************************************
********************************************************************************
 */


#ifndef _MEM_H
#define _MEM_H

void   memcpy32_r (INT32U * dst, INT32U * src, INT32S len);
void   memcpy32 (INT32U * dst, INT32U * src, INT32S len);
void   memcpy8_r (BYTE * dst, BYTE * src, INT32S len);
void   memcpy8 (BYTE * dst, BYTE * src, INT32S len);
void   memset32 (INT32U * dst, INT32U val, INT32S len);
void   memset8 (BYTE * dst, INT32U val, INT32S len);
void   memcpy32 (INT32U * dst, INT32U * src, INT32S len);
void   memcpy32htonl (INT32U * dst, INT32U * src, INT32S len);
void   memcpy32htonl_r (INT32U * dst, INT32U * src, INT32S len);
INT32S memcmp32(INT32U *src1, INT32U *src2, INT32S len);
INT32S memcmp8(BYTE *src1, BYTE *src2, INT32S len);
BYTE*  memmem(const BYTE* haystack, INT32U haystack_len, const BYTE* needle, INT32U needle_len);
INT32U htonl(INT32U x);
INT32U read_uint32(BYTE   *p, INT32U offset);
INT16U read_uint16(BYTE   *p, INT32U offset);
void   write_uint16(BYTE *p, INT32U offset, INT16U data);
void   write_uint32(BYTE *p, INT32U offset, INT32U data);
void   write_uint64(BYTE *p, INT32U offset, INT64U data);
void*  memcpy (void* dst, const void* src, INT32U len);
void*  memset (void* dst, INT32S val, INT32U len);

#endif


