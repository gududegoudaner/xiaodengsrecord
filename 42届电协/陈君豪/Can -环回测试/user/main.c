#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "key.h" 
#include "my_can.h"

uint8_t key_number;
uint32_t TxID = 0x555;
int8_t TxLength = 4;
uint8_t TxData[8] = {0x11, 0x22, 0x33, 0x44};

uint32_t RxID;
uint8_t RxLength;
uint8_t RxData[8];
	
int main (void)
{
	
	
	OLED_Init();
	key_init ();
    My_can_Init ();
	  
	OLED_ShowString(1, 1, "TxID:");
	OLED_ShowHexNum(1, 6, TxID, 3);
	OLED_ShowString(2, 1, "RxID:");
	OLED_ShowString(3, 1, "Leng:");
	OLED_ShowString(4, 1, "Data:");
    while(1)
     {
		
		 key_number = key_read ();
		 if(key_number == 1)
		 {
			TxData [0] ++;
			TxData [1] ++;
			TxData [2] ++;
			TxData [3] ++;
		    My_CanStransmit (TxID,TxLength,TxData );
		 }
		 
		 if(MyCAN_ReceiveFlag() )
		{
			My_CanReceive(&RxID, &RxLength, RxData);
			
			OLED_ShowHexNum(2, 6, RxID, 3);
			OLED_ShowHexNum(3, 6, RxLength, 1);
			OLED_ShowHexNum(4, 6, RxData[0], 2);
			OLED_ShowHexNum(4, 9, RxData[1], 2);
			OLED_ShowHexNum(4, 12, RxData[2], 2);
			OLED_ShowHexNum(4, 15, RxData[3], 2);
		}
	 }
}
