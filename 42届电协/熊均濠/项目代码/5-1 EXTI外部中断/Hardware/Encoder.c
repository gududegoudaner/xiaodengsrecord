#include "stm32f10x.h"                  // Device header

int16_t Encoder_Count;

/**
	*	@brief	旋转编码器
	*	@param	
	*	@retval	
	*/
void Encoder_Init(void)
{
	//复制CountSensor的配置代码
	
	//改：PB0和PB1
	//14->GPIO_PinSoruce0;GPIO_PinSoruce1
	
}

/*
正转
		--       ---
		  |      |
		  |______|
		  @!
       ------       ------		高
		     |      |
			 |______|

反转
	 -----       ----
		  |      |
		  |______|
	      @!
   ----       -----				低
	   |      |
	   |______|

*/

int16_t Encoder_Get(void)
{
	int16_t Temp;
	Temp = Encoder_Count;
	Encoder_Count = 0;
	return Temp;
}

void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0) == SET)
	{
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0)
		{
			Encoder_Count --;
		}
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}

void EXTI1_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line1) == SET)
	{
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0)
		{
			Encoder_Count ++;
		}
		EXTI_ClearITPendingBit(EXTI_Line1);
	}
}
