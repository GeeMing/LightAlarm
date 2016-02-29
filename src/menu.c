/*************************************************/
/*Edit by GeeMing, 2016/02/28
/*************************************************/

#include <STC89C5xRC.H>

#include "menu.h"
#include "lcd1602.h"

#define NULL 0

// mainMenu
//	setTime
//	setAlarm
//	about

_navigation nav;

_menu mainMenu;
_item item_mainMenu[5];


void Menu_Down()
{
    if (nav.itemIndex < nav.menu->itemCount - 1)
		{
			if (nav.headerIndex + nav.maxColum-1 == nav.itemIndex){
				nav.headerIndex++;
			}
				nav.itemIndex++;
			
		}    
}

void Menu_Up()
{			
		if (nav.itemIndex > 0){
			if (nav.headerIndex==nav.itemIndex){
				nav.headerIndex--;
			}
			nav.itemIndex--;
		}		
}

int Menu_Enter()	// if no childMenu return 0
{
    if (nav.menu->item[nav.itemIndex].childMenu != NULL)
    {
        nav.menu = nav.menu->item[nav.itemIndex].childMenu;
        nav.itemIndex = 0;
				nav.headerIndex = 0;
        return 1;
    }
    else
        return 0;
}

int Menu_Back()
{
    if (nav.menu->fatherMenu != NULL)
    {
        nav.menu = nav.menu->fatherMenu;
        nav.itemIndex = 0;
				nav.headerIndex = nav.itemIndex;
        return 1;
    }
    else
        return 0;
}

void Menu_Display()
{	
	uint8_t y=0;

	LCD1602_Cls();

	for (y=0; (y+nav.headerIndex < nav.menu->itemCount) && (y<nav.maxColum); y++)
	{
			if (y+nav.headerIndex == nav.itemIndex)
			{
					LCD1602_Puts(0,y,"*");
					LCD1602_Puts(1,y,nav.menu->item[y+nav.headerIndex].text);
			}
			else
			{
					LCD1602_Puts(0,y," ");
					LCD1602_Puts(1,y,nav.menu->item[y+nav.headerIndex].text);
			}
	}
	
}

void Menu_Init()
{
    nav.fatherMenu = NULL;
    nav.menu = &mainMenu;
    nav.itemIndex = 0;
		nav.headerIndex = 0;
		nav.maxColum = 2;//lcd1602

    mainMenu.fatherMenu = NULL;
    mainMenu.itemCount = 5;
    mainMenu.item = item_mainMenu;

    item_mainMenu[0].text = "1.Time Setting";
    item_mainMenu[0].childMenu = NULL;

    item_mainMenu[1].text = "2.Alarm Setting";
    item_mainMenu[1].childMenu = NULL;

	  item_mainMenu[2].text = "3.Alarm Ahead";
    item_mainMenu[2].childMenu = NULL;

	  item_mainMenu[3].text = "4.Brightness";
    item_mainMenu[3].childMenu = NULL;

	  item_mainMenu[4].text = "5.About";
    item_mainMenu[4].childMenu = NULL;
	
}

uint8_t Menu_ItemIndex()
{
	return nav.itemIndex;
}
