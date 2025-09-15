#include "stm32f10x.h"                  // Device header
#include "Delay.h"
void key_init(void)
{
    RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init (GPIOB,&GPIO_InitStructure);
}

uint8_t  key_read(void)
{
     uint8_t state=0;
	/*
	** GPIO_StructInit(GPIO_InitTypeDef* GPIO_InitStruct);初始化GPIO配置结构体为默认值
	**uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);读取指定GPIO端口的单个输入引脚电平状态。返回值：0：引脚为低电平。1：引脚为高电平。
	**uint16_t GPIO_ReadInputData(GPIO_TypeDef* GPIOx);读取整个GPIO端口的输入数据寄存器值（所有引脚状态）,返回值：16位无符号整数，表示端口所有引脚的输入状态（每位对应一个引脚）。
	**uint8_t GPIO_ReadOutputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);读取指定GPIO端口的单个输出引脚电平状态。
	**uint16_t GPIO_ReadOutputData(GPIO_TypeDef* GPIOx);读取整个GPIO端口的输出数据寄存器值（所有输出引脚状态）
	*/
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==0)
	{
		Delay_ms(20);
		 if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==0)
		{
		while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0);
		Delay_ms(20);
		state=1;
		}
	}
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)==0)
	{
		Delay_ms(20);
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)==0)
		{
		while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == 0);
		Delay_ms(20);
		state=2;
		}
	}
	return state;
	
}
