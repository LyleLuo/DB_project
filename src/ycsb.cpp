#include "../include/pm_ehash.h"
#include <iostream>
using namespace std;

int main() {

    int x;
    PmEHash* ehash = new PmEHash;
    kv temp;
    for (int i = 0; i < 31; ++i) {
        temp.key = temp.value = i;
        cout << i << ' ' <<  ehash->insert(temp) << endl;
    }
}