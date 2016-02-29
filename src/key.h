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

//#define KEY_PRESS_TIME			2			//��������ȷ�Ϸ�ֵ��20msɨ��һ���£�����*20ms(�����ɰ���ɨ�����ʱ�������⣿ɨ�����ڴ�����������)
#define KEY_HOLD_TIME					30		//������������ֵ����20msɨ��һ���£�������ʱ�䷧ֵΪ ����*20ms��
#define KEY_HOLD_TRIG_TIME		2			//����״̬��,��������������ʱ�䣨��λ��ɨ�������( ������״̬�¹�����ٴ���һ��)

#define KEY_FIFO_SIZE 5

typedef enum {
	KV_NONE = 0,
	KV_OK,
	KV_CANCEL,
	KV_UP,
	KV_DOWN,		
}keyVal_e;

typedef enum {
	KT_PRESS = 0,	//����
	KT_RELEASE,	//�ɿ�
	KT_HOLD		//����
}keyType_e;

typedef struct{
	keyVal_e 	keyVal;
	keyType_e	keyType;
}keyMsg_t;


void KeyInit();
void KeyScanSer_Hook();//�������ն˶�ʱɨ��
boolean Key_GetMsg(keyMsg_t *keyMsg);//�ɹ�����true���ն��з���false
