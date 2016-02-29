#include <STC89C5xRC.H>
#include <intrins.h>

#include "lcd1602.h"


//uint8_t LCD1602_DIYchar[8][8];


uint8_t LCD1602_WaitBusy();
void LCD1602_SetPortData(uint8_t dat);

void Delay15ms();
void Delay5ms();
void Delay2ms();
void Delay200us();
void Delay100us();
void Delay5us();
void Delay1us();


void LCD1602_Init()
{
	
	LCD1602_EN_L;
	LCD1602_BL_H;

	Delay15ms();//绛夊緟15ms锛岀瓑寰匧CD鐢垫簮绋冲畾

	LCD1602_WriteCom(0x03); // - - 璁剧疆4浣嶆牸寮忥紝2琛岋紝5x7
	Delay5ms();
	LCD1602_WriteCom(0x03); // - - 璁剧疆4浣嶆牸寮忥紝2琛岋紝5x7
	Delay200us();
	LCD1602_WriteCom(0x03); // - - 璁剧疆4浣嶆牸寮忥紝2琛岋紝5x7

	LCD1602_WaitBusy();
	LCD1602_WriteCom(0x02); // - - 璁剧疆4浣嶆牸寮忥紝2琛岋紝5x7
	LCD1602_WaitBusy();
	LCD1602_WriteCom(0x28); // - - 璁剧疆4浣嶆牸寮忥紝2琛岋紝5x7
	LCD1602_WaitBusy();
	LCD1602_WriteCom(0x0c); // - - 鏁翠綋鏄剧ず锛屽叧鍏夋爣锛屼笉闂儊
	LCD1602_WaitBusy();
	LCD1602_WriteCom(0x06); // - - 璁惧畾杈撳叆鏂瑰紡锛屽閲忎笉绉讳綅
	LCD1602_WaitBusy();
	LCD1602_WriteCom(0x01); // - - 娓呴櫎灞忓箷鏄剧ず
}

void LCD1602_BackLight(uint8_t onoff)
{
	onoff++;
	;//TODO
}

uint8_t LCD1602_WaitBusy()
{
	uint16_t timeout=0;
	
	Delay2ms();

	LCD1602_RS_L;
	LCD1602_RW_H;
	Delay100us();
	LCD1602_EN_H;
	
	while(LCD1602_DB7_READ)//缁涘绶�602鏉╂柨娲栨担搴ｆ暩楠烇拷
	{
		timeout++;
		Delay1us();
		if(timeout>200)
		{
			LCD1602_EN_L;
			return 1;//Still Busy
		}
	}	
	LCD1602_EN_L;

return 0;
}

void LCD1602_SetPortData(uint8_t dat)
{
	if(dat & 0x01)	LCD1602_DB4_H;
	else			LCD1602_DB4_L;
	if(dat & 0x02)	LCD1602_DB5_H;
	else			LCD1602_DB5_L;
	if(dat & 0x04)	LCD1602_DB6_H;
	else			LCD1602_DB6_L;
	if(dat & 0x08)	LCD1602_DB7_H;
	else			LCD1602_DB7_L;
}

void LCD1602_WriteCom(uint8_t command)
{
	LCD1602_RS_L;
	LCD1602_RW_L;

	LCD1602_SetPortData(command>>4);//閸ユ稓鍤庡Ο鈥崇础閸忓牆鍟撴妯虹摟閼猴拷
	Delay5us();
	LCD1602_EN_H;
	Delay5us();
	LCD1602_EN_L;
	LCD1602_SetPortData(command&0x0F);//閸愬秴鍟撴担搴℃磽娴ｅ秴鐡ч懞锟�
	Delay5us();
	LCD1602_EN_H;
	Delay5us();
	LCD1602_EN_L;	
	Delay5us();
}

void LCD1602_WriteDat(uint8_t dat)
{
	LCD1602_RS_H;
	LCD1602_RW_L;

	LCD1602_SetPortData(dat>>4);//閸ユ稓鍤庡Ο鈥崇础閸忓牆鍟撴妯虹摟閼猴拷
	Delay5us();
	LCD1602_EN_H;
	Delay5us();
	LCD1602_EN_L;
	LCD1602_SetPortData(dat&0x0F);//閸愬秴鍟撴担搴℃磽娴ｅ秴鐡ч懞锟�
	Delay5us();
	LCD1602_EN_H;
	Delay5us();
	LCD1602_EN_L;	
	Delay5us();
}

void LCD1602_SetXY(uint8_t x,uint8_t y)
{
	LCD1602_WaitBusy();
	LCD1602_WriteCom(0x80+(y<<6)+x); //閸氬牆鑻熼悽鐔稿灇閸︽澘娼�
}

void LCD1602_PutChar(uint8_t x,uint8_t y,char byte)	//閸︺劌娴愮�姘秴缂冾喖鍟撻崗锟芥稉顏勭摟缁楋讣绱濋崗銉ュ經閸欏倹鏆熼敍姝洪柅澶嬪鐞涘矉绱�~15閿涘绱漼闁瀚ㄩ崚妤嬬礄0~1閿涳拷ASCII
{
	LCD1602_SetXY(x,y);
	LCD1602_WaitBusy();
	LCD1602_WriteDat(byte);
}

