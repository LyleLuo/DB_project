#include "../include/pm_ehash.h"
#include<iostream>
#include<fstream>
#include<string>
#include<ctime>
using namespace std;

int main() {
    clock_t start1,end1,start2,end2;
    PmEHash* ehash = new PmEHash;
    kv temp;
    string a;
    string b;
    uint64_t ke = 0;
    ifstream in;
    in.open("../../workloads/10w-rw-50-50-load.txt");
    ifstream in2;
    double count_total = 0;
    double count_insert = 0;
    double count_update = 0;
    double count_remove = 0;
    start1 = clock();
    while(!in.eof()){
        in >> a;
        in >> b;
        ++count_total;
        for(int i = 0; i < 8; ++i){
            ke += b[i] - '0';
            ke *= 10;
        }
        if(a == "INSERT"){
            temp.key = temp.value = ke;
            ehash->insert(temp);
            ++count_insert;             
        }
        if(a == "UPDATE"){
            temp.key = temp.value = ke;
            ehash->update(temp);
            ++count_update;
        }
        if(a == "READ"){
            ehash->remove(ke);
            ++count_remove;
        }
        ke = 0;
    }
    end1 = clock();
    cout<<"load time is: "<<(double)(end1-start1)/CLOCKS_PER_SEC<<"s"<<endl;
    in.close();
    in2.open("../../workloads/10w-rw-50-50-run.txt");
    start2 = clock();
    while(!in2.eof()){
        in2 >> a;
        in2 >> b;
        ++count_total;
        for(int i = 0; i < 8; ++i){
            ke += b[i] - '0';
            ke *= 10;
        }
        if(a == "INSERT"){
            temp.key = temp.value = ke;
            ehash->insert(temp);
            ++count_insert;
        }
        if(a == "UPDATE"){
            temp.key = temp.value = ke;
            ehash->update(temp);
            ++count_update;
        }
        if(a == "READ"){
            ehash->remove(ke);
             ++count_remove;
        }
        ke = 0;
    }
    end2 = clock();
    cout<<"run time is: "<<(double)(end2-start2)/CLOCKS_PER_SEC<<"s"<<endl;
    in2.close();
    cout<<"total operation: "<<count_total<<endl;
    cout<<"insert%: "<<count_insert / count_total<<endl;
    cout<<"update%: "<<count_update / count_total<<endl;
    cout<<"remove%: "<<count_remove / count_total<<endl;
    cout<<"OPS: "<<count_total/((double)(end2+end1-start1-start2)/CLOCKS_PER_SEC)<<endl;
}
