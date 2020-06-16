#include"pm_ehash.h"

	 
/**
 * @description: construct a new instance of PmEHash in a default directory
 * @param NULL
 * @return: new instance of PmEHash
 */
PmEHash::PmEHash() {

}
/**
 * @description: persist and munmap all data in NVM
 * @param NULL 
 * @return: NULL
 */
PmEHash::~PmEHash() {

}

/**
 * @description: 插入新的键值对，并将相应位置上的位图置1
 * @param kv: 插入的键值对
 * @return: 0 = insert successfully, -1 = fail to insert(target data with same key exist)
 */
int PmEHash::insert(kv new_kv_pair) {
    return 1;
}

/**
 * @description: 删除具有目标键的键值对数据，不直接将数据置0，而是将相应位图置0即可
 * @param uint64_t: 要删除的目标键值对的键
 * @return: 0 = removing successfully, -1 = fail to remove(target data doesn't exist)
 */
int PmEHash::remove(uint64_t key) {
    return 1;
}
/**
 * @description: 更新现存的键值对的值
 * @param kv: 更新的键值对，有原键和新值
 * @return: 0 = update successfully, -1 = fail to update(target data doesn't exist)
 */
int PmEHash::update(kv kv_pair) {
    return 1;
}
/**
 * @description: 查找目标键值对数据，将返回值放在参数里的引用类型进行返回
 * @param uint64_t: 查询的目标键
 * @param uint64_t&: 查询成功后返回的目标值
 * @return: 0 = search successfully, -1 = fail to search(target data doesn't exist) 
 */
int PmEHash::search(uint64_t key, uint64_t& return_val) {
    return 1;
}

/**
 * @description: 用于对输入的键产生哈希值，然后取模求桶号(自己挑选合适的哈希函数处理)
 * @param uint64_t: 输入的键
 * @return: 返回键所属的桶号
 */
uint64_t PmEHash::hashFunc(uint64_t key) {
	return key % (1 << metadata->global_depth);
}

/**
 * @description: 获得供插入的空闲的桶，无空闲桶则先分裂桶然后再返回空闲的桶
 * @param uint64_t: 带插入的键
 * @return: 空闲桶的虚拟地址
 */
pm_bucket* PmEHash::getFreeBucket(uint64_t key) {
	uint64_t bucket_id = hashFunc(key);//求得桶号
	uint8_t* bitmap1 = catalog.buckets_virtual_address[bucket_id]->bitmap;
	bool bit[16];
	for(int i = 0; i < 16; ++i){
	bit[i] = getBitFromBitmap(bitmap1, i);
	}
	for(int i = 0; i < 15; ++i){
		if(!bit[i]){
			return catalog.buckets_virtual_address[bucket_id];//查找15个空位中是否有空位，有则返回	
		}
		if(i == 14 && bit[i]){//没有则分裂桶，再返回空闲桶
			splitBucket(bucket_id);
			return getFreeBucket(key);
		}
	}	  
}

pm_bucket* PmEHash::getNewBucket() {
    if (free_list.empty()) {
        allocNewPage();
    }
    pm_bucket* new_bucket = free_list.front();
    free_list.pop();

    //set bitmap
    pm_address temp = vAddr2pmAddr[new_bucket];
    uint32_t pos = temp.offset / sizeof(pm_bucket);
    temp.offset = 0;
    data_page* page_virtual_address = reinterpret_cast<data_page*>(pmAddr2vAddr[temp]);
    setBitToBitmap(page_virtual_address->bitmap, pos, true);

    return new_bucket;
}

void PmEHash::freeEmptyBucket(pm_bucket* bucket) {
    free_list.push(bucket);

    //set bitmap
    pm_address temp = vAddr2pmAddr[bucket];
    uint32_t pos = temp.offset / sizeof(pm_bucket);
    temp.offset = 0;
    data_page* page_virtual_address = reinterpret_cast<data_page*>(pmAddr2vAddr[temp]);
    setBitToBitmap(page_virtual_address->bitmap, pos, false);
}


/**
 * @description: 获得空闲桶内第一个空闲的位置供键值对插入
 * @param pm_bucket* bucket
 * @return: 空闲键值对位置的虚拟地址
 */
kv* PmEHash::getFreeKvSlot(pm_bucket* bucket) {
	kv* result;
	for (int i = 0; i < 15; ++i) {
		if (!getBitFromBitmap(bucket->bitmap, i)) {
			result = bucket->slot + i;
			setBitToBitmap(bucket->bitmap, i, true);
			break;
		}
	}
	return result;
}

/**
 * @description: 桶满后进行分裂操作，可能触发目录的倍增
 * @param uint64_t: 目标桶在目录中的序号
 * @return: NULL
 */
