#include <STC89C5xRC.H>

#include "key.h"
#include "type_redef.h"																	


boolean IsFIFOFull();
boolean IsFIFOEmpty();
boolean Key_PutMsg(keyMsg_t *keyMsg);//尾部入列，头部出列，空一个元素来判断是否满队列


keyMsg_t keyMsg_FIFO[KEY_FIFO_SIZE];	//尾部入列，头部出列，空一个元素来判断是否满队列
uint8_t keyMsg_head=0;				//FIFO队列头（出队）
uint8_t keyMsg_tail=0;				//FIFO队列尾（入队）



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
	//static int keyPressTime= 0;//记录按键保持按下时间，触发按下事件的门槛(消抖由按键扫描调用时间来避免？扫描周期大于消抖周期)
	static int keyHoldTime = 0;//记录按键保持按下时间，触发长按键事件的门槛
	static int keyHoldTrig = 0;//长按状态下连续触发长按键时间计数
		
	keyGetCur = KeyScan();
	
	if (keyGetLast == keyGetCur)
	{	
		if (keyGetCur == KV_NONE)		
			return;		
		else	//可能触发长按键
		{
			if (keyHoldTime<KEY_HOLD_TIME)
			{
				keyHoldTime++;
				keyHoldTrig = KEY_HOLD_TRIG_TIME;//到时候先触发一次消息
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
		keyHoldTime = 0;//键值改变了，就不会是长按键
	
	
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

boolean Key_PutMsg(keyMsg_t *keyMsg)//尾部入列，头部出列，空一个元素来判断是否满队列
{
	if (IsFIFOFull())//满队列
		return false;//入列失败

	keyMsg_FIFO[keyMsg_tail] = *keyMsg;//入队	
	keyMsg_tail = (keyMsg_tail+1)%KEY_FIFO_SIZE;
	
return true;
}

boolean Key_GetMsg(keyMsg_t *keyMsg)
{
	if (IsFIFOEmpty())//空队列
		return false;
		
	*keyMsg = keyMsg_FIFO[keyMsg_head];//出队	
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
