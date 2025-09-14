#include "stm32f10x.h"                  // Device header

void LED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;	//类似与 int a;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;	//选用1，2引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//最高输出速率50;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	//初始化GPIOA， 结构体变量名(地址传递)
	
	GPIO_SetBits(GPIOA, GPIO_Pin_1 | GPIO_Pin_2);	//高电平
}

void LED1_ON(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_1);	//低电平
	//GPIO_SetBits(GPIOA, GPIO_Pin_2);	//高电平
}

void LED1_OFF(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_1);	//高电平
}

void LED2_ON(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_2);	//低电平
}

void LED2_OFF(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_2);	//高电平
}

void LED1_Turn(void)	//端口的电平翻转
{
	if(GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_1) == 0)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_1);	//PA1置1
	}else{
		GPIO_ResetBits(GPIOA, GPIO_Pin_1);	//PA1置0
	}
}

void LED2_Turn(void)	//端口的电平翻转
{
	if(GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_2) == 0)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_2);	//PA2置1
	}else{
		GPIO_ResetBits(GPIOA, GPIO_Pin_2);	//PA2置0
	}
}
