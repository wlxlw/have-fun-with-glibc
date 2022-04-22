#include<iostream>
using namespace std;
int in_data_segment=1;//保存在data segment
int main(){
    static int in_bss_segment;//保存在data segment中的bss segment
    int* data = new int[5];//data本身保存在栈上，new申请的内存空间保存在堆上
    return 0;
}