void PmEHash::splitBucket(uint64_t bucket_id) {
	uint64_t local_depth1 = catalog.buckets_virtual_address[bucket_id]->local_depth;//先得到本地深度
	if(local_depth1 == metadata->global_depth){  //如果本地深度等于全局深度，需要倍增列表
		extendCatalog();//此时全局深度和catalog_size都已经发生变化
		for(int i = 0; i < metadata->catalog_size / 2; ++i){
			catalog.buckets_pm_address[i + (1 << metadata->global_depth-1)] = catalog.buckets_pm_address[i];
			catalog.buckets_virtual_address[i + (1 << metadata->global_depth-1)] = catalog.buckets_virtual_address[i];
			pmem_persist(catalog.buckets_pm_address[i + (1 << metadata->global_depth - 1)], map_len);
			pmem_persist(catalog.buckets_virtual_address[i + (1 << metadata->global_depth - 1)], map_len);//持久化
			}
	}
	pm_bucket* new_bucket = reinterpret_cast<pm_bucket*>(getFreeSlot(catalog.buckets_pm_address[bucket_id + (1 << local_depth1)])); //分裂出的新桶
	new_bucket->local_depth = local_depth1+1; //分裂后两个桶的本地深度都+1； 
	catalog.buckets_virtual_address[bucket_id]->local_depth += 1;
	local_depth1++;
	uint8_t* bitmap_old = catalog.buckets_virtual_address[bucket_id]->bitmap;
	uint8_t* bitmap_new = new_bucket->bitmap;
	bool bit_old[16];
	bool bit_new[16];
	for(int i = 0; i < 16; ++i){
		bit_old[i] = getBitFromBitmap(bitmap_old, i);//得到位图
		bit_new[i] = getBitFromBitmap(bitmap_new, i);
	}
	int count = 0;//存入分裂的空桶中的计数器
	for(int i = 0; i < 15; ++i){
		if(bit_old[i]){
			if(catalog.pm_bucket[bucket_id]->kv[i].key % (1<<local_depth1)==(bucket_id+(1<<local_depth1-1))){//将旧桶中需要移动的移到新桶
				bit_old[i] = 0;	
				bit_new[count] = 1;
				new_bucket->slot[count] = catalog.buckets_virtual_address[bucket_id]->slot[i];
				count++; 
			}
		}					
	}
	for(int i=0;i<16;++i){
		setBitToBitmap(catalog.buckets_virtual_address[bucket_id]->bitmap, i, bit_old[i]);//位图恢复
		setBitToBitmap(new_bucket->bitmap, i, bit_new[i]);
	}
	catalog.buckets_virtual_address[bucket_id+(1<<local_depth1-1)]=new_bucket;
	pmem_persist(catalog.buckets_virtual_address[bucket_id], map_len);
	pmem_persist(catalog.buckets_virtual_address[bucket_id]->slot, map_len);
	pmem_persist(catalog.buckets_virtual_address[bucket_id+(1<<local_depth1-1)], map_len);
	pmem_persist(catalog.buckets_virtual_address[bucket_id+(1<<local_depth1-1)]->slot, map_len);//持久化
	for(int i = 0; i<metadata->catalog_size; ++i){
		if(hashFunc(i,local_depth1)==(bucket_id + (1<<local_depth1-1))){//将所有应该指向新桶的指针都指向新桶
			catalog.buckets_virtual_address[i]=catalog.buckets_virtual_address[bucket_id + (1 << local_depth1-1)];
			catalog.buckets_pm_address[i]=catalog.buckets_pm_address[bucket_id + (1 << local_depth1-1)];
			pmem_persist(catalog.buckets_pm_address[i], map_len);
			pmem_persist(catalog.buckets_virtual_address[i], map_len);
			} 
	}	
}

/**
 * @description: 桶空后，回收桶的空间，并设置相应目录项指针
 * @param uint64_t: 桶号
 * @return: NULL
 */
void PmEHash::mergeBucket(uint64_t bucket_id) {
    uint64_t local_depth1=catalog.buckets_virtual_address[bucket_id]->local_depth;
    if(local_depth1>4){//当桶的深度<=4时就不合并 
    	if(bucket_id >= (1<<local_depth1-1)){//找兄弟桶
    		uint64_t brother_id=bucket_id-(1<<local_depth1-1);
    	}
    	else{
    		uint64_t brother_id=bucket_id+(1<<local_depth1-1);
		}
    	if(catalog.buckets_virtual_address[brother_id]->local_depth==local_depth1){//如果兄弟桶深度相同就可以合并
    		catalog.buckets_pm_address[bucket_id]=catalog.buckets_pm_address[brother_id];
    		catalog.buckets_virtual_address[brother_id]->local_depth -= 1;
    		pmem_persist(catalog.buckets_virtual_address[brother_id]->local_depth, map_len);
    		pmem_persist(catalog.buckets_pm_address[bucket_id], map_len);
    		pm_bucket* temp = catalog.buckets_virtual_address[bucket_id];
    		catalog.buckets_virtual_address[bucket_id]=catalog.buckets_virtual_address[brother_id];
    		pmem_persist(catalog.buckets_virtual_address[bucket_id], map_len);
    		freeEmptyBucket(temp);
		}
	}	
}


