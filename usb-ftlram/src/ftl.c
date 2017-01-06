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


/*----------------------------*/
/* NAND FLASH LAYOUT */
/*----------------------------*/
// Block #0 : firmware binary image, etc.
// Block #1 : misc, metadata
// Block #2 ~ #31 : page map table
// BLock#32: garbage collect
// Block #33: user data blocks

//format mark at Block#0Page#0
#define FORMAT_MARK 0x12345678
#define NUM_BLOCKS  255  //2048 blocks per plane, J3223 has 2 plane

#define VC_MAX      0xCDCD

#define FWBLK_VBN   0x00
#define MISCBLK_VBN 0x01
#define GCBLK_VBN   0x32
#define USRBLK_VBN  0x33
#define MAPBLKS_PER_BANK    (((PAGE_MAP_BYTES / NUM_BANKS) + BYTES_PER_PAGE-1) /  BYTES_PER_PAGE)
#define META_BLKS_PER_BANK  (1  + 1 + MAPBLKS_PER_BANK)

#define NUM_MISC_META_SEC   ((sizeof(misc_metadata) + BYTES_PER_SECTOR - 1) / BYTES_PER_SECTOR)
#define NUM_VCOUNT_SEC      ((VBLKS_PER_BANK * sizeof(INT16U) + BYTES_PER_SECTOR - 1) / BYTES_PER_SECTOR)

typedef struct _misc_metadata
{
    INT32U current_miscblk_vpn;

    INT32U free_blk_cnt;

}misc_metadata;

typedef struct _filemap_
{
    INT32U current_file_vpn;

    INT32U free_file_cnt;

}filemap;



//static misc_metadata g_misc_metadata[NUM_BANKS];
//static misc_metadata g_misc_blktable[NUM_BANKS];

static misc_metadata g_misc_metadata[NUM_BANKS];
//static misc_metadata g_misc_blktable[NUM_BANKS];
misc_metadata g_misc_blktable[500];


filemap g_filemap[500];
//static filemap g_filemap[NUM_BANKS];




INT32U page_buff[BYTES_PER_PAGE >> 2];


#define inc_miscblk_vpn(bank)       (g_misc_metadata[bank].current_miscblk_vpn++)


#define get_miscblk_vpn(bank)       (g_misc_metadata[bank].current_miscblk_vpn)
#define set_miscblk_vpn(bank, vpn)  (g_misc_metadata[bank].current_miscblk_vpn = vpn)

//$$$========================by zxh===============================//

#define get_mapblk_vpn(bank)        (g_misc_blktable[bank].current_miscblk_vpn)
#define set_mapblk_vpn(bank, vpn)   (g_misc_blktable[bank].current_miscblk_vpn = vpn)

#define get_mapfile_vpn(file)       (g_filemap[file].current_file_vpn)
#define set_mapfile_vpn(file, vpn)  (g_filemap[file].current_file_vpn = vpn)

INT32U vpn;//0~13 block lpn; 14~27 file lpn 
INT32U file_start;
INT32U file_end;
INT32U FLIEVPN; 

#define FILE_SIZE  8388508
#define verify_id 0x80000000
//============================by zxh============================$$$//




#define CHECK_VPAGE(vpn) ASSERT((vpn) < (PAGES_PER_VBLK * VBLKS_PER_BANK ))


static void sanity_check(void)
{
    ASSERT(sizeof(g_misc_metadata) <= BYTES_PER_PAGE);
}

static INT32U get_vpn(INT32U const lpn)
{
    return lpn;
}
/*

static void set_format_mark(void)
{
    page_buff[0] = FORMAT_MARK;
    WritePage(0, 0, 1, page_buff);
}

static INT8U get_format_mark(void)
{
    INT8U ret = 0;
    
    ReadPage(0, 0, 1, page_buff);
    ret = (page_buff[0] == FORMAT_MARK) ? 0 : 1;

    return ret;
}

*/
//================by zxh=========//
//#define NUM_BLOCKSS  8   //2048 blocks per plane, J3223 has 2 plane





