/*
********************************************************************************
********************************************************************************

                    Copyright (c) 2017, JETWAY, WuHan
                            All Rights Reserved.

********************************************************************************
********************************************************************************
 */


//update 2011-05-06 14:29
#ifndef _NANDFLASHCFG_H_
#define _NANDFLASHCFG_H_

#define     OPEN                        0x01
#define     CLOSE                       0x00
#define     NFC_OVERTIME                0xffff
#define     ASYNC16_MODE                0x01
#define     SYNC16_MODE                 0x02
#define     ASYNC8_MODE                 0x03
#define     SYNC8_MODE                  0x04

//////////////////////////////////////////////////////////////////////////////////////////////////////////
#define     NFC_FACE_MODE               ASYNC16_MODE
#define     NFC_SPEED_MODE              5
#define     NFC_ECC_LEN                 24         //(bit)
#define     NFC_MAIN_CLK                66         //(MHz)
#define     NFC_CLK_DIV                 6 //4//4 6 8 12
//////////////////////////////////////////////////////////////////////////////////////////////////////////
#define     NAND_FLASH_PAGE_SIZE        8192
#define     BLOCK_NUM_PER_FLASH         4096
#define     PAGE_PER_BLOCK              256
//////////////////////////////////////////////////////////////////////////////////////////////////////////
#define     NFC_CACHE_EN                OPEN
#define     NFC_MULTI_PLANE_EN          CLOSE//OPEN
#define     NFC_MULTI_LUN_EN            CLOSE
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#define     TEST_DATA_START             0x55555555

#define     NFC_FTL_WRITE_EN            CLOSE
#define     NFC_FTL_READ_EN             CLOSE
#define     NFC_AUX_VERIFY_EN           OPEN
#define     NFC_CPUMODE_TEST_EN         0

#define     NFC_CPUMODE_TEST_AUX_SIZE   2 //bsize =2*sector  
#define     NFC_CPUMODE_TEST_AUX_TIME   0 //IF =0 TEST WILL BY PASS
#define     NFC_CPUMODE_TEST_FTL_SIZE   2 //bsize =2*sector 
#define     NFC_CPUMODE_TEST_FTL_TIME   0 //IF =0 TEST WILL BY PASS

#define     NFC_CPUMODE_TEST_TIME       2

#define     NFC_CPUMODE_TEST_ROWADR     256008  //sectorAdr/SECTOR_PER_PAGE;
#define     NFC_CPUMODE_TEST_COLUMNADR  0

#define     NFC_STD_DATA_USE            1

#define     NFC_AUX_DAT_ADR             0x000
#define     NFC_FTL_DAT_ADR             0x200
//#define     NFC_FTL_DAT_ADR           0x400
#define     NFC_FTL_CMD_ADR             0x100



///////////////////////////////////////////////////////////////////////////////////////////////////////////



#if((NFC_FACE_MODE == ASYNC16_MODE)||(NFC_FACE_MODE==SYNC16_MODE))
#define NFLASHWITH      16
#define NSIZE           2
#elif ((NFC_FACE_MODE == ASYNC8_MODE)||(NFC_FACE_MODE==SYNC8_MODE))
#define NFLASHWITH      8
#define NSIZE           1
#endif


/*

# ns of differnet modest

ADL:    [   200,    100,    100,    100,     70,     70]     //ALE to data loading time
tALH:   [     20,      10,     10,      5,      5,      5]      //ALE Hold time     
tCCS:   [   200,     200,   200,    200,    200,    200]   //Change Column Setup time
tCLH:   [    20,       10,     10,      5,      5,      5]       //CLE Hold time
tCS:    [     70,      35,      25,     25,     20,     20]    //CE# Setup time
tRC:    [   100,      50,      35,     30,     25,     25]    //RE# Cycle time
tRHW:   [   200,    100,    100,    100,    100,    100]  //RE# High to WE# Low
tRP:    [     50,       25,     17,     15,     12,     12]     //RE#pulse width
tRR:    [     40,      20,      20,     20,     20,     20]    // Ready to RE#low(data only)
tWB:    [    200,    100,    100,    100,    100,    100]  //we# high to r/b# low
tWHR:   [    120,     80,      80,     60,     60,     60]    //WE#High to RE# low
tWP:    [      50,     25,      17,     15,     12,     12]   //WE#PULSE width
tCLS:   [     50,      25,      15,     10,      10,      10,   //CLE setup time
tALS:   [    50 ,     25,         15,    10,      10,      10]
# ns of RESET
tRST_power_on:    5000
# ns of DQS delay
tDQSD:              35


*/
#if(NFC_MAIN_CLK==100)

