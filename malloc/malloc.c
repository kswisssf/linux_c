#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <sys/mman.h>


/*内存控制篇*/

/*
calloc（配置内存空间）
相关函数
	malloc，free，realloc，brk
表头文件
	#include <stdlib.h>
定义函数
	void *calloc(size_t nmemb，size_t size);
函数说明
	calloc()用来配置nmemb个相邻的内存单位，每一单位的大小为size，并返回指向第一个元素的指针。
	这和使用下列的方式效果相同:malloc(nmemb*size);
	不过，在利用calloc()配置内存时会将内存内容初始化为0。
返回值
	若配置成功则返回一指针，失败则返回NULL。
*/
/* 动态配置10个struct test 空间*/

struct test
{
	int a[10];
	char b[20];
};

void calloc_demo()
{
	struct test *ptr=calloc(sizeof(struct test),10);
}
/*
free（释放原先配置的内存)

定义函数
	void free(void *ptr);
函数说明
	参数ptr为指向先前由malloc()、calloc()或realloc()所返回的内存指针。调用free()后ptr所指的内存空间
	便会被收回。假若参数ptr所指的内存空间已被收回或是未知的内存地址，则调用free()可能会有无法预期的情况发生。
	若参数ptr为NULL，则free()不会有任何作用。
*/

/*
getpagesize（取得内存分页大小）
相关函数
	sbrk
表头文件
	#include<unistd.h>
定义函数
	size_t getpagesize(void);
函数说明
	返回一分页的大小，单位为字节（byte）。此为系统的分页大小，不一定会和硬件分页大小相同。
返回值
	内存分页大小。附加说明在Intel x86 上其返回值应为4096bytes。
*/

void getpagesize_demo()
{
	printf("pagesize=%d\n",getpagesize());
}

/*
malloc（配置内存空间）
相关函数
	calloc，free，realloc，brk
表头文件
	#include<stdlib.h>
定义函数
	void * malloc(size_t size);
函数说明
	malloc()用来配置内存空间，其大小由指定的size决定。
返回值
	若配置成功则返回一指针，失败则返回NULL。
范例
*/
// C语言初始化一个全局变量或static变量时，只能用常量赋值，不能用变量赋值！
void malloc_demo()
{
	char *pp = (char *)malloc(1024*sizeof(char)); /*配置1k的内存*/
}
/*
mmap（建立内存映射）
相关函数
	munmap，open
表头文件
	#include <unistd.h>
	#include <sys/mman.h>
定义函数
	void *mmap(void *start,size_t length,int prot,int flags,int fd,off_t offsize);
函数说明
	mmap()用来将某个文件内容映射到内存中，对该内存区域的存取即是直接对该文件内容的读写。
	
	参数start指向欲对应的内存起始地址，通常设为NULL，代表让系统自动选定地址，对应成功后该地址会返回。
	
	参数length代表将文件中多大的部分对应到内存。
	
	参数prot代表映射区域的保护方式有下列组合:
	PROT_EXEC 映射区域可被执行
	PROT_READ 映射区域可被读取
	PROT_WRITE 映射区域可被写入
	PROT_NONE 映射区域不能存取
	
	参数flags会影响映射区域的各种特性:
	MAP_FIXED 如果参数start所指的地址无法成功建立映射时，则放弃映射，不对地址做修正。通常不鼓励用此旗标。
	MAP_SHARED对映射区域的写入数据会复制回文件内，而且允许其他映射该文件的进程共享。
	MAP_PRIVATE 对映射区域的写入操作会产生一个映射文件的复制，即私人的“写入时复制”（copy on write）
	对此区域作的任何修改都不会写回原来的文件内容。
	
	MAP_ANONYMOUS建立匿名映射。此时会忽略参数fd，不涉及文件，而且映射区域无法和其他进程共享。
	MAP_DENYWRITE只允许对映射区域的写入操作，其他对文件直接写入的操作将会被拒绝。
	MAP_LOCKED 将映射区域锁定住，这表示该区域不会被置换（swap）。
	
	在调用mmap()时必须要指定MAP_SHARED 或MAP_PRIVATE。参数fd为open()返回的文件描述词，代表欲映射到内存
	的文件。参数offset为文件映射的偏移量，通常设置为0，代表从文件最前方开始对应，offset必须是分页大小的整数倍。
返回值
	若映射成功则返回映射区的内存起始地址，否则返回MAP_FAILED(－1)，错误原因存于errno 中。
错误代码
	EBADF 参数fd 不是有效的文件描述词
	EACCES 存取权限有误。如果是MAP_PRIVATE 情况下文件必须可读，使用MAP_SHARED则要有PROT_WRITE以及该文件
	要能写入。
	EINVAL 参数start、length 或offset有一个不合法。
	EAGAIN 文件被锁住，或是有太多内存被锁住。
	ENOMEM 内存不足。
*/
/* 利用mmap()来读取/etc/passwd 文件内容*/