/**
 * @description: 对目录进行倍增，需要重新生成新的目录文件并复制旧值，然后删除旧的目录文件
 * @param NULL
 * @return: NULL
 */
void PmEHash::extendCatalog() {
    //copy pm_address to mem
    pm_address * temp_buckets_pm_address = new pm_address[metadata->catalog_size];
    memcpy(temp_buckets_pm_address, catalog.buckets_pm_address, sizeof(pm_address) * metadata->catalog_size);
    pmem_unmap(catalog.buckets_pm_address, sizeof(pm_address) * metadata->catalog_size);

    //re map
    size_t map_len;
    catalog.buckets_pm_address = \
        reinterpret_cast<pm_address*>(pmem_map_file("../data/pm_ehash_catalog", \
        sizeof(pm_address) * metadata->catalog_size * 2, PMEM_FILE_CREATE, 0777, &map_len, NULL));

    //copy origin pm_address to new pm_address
    memcpy(catalog.buckets_pm_address, temp_buckets_pm_address, sizeof(pm_address) * metadata->catalog_size);
    delete[] temp_buckets_pm_address;
    pmem_persist(catalog.buckets_pm_address, metadata->catalog_size); 

	pm_bucket** temp_buckets_virtual_address = new pm_bucket*[metadata->catalog_size * 2];
	for (int i = 0; i < metadata->catalog_size; ++i) {
		temp_buckets_virtual_address[i] = catalog.buckets_virtual_address[i];
	}

	delete[] catalog.buckets_virtual_address;
	catalog.buckets_virtual_address = temp_buckets_virtual_address;

	metadata->catalog_size *= 2;  
	metadata->global_depth++;
}

/**
 * @description: 获得一个可用的数据页的新槽位供哈希桶使用，如果没有则先申请新的数据页
 * @param pm_address&: 新槽位的持久化文件地址，作为引用参数返回
 * @return: 新槽位的虚拟地址
 */
void* PmEHash::getFreeSlot(pm_address& new_address) {
    pm_bucket* result = getNewBucket();
    new_address = vAddr2pmAddr[result];
    return result;
}

/**
 * @description: 申请新的数据页文件，并把所有新产生的空闲槽的地址放入free_list等数据结构中
 * @param NULL
 * @return: NULL
 */
void PmEHash::allocNewPage() {
    metadata->max_file_id++;
    data_page * p = reinterpret_cast<data_page*>(createNewPage(metadata->max_file_id));
	page_list.push_back(p);
    pm_address temp = {metadata->max_file_id, 0};
    for (int i = 0; i < 16; ++i) {
        temp.offset = sizeof(pm_bucket) * i;
        free_list.push(p->slot + i);
        vAddr2pmAddr[p->slot + i] = temp;
        pmAddr2vAddr[temp] = p->slot + i;
    }
}

/**
 * @description: 读取旧数据文件重新载入哈希，恢复哈希关闭前的状态
 * @param NULL
 * @return: NULL
 */
void PmEHash::recover() {
	size_t map_len;
	metadata = reinterpret_cast<ehash_metadata*>(pmem_map_file("../data/pm_ehash_metadata", sizeof(ehash_metadata), PMEM_FILE_CREATE, 0777, &map_len, NULL));
	catalog.buckets_pm_address = reinterpret_cast<pm_address*>(pmem_map_file("../data/pm_ehash_catalog", \
        sizeof(pm_address) * metadata->catalog_size, PMEM_FILE_CREATE, 0777, &map_len, NULL));
	catalog.buckets_virtual_address = new pm_bucket*[metadata->catalog_size];
  
}

/**
 * @description: 重启时，将所有数据页进行内存映射，设置地址间的映射关系，空闲的和使用的槽位都需要设置 
 * @param NULL
 * @return: NULL
 */
void PmEHash::mapAllPage() {

}

/**
 * @description: 删除PmEHash对象所有数据页，目录和元数据文件，主要供gtest使用。即清空所有可扩展哈希的文件数据，不止是内存上的
 * @param NULL
 * @return: NULL
 */
void PmEHash::selfDestory() {

}
