#include <stdio.h>
#include <stdlib.h>
/*
函数指针：指向函数的指针变量。 
	因此“函数指针”本身首先应是指针变量，只不过该指针变量指向函数。这正如用指针变量
	可指向整型变量、字符型、数组一样，这里是指向函数。
	C在编译时，每一个函数都有一个入口地址，该入口地址就是函数指针所指向的地址。
	有了指向函数的指针变量后，可用该指针变量调用函数，就如同用指针变量可引用其他类型变量一样，
	在这些概念上是大体一致的。

函数指针有两个用途：
	调用函数和做函数的参数。
*/

/*
第一种用法示例:
	首先定义函数指针pfun,这个函数指针的返回值为void型,然后给函数指针赋值，赋值为myfun,
	也就是myfun函数的首地址，在C99中myfun函数名就是myfun函数的首地址，此时pfun获得了
	myfun的地址，pfun的地址等于myfun的地址，所以最终调用pfun();也就相当于调用了myfun();
*/
void (*pfun1)(int data);
void myfun1(int data)
{
	printf("get data:%d\n",data);
}
int func_piont1(int argc,char *argv[])
{
	pfun1 = myfun1;
	(*pfun1)(100);
	return 0;
}
/*
第二种用法示例：typedef 原变量类型 别名
	也可以用typedef来定义一个指针函数这样使在大型代码中更加简洁.
	第一句就是typedef定义了一种pfun的类型，并定义这种类型为指向某种函数的指针，
	这种函数以一个int为参数并返回void类型。
	这里面的pfun代表的是函数的类型，通过pfun来代表void (*)(int)函数类型
	即pfun是指针函数的别名，pfun p相当于定义了一个void (*p)(int)函数指针。
	p = myfun可以理解为将函数指针p指向myfun函数的地址，p(100);相当于执行myfun(100);
*/
typedef void (*pfun2)(int data);
void myfun2(int data)
{
	printf("get data:%d\n",data);
}
int func_piont2(int argc,char *argv[])
{
	pfun2 p= myfun2;      //函数指针指向执行函数的地址
	p(100);				 // p = myfun2-->p() = myfun2()
	return 0;
}

/*
第三种用法示例：用结构体函数指针
*/
typedef struct gfun{
	void (*pfun3)(int);	
}gfun;

void myfun3(int data)
{
	printf("get data:%d\n",data);
}
int func_piont3(int argc,char *argv[])
{
	gfun gcode={
		.pfun3 = myfun3,   //将函数指针指向要调用函数的地址
	};

	gcode.pfun3(100);

	return 0;
} 
/*******************************************************************/

/*
demo0 desc:
直接使用函数指针调用函数 
*/
int *add(int a, int b)
{
	int *p = (int *)malloc(sizeof(int));
	printf("p memory address: %p \n", p);
	int c = a + b;
	p = &c;
	printf("p value is %d int the add\n", *p);
	//因为malloc没有释放，所以可以返回局部变量的指针，如果释放就会出现段错误.
	return p;
}

int (*sub_fp)(int, int );
int sub(int a, int b){
	return a-b;
}

//#号运算符，用于在预编译期将宏参数转换为字符串
#define CONVER(x)		#x

int func_addr_demo1()
{
	printf("\n\n%s run ...\n", __func__);

	int *fp = add(5, 9);
	printf("fp value is :%d\n", *fp);
	if(add == &add)
		printf("add address : %d , &add val: %d, add == &add!\n", add, &add);

	sub_fp = sub;
	int ret;
	ret = (*sub_fp)(5,9);
	printf("%s ret: %d\n", CONVER(sub_fp), ret);

	return 0;
} 

int max(int a, int b)
{
	if(a > b)
		return a;
	else
		return b;
}

// max address = &max address
int func_addr_demo2() 
{
	printf("\n\n%s run ...\n", __func__);

    printf("[max]: %p\n", max);
    printf("[max_pointer]: %p\n", &max); 
    
    int(*p)(int, int);
    int(*pp)(int, int);
    
    p = max;
    pp = &max;
    printf("[p]: %p\n", p); 
    printf("[*pp]: %p\n", pp);

    return 0;
}

