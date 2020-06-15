#include"pm_ehash.h"

//get 0-15 bit from bitmap, false repersents 0, true repersents 1
inline bool getBitFromBitmap(uint8_t * bitmap, int pos) {
    return bitmap[pos / 8] & (1 << (7 - pos % 8));
}

//set 0-15 bit to bitmap, false repersents 0, true repersents 1
inline void setBitToBitmap(uint8_t * bitmap, int pos, bool flag) {
    if (flag) {
        bitmap[pos / 8] = bitmap[pos / 8] | (1 << (7 - pos % 8));
    }
    else {
        bitmap[pos / 8] = bitmap[pos / 8] & ~(1 << (7 - pos % 8));
    }
    pmem_persist(bitmap, 2);
}

//create a new page
void * createNewPage(uint64_t page_id) {
    size_t map_len;
    std::string page_location = "../data/" + std::to_string(page_id);
    data_page * p = reinterpret_cast<data_page*>(pmem_map_file(page_location.c_str(), sizeof(data_page), PMEM_FILE_CREATE, 0777, &map_len, NULL));
    for (int i = 0; i < DATA_PAGE_SLOT_NUM; ++i) {
        setBitToBitmap(p->bitmap, i, false);
    }
    return p;
}

//delete a page
void deletePage(uint64_t page_id) {
    std::string cmd = "rm ../data/" + std::to_string(page_id);
    system(cmd.c_str());
}

// 数据页表的相关操作实现都放在这个源文件下，如PmEHash申请新的数据页和删除数据页的底层实现