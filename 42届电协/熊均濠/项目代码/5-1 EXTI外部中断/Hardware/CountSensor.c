#include "stm32f10x.h"                  // Device header

void CountSensor_Init(void)
{
	/*
	**一：配置RCC，开启涉及的外设时钟
	**二：配置GPIO，选择输入模式
	**三：配置AFIO，选择某路GPIO，连接后面的EXTI
	**四：配置EXTI，选择边沿触发模式（上、下或双边），还有选择触发响应模式，中断响应和事件响应
	**五：配置NVIC，选择合适的优先级
	**最后，通过NVIC，外部中断信号进入CPU
	**跳转中断执行中断函数
	*/
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	/*
	**EXTI和NVIC两个外设的时钟是一直都打开着的，不需要我们再开启时钟
	**虽然EXTI作为独立外设，按理应该配置，但是寄存器里没有EXTI时钟的控制位，maybe电路设计考虑，so不管
	**NVIC是内核的外设
	*/
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);	//AFIO
	
	//将EXTI的第14个线路配置为中断模式，在stm~_exti.h文件里
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line14;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	//中断模式。/_Event是事件模式
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	//下降沿触发
		//EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;	上升沿触发
		//EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;	上下都触发
	EXTI_Init(&EXTI_InitStructure);
	
	//第五布，配置NVIC，在杂项misc.h文件里
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//这个分组方式整个芯片只能用一种
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);	//把结构体变量的地址放在NVIC_Init函数里
	
	//外部中断配置止
	
	
	
}

/**
	*	@brief	不用声明，因为中断函数不需要调用，是自动执行的
	*	@param	无参无返回值
	*	@retval	
	*/
void EXTI15_10_IRQHandler(void)	
{
	if(EXTI_GetITStatus(EXTI_Line14) == SET)
	{
		
		EXTI_ClearITPendingBit(EXTI_Line14);	//清除中断标志位，避免一直中断、卡住程序
	}
}
