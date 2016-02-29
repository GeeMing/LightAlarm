#include <STC89C5xRC.H>

#include "key.h"
#include "type_redef.h"																	


boolean IsFIFOFull();
boolean IsFIFOEmpty();
boolean Key_PutMsg(keyMsg_t *keyMsg);//β�����У�ͷ�����У���һ��Ԫ�����ж��Ƿ�������


keyMsg_t keyMsg_FIFO[KEY_FIFO_SIZE];	//β�����У�ͷ�����У���һ��Ԫ�����ж��Ƿ�������
uint8_t keyMsg_head=0;				//FIFO����ͷ�����ӣ�
uint8_t keyMsg_tail=0;				//FIFO����β����ӣ�



void KeyInit()
{
	keyMsg_head=0;
  keyMsg_tail=0;
	
	key_ok 		 = 1;
	key_cancel = 1;
	key_up 		 = 1;
	key_down 	 = 1;
}

keyVal_e KeyScan()
{
	key_ok = 1;
	key_cancel = 1;
	key_up = 1;
	key_down = 1;
	
	if (!key_ok)
		return KV_OK;
	if (!key_cancel)
		return KV_CANCEL;
	if (!key_up)
		return KV_UP;
	if (!key_down)
		return KV_DOWN;
	
	return KV_NONE;
}

void KeyScanSer_Hook()
{
	keyMsg_t keyMsg;
	keyVal_e keyGetCur;
	static keyVal_e keyGetLast = KV_NONE;	
	//static int keyPressTime= 0;//��¼�������ְ���ʱ�䣬���������¼����ż�(�����ɰ���ɨ�����ʱ�������⣿ɨ�����ڴ�����������)
	static int keyHoldTime = 0;//��¼�������ְ���ʱ�䣬�����������¼����ż�
	static int keyHoldTrig = 0;//����״̬����������������ʱ�����
		
	keyGetCur = KeyScan();
	
	if (keyGetLast == keyGetCur)
	{	
		if (keyGetCur == KV_NONE)		
			return;		
		else	//���ܴ���������
		{
			if (keyHoldTime<KEY_HOLD_TIME)
			{
				keyHoldTime++;
				keyHoldTrig = KEY_HOLD_TRIG_TIME;//��ʱ���ȴ���һ����Ϣ
			}
			else
			{	
				keyMsg.keyType = KT_HOLD;
				if (keyHoldTrig < KEY_HOLD_TRIG_TIME)
					keyHoldTrig++;
				else
				{
					keyHoldTrig = 0;
					keyMsg.keyVal = keyGetCur;
					Key_PutMsg(&keyMsg);
				}					
			}				
		}	
	}
	else	
		keyHoldTime = 0;//��ֵ�ı��ˣ��Ͳ����ǳ�����
	
	
	if ((keyGetLast == KV_NONE) && (keyGetCur != KV_NONE))
	{
		keyMsg.keyType = KT_PRESS;
		keyMsg.keyVal = keyGetCur;		
		Key_PutMsg(&keyMsg);
		keyGetLast = keyGetCur;		
	}
	
	if ((keyGetCur == KV_NONE) && (keyGetLast != KV_NONE))
	{
		keyMsg.keyType = KT_RELEASE;	
		keyMsg.keyVal = keyGetLast;		
		Key_PutMsg(&keyMsg);
		keyGetLast = keyGetCur;
	}	
}

boolean Key_PutMsg(keyMsg_t *keyMsg)//β�����У�ͷ�����У���һ��Ԫ�����ж��Ƿ�������
{
	if (IsFIFOFull())//������
		return false;//����ʧ��

	keyMsg_FIFO[keyMsg_tail] = *keyMsg;//���	
	keyMsg_tail = (keyMsg_tail+1)%KEY_FIFO_SIZE;
	
return true;
}

boolean Key_GetMsg(keyMsg_t *keyMsg)
{
	if (IsFIFOEmpty())//�ն���
		return false;
		
	*keyMsg = keyMsg_FIFO[keyMsg_head];//����	
	keyMsg_head = (keyMsg_head+1)%KEY_FIFO_SIZE;

return true;
}	

boolean IsFIFOEmpty()
{
	if (keyMsg_tail == keyMsg_head)
		return true;
	else
		return false;
}

boolean IsFIFOFull()
{
	if ( (1+keyMsg_tail)%KEY_FIFO_SIZE == keyMsg_head)
			return true;		
		else
			return false;	
	
}