void LCD1602_Puts(uint8_t x,uint8_t y,char *p)	    //閺佺顢戠�妤冾儊娑撴彃鍟撻崗銉礉閸忋儱褰涢崣鍌涙殶閿涙閿涳拷~15閿涘绱漼閿涳拷~1閿涘绱濈�妤冾儊娑撳弶鏆熺紒锟�
{
	uint8_t max_len = 0x27;//1602濮ｅ繗顢戦張锟芥毐

	LCD1602_SetXY(x,y);
	while( (*p)&&(max_len--) )
	{
		LCD1602_WaitBusy();
		LCD1602_WriteDat(*p);
		p++;
	}
}

void LCD1602_Cls()//濞撳懎鐫嗛敍灞炬殶閹诡喗瀵氶柦鍫濈秺闂嗭拷
{
	LCD1602_WaitBusy();
	LCD1602_WriteCom(0x01);
}

void LCD1602_SetCursorFlash(uint8_t cursor,uint8_t flash)
{
	uint8_t command=0x0C;
	if(cursor)
		command |= 0x02;
	else
		command &= ~0x02;
	if(flash)
		command |= 0x01;
	else
		command &= ~0x01;

	LCD1602_WaitBusy();
	LCD1602_WriteCom(command);
}

void LCD1602_WriteCGRAM(uint8_t select,uint8_t * dat)//select 0~8 CGRAM  dat 閸屻劌鐡�x7閻愬綊妯�惃锟芥担宥嗘殶缂佸嫰顪氶崷鏉挎絻
{
	uint8_t i;

	select <<= 3;
	LCD1602_WaitBusy();
	LCD1602_WriteCom(0x40|select);//CGRAM閸︽澘娼�
	for(i=0;i<8;i++)
	{
		LCD1602_WaitBusy();
		LCD1602_WriteDat(*dat++);
	}
}

//void LCD1602_WriteAllDIYchar()
//{
//	uint8_t i;
//	for(i=0;i<8;i++)
//	{
//		LCD1602_WriteCGRAM(i,LCD1602_DIYchar[i]);	
//	}
//}

//length:定长格式， 0则不保留有效数字前的0
void LCD1602_WriteNumber(uint8_t x, uint8_t y, uint8_t length, uint16_t number)
{	
	unsigned char index=0;
	char strBuf[6];
	strBuf[5]=0;

	switch (length){
		default:
		case 0:
		case 5:
			strBuf[0] = '0'+ number/10000%10;
		case 4:
			strBuf[1] = '0'+ number/1000%10;
		case 3:
			strBuf[2] = '0'+ number/100%10;
		case 2:
			strBuf[3] = '0'+ number/10%10;
		case 1:
			strBuf[4] = '0'+ number%10;
			index = 5-length;
			
			if (length!=0)
				break;		
		
			for(index=0; index<5; index++)
			{
				if (strBuf[index]=='0')
					strBuf[index] = '\0';
				else
					break;								
			}
			break;		
	}
			
	LCD1602_Puts(x,y,strBuf+index);
}

//void LCD1602_WriteLongNumber(uint8_t x,uint8_t y,uint8_t length, uint32_t number)
//{
//	char strBuf[17];
//	strBuf[16]=0;

//	strBuf[15] = '0'+ number%10;
//	strBuf[14] = '0'+ number/10%10;
//	strBuf[13] = '0'+ number/100%10;
//	strBuf[12] = '0'+ number/1000%10;
//	strBuf[11] = '0'+ number/10000%10;
//	strBuf[10] = '0'+ number/100000%10;
//	strBuf[9]  = '0'+ number/1000000%10;
//	strBuf[8]  = '0'+ number/10000000%10;
//	strBuf[7]  = '0'+ number/100000000%10;
//	strBuf[6]  = '0'+ number/1000000000%10;
//	strBuf[5]  = '0'+ number/10000000000%10;
//	strBuf[4]  = '0'+ number/100000000000%10;
//	strBuf[3]  = '0'+ number/1000000000000%10;
//	strBuf[2]  = '0'+ number/10000000000000%10;
//	strBuf[1]  = '0'+ number/100000000000000%10;
//	strBuf[0]  = '0'+ number/1000000000000000%10;

//	LCD1602_Puts(x,y,strBuf+(16-length));
//}

void Delay15ms()		//@11.0592MHz
{
	unsigned char i, j;

	i = 27;
	j = 226;
	do
	{
		while (--j);
	} while (--i);
}

void Delay5ms()		//@11.0592MHz
{
	unsigned char i, j;

	i = 9;
	j = 244;
	do
	{
		while (--j);
	} while (--i);
}

void Delay2ms()		//@11.0592MHz
{
	unsigned char i, j;

	_nop_();
	i = 4;
	j = 146;
	do
	{
		while (--j);
	} while (--i);
}

void Delay200us()		//@11.0592MHz
{
	unsigned char i;

	_nop_();
	i = 89;
	while (--i);
}

void Delay100us()		//@11.0592MHz
{
	unsigned char i;

	_nop_();
	i = 43;
	while (--i);
}

void Delay5us()		//@11.0592MHz
{
}

void Delay1us()		//@11.0592MHz
{
}



