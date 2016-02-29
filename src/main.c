#include <STC89C5xRC.H>
#include <intrins.H>
#include "type_redef.h"

#include "lcd1602.h"
#include "ds1302.h"
#include "key.h"
#include "menu.h"

const char *weeks[]={"N/A","Mon","Tue","Wed","Thu","Fri","Sat","Sun"};


void Timer1_Init(void);		//20毫秒@11.0592MHz
void TimeDisplay();
void MenuSet();
void WaitForKeyPress(keyVal_e keyVal);
boolean SetValueNum(uint8_t x,uint8_t y, uint8_t length, uint16_t min, uint16_t max, uint16_t defValue, uint16_t *value);
uint16_t CalcWeekDay(uint16_t year, uint16_t month, uint16_t day); 

struct alarm_s{
	uint8_t hour;
	uint8_t minute;
	uint8_t week;
	
}alarm;

boolean flag_dotFlash = true;
SYSTEMTIME time;
keyMsg_t keyMsg;
boolean alarmEnable = true;	
char alarmStr[] = {"#06:30"};
uint8_t lightAhead=30;	



void main()
{
	
	LCD1602_Init();
	DS1302_Init();
	KeyInit();
	Menu_Init();	
	Timer1_Init();
	EA = 1;
	
	LCD1602_Cls();
	
	
	alarm.hour = 6;
	alarm.minute = 30;
	alarm.week = 0x7F;
	
	time.Second = 0;
	time.Minute = 32;
	time.Hour = 10;
	time.Week = 7;
	time.Day = 18;
	time.Month = 2;
	time.Year = 16;
	DS1302_Set_All_Time(&time);


//Menu_Display();
//while(1)
//		if (Key_GetMsg(&keyMsg))
//		{
//			if ((keyMsg.keyVal == KV_DOWN) && (keyMsg.keyType == KT_PRESS)){
//				Menu_Down();

//				
//				Menu_Display();
//			}	
//					
//			if ((keyMsg.keyVal == KV_UP) && (keyMsg.keyType == KT_PRESS)){
//				Menu_Up();

//				Menu_Display();
//			}
//		}
	while(1)
	{

		DS1302_GetTime(&time);

		TimeDisplay();
		
		
		if (Key_GetMsg(&keyMsg))
		{
			if ((keyMsg.keyVal == KV_CANCEL) && (keyMsg.keyType == KT_PRESS)){					
				alarmEnable = !alarmEnable;
			}
						
			if (keyMsg.keyVal == KV_OK){
				MenuSet();
			}

		}		


			
	}

}


//按确认键返回true, 取消键返回false
boolean SetValueNum(uint8_t x,uint8_t y, uint8_t length, uint16_t min, uint16_t max, uint16_t defValue, uint16_t *value)
{
	uint16_t tmpVal;
	
	tmpVal = defValue;
	LCD1602_WriteNumber(x,y,length,defValue);
	LCD1602_SetXY(x+length-1,y);
	LCD1602_SetCursorFlash(0,1);
	
	for(;;)
	{
		if (Key_GetMsg(&keyMsg))
		{
			if ((keyMsg.keyVal == KV_CANCEL) && (keyMsg.keyType == KT_PRESS)){					
				*value = defValue;
				LCD1602_SetCursorFlash(0,0);
				return false;
			}
						
			if ((keyMsg.keyVal == KV_OK) && (keyMsg.keyType == KT_PRESS)){					
				*value = tmpVal;
				LCD1602_SetCursorFlash(0,0);
				return true;
			}

			if ((keyMsg.keyVal == KV_UP) && (keyMsg.keyType != KT_RELEASE)){	
				if (tmpVal<max)
					tmpVal++;
					LCD1602_WriteNumber(x,y,length,tmpVal);
					LCD1602_SetXY(x+length-1,y);
			}

			if ((keyMsg.keyVal == KV_DOWN) && (keyMsg.keyType != KT_RELEASE)){					
				if (tmpVal>min)
					tmpVal--;
					LCD1602_WriteNumber(x,y,length,tmpVal);
					LCD1602_SetXY(x+length-1,y);				
			}			
		}	
	}
}