/*
 * Asynchronous ONFI timing modes
 *  System Clock = 100Mhz
 *  Pulse Width  = 10ns.
 *
 *                        Mode  0   1   2   3   4   5
 *======================================================*/
static const INT8U tADL [] = { 20,  10,  10,  10,  7,  7};
static const INT8U tALH [] = {  2,  1,  1,  0,  0,  0};
static const INT8U tCCS [] = { 20, 20, 20, 20, 20, 20};
static const INT8U tCLH [] = {  2,  1,  1,  0,  0,  0};
static const INT8U tCS  [] = {  7,  3,  2,  2,  2,  2};
static const INT8U tRC  [] = {  10,  5,  3,  3,  2,  2};    
static const INT8U tRHW [] = { 20,  10,  10,  10,  10,  10};
static const INT8U tRP  [] = {  5,  2,  1,  1,  1,  1};//change mode 4,5  0-->1
static const INT8U tRR  [] = {  4,  2,  2,  2,  2,  2};
static const INT8U tWB  [] = { 20,  10,  10,  10,  10,  10};
static const INT8U tWHR [] = {  12,  8,  8,  6,  6,  6};
static const INT8U tWP  [] = {  5,  2,  1,  1,  1,  0};
static const INT8U tCLS[]=   {  5,  2,  1,  1,  1,  1};
static const INT8U tALS[]=   {  5,  2,  1,  1,  1,  1};
// Constant values for all modes.
#define tRST_power_on       333
#define tDQSD               2
#define NFC_SYSTEM_CLK      100

#elif (NFC_MAIN_CLK==66)
/*
 * Asynchronous ONFI timing modes
 *  System Clock = 66Mhz
 *  Pulse Width  = 15ns.
 *
 *                        Mode  0   1   2   3   4   5
 *======================================================*/
static const INT8U tADL [] = { 13,  6,  6,  6,  4,  4};
static const INT8U tALH [] = {  1,  0,  0,  0,  0,  0};
static const INT8U tCCS [] = { 13, 13, 13, 13, 13, 13};
static const INT8U tCLH [] = {  1,  0,  0,  0,  0,  0};
static const INT8U tCS  [] = {  4,  2,  1,  1,  1,  1};
static const INT8U tRC  [] = {  6,  3,  2,  2,  1,  1}; 
static const INT8U tRHW [] = { 13,  7,  7,  7,  7,  7};
static const INT8U tRP  [] = {  3,  1,  1,  1,  1,  1};//change mode 4,5  0-->1
static const INT8U tRR  [] = {  2,  1,  1,  1,  1,  1};
static const INT8U tWB  [] = { 13,  6,  6,  6,  6,  6};
static const INT8U tWHR [] = {  8,  5,  5,  4,  4,  4};
static const INT8U tWP  [] = {  3,  1,  1,  1,  0,  0};
static const INT8U tCLS[]=   {  3,  1,  1,  1,  0,  0};
static const INT8U tALS[]=   {  3,  1,  1,  1,  0,  0};
// Constant values for all modes.
#define tRST_power_on       333
#define tDQSD               2
#define NFC_SYSTEM_CLK      66

#elif(NFC_MAIN_CLK==50)
/*
 * Asynchronous ONFI timing modes
 *  System Clock = 50Mhz
 *  Pulse Width  = 20ns.
 *
 *                          Mode  0   1   2   3   4   5
 *======================================================*/
