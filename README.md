# DB_project
The persistent storage of Extendible hash

## 成员
* 罗炜乐(LyleLuo)
* 马靖成(437574680)
* 周圆(Annecle)

## task1
* 所有人都要配一遍环境
* 罗炜乐写报告&测试PMDK
* 周圆、马靖成使用FIO测试

## task2
* 罗炜乐：
	* 函数实现
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
		* PmEHash(); 
		* ~PmEHash();
	* 构建测试
	* debug & fix bug
	* 优化思路：
		* 使用位域代替原有的位图，以减少原本获得位或者置位函数的重复调用
		* ~~目测~~创建映射所花费的时间比较多，对大规模插入不太友好，尝试增加页的大小以较少创建新页次数
		* 使用编译器优化
* 马靖成:
	* 函数实现
		* 编写和测试ycsb.cpp
		* uint64_t hashFunc(uint64_t key);
		* void splitBucket(uint64_t bucket_id);
		* void mergeBucket(uint64_t bucket_id);
		* pm_bucket* getFreeBucket(uint64_t key);
	* 修了mergeBucket的bug  

* 周圆：
	* 函数实现
		* int insert(kv new_kv_pair);
		* int remove(uint64_t key);
		* int update(kv kv_pair);
		* int search(uint64_t key, uint64_t& return_val);
	* 写报告
	* 构建测试
