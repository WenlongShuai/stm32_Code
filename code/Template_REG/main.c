#include "stm32f10x.h"

int main()
{

	return 0;
}

//因为启动文件会去调用外部的SystemInit函数，为了不让编译器报错，所以先定义一个空的SystemInit函数
void SystemInit()
{

}
