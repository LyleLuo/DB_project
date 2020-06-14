#ifndef DATA_PAGE
#define DATA_PAGE

#include <libpmem.h>
#include <string>
#include <cstdlib>

#define DATA_PAGE_SLOT_NUM 16
// use pm_address to locate the data in the page

// uncompressed page format design to store the buckets of PmEHash
// one slot stores one bucket of PmEHash
typedef struct data_page {
    // fixed-size record design
    pm_bucket slot[DATA_PAGE_SLOT_NUM];
    uint8_t bitmap[DATA_PAGE_SLOT_NUM / 8];
    // uncompressed page format
} data_page;

//get 0-15 bit from bitmap, false repersents 0, true repersents 1
inline bool getBitFromBitmap(uint8_t * bitmap, int pos);

//set 0-15 bit to bitmap, false repersents 0, true repersents 1
inline void setBitToBitmap(uint8_t * bitmap, int pos, bool flag);

//create a new page
void * createNewPage(uint64_t page_id);

//delete a page
void deletePage(uint64_t page_id);
#endif