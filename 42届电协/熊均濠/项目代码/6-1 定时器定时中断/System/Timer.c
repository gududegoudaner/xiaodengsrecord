#include "stm32f10x.h"                  // Device header

void Timer_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_InternalClockConfig(TIM2);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 10000 - 1;				//周期，ARR自动重装器的值,0~65535
	TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1;				//就是PSC预分频器的值,0~65535
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;	//是重复计数器的值
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	/*CK_CNT_OV = CK_CNT/(ARR+1)
	**			= CK_PSC/(PSC+1)/(ARR+1)	定时频率=72M/(PSC+1)/(ARR+1)
	**“-1”因为预分频器和计数器都有1个数的偏差，故减一
	**此处预分频：对72M进行7200分频，得到10K的计数频率。10K下，计10000个数，就是1s的时间
	*/
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM2, ENABLE);
	//定时中断的初始化止。
}

