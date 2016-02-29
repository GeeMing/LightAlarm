#ifndef _LCD1602_H_
#define _LCD1602_H_

#include <STC89C5xRC.H>
#include "type_redef.h"

//4-wire drive
//D4~D7	-> P4.3	P1.2~P1.4
//RS	-> P8.2(H���� L����)
//RW	-> P3.7(H��   Lд)
//EN	-> P4.0
//BL  -> P1.5


#define LCD1602_DB7_PIN 	P27 
#define LCD1602_DB6_PIN 	P26 
#define LCD1602_DB5_PIN 	P25 
#define LCD1602_DB4_PIN 	P24 
#define LCD1602_RS_PIN		P07 
#define LCD1602_RW_PIN		P06 
#define LCD1602_EN_PIN		P05 
#define LCD1602_BL_PIN		P23 //no connect on board

#define LCD1602_DB7_L		LCD1602_DB7_PIN = 0
#define LCD1602_DB6_L		LCD1602_DB6_PIN = 0
#define LCD1602_DB5_L		LCD1602_DB5_PIN = 0
#define LCD1602_DB4_L		LCD1602_DB4_PIN = 0
#define LCD1602_DB7_H		LCD1602_DB7_PIN = 1
#define LCD1602_DB6_H		LCD1602_DB6_PIN = 1
#define LCD1602_DB5_H		LCD1602_DB5_PIN = 1
#define LCD1602_DB4_H		LCD1602_DB4_PIN = 1
#define LCD1602_RS_L		LCD1602_RS_PIN = 0
#define LCD1602_RW_L		LCD1602_RW_PIN = 0
#define LCD1602_EN_L		LCD1602_EN_PIN = 0
#define LCD1602_BL_L		LCD1602_BL_PIN = 0
#define LCD1602_RS_H		LCD1602_RS_PIN = 1
#define LCD1602_RW_H		LCD1602_RW_PIN = 1
#define LCD1602_EN_H		LCD1602_EN_PIN = 1
#define LCD1602_BL_H		LCD1602_BL_PIN = 1


#define LCD1602_DB7_READ	LCD1602_DB7_PIN

void LCD1602_Init();
void LCD1602_BackLight(uint8_t onoff);
void LCD1602_WriteCom(uint8_t command);
void LCD1602_WriteDat(uint8_t dat);

void LCD1602_SetXY(uint8_t x,uint8_t y);
void LCD1602_PutChar(uint8_t x,uint8_t y,char byte);	//�ڹ̶�λ��д��1���ַ�����ڲ�����xѡ���У�0~15����yѡ���У�0~1��,ASCII
void LCD1602_Puts(uint8_t x,uint8_t y,char *p);  //�����ַ���д�룬��ڲ�����x��0~15����y��0~1�����ַ�������
void LCD1602_Cls();//����������ָ�����
void LCD1602_WriteCGRAM(uint8_t select,uint8_t * dat);//select 0~8 CGRAM  dat ����5x7�����8λ�����׵�ַ
void LCD1602_WriteAllDIYchar();
void LCD1602_WriteNumber(uint8_t x, uint8_t y, uint8_t length, uint16_t number);//length:������ʽ�� 0�򲻱�����Ч����ǰ��0
void LCD1602_SetCursorFlash(uint8_t cursor,uint8_t flash);
void LCD1602_WriteLongNumber(uint8_t x,uint8_t y,uint8_t length, uint32_t number);

#endif 
