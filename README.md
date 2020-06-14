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
* 罗炜乐

* 马靖成

* 周圆：
  * PmEHash(); 
  * ~PmEHash();
  * int insert(kv new_kv_pair);
  * int remove(uint64_t key);
  * int update(kv kv_pair);
  * int search(uint64_t key, uint64_t& return_val);
  * void selfDestory();
  * 写报告
