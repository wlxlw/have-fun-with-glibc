/* Per thread arena example. */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include<string.h>

void* threadFunc(void* arg) {
    printf("Before malloc in thread 1 %ld\n",pthread_self());
    getchar();
    char* addr = (char*) malloc(1000);
    printf("After malloc and before free in thread 1\n");
    getchar();
    free(addr);
    printf("After free in thread 1\n");
    getchar();
}
int main() {
    pthread_t t1;
    void* s;
    int ret;
    char* addr;
    printf("Welcome to per thread arena example::%d %ld\n",getpid(),pthread_self());
    printf("Before malloc in main thread\n");
    getchar();
    addr = (char*) malloc(1024*150); //调用mmap
    printf("addr:%p-%p\n",addr,addr+1024*150);
    addr = (char*) malloc(1024*100);    //在heap上申请空间
    printf("addr:%p-%p\n",addr,addr+1024*100);
    addr = (char*) malloc(1024*100);    //在heap上申请空间，调用brk,heap扩容
    printf("addr:%p-%p\n",addr,addr+1024*100);
    // 7f9f5f587000-7f9f5f5b0000
    // 7f9f5f587010-7f9f5f5ac810
    printf("After malloc and before free in main thread\n");
    getchar();
    free(addr);
    printf("After free in main thread\n");
    getchar();
    ret = pthread_create(&t1, NULL, threadFunc, NULL);
    if(ret)
    {
            printf("Thread creation error\n");
            return -1;
    }
    ret = pthread_join(t1, &s);
    if(ret)
    {
            printf("Thread join error\n");
            return -1;
    }
    return 0;
}