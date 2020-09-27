# 中山大学2020春《数据库系统》课程设计
The persistent storage of Extendible hash  
使用NVM实现可扩展哈希

使用方法：
* 在include同级目录创建data文件夹。
```sh
mkdir data
```
* 修改include/pm_ehash_struct.h里PM_EHASH_DIRECTORY，文件夹名字是data，将其改为与include在同一级目录（比如include在/mnt/pmem0/DB_project/include/就要设为/mnt/pmem0/DB_project/data/）
* 直接进入test文件夹（测试gtest）或者src文件夹（测试ycsb）目录make即可，然后再进入bin文件夹执行二进制文件。

## 成员
* 罗炜乐(LyleLuo)
* 马靖成(437574680)
* 周圆(Annecle)

## task1
* 所有人都要配一遍环境
* 罗炜乐写报告&测试PMDK
* 周圆、马靖成使用FIO测试

## task2

### DB_project分支：
Master，默认分支，完成项目基本要求和主要功能；
Test，测试分支，完成自行构造的测试；
Optimization，优化分支，所有优化项目的实现。

### 成员分工：
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
	* 优化实现
	

* 周圆：
	* 函数实现
		* int insert(kv new_kv_pair);
		* int remove(uint64_t key);
		* int update(kv kv_pair);
		* int search(uint64_t key, uint64_t& return_val);
	* 写报告
	* 构建测试
	* 优化实现
