#include <STC89C5xRC.H>

#include "DS1302.h"
#include "type_redef.h"

void  DS1302_SetProtect(bit flag);        				//是否写保护
uint8_t DS1302_ReadByte();
void  DS1302_WriteByte(uint8_t dat);
uint8_t DS1302_ReadData(uint8_t addr);
void  DS1302_WriteData(uint8_t addr, uint8_t dat);

void Delay2us();		//@11.0592MHz

/***************************************************
1302写入一字节(内部函数)
说明：1302在CLK上升沿写入，数据从低位开始	
***************************************************/
void DS1302_WriteByte(uint8_t dat) 
{ 
    uint8_t i;

    for(i=8; i>0; i--)
    {
        DS1302_IO = (dat&0x01);           //相当于汇编中的 RRC
        DS1302_CLK = 1;
        Delay2us();
        DS1302_CLK = 0;
        Delay2us();
        dat >>= 1;
    }
}

/***************************************************
实时时钟读取一字节(内部函数)
说明：1302在CLK下降沿读出，数据从低位开始	
***************************************************/
uint8_t DS1302_ReadByte() 
{ 
    uint8_t i;
    uint8_t dat;

    for (i=0; i<8; i++)
    {
        DS1302_CLK = 0;                   //时钟线拉低
        Delay2us();
        dat >>= 1;                  		//数据右移一位
        if (DS1302_IO) dat |= 0x80;        //读取数据
        DS1302_CLK = 1;                   //时钟线拉高
        Delay2us();
    }

    return dat;
}


/***************************************************
实时时钟写函数
入口参数：ucAddr: DS1302地址, ucData: 要写的数据
***************************************************/
void DS1302_WriteData(uint8_t addr, uint8_t dat)		
{
    DS1302_RST = 0;
    Delay2us();
    DS1302_CLK = 0;
    Delay2us();
    DS1302_RST = 1;
    Delay2us();
    DS1302_WriteByte(addr);         //写地址
    DS1302_WriteByte(dat);          //写数据
    DS1302_CLK = 1;
    DS1302_RST = 0;
}
/***************************************************
实时时钟读函数
入口参数：ucAddr: DS1302地址
出口: 读出的数据，BCD码
***************************************************/
uint8_t DS1302_ReadData(uint8_t addr)					  
{
    uint8_t dat;

    DS1302_RST = 0;
    Delay2us();
    DS1302_CLK = 0;
    Delay2us();
    DS1302_RST = 1;
    Delay2us();

    DS1302_WriteByte(addr);         //写地址
    dat = DS1302_ReadByte();        //读数据
    DS1302_CLK = 1;
    DS1302_RST = 0;

    return dat;
}
/***************************************************
写保护操作子程序
入口参数：0:写允许, 1: 禁止
***************************************************/
void DS1302_SetProtect(bit protect)        //是否写保护
{
    if(protect)
        DS1302_WriteData(REG_W_WP,0x80);
    else
        DS1302_WriteData(REG_W_WP,0x00);
}

/***************************************************
设定时间子程序
入口参数：Address：1302寄存器地址；Value：二进制时间
***************************************************/
void DS1302_SetTime(uint8_t Address, uint8_t Value)        // 设置时间函数
{
    DS1302_SetProtect(0);
    DS1302_WriteData(Address, ((Value/10)<<4 | (Value%10)));
    DS1302_SetProtect(1);
}

/***************************************************
设定全部时间
入口参数：结构体首址
***************************************************/
void DS1302_Set_All_Time(SYSTEMTIME *time)
{
    DS1302_SetTime(REG_W_SECOND ,time->Second );
    DS1302_SetTime(REG_W_MINUTE ,time->Minute );
    DS1302_SetTime(REG_W_HOUR 	,time->Hour );
    DS1302_SetTime(REG_W_DATE 	,time->Day );
    DS1302_SetTime(REG_W_WEEK 	,time->Week );
    DS1302_SetTime(REG_W_MONTH 	,time->Month );
    DS1302_SetTime(REG_W_YEAR	,time->Year );
}