void mmap_test()
{
	int fd;
	void *start;
	struct stat sb;
	
	fd = open("/etc/passwd",O_RDONLY); /*打开/etc/passwd*/
	fstat(fd, &sb); /*取得文件大小*/
	start = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	
	if(start == MAP_FAILED) /*判断是否映射成功*/
		return;

	printf("%s",start);
	
	munmap(start, sb.st_size); /*解除映射*/

	close(fd);
}

/*
munmap（解除内存映射）
相关函数
	mmap
表头文件
	#include<unistd.h>
	#include<sys/mman.h>
定义函数
	int munmap(void *start,size_t length);
函数说明
	munmap()用来取消参数start所指的映射内存起始地址，参数length则是欲取消的内存大小。
	当进程结束或利用exec相关函数来执行其他程序时，映射内存会自动解除，但关闭对应的文件描述词时不会解除映射。
返回值
	如果解除映射成功则返回0，否则返回－1，错误原因存于errno中错误代码EINVAL
参数
	start或length 不合法。
范例
	参考mmap（）
*/

/*
 int msync ( void * addr , size_t len, int flags) 
 一般说来，进程在映射空间的对共享内容的改变并不直接写回到磁盘文件中，往往在调用munmap（）后才执行该操作。
 可以通过调用msync()实现磁盘上文件内容与共享内存区的内容一致。
*/

#define   SIZE	100
#define		DIVI		printf("------------------\n");
typedef	struct __tag_store{

	char name[SIZE];
	int  num;
}PRODUCT;

int main(int argc, char *argv[], char *env[])
{
	

	PRODUCT *p = (PRODUCT *)malloc(sizeof(PRODUCT));
	if(NULL  == p)
	{
		printf("malloc failed \n");
		return -1;
	}

	int *p1 = (int *)calloc(5, sizeof(int));
	if(NULL == p1)
	{
		printf("calloc failed\n");
		return -2;
	}

	int i = 0;
	for(i = 0; i< 5; i++)
	{
		*(p1+i) = i+10;
		printf("i:%d--%d\n", i, *(p1+i));
	}

	printf("sizeof PRODUCT:%d\n", sizeof(PRODUCT));
	printf("sizeof int:%d \n",sizeof(int));
	DIVI
	p1 = (int *)realloc(p1, 10);
	for(i = 0; i< 10; i++)
	{
		printf("i:%d--%d\n", i, *(p1+i));
	}

	DIVI
	strcpy(&((*p).name), "HELLO");
	(*p).num		= 10;
	
	printf("name:%s, num:%d\n", (*p).name,	(*p).num);
	printf("name addr:%p, num addr:%p\n", (*p).name, &((*p).num));
	
	DIVI
	strcpy(&(p->name), "hello");
	p->num			= 12;
	printf("name:%s, num:%d\n", p->name, p->num);
	printf("name addr:%p, name adddr:0x%p,  num addr:%p, num addr value: 0x%x\n", p->name, &(p->name), &(p->num), &(p->num));

	DIVI
	printf("p val:0x%x, %p, *p val:0x%x\n", p, p, *p);

	free(p);
	free(p1);
	return 0;
}