#ifndef _MENU_H_
#define _MENU_H_

#include "type_redef.h"

typedef struct _item_ _item;
typedef struct _menu_ _menu;
typedef struct _navigation_ _navigation;

struct _item_
{	
	_menu *childMenu;
	char *text;
};

struct _menu_
{
	_menu *fatherMenu;
	uint8_t itemCount;
	_item *item;
};

struct _navigation_
{
	_menu *fatherMenu;
	_menu *menu;
	uint8_t maxColum;//记录屏幕可以显示的行数
	uint8_t itemIndex;
	uint8_t headerIndex;//记录屏幕显示的第一行items的index
};

void Menu_Init();
void Menu_Down();
void Menu_Up();
int  Menu_Enter();
int  Menu_Back();
void Menu_Display();
uint8_t Menu_ItemIndex();


#endif