/*
demo1 desc:
直接使用函数指针调用函数 
*/
//int (int)函数类型, 定义FUC函数变量int (int) FUC, 简写方式： int FUC(int);
typedef int FUC(int);

int test(int x)
{
	printf("test...:%d\n", x*x);

	return x;
}

int  fuc_1(int a, int b)
{
	printf("fuc_1...:%d\n", a+b);

	return 0;
}

void fuc_2()
{
	printf("fuc_2...\n");
}

void func_call_demo1()
{
	printf("\n\n%s run ...\n", __func__);

	//定义void () *函数类型指针pf1, 简写方式：void (*pf1)()
	int (*pf1)(int a, int b) = &fuc_1;
	void (*pf2)() = &fuc_2;
	printf("pf1 val: %p fuc_1 address: %p &fuc_1 val: %p \n", pf1, fuc_1, &fuc_1);
	printf("pf2 val: %p fuc_2 address: %p &fuc_2 val: %p \n", pf2, fuc_2, &fuc_2);
	
	printf("\n\n");
	//定义void () *函数类型指针pf1, 简写方式：void (*pf1)()
	int (*ppf1)(int a, int b); 
	void (*ppf2)();
	ppf1 = &fuc_1;
	ppf2 = &fuc_2;
	printf("ppf1 val: %p fuc_1 address: %p &fuc_1 val: %p\n", ppf1, fuc_1, &fuc_1);
	printf("ppf2 val: %p fuc_2 address: %p &fuc_2 val: %p\n", ppf2, fuc_2, &fuc_2);
	
	printf("\n\n");
	pf1(10, 20);
	(*pf1)(10, 20);
	(*pf2)();
	printf("pf1 val: %p *pf1 val: %p &pf1 val: %p \n", pf1, *pf1, pf1);
	
	printf("\n\n");
	FUC * pt = test;
	printf("pt(2) ret: %d\n" ,pt(2));
	printf("(*pt)(2) ret: %d\n",(*pt)(2));
	printf("pt val: %d *pt val: %d &test val: %d \n", pt, *pt, &test);

}

/*
概念二:回调函数
	回调函数就是一个通过函数指针调用的函数。如果你把函数的指针（地址）作为参数传递给另一个函数，
	当这个指针被用来调用其所指向的函数时，我们就说这是回调函数。回调函数不是由该函数的实现方直接调用，
	而是在特定的事件或条件发生时由另外的一方调用的，用于对该事件或条件进行响应。

为什么要使用回调函数？
	很多朋友可能会想，为什么不像普通函数调用那样，在回调的地方直接写函数的名字呢？这样不也可以吗？
	为什么非得用回调函数呢？有这个想法很好，因为在网上看到解析回调函数的很多例子，其实完全可以
	用普通函数调用来实现的。要回答这个问题，我们先来了解一下回到函数的好处和作用，那就是解耦，
	对，就是这么简单的答案，就是因为这个特点，普通函数代替不了回调函数。
	所以，在我眼里，这才是回调函数最大的特点。
	

*/

/*
demo2 desc:
使用带参数的回调函数
*/
/****************************************
 * 函数指针结构体
 ***************************************/
typedef struct _OP {
    float (*p_add)(float, float); 
    float (*p_sub)(float, float); 
    float (*p_mul)(float, float); 
    float (*p_div)(float, float); 
} OP; 
/*
实现加减乘除的运算功能函数:
*/
float ADD(float a, float b) 
{
	return a + b;
}

float SUB(float a, float b) 
{
    return a - b;
}

float MUL(float a, float b) 
{
    return a * b;
}

float DIV(float a, float b) 
{
    return a / b;
}

/****************************************
 * 初始化函数指针
 ***************************************/
void init_op(OP *op)
{
    op->p_add = ADD;
    op->p_sub = SUB;
    op->p_mul = &MUL;
    op->p_div = &DIV;
}

