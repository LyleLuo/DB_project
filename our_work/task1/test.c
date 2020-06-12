#include <libpmem.h>
#include <stdint.h>
#include <stdio.h>

int main() {
    size_t map_len;
    int* data = pmem_map_file("test_array", sizeof(int) * 10, PMEM_FILE_CREATE, 0777, &map_len, NULL);
    if (pmem_is_pmem(data, map_len)) {
        for (int i = 0; i < 10; ++i) {
            data[i] = i;
        }
        pmem_persist(data, map_len);
        pmem_unmap(data, map_len);

        data = pmem_map_file("test_array", sizeof(int) * 10, PMEM_FILE_CREATE, 0777, &map_len, NULL);
        printf("This is PMEM!!!\nTest array:");
        for (int i = 0; i < 10; ++i) {
            printf(" %d", data[i]);
        }
        printf("\n");
    }
    else {
        printf("This is not PMEM!\n");
    }
}
    
