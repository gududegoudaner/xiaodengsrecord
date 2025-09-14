#include <REGX52.H>

/*
 *起始信号->选择从机地址码（地址共7bit）并呼叫->指示W/R 写/读 0/1 低/高
 *从机应答（ACK）：数据线在应答位释放给从机（即数据线处于默认的高电平），从机通过拉低电平应答/高点平非应答 0/1
 *主机继续接管总线，并写入数据11110011
 *从机应答：重复第二个（此例子不交流数据）
 *停止位：在时钟线为高电平的时候，数据线产生一个上升沿0——>1
 *
 *device address是外设的从机地址码，可在数据手册查找啊
 *数据：1 1 1 1 0 0 1 1 是指某一指令
 *这一指令可以在数据手册中查找
 */

//位声明
sbit I2C_SCL=P2^1;
sbit I2C_SDA=P2^0;

//6个时序结构

/**
	*	@brief	I2C开始
	*	@param	无
	*	@retval	无
	*	@overview 起始条件，SCL高电平期间，SDA从高变低
	*/
void I2C_Start(void)
{
	I2C_SDA=1;	//
	I2C_SCL=1;	//此操作避免SDASCL原先处于低电平
	I2C_SDA=0;	//
	I2C_SCL=0;	//切换低电平
}

/**
	*	@brief	I2C停止
	*	@param	无
	*	@retval	无
	*	@overview 终止条件，SCL高电平期间，SDA从低变高
	*/
void I2C_Stop(void)
{
	I2C_SDA=0;	//在停止之前，确保SDA是低电平0
	I2C_SCL=1;	//电平拉高
	I2C_SDA=1;	//电平拉高
}

/**
	*	@brief	I2C发送一个字节
	*	@param	Byte —— 要发送的字节
	*	@retval	无
	*/
void I2C_SendByte(unsigned char Byte)
{
	/*最小单元
	I2C_SDA=Byte&0x80;将Byte的最高位取出来，赋给SDA
	I2C_SCL=1;	拉高
	I2C_SCL=0;	拉低
	拉高拉低之间，数据可以被读到
	因为51单片机速度比较慢，大于手册的0.4us
	*/
	unsigned char i;
	for(i=0;i<8;i++)	//循环8次
	{
		I2C_SDA=Byte&(0x80>>i);	//从高位到低位发，
		//依次取出放在SDA线上
		I2C_SCL=1;
		I2C_SCL=0;
	}
}

/**
	*	@brief	I2C接受一个字节
	*	@param	无
	*	@retval	接受到的一个字节数据
	*/
unsigned char I2C_ReceiveByte(void)
{
	unsigned char i,Byte=0x00;
	I2C_SDA=1;	//拉高。释放SDA
	for(i=0;i<8;i++)
	{
		I2C_SCL=1;
		if(I2C_SDA) {Byte|=(0x80>>i);}
		I2C_SCL=0;
	}
	return Byte; 
}

/**
	*	@brief	I2C发送应答
	*	@param	AckBit 应答位，0为应答，1为非应答
	*	@retval	无
	*/
void I2C_SendAck(unsigned char AckBit)
{
	I2C_SDA=AckBit;
	I2C_SCL=1;
	I2C_SCL=0;
}

/**
	*	@brief	I2C接收应答位
	*	@param	无
	*	@retval	接收到的应答位，0为应答，1为非应答
	*/
unsigned char I2C_ReceiveAck(void)
{
	unsigned char AckBit;
	I2C_SDA=1;
	I2C_SCL=1;
	AckBit=I2C_SDA;
	I2C_SCL=0;
	return AckBit;
}