static const INT8U tADL [] = { 10,  5,  5,  5,  3,  3};
static const INT8U tALH [] = {  1,  0,  0,  0,  0,  0};
static const INT8U tCCS [] = { 10, 10, 10, 10, 10, 10};
static const INT8U tCLH [] = {  1,  0,  0,  0,  0,  0};
static const INT8U tCS  [] = {  3,  1,  1,  1,  1,  1};
static const INT8U tRC  [] = {  5,  2,  1,  1,  1,  1}; 
static const INT8U tRHW [] = { 10,  5,  5,  5,  5,  5};
static const INT8U tRP  [] = {  2,  1,  1,  1,  1,  1}; //change tRP mode 2,3,4,5 --   0 -->1
static const INT8U tRR  [] = {  2,  1,  1,  1,  1,  1};
static const INT8U tWB  [] = { 10,  5,  5,  5,  5,  5};
static const INT8U tWHR [] = {  6,  4,  4,  3,  3,  3};
static const INT8U tWP  [] = {  2,  1,  0,  0,  0,  0};
static const INT8U tCLS[]=   {  2,  1,  1,  0,  0,  0};
static const INT8U tALS[]=   {  2,  1,  1,  0,  0,  0};
// Constant values for all modes.
#define tRST_power_on       250
#define tDQSD               1
#define NFC_SYSTEM_CLK      50


#elif (NFC_MAIN_CLK==33)

/*
 * Asynchronous ONFI timing modes
 *  System Clock = 33Mhz
 *  Pulse Width  = 30ns.
 *
 *                          Mode  0   1   2   3   4   5
 *======================================================*/
static const INT8U tADL [] = {  6,  3,  3,  3,  2,  2};
static const INT8U tALH [] = {  0,  0,  0,  0,  0,  0};
static const INT8U tCCS [] = {  6,  6,  6,  6,  6,  6};
static const INT8U tCLH [] = {  0,  0,  0,  0,  0,  0};
static const INT8U tCS  [] = {  2,  1,  0,  0,  0,  0};
static const INT8U tRC  [] = {  3,  1,  1,  1,  0,  0};
static const INT8U tRHW [] = {  6,  3,  3,  3,  3,  3};
static const INT8U tRP  [] = {  1,  0,  0,  0,  0,  0};
static const INT8U tRR  [] = {  1,  0,  0,  0,  0,  0};
static const INT8U tWB  [] = {  6,  3,  3,  3,  3,  3};
static const INT8U tWHR [] = {  4,  2,  2,  2,  2,  2};
static const INT8U tWP  [] = {  1,  0,  0,  0,  0,  0};
static const INT8U tCLS[]=   {  1,  0,  0,  0,  0,  0};
static const INT8U tALS[]=   {  1,  0,  0,  0,  0,  0};
// Constant values for all modes.
#define tRST_power_on       166
#define tDQSD               1
#define NFC_SYSTEM_CLK      33

#elif (NFC_MAIN_CLK==25)
/*
 * Asynchronous ONFI timing modes
 *  System Clock = 25Mhz
 *  Pulse Width  = 40ns.
 *
 *                          Mode  0   1   2   3   4   5
 *======================================================*/
static const INT8U tADL [] = {  5,  2,  2,  2,  1,  1};
static const INT8U tALH [] = {  0,  0,  0,  0,  0,  0};
static const INT8U tCCS [] = {  5,  5,  5,  5,  5,  5};
static const INT8U tCLH [] = {  0,  0,  0,  0,  0,  0};
static const INT8U tCS  [] = {  1,  0,  0,  0,  0,  0};
static const INT8U tRC  [] = {  2,  1,  0,  0,  0,  0};
static const INT8U tRHW [] = {  5,  2,  2,  2,  2,  2};
static const INT8U tRP  [] = {  1,  0,  0,  0,  0,  0};
static const INT8U tRR  [] = {  1,  0,  0,  0,  0,  0};
static const INT8U tWB  [] = {  5,  2,  2,  2,  2,  2};
static const INT8U tWHR [] = {  3,  2,  2,  1,  1,  1};
static const INT8U tWP  [] = {  1,  0,  0,  0,  0,  0};
static const INT8U tCLS[]=   {  0,  0,  0,  0,  0,  0};
static const INT8U tALS[]=   {  0,  0,  0,  0,  0,  0};
// Constant values for all modes.
#define tRST_power_on       125
#define tDQSD               0
#define NFC_SYSTEM_CLK      25
#endif


#define NFLASH_PAGE_SIZE        512
#define NFLASH_PAGE_PBLK        1024
#define CDFS_PAGE_SIZE          2048
#define CDFS_PAGE_PBLK          1024

void nandFlashWrite(INT32U sectorAddr,INT32U sectorNum);
void nandFlashRead(INT32U sectorAddr,INT32U sectorNum);

#endif

