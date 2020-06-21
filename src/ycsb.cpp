#include "../include/pm_ehash.h"
#include <iostream>
using namespace std;

int main() {

    int x;
    PmEHash* ehash = new PmEHash;
    kv temp;
    for (int i = 110000; i < 122200; ++i) {
        temp.key = i;
        if(ehash->search(temp.key, temp.value) == -1) {
            for (int j = 0; j < 500; ++j) {
                cout << j << endl;
            }
        }
        else {
            cout << i << ' ' <<  temp.value << endl;
        }
    }
}