#include "gtest/gtest.h"
#include "pm_ehash.h"
#include <iostream>
#include <string>

using namespace std;

TEST(InsertTest, SingleInsert) {
    PmEHash* ehash = new PmEHash;
    kv temp;
    temp.key = temp.value = 1;
    int result = ehash->insert(temp);
    GTEST_ASSERT_EQ(result, 0);
    uint64_t val;
    result = ehash->search(1, val);
    GTEST_ASSERT_EQ(result, 0);
    GTEST_ASSERT_EQ(val, 1);
    result = ehash->search(0, val);
    GTEST_ASSERT_EQ(result, -1);
    ehash->selfDestory();
}

TEST(InsertTest, DuplicateInsert) {
    PmEHash* ehash = new PmEHash;
    kv temp;
    temp.key = temp.value = 1;
    int result = ehash->insert(temp);
    GTEST_ASSERT_EQ(result, 0);
    result = ehash->insert(temp);
    GTEST_ASSERT_EQ(result, -1);
    ehash->selfDestory();
}

TEST(UpdateTest, SingleUpdate) {
    PmEHash* ehash = new PmEHash;
    kv temp;
    temp.key = temp.value = 1;
    int result = ehash->insert(temp);
    GTEST_ASSERT_EQ(result, 0);
    temp.value = 2;
    result = ehash->update(temp);
    GTEST_ASSERT_EQ(result, 0);
    temp.key = 2;
    result = ehash->update(temp);
    GTEST_ASSERT_EQ(result, -1);
    ehash->selfDestory();
}

TEST(SearchTest, SingleSearch) {
    PmEHash* ehash = new PmEHash;
    kv temp;
    temp.key = temp.value = 1;
    int result = ehash->insert(temp);
    uint64_t val = 0;
    result = ehash->search(1, val);
    GTEST_ASSERT_EQ(result, 0);
    GTEST_ASSERT_EQ(val, 1);
    ehash->selfDestory();
}

TEST(RemoveTest, SingleRemove) {
    PmEHash* ehash = new PmEHash;
    kv temp;
    temp.key = temp.value = 1;
    int result = ehash->insert(temp);
    GTEST_ASSERT_EQ(result, 0);
    temp.key = temp.value = 2;
    result = ehash->insert(temp);
    GTEST_ASSERT_EQ(result, 0);
    result = ehash->remove(1);
    GTEST_ASSERT_EQ(result, 0);
    result = ehash->remove(1);
    GTEST_ASSERT_EQ(result, -1);
    uint64_t val = 0;
    result = ehash->search(1, val);
    GTEST_ASSERT_EQ(result, -1);
    GTEST_ASSERT_EQ(val, 0);
    ehash->selfDestory();
}


TEST(InsertTest, MultipleInsert) {
    PmEHash* ehash = new PmEHash;
    int result;
    kv temp;
    uint64_t val;
    
    for(int i = 0; i < 1000; ++i) {
	temp.key = temp.value = i;
        int result = ehash->insert(temp);
        GTEST_ASSERT_EQ(result, 0);
    }
    kv kv;
    kv.value = kv.key = 1;
    result = ehash->insert(kv);
    GTEST_ASSERT_EQ(result, -1);
    
    for(int i = 0; i < 1000; ++i) {
        result = ehash->search(i, val);
        GTEST_ASSERT_EQ(result, 0);
        GTEST_ASSERT_EQ(val, i);
    } 
    result = ehash->search(1000, val);
    GTEST_ASSERT_EQ(result, -1);
	
    ehash->selfDestory();
}


TEST(UpdateTest, MultipleUpdate) {
    PmEHash* ehash = new PmEHash;
    kv temp;
    int result;
    for(int i = 0; i < 1000; ++i) {
    	temp.key = temp.value = i;
    	result = ehash->insert(temp);
    	GTEST_ASSERT_EQ(result, 0);
    }
    for(int i = 1000; i < 2000; ++i) {
	temp.value = i;
	result = ehash->update(temp);
        GTEST_ASSERT_EQ(result, 0);
        temp.key = i;
        result = ehash->update(temp);
	GTEST_ASSERT_EQ(result, -1);
    }
    ehash->selfDestory();
}

TEST(SearchTest, MultipleSearch) {
    PmEHash* ehash = new PmEHash;
    kv temp;
    int result;
    for(int i = 0; i < 1000; ++i) {
    	temp.key = temp.value = i;
	result = ehash->insert(temp);
        uint64_t val = 0;
        result = ehash->search(i, val);
        GTEST_ASSERT_EQ(result, 0);
        GTEST_ASSERT_EQ(val, i);
    }
    ehash->selfDestory();
}

TEST(RemoveTest, MultipleRemove) {
    PmEHash* ehash = new PmEHash;
    kv temp;
    int result;
    for(int i = 0; i < 1000; ++i) {
	temp.key = temp.value = i;
        result = ehash->insert(temp);
        GTEST_ASSERT_EQ(result, 0);
    }
    for(int i = 0; i < 1000; ++i) {
	result = ehash->remove(i);
	GTEST_ASSERT_EQ(result, 0);
	result = ehash->remove(i);
	GTEST_ASSERT_EQ(result, -1);
    }
	
    uint64_t val = 0;
    for(int i = 0; i < 1000; ++i) {
	result = ehash->search(i, val);
	GTEST_ASSERT_EQ(result, -1);
	GTEST_ASSERT_EQ(val, 0);
    }
    ehash->selfDestory();
}
