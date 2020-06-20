#ifndef _PM_E_HASH_H
#define _PM_E_HASH_H

#include<queue>
#include<map>
#include"data_page.h"

using std::queue;
using std::map;

class PmEHash
{
private:
    
    ehash_metadata* metadata;                    // virtual address of metadata, mapping the metadata file
    ehash_catalog catalog;                        // the catalog of hash

    queue<pm_bucket*> free_list;                      //all free slots in data pages to store buckets
    map<uint64_t, data_page*> page_list;
    map<pm_bucket*, pm_address> vAddr2pmAddr;       // map virtual address to pm_address, used to find specific pm_address
    map<pm_address, pm_bucket*> pmAddr2vAddr;       // map pm_address to virtual address, used to find specific virtual address
    
    uint64_t hashFunc(uint64_t key, uint64_t depth);

    pm_bucket* getFreeBucket(uint64_t key);
    pm_bucket* getNewBucket();
    void freeEmptyBucket(pm_bucket* bucket);
    kv* getFreeKvSlot(pm_bucket* bucket);

    void splitBucket(uint64_t bucket_id);
    void mergeBucket(uint64_t bucket_id);

    void extendCatalog();
    void* getFreeSlot(pm_address& new_address);
    void allocNewPage();

    void recover();
    void mapAllPage();

public:
    PmEHash();
    ~PmEHash();

    int insert(kv new_kv_pair);
    int remove(uint64_t key);
    int update(kv kv_pair);
    int search(uint64_t key, uint64_t& return_val);

    void selfDestory();
};

#endif
