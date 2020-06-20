# DB_project
The persistent storage of Extendible hash

## 成员
* 罗炜乐
* 马靖成
* 周圆

## task1
* 所有人都要配一遍环境
* 罗炜乐写报告&测试PMDK
* 周圆、马靖成使用FIO测试

## task2
* 罗炜乐：
	* data_page
	* void allocNewPage();
	* pm_bucket* getNewBucket();
	* void freeEmptyBucket(pm_bucket* bucket);
	* void* getFreeSlot(pm_address& new_address);
	* kv* getFreeKvSlot(pm_bucket* bucket);
	* void extendCatalog();
	* void recover();
	* void mapAllPage();
	* void selfDestory();

* 马靖成:
	* ycsb.cpp
	* uint64_t hashFunc(uint64_t key);
	* void splitBucket(uint64_t bucket_id);
	* void mergeBucket(uint64_t bucket_id);
	* pm_bucket* getFreeBucket(uint64_t key);

* 周圆：
  * PmEHash(); 
  * ~PmEHash();
  * int insert(kv new_kv_pair);
  * int remove(uint64_t key);
  * int update(kv kv_pair);
  * int search(uint64_t key, uint64_t& return_val);
  * 写报告
