# [C程序的虚拟内存空间布局](https://www.geeksforgeeks.org/memory-layout-of-c-program/)
1. 简介
   1. 文本段 text segment：存放程序代码本身，在程序运行期间长度不会发生变化
   2. 数据段 data segment：存放全局变量以及静态变量
      1. BSS段(未初始化数据段)
      2. 初始化数据段
   3. 栈 stack：自动分配的内存空间(由编译器以及操作系统控制)
   4. 堆 heap： 动态分配的内存空间(由程序员控制)
2. 结合代码分析
# glibc如何管理内存
1. 从调用malloc开始 code-> utilize_malloc.cc  
   ```bash
   g++ -m32 utilize_malloc.cc # 按32位寻址空间编译编译
   ./a.out
   ```
2. malloc做了什么  
   调用`malloc(size_t)`时，会向系统**提交**“分配长度为size_t的**虚拟内存空间**”的请求，只有在访问到虚拟地址时才会分配物理空间。  
   这种策略被称为**OVERCOMMIT策略**
3. [OVER COMMIT策略](http://www.wowotech.net/memory_management/overcommit.html)  
   linux下有三种OVER COMMIT策略  
   ```c++
   #define OVERCOMMIT_GUESS 0 // 内核会自动评估哪部已分配的物理页可以被替换掉，试图申请更多的空间。是ALWAYS和NEVER策略的折中
   #define OVERCOMMIT_ALWAYS 1 // 近乎随意申请，即使申请的空间大于实际的虚拟地址空间(内存+磁盘交换区)
   #define OVERCOMMIT_NEVER 2 // 申请的空间必须小于实际可用的虚拟内存的大小
   ```
4. malloc单次最多能申请多长的内存空间 
   1. 受malloc的实现影响：glibc的malloc实现，限制malloc单次最大申请[`PTRDIFF_MAX`](https://sourceware.org/bugzilla/show_bug.cgi?id=23741#c2)字节的虚拟地址空间(在32位系统下为$2^{32}$-1,在64位系统下为$2^{64}$-1)
   2. 受到操作系统采取的OVERCOMMIT策略影响
5. malloc总共能申请多长的内存空间
   受到操作系统采取的OVERCOMMIT策略影响
6. code-> utilize_malloc.cc 测试并记录了在不同OVERCOMMIT模式下单次malloc能请求的空间

# references
1. [在malloc上添加PTRDIFF_MAX限制的原因](https://sourceware.org/bugzilla/show_bug.cgi?id=23741#c2)
2. [OVER COMMIT策略](http://www.wowotech.net/memory_management/overcommit.html)