void MenuSet()
{
	SYSTEMTIME timeSet;
	uint16_t tmpVal;
	
	Menu_Display();
	
	for(;;)
	{
		
		if (Key_GetMsg(&keyMsg))
		{
			if ((keyMsg.keyVal == KV_DOWN) && (keyMsg.keyType == KT_PRESS)){
				Menu_Down();
				Menu_Display();
			}	
					
			if ((keyMsg.keyVal == KV_UP) && (keyMsg.keyType == KT_PRESS)){
				Menu_Up();
				Menu_Display();
			}
			
			if ((keyMsg.keyVal == KV_CANCEL) && (keyMsg.keyType == KT_PRESS)){
				LCD1602_Cls();
				Menu_Init();
				break;
			}
			
			if ((keyMsg.keyVal == KV_OK) && (keyMsg.keyType == KT_PRESS)){
				LCD1602_Cls();
				switch (Menu_ItemIndex()){
					case 0:
						LCD1602_Puts(0,0, " <Time Setting>");
						LCD1602_Puts(0,1, "15:57 2015/02/28");	
						LCD1602_WriteNumber(0,1,2, time.Hour);
						LCD1602_WriteNumber(3,1,2, time.Minute);
						LCD1602_WriteNumber(8,1,2, time.Year);
						LCD1602_WriteNumber(11,1,2, time.Month);
						LCD1602_WriteNumber(14,1,2, time.Day);
					
						if (SetValueNum(0,1, 2, 0, 23, time.Hour, &tmpVal)){
							timeSet.Hour = tmpVal;
							if (SetValueNum(3,1, 2, 0, 59, time.Minute, &tmpVal)){
								timeSet.Minute = tmpVal;
								if (SetValueNum(8,1, 2, 16, 50, time.Year, &tmpVal)){
									timeSet.Year = tmpVal;
									if (SetValueNum(11,1, 2, 1, 12, time.Month, &tmpVal)){
										timeSet.Month = tmpVal;
										if (SetValueNum(14,1, 2, 1, 31, time.Day, &tmpVal)){
											timeSet.Day = tmpVal;
											timeSet.Second=0;
											timeSet.Week = CalcWeekDay(timeSet.Year, timeSet.Month, timeSet.Day);
											DS1302_Set_All_Time(&timeSet);																						
											LCD1602_Puts(0,1, " -Time Set OK!- ");
										}}}}													
						}
						
						WaitForKeyPress(KV_CANCEL);
						LCD1602_Cls();
						break;
						
					case 1:
						LCD1602_Puts(0,0, " <Alarm Setting>");					
						LCD1602_Puts(0,1, "06:30  @1234567");
					
						if (SetValueNum(0,1, 2, 0, 23, alarm.hour, &tmpVal)){
							alarm.hour = tmpVal;
							if (SetValueNum(3,1, 2, 0, 59, alarm.minute, &tmpVal)){
								alarm.minute = tmpVal;
								alarmStr[1] = '0'+ alarm.hour/10;
								alarmStr[2] = '0'+ alarm.hour%10;
								alarmStr[4] = '0'+ alarm.minute/10;
								alarmStr[5] = '0'+ alarm.minute%10;								
								LCD1602_Puts(0,1, " -Alarm Set OK- ");							
							}
						}
					
						WaitForKeyPress(KV_CANCEL);
						LCD1602_Cls();
						break;	
						
					case 2:
						LCD1602_Puts(0,0, " <Light Ahead>");
						LCD1602_Puts(0,1, "Set: 30 minutes");
					
						if (SetValueNum(5,1, 2, 0, 99, lightAhead, &tmpVal)){
							lightAhead = tmpVal;
							LCD1602_Puts(0,1, " -Light Set OK- ");
						}					
					
						WaitForKeyPress(KV_CANCEL);
						LCD1602_Cls();
						break;	
					case 3:
						LCD1602_Puts(0,0, "  <Brightness>");
						LCD1602_Puts(0,1, "Set: 80% Maximum");
						
						WaitForKeyPress(KV_CANCEL);
						LCD1602_Cls();
						break;	
					
					case 4:
						LCD1602_Puts(0,0, "    <About>");
						LCD1602_Puts(0,1, "Build by GeeMing");
						
						WaitForKeyPress(KV_CANCEL);
						LCD1602_Cls();
						break;	
				}
				break;
			}

		}
	
	}	
	
}


void WaitForKeyPress(keyVal_e keyVal)
{	
	while(1){
		if (Key_GetMsg(&keyMsg))
			if ((keyMsg.keyVal == keyVal) && (keyMsg.keyType == KT_PRESS))
				return;
	}
}

void TimeDisplay()
{

		LCD1602_PutChar(5,0, '0'+time.Hour/10);
		LCD1602_PutChar(6,0, '0'+time.Hour%10);
		LCD1602_PutChar(8,0, '0'+time.Minute/10);
		LCD1602_PutChar(9,0, '0'+time.Minute%10);
		if (flag_dotFlash){						
			LCD1602_PutChar(7,0, ':');
		}else{
			LCD1602_PutChar(7,0, ' ');
		}

		LCD1602_Puts(7,1, weeks[time.Week]);
		
		LCD1602_PutChar(11,1, '0'+time.Month/10);
		LCD1602_PutChar(12,1, '0'+time.Month%10);
		LCD1602_PutChar(13,1, '/');
		LCD1602_PutChar(14,1, '0'+time.Day/10);
		LCD1602_PutChar(15,1, '0'+time.Day%10);
		
		if (alarmEnable){								
			LCD1602_Puts(0,1, alarmStr);
		}else{
			LCD1602_Puts(0,1, "      ");//TODO
		}
		

}

void Timer1_Init(void)		//20毫秒@11.0592MHz
{
	AUXR &= 0xBF;		//定时器时钟12T模式
	TMOD &= 0x0F;		//设置定时器模式
	TMOD |= 0x10;		//设置定时器模式
	TL1 = 0x00;		//设置定时初值
	TH1 = 0xB8;		//设置定时初值
	TF1 = 0;		//清除TF1标志
	TR1 = 1;		//定时器1开始计时
	ET1 = 1;	//使能T1中断
}

void T1_20ms_ISR() interrupt 3
{
	static uint8_t dotFlash_counter = 0;
	
	TL1 = 0x00;		//设置定时初值
	TH1 = 0xB8;		//设置定时初值
	
	if (dotFlash_counter<25){//0.5s闪一次
		dotFlash_counter++;
	}else{
		dotFlash_counter = 0;
		flag_dotFlash = !flag_dotFlash;		
	}
	
	KeyScanSer_Hook();
	
}



uint16_t CalcWeekDay(uint16_t year, uint16_t month, uint16_t day) 
{
	uint16_t week;
		
	if(month==1||month==2) {
		month+=12;
		year--;
  }
	
	//基姆拉尔森计算公式
  week = (day+2*month+3*(month+1)/5+year+year/4-year/100+year/400)%7;
	
	return week;
}

