#ifndef __AT24C02_H__
#define __AT24C02_H__

void AT24C02_WriteByte(unsigned char WordAddress,Data);
//第一位指定地址， 第二位指定数据
//地址范围：由于256字节，故0~~255；
unsigned char AT24C02_ReadByte(unsigned char WordAddress);

#endif