void ftl_open(void)
{
    INT32U lblock=0;

    file_start = file_end = 0;


    for(lblock=0;lblock <= NUM_BLOCKS;lblock++) {
        EraseBlock(lblock*256); 
        jprintf(" ");

    }
}
    
//================by zxh=========//



void set_format_mark(INT32U filename)
{
    INT32U wlblock = 0;//all block
    INT32U llblock = 0;//have writen block

    INT32U pagenumber = 0;//2048;//file size // file number
    INT32U blocknumber = 0;//file size // block number
    INT16U i = 0;



    INT16U filenumber = 0;
/*
    if (file_end == 248) {
        for (llblock=0; llblock<=240; llblock++) {
            EraseBlock(llblock*256);
            jprintf(" ");
        }
    }
*/
    if (file_end == 8) {
        for (llblock=240; llblock<=NUM_BLOCKS; llblock++) {
            EraseBlock(llblock*256);
            jprintf(" ");
        }
    }

    //filenumber++;

    file_start = file_end+0;

    wlblock = file_start;
    //jprintf("file_start  = %d   file_end  = %d .\n",file_start,file_end);


    pagenumber = FILE_SIZE/4096 ;
    if(FILE_SIZE%1024)
    pagenumber = pagenumber +1;
    
    blocknumber = FILE_SIZE/4096/256;
    if(FILE_SIZE/1024%256!=0)
    blocknumber = blocknumber +1;   

    file_end =((file_end + blocknumber)&0x000000ff); 

    //jprintf("get_mapblk_vpn  = %d  .\n",wlblock);

    //jprintf("get_mapblk_vpn  = %d  .\n",get_mapblk_vpn(wlblock));
    while (blocknumber) {
        //jprintf("get_mapblk_vpn  = %d  .\n",get_mapblk_vpn(wlblock));

        //jprintf("get_mapblk_vpn  = %d  .\n",wlblock);
        //if(get_mapblk_vpn(wlblock)==248)       
        //jprintf("get_mapblk_vpn  = %d  .\n",get_mapblk_vpn(wlblock));
        //jprintf("get_mapblk_vpn  = %d  .\n",wlblock);
        
        if (wlblock <= NUM_BLOCKS) {
            set_mapfile_vpn(filename,(filename<<16)|(wlblock));
            //jprintf("1\n");

            for (i=0; i<256; i++) {
                NFC_ProgramFtl_T(1, wlblock*256+i,1,0x12345678);
                pagenumber--;
                if(!pagenumber) break;
            }   
        }
        //set_mapblk_vpn(wlblock,wlblock);
        wlblock++;
        blocknumber--;
        //jprintf("write finish  wlblock = %d  blocknumber = %d\n",wlblock,blocknumber);
    }

    FLIEVPN = get_mapfile_vpn(filename);
    //jprintf("filename = %d  .\n",filename);
    //jprintf("FLIEVPN=== = 0x%8.8x  .\n",FLIEVPN);


}


void get_format_mark(INT32U filename)
{
    INT32U rlblock = 0;
    INT32U file_block;
    INT16U i = 0;
    INT16U filenumber,blocknumber;

    if (file_start > file_end) {
        for (rlblock=file_start; rlblock<file_end; rlblock++) {
            for (i=0; i<256; i++) {
                NFC_ReadFtl_T(1, rlblock*256+i, 1);
            }   
            //jprintf("get_mapblk_vpn  = %d  .\n",rlblock);
        }   
        
    }
    if (file_end < file_start) {
        for (rlblock=file_start; rlblock<NUM_BLOCKS; rlblock++) {
            for (i=0; i<256; i++) {
                NFC_ReadFtl_T(1, rlblock*256+i, 1);
            }
        }
        if (file_end > 0) {
            for (rlblock=0; rlblock<file_start; rlblock++) {
                for (i=0; i<256; i++) {
                    NFC_ReadFtl_T(1, rlblock*256+i, 1);
                }   
            }
        }
    }
}



