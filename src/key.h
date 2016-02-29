#include <STC89C5xRC.H>

#include "type_redef.h"

sbit k1 = P3^5;
sbit k2 = P3^4;
sbit k3 = P3^3;
sbit k4 = P3^2;

#define key_ok 			k1
#define key_cancel	k2
#define key_up 			k3
#define key_down 		k4

//#define KEY_PRESS_TIME			2			//按键按下确认阀值，20ms扫描一次下，次数*20ms(消抖由按键扫描调用时间来避免？扫描周期大于消抖周期)
#define KEY_HOLD_TIME					30		//长按键次数阀值，（20ms扫描一次下，触发的时间阀值为 次数*20ms）
#define KEY_HOLD_TRIG_TIME		2			//长按状态下,触发连续长按键时间（单位：扫描次数）( 长按键状态下过多久再触发一次)

#define KEY_FIFO_SIZE 5

typedef enum {
	KV_NONE = 0,
	KV_OK,
	KV_CANCEL,
	KV_UP,
	KV_DOWN,		
}keyVal_e;

typedef enum {
	KT_PRESS = 0,	//按下
	KT_RELEASE,	//松开
	KT_HOLD		//长按
}keyType_e;

typedef struct{
	keyVal_e 	keyVal;
	keyType_e	keyType;
}keyMsg_t;


void KeyInit();
void KeyScanSer_Hook();//可用于终端定时扫描
boolean Key_GetMsg(keyMsg_t *keyMsg);//成功返回true，空队列返回false
