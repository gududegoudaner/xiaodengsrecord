#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "OLED.h"
#include "CountSensor.h"
#include "Encoder.h"

uint8_t KeyNum;

int main(void)
{	
	LED_Init();
	Key_Init();
	OLED_Init();
	CountSensor_Init();
	
	while(1)
	{
		
	}
	
}

/*
**1、中断函数里，最好不要执行耗时过长的代码，中断函数要简短快速，最好不要有delay这样，
		另外，最好不要在中断函数和主函数调用相同的函数或操作同一个硬件。例，既在主程序里调用oled，又在中断里调oled，则会显示错误
**实现功能时，可以在中断操作变量或标志位，当中断返回时，再对变量进行显示和操作。
*/