static void init_misc_metadata(void)
{
    INT32U bank = 0;
    INT32U vblock = 0;
    INT16U *vcountptr = NULL;

    for (bank = 0; bank < NUM_BANKS; bank++) {
        // init misc block
        g_misc_metadata[bank].free_blk_cnt = VBLKS_PER_BANK - META_BLKS_PER_BANK;

        set_miscblk_vpn(bank, MISCBLK_VBN * PAGES_PER_BLK - 1);

        // init page mapping table
      
        // init garbage collect block
    }
}

static void log_pagemaptable(void)
{

}

static void log_misc_metadata(void)
{
    INT32U bank = 0;

    for (bank = 0; bank < NUM_BANKS; bank ++) {
        inc_miscblk_vpn(bank);

        // if misc meta data block is full, erase old block and write vpn again
        if ((get_miscblk_vpn(bank)/PAGES_PER_BLK) != MISCBLK_VBN) {
            //EraseBlock(MISCBLK_VBN);
            set_miscblk_vpn(bank,MISCBLK_VBN * PAGES_PER_BLK);
        }

        //memcpy(page_buff, g_misc_metadata, sizeof(g_misc_metadata));
        WritePage(get_miscblk_vpn(bank), 0, NUM_MISC_META_SEC, page_buff);
    }
    
}

static void setup_badblock_list(void)
{

}

static void ftl_format(void)
{
    INT32U  vblock = 0;
    INT32U  bank = 0;
    
    ASSERT (NUM_MISC_META_SEC > 0);
    ASSERT(NUM_VCOUNT_SEC > 0);

    jprintf("VBLKS_PER_BANK:%d\n", VBLKS_PER_BANK);
    jprintf("LBLKS_PER_BANK:%d\n", NUM_LPAGES/PAGES_PER_BLK/NUM_BANKS);
    jprintf("META_BLKS_PER_BANK: %d\n", META_BLKS_PER_BANK);

    // erase all block exept block #0;
    for (vblock = MISCBLK_VBN; vblock < VBLKS_PER_BANK; vblock ++) {
        for (bank = 0; bank < NUM_BANKS; bank ++) {
            //EraseBlock(vblock + bank * VBLKS_PER_BANK);
        }

    }

//  init_misc_metadata();

//  log_pagemaptable();
//  log_misc_metadata();

    //set_format_mark();
    jprintf("FTL format done !\n");
}

void ftl_flush(void)
{   
    log_pagemaptable();
    log_misc_metadata();
}
/*
void ftl_open(void)
{
    sanity_check();
    
    setup_badblock_list();

    if (get_format_mark()) {
        jprintf("ftl_format \n");
        ftl_format();
    }
    else {
        jprintf("load misc and ptable data \n");
        log_misc_metadata();
        log_pagemaptable();
    }

}
*/





void ftl_read(INT32U const lba, INT32U const secnum)
{
    INT32U lpn = 0;
    INT32U sectoroffset = 0;
    INT32U remainsectors = 0;
    INT32U readsectornum = 0;   
        INT32U vpn = 0;
    

    lpn = lba / SECTORS_PER_PAGE;
    sectoroffset = lba % SECTORS_PER_PAGE;
    remainsectors = secnum;

    while (remainsectors != 0) {
        if ((sectoroffset + remainsectors) < SECTORS_PER_PAGE) {
            readsectornum = remainsectors;
        }
        else {
            readsectornum = SECTORS_PER_PAGE - sectoroffset;
        }

        vpn = get_vpn(lpn);
        CHECK_VPAGE(vpn);

        if (vpn != 0) {
            nand_read_to_host(vpn, sectoroffset, readsectornum);
        }
        else {
            /* host request to read a page which is never written */
            while(1);
        }
        sectoroffset = 0;
        remainsectors -= readsectornum;
        lpn ++;
    }
}

void ftl_write(INT32U const lba, INT32U const secnum)
{


}

void ftl_isr(void)
{

}





