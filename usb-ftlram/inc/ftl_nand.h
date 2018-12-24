/*
********************************************************************************
********************************************************************************

                    Copyright (c) 2017, JETWAY, WuHan
                            All Rights Reserved.

********************************************************************************
********************************************************************************
 */

#ifndef __FTL_NAND__
#define __FTL_NAND__




#define OPTION_ASSERT

#ifdef OPTION_ASSERT
#define  ASSERT(flag)   \
    do {\
        if (!(flag))\
        {\
            jprintf("%s-%s LINE:%d Assert!\n", __FILE__, __FUNCTION__, __LINE__);\
            while(1);\
        }\
    }while(0)
#else
#define  ASSERT(flag)
#endif



/*  MT29F64G08CABA flash attribution */


#define CHN_WIDTH   2   // 16bit IO

//#define USING_2_PLANE
#ifdef USING_2_PLANE
#define NUM_PLANE   2
#else
#define NUM_PLANE   1
#endif

#define NUM_BANKS   1

#define BYTES_PER_SECTOR        1024
#define SECTORS_PER_PHYPAGE     4
#define PAGES_PER_PHYBLK        256
#define PAGES_PER_VBLK          (256/CHN_WIDTH)
#define BLKS_PER_BANK           4096
#define SPARE_BLKS_PER_BANK     0
#define SPARE_PER_PHYPAGE       224

#define BYTES_PER_PHYPAGE      (BYTES_PER_SECTOR * SECTORS_PER_PHYPAGE)
#define BYTES_PER_PAGE         (BYTES_PER_SECTOR * SECTORS_PER_PAGE)
#define BYTES_PER_PAGE_EXT     ((BYTES_PER_PHYPAGE + SPARE_PER_PHYPAGE)*PHYPAGES_PER_PAGE)
#define BYTES_PER_VBLK         (BYTES_PER_PAGE       * PAGES_PER_VBLK)
#define BYTES_PER_BANK         ((INT64U)BYTES_PER_PAGE * PAGES_PER_BANK)
#define BYTES_PER_SMALL_PAGE   (BYTES_PER_PHYPAGE * CHN_WIDTH)

#define SECTORS_PER_PAGE       (SECTORS_PER_PHYPAGE *  PHYPAGES_PER_PAGE)
#define SECTORS_PER_SMALL_PAGE (SECTORS_PER_PHYPAGE * CHN_WIDTH)
#define SECTORS_PER_VBLK       (SECTORS_PER_PAGE    * PAGES_PER_VBLK)
#define SECTORS_PER_BANK       (SECTORS_PER_PAGE    * PAGES_PER_BANK)

#ifdef USING_2_PLANE
#define PHYPAGES_PER_PAGE      (CHN_WIDTH * NUM_PLANE)
#else
#define PHYPAGES_PER_PAGE      (CHN_WIDTH)
#endif


#define PAGES_PER_BLK          (PAGES_PER_VBLK) 
#define PAGES_PER_BANK         (PAGES_PER_VBLK * VBLKS_PER_BANK)

#define PHYBLKS_PER_BANK       (BLKS_PER_BANK + SPARE_BLKS_PER_BANK)
#ifdef USING_2_PLANE    
#define VBLKS_PER_BANK         (PHYBLKS_PER_BANK/NUM_PLANE)
#define SPARE_VBLKS_PER_BANK   (SPARE_BLKS_PER_BANK/NUM_PLANE)
#else
#define VBLKS_PER_BANK         (PHYBLKS_PER_BANK)
#define SPARE_VBLKS_PER_BANK   (SPARE_BLKS_PER_BANK)
#endif

#define NUM_VBLKS              (VBLKS_PER_BANK * NUM_BANKS)
#define NUM_VPAGES             (PAGES_PER_VBLK * NUM_VBLKS)
#define NUM_PSECTORS           (SECTORS_PER_VBLK * (VBLKS_PER_BANK - SPARE_VBLKS_PER_BANK) * NUM_BANKS)
#define NUM_LSECTORS           (NUM_PSECTORS)  * 50 / 100
#define NUM_LPAGES             ((NUM_LSECTORS + SECTORS_PER_PAGE - 1) / SECTORS_PER_PAGE)         



#define PAGE_MAP_BYTES         ((NUM_LPAGES * sizeof(INT32U)+BYTES_PER_SECTOR-1) / BYTES_PER_SECTOR * BYTES_PER_SECTOR)

#endif

