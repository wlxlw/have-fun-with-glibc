# [C程序的虚拟内存空间布局](https://www.geeksforgeeks.org/memory-layout-of-c-program/)
## 简介
   1. 文本段 text segment：存放程序代码本身，在程序运行期间长度不会发生变化
   2. 数据段 data segment：存放全局变量以及静态变量
      1. BSS段(未初始化数据段)
      2. 初始化数据段
   3. 栈 stack：自动分配的内存空间(由编译器以及操作系统控制)
   4. 堆 heap： 动态分配的内存空间(由程序员控制)
## 结合代码分析 memory_layout_of_c_program.cc
# glibc如何管理内存
## malloc与虚拟内存 
### malloc做了什么
   调用`malloc(size_t)`时，会向系统**提交**“分配长度为size_t的**虚拟内存空间**”的请求，只有在访问到虚拟地址时才会分配物理空间。  
   这种策略被称为**OVERCOMMIT策略**
### [OVER COMMIT策略](http://www.wowotech.net/memory_management/overcommit.html)  
   linux下有三种OVER COMMIT策略  
   ```c++
   #define OVERCOMMIT_GUESS 0 // 内核会自动评估哪部已分配的物理页可以被替换掉，试图申请更多的空间。是ALWAYS和NEVER策略的折中
   #define OVERCOMMIT_ALWAYS 1 // 近乎随意申请，即使申请的空间大于实际的虚拟地址空间(内存+磁盘交换区)
   #define OVERCOMMIT_NEVER 2 // 申请的空间必须小于实际可用的虚拟内存的大小
   ```
### malloc单次最多能申请多长的内存空间 
   1. 受malloc的实现影响：glibc的malloc实现，限制malloc单次最大申请[`PTRDIFF_MAX`](https://sourceware.org/bugzilla/show_bug.cgi?id=23741#c2)字节的虚拟地址空间(在32位系统下为$2^{32}$-1,在64位系统下为$2^{64}$-1)
   2. 受到操作系统采取的OVERCOMMIT策略影响
### malloc总共能申请多长的内存空间
   受到操作系统采取的OVERCOMMIT策略影响
### code-> utilize_malloc.cc 测试并记录了在不同OVERCOMMIT模式下单次malloc能请求的空间  
   ```bash
   # 修改OVERCOMMIT模式的脚本
   sudo vi /etc/sysctl.conf # 修改 vm.overcommit_memory=0 或者 1 或者 2
   sysctl -p
   ```
## The GNU Allocator
### 初识glibc malloc  
   1. 阅读博客[中文版](https://blog.csdn.net/maokelong95/article/details/51989081)，[英文版](https://sploitfun.wordpress.com/2015/02/10/understanding-glibc-malloc/comment-page-1/?blogsub=confirming#subscribe-blog%E3%80%82)
   2. code  
      ```bash
      cd ~/have-fun-with-glibc/glibc_memory_management
      g++ -pthread blog_understanding_glibc_malloc.cc # 多线程编译
      ./a.out
      # output
      # Welcome to per thread arena example::20093
      # Before malloc in main thread
      cat /proc/20093/maps # 每次运行线程id都不同，需要替换
      # output
      # 55668c914000-55668c935000 rw-p 00000000 00:00 0                          [heap]
      回车 # 按下回车
      # output 
      # After malloc and before free in main thread

      # 55668c914000-55668c935000 rw-p 00000000 00:00 0                          [heap]
      ```
### 设计理念  
   1. 减少碎片  
   不限制分配空间(chunk)的大小为2的幂次，free函数可以合并任意两块相邻的空间，从而减少内存空间的碎片化
   1. 支持多线程  
   GNU C Library的malloc实现是将虚拟内存空间划分成多个**arena**，允许多点菜同时地(simultaneously)**在不同的arena**中分配空间
### [文档阅读](https://sourceware.org/glibc/wiki/MallocInternals)
   1. 简述malloc中的3个重要名词 arena->heap->chunk  
      1. arena：每一个arena中包括多个heap，被一个/多个线程共享
      2. heap：每个heap是一段连续的虚拟内存，其中包括多个chunk
      3. chunk：一小段可以被分配、释放、与其他chunk合并的虚拟内存
   2. 
### 源码阅读
   1. 源码下载
   ```bash
   cd ~
   wget http://ftp.gnu.org/gnu/glibc/glibc-2.35.tar.bz2
   tar -jxvf glibc-2.35.tar.bz2
   cd ~/glibc-2.35/malloc
   ```
   1. 源码解析
# references
1. [在malloc上添加PTRDIFF_MAX限制的原因](https://sourceware.org/bugzilla/show_bug.cgi?id=23741#c2)
2. [OVER COMMIT策略](http://www.wowotech.net/memory_management/overcommit.html)