#include "stm32f10x.h"                  // Device header

void My_can_Init(void)
 {
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_CAN1 ,ENABLE );
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA ,ENABLE );
	/*gpio配置*/
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP ;//GPIO_Pin_12为CAN——tx,配置为复用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_Init (GPIOA ,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU  ;//GPIO_Pin_11为CAN——rx,配置为上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_Init (GPIOA ,&GPIO_InitStructure);
	/*发送与接收控制器配置*/
	CAN_InitTypeDef  Can_InitStructure;
	Can_InitStructure.CAN_Mode = CAN_Mode_Silent_LoopBack   ;//此时配置为环回模式
	Can_InitStructure.CAN_Prescaler = 48;//波特率 
	Can_InitStructure.CAN_BS1 = CAN_BS1_2tq ;//相位缓冲段1的大小
	Can_InitStructure.CAN_BS2 = CAN_BS2_3tq ;//相位缓冲段2的大小
	Can_InitStructure.CAN_SJW = CAN_SJW_2tq ;//再同步时的补偿宽度值
	Can_InitStructure.CAN_ABOM = DISABLE ;//自动离线管理，即检测连续128次连续隐性位，自动从总线关闭状态恢复到主动错误状态
	Can_InitStructure.CAN_NART = DISABLE ;//disable为自动重装，enable为禁止自动重装
	Can_InitStructure.CAN_AWUM = DISABLE ;//disable为手动唤醒，enable为自动唤醒
	Can_InitStructure.CAN_RFLM = DISABLE ;//disable为覆盖模式，会覆盖FIFO数据，enable为锁定模式，会丢弃新的数据
	Can_InitStructure.CAN_TTCM = DISABLE ;//disable为禁用时间触发通信模式，enable为启用时间触发通信
	Can_InitStructure.CAN_TXFP = DISABLE ;//disable由发送请求顺序决定FIFO，先请求先处理；enable为优先级为报文ID决定
	CAN_Init (CAN1 ,&Can_InitStructure);
	/*配置过滤器*/
	CAN_FilterInitTypeDef Can_FilterInitStructure;	
	Can_FilterInitStructure.CAN_FilterNumber = 0;//过滤器标号0~13
	Can_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit ;//32位
	Can_FilterInitStructure.CAN_FilterMode =  CAN_FilterMode_IdMask;//屏蔽位
	Can_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
	Can_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
	Can_FilterInitStructure.CAN_FilterIdLow = 0x0000;
	Can_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
	Can_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0 ;//选择接收器为FIFO0
	Can_FilterInitStructure.CAN_FilterActivation = ENABLE ;//使能
	CAN_FilterInit (&Can_FilterInitStructure);
}
/**
  * 函    数：发送函数
  * 参    数：要发送设备ID，数据长度Length，数据*Data
  * 返 回 值：无
  */

void My_CanStransmit(uint32_t ID, uint8_t Length, uint8_t *Data)
{
	uint32_t count = 0;
	
	CanTxMsg TxMassage;
	TxMassage.StdId = ID;//标准帧ID
	TxMassage.ExtId = ID;//拓展帧ID
	TxMassage.DLC = Length;//数据长度
	TxMassage.IDE = CAN_Id_Standard ;//规定是否使用拓展帧，CAN_Id_Standard为标准帧；CAN_Id_Standard为拓展帧
	TxMassage.RTR = CAN_RTR_Data ;//规定是否使用遥控帧，CAN_RTR_Data为数据帧；CAN_RTR_Remote为遥控帧
	for(uint8_t i = 0; i < Length ; i++)
	{
	TxMassage.Data[i] = Data[i];
	}
	
	uint8_t TransmitMailbox = CAN_Transmit (CAN1 ,&TxMassage);//返回信箱状态,0~2为邮箱空，编号为空邮箱标号；返回0x4为非空
	while(CAN_TransmitStatus (CAN1 ,TransmitMailbox) != CAN_TxStatus_Ok )//检查是否成功发送
	{
		count ++;
		if(count > 100000)
		{
			break ;
		}
	}
}
/**
  * 函    数：判断此时是否有消息要处理
  * 参    数：无
  * 返 回 值：有消息为1，没有为0
  */

uint8_t MyCAN_ReceiveFlag(void)
{
	if(CAN_MessagePending (CAN1 ,CAN_FIFO0 ) > 0)
	{
		return 1;
	}
	    return 0;
}
/**
  * 函    数：接受函数
  * 参    数：
  * 返 回 值：
  */
void My_CanReceive(uint32_t *ID, uint8_t *Length, uint8_t *Data)
{
	CanRxMsg RxMessage;
	CAN_Receive (CAN1 ,CAN_FIFO0 ,&RxMessage);//从can邮箱中读取一帧数据
	if(RxMessage.IDE == CAN_Id_Standard )
	{
		*ID = RxMessage.StdId;
	}
	else 
	{
	    *ID = RxMessage.ExtId;
	}		
	if(RxMessage.RTR == CAN_RTR_Data )
	{
	   *Length = RxMessage.DLC ;
	   for(uint8_t i = 0 ; i < *Length ; i++)//将数据放到data中
		{
		  Data[i]  = RxMessage.Data[i];
		} 
	//RxMessage.FMI为过滤器标号
	}
	else 
	{
		
	}
}