/***************************************************
获取全部时间
***************************************************/
void DS1302_GetTime(SYSTEMTIME *time)
{
    uint8_t ReadValue;
    ReadValue = DS1302_ReadData(REG_R_SECOND);
    time->Second = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);
    ReadValue = DS1302_ReadData(REG_R_MINUTE);
    time->Minute = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);
    ReadValue = DS1302_ReadData(REG_R_HOUR);
    time->Hour = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);
    ReadValue = DS1302_ReadData(REG_R_DATE);
    time->Day = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);
    ReadValue = DS1302_ReadData(REG_R_WEEK);
    time->Week = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);
    ReadValue = DS1302_ReadData(REG_R_MONTH);
    time->Month = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);
    ReadValue = DS1302_ReadData(REG_R_YEAR);
    time->Year = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);
}

void DS1302_GetTimeBCD(SYSTEMTIME *time)
{
    uint8_t ReadValue;
    ReadValue = DS1302_ReadData(REG_R_SECOND);
    time->Second = ReadValue & 0x7F;
    ReadValue = DS1302_ReadData(REG_R_MINUTE);
    time->Minute = ReadValue & 0x7F;
    ReadValue = DS1302_ReadData(REG_R_HOUR);
    time->Hour = ReadValue & 0x7F;
    ReadValue = DS1302_ReadData(REG_R_DATE);
    time->Day = ReadValue & 0x7F;
    ReadValue = DS1302_ReadData(REG_R_WEEK);
    time->Week = ReadValue & 0x7F;
    ReadValue = DS1302_ReadData(REG_R_MONTH);
    time->Month = ReadValue & 0x7F;
    ReadValue = DS1302_ReadData(REG_R_YEAR);
    time->Year = ReadValue & 0x7F;
}

char DS1302_IsExist()
{
    uint8_t value;
    value = DS1302_ReadData(REG_R_WP);
    if(value&0x0F)//WP位只有bit7有1或0，其他位全为0，因此可以判断ds1302是否存在
        return 0;
    else
        return 1;
}
///***************************************************
//将年月日转换为ascii码：XX-XX-XX
//***************************************************/
//void DateToStr(SYSTEMTIME *time, char *DateString)
//{
//    DateString[0] = time->Year/10 + '0';
//    DateString[1] = time->Year%10 + '0';
//    DateString[2] = '-';
//    DateString[3] = time->Month/10 + '0';
//    DateString[4] = time->Month%10 + '0';
//    DateString[5] = '-';
//    DateString[6] = time->Day/10 + '0';
//    DateString[7] = time->Day%10 + '0';
//    DateString[8] = '\0';
//}

///***************************************************
//将时间转换为ascii码：XX：XX：XX
//***************************************************/
//void TimeToStr(SYSTEMTIME *time, char *TimeString)
//{
//    TimeString[0] = time->Hour/10 + '0';
//    TimeString[1] = time->Hour%10 + '0';
//    TimeString[2] = ':';
//    TimeString[3] = time->Minute/10 + '0';
//    TimeString[4] = time->Minute%10 + '0';
//    TimeString[5] = ':';
//    TimeString[6] = time->Second/10 + '0';
//    TimeString[7] = time->Second%10 + '0';
//    TimeString[8] = '\0';
//}

/***************************************************
1302初始化，second寄存器最高位CH=0正常工作
***************************************************/
void DS1302_Init(void)
{
    uint8_t sec_reg;

    sec_reg=DS1302_ReadData(REG_R_SECOND);
    if( sec_reg&0x80 )//clock half
        DS1302_SetTime(REG_W_SECOND,0);

}

void Delay2us()		//@11.0592MHz
{
}


//修改时间
//time.Second = 0;
//time.Minute = 32;
//time.Hour = 20;
//time.Week = 7;
//time.Day = 3;
//time.Month = 5;
//time.Year = 15;
//DS1302_Set_All_Time(&time);
