#ifndef _DS1302_H_
#define _DS1302_H_

#include <STC89C5xRC.H>
#include <INTRINS.H>

#include "type_redef.h"


sbit DS1302_CLK = P1^2;              
sbit DS1302_IO  = P1^1;	
sbit DS1302_RST = P1^0;              

typedef struct __SYSTEMTIME__
{
uint8_t Second;
uint8_t Minute;
uint8_t Hour;
uint8_t Week;
uint8_t Day;
uint8_t Month;
uint8_t Year;
//uint8_t DateString[9];
//uint8_t TimeString[9];
}SYSTEMTIME; //定义的时间类型

#define AM(X)  X
#define PM(X) (X+12)               					// 转成24小时制

//读寄存器
#define REG_R_SECOND 	0x81
#define REG_R_MINUTE 	0x83
#define REG_R_HOUR  	0x85 
#define REG_R_DATE  	0x87 //日期
#define REG_R_MONTH   	0x89
#define REG_R_WEEK		0x8B //星期
#define REG_R_YEAR  	0x8D
#define REG_R_WP		0x8F //写保护
#define REG_R_TCS		0x91 //涓流充电


//写寄存器：
#define REG_W_SECOND 	0x80 //最高位CH 为1时钟暂停 0开始
#define REG_W_MINUTE 	0x82
#define REG_W_HOUR  	0x84 
#define REG_W_DATE  	0x86 //日期
#define REG_W_MONTH   	0x88
#define REG_W_WEEK		0x8A //星期
#define REG_W_YEAR  	0x8C
#define REG_W_WP		0x8E //写保护
#define REG_W_TCS		0x90 //涓流充电 TCS3 TCS2 TCS1 TCS0  DS1 DS0 ROUT1 ROUT0


void DS1302_Init();
void DS1302_SetTime(uint8_t Address, uint8_t Value);        //设置时间函数
void DS1302_Set_All_Time(SYSTEMTIME *time);				//设置全部时间函数
void DS1302_GetTime(SYSTEMTIME *time);					//获取时间函数
void DS1302_GetTimeBCD(SYSTEMTIME *time);//BCD 格式获取时间
char DS1302_IsExist();
void DateToStr(SYSTEMTIME *time, char *DateString);		//将年月日转换到指定字符串
void TimeToStr(SYSTEMTIME *time, char *TimeString);		//将时分秒转换到指定字符串


#endif
