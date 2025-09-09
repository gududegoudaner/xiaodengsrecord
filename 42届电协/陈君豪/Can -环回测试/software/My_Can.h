#ifndef __MY_CAN_H
#define __MY_CAN_H
#include "stm32f10x.h"                  // Device header

void My_can_Init(void);
void My_CanStransmit(uint32_t ID, uint8_t Length, uint8_t *Data);
void My_CanReceive(uint32_t *ID, uint8_t *Length, uint8_t *Data);
uint8_t MyCAN_ReceiveFlag(void);

#endif
