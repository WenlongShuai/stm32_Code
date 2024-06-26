#include "stm32f10x.h"

//点亮RGB灯的其中一种颜色

#define GREEN_LED 0
#define RED_LED   0
#define BLUE_LED  1

int main()
{
	#if GREEN_LED
	//控制PB0引脚
	//打开GPIOB的时钟
	*(unsigned int*)(0x40021000+0x18) |= (1<<3);
	//配置GPIOB的输入输出模式
	*(unsigned int*)0x40010C00 |= ( 1 << (4*0) );
	//控制GPIOB输出高低电平
	*(unsigned int*)(0x40010C00+0x0C) &= ~(1<<0);
	#endif
	
	#if RED_LED
	//控制PB5引脚
	//打开GPIOB的时钟
	*(unsigned int*)(0x40021000+0x18) |= (1<<3);
	
	//配置GPIOB的输入输出模式
	*(unsigned int*)0x40010C00 |= ( 1 << (4*5) );
	
	//控制GPIOB输出高低电平
	*(unsigned int*)(0x40010C00+0x0C) &= ~(1<<5);
	
	#endif
	
	#if BLUE_LED
	//控制PB1引脚
	//打开GPIOB的时钟
	*(unsigned int*)(0x40021000+0x18) |= (1<<3);
	
	//配置GPIOB的输入输出模式
	*(unsigned int*)0x40010C00 |= ( 1 << (4*1) );
	
	//控制GPIOB输出高低电平
	*(unsigned int*)(0x40010C00+0x0C) &= ~(1<<1);
	
	#endif

	return 0;
}

//因为启动文件会去调用外部的SystemInit函数，为了不让编译器报错，所以先定义一个空的SystemInit函数
void SystemInit()
{

}
