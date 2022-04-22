#include<iostream>
#include<string.h>
size_t MAX=1024;
size_t GB=MAX*MAX*MAX;
size_t TB=GB*MAX;

// :~/have-fun-with-glibc/malloc$ free -h
//               total        used        free      shared  buff/cache   available
// Mem:           24Gi       2.3Gi        22Gi       0.0Ki       150Mi        22Gi
// Swap:         4.0Gi       1.9Gi       2.1Gi


void test(char*p,size_t len){
    if(p){
        std::cout<<"Virtual memory commit successfully"<<std::endl;
        memset(p,0,len);
        std::cout<<"Physical memory allocated successfully"<<std::endl;
    }else{
        std::cout<<"Failed to commit virtual memory"<<std::endl;
    }
}
void TEST(size_t len){
    char*p = (char*) malloc(len);
    test(p,len);
}


int main(){
    size_t test_always_len = 70*TB;
    size_t test_guess_len = 16*GB;
    size_t test_never_len = 16*GB;

    //# 设置成OVERCOMMIT_ALWAYS模式
    TEST(test_always_len);
    //Virtual memory commit successfully 表示在OVERCOMMIT_ALWAYS模式下，申请70TB的虚拟地址空间也能成功
    //Killed    表示在OVERCOMMIT_ALWAYS模式下，分配70TB的物理地址空间失败，因为远大于实际的虚拟内存的大小

    //# 设置成OVERCOMMIT_GUESS模式
    // TEST(test_guess_len);
    // Virtual memory commit successfully
    // Physical memory allocated successfully

    //# 设置成OVERCOMMIT_NEVER模式
    // TEST(test_never_len);
    //Failed to commit virtual memory
    return 0;
}