/****************************************
 * 库函数(使用带参数的回调函数)
 因为可以把调用者与被调用者分开，所以调用者不关心谁是被调用者。它只需知道存在一个具有
 特定原型和限制条件的被调用函数。简而言之，回调函数就是允许用户把需要调用的函数的指针
 作为参数传递给一个函数，以便该函数在处理相似事件的时候可以灵活的使用不同的方法。

2020/03/18
-----------------------------------------------------------------------
1) 不是user主动调用了usr-api，而是sys在合适的时候，使用usr-api注册好的callback函数，
完成对usr-api的调用。等于是 usr-api像callback注册了函数指针，
绕了一圈，回过头来，让sys调用了自己的过程。

	⑴定义一个回调函数（usr-api）；
	⑵提供函数实现的一方在初始化的时候，将回调函数（usr-api）的函数指针注册给调用者(callback)；
	⑶当特定的事件或条件发生的时候，调用者使用函数指针调用回调函数对事件进行处理。

2) 通过callback函数间接调用usr-api过程，而非在底层系统需要用到usr-api时，usr-api自己调用自己。
只需要usr-api把自己的函数指针传递给callback，底层何时使用就是底层自己决定的事情了，usr不许要再关心！

 ***************************************/
float add_sub_mul_div_callback(float a, float b, float (*op_func)(float, float))
{
    return (*op_func)(a, b);
}

int callback_demo2() 
{
	printf("\n\n%s run ...\n", __func__);

    OP *op = (OP *)malloc(sizeof(OP)); 
    init_op(op);
    
    /* 直接使用函数指针调用函数 */ 
    printf("ADD = %f, SUB = %f, MUL = %f, DIV = %f\n", (op->p_add)(2, 2), (*op->p_sub)(2, 2), 
            (op->p_mul)(2, 2), (*op->p_div)(2, 2));
     
    /* 调用带参数的回调函数 */ 
    printf("ADD = %f, SUB = %f, MUL = %f, DIV = %f\n", 
            add_sub_mul_div_callback(1.3, 2.2, ADD), 
            add_sub_mul_div_callback(1.3, 2.2, SUB), 
            add_sub_mul_div_callback(1.3, 2.2, MUL), 
            add_sub_mul_div_callback(1.3, 2.2, DIV));

    free(op);

    return 0; 
}

/* 调用**不带参数的回调函数 */ 
int Callback_1() // Callback Function 1
{
	printf("Hello, this is Callback_1 \n");
	return 0;
}

int Callback_2() // Callback Function 2
{
	printf("Hello, this is Callback_2 \n");
	return 0;
}

int Callback_3() // Callback Function 3
{
	printf("Hello, this is Callback_3 \n");
	return 0;
}

int Handle(int (*Callback)())
{
	printf("\nEntering Handle Function \n");
	Callback();
}

int callback_demo3()
{
	printf("\n\n%s run ...\n", __func__);
	Handle(Callback_1);
	Handle(Callback_2);
	Handle(Callback_3);

	return 0;
}

int main(int argc, char *argv[])
{
	//直接使用函数指针调用函数
	func_addr_demo1();
	//直接使用函数指针调用函数
	func_addr_demo2();
	//直接使用函数指针调用函数
	func_call_demo1();
	//使用带参数的回调函数
	callback_demo2();
	//调用**不带参数的回调函数
	callback_demo3();

	return 0;
}

/*个人理解
C语言一切都是用地址（编号）在运作！来完成处理数据（算法）和存储数据（数据结构）两个工作！
(把内存空间假想成按照内存总大小按以字节为单位的一个个连续的格子)

一、以单个字节为最小访问单位，用地址来编号每个字节单元！

二、用各种类型名字方式定义不同连续内存空间的访问范围！

	1）变量类型是一种定义固定大小的内存段（int4字节 char1字节）。
	2）结构体是一段包涵各个内型变量的内存段。
	3）数组是一段同类型变量集合的连续内存段。

	4）函数是一段综合程序的连续内存段。（函数指针定义了这个指针，就意味着可以访问函数所拥有
	的内存段，函数首地址赋给了指针，即所谓的指针指向了函数，这个指针就是函数首地址的变量名字，
	间接来说，函数首地址就是函数名，函数指针就是函数首地址，那么函数指针＝函数名）
	5）malloc是申请一段连续的内存段。

三、指针：（指向任意内存段首地址，并保持和其指向的内存段类型一致，就可以随意访问任何内存上的资源，
如变量、结构体、函数）同类型的指针只要指向其要访问的内存段的首地址，就可以直接把指针变量放到要
运行的地方，相当于完成对指向的内存段的访问和调用！

四、通过指针可以实现对数据的随意调度和操控！

*/