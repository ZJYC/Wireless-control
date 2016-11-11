/*
****************************************************
*  文件名             : HMI.c
*  作者               : -5A4A5943-
*  版本               : V1.0.0
*  编写日期           : 2016--10--29--13
*  简介               : HMI屛驱动函数
*  函数列表           : 
*  历史版本           : 
*****************************************************
*/


/*头文件  */

#include "board.h"



/*宏定义  */





/*变量定义*/

//UART_HandleTypeDef huart1;

uint8_t BuffHMI[30] = {0x00};


uint8_t CurPageID = 0,CurItem = 0;
PageInfTypedef PageAll[] = 
{
    {
	"大北屋",
	2,
    {
    {0,"c0",17,0},
	{0,"c1",18,0},
	{0,"c2",19,0},
    {0,"n0",18,0},
	{0,"n1",19,0},
	{0,"n2",20,0},
    }
    },
    {
	"南屋",
	3,
    {
    {0,"c0",18,0},
	{0,"c1",19,0},
	{0,"c2",20,0},
    {0,"n0",18,0},
	{0,"n1",19,0},
	{0,"n2",20,0},
    }
	},
    {
	"大门",
	4,
    {
    {0,"c0",18,0},
	{0,"c1",19,0},
	{0,"c2",20,0},
    {0,"n0",18,0},
	{0,"n1",19,0},
	{0,"n2",20,0},
    }
	},
    {
	"庭院",
	5,
    {
    {0,"c0",18,0},
	{0,"c1",19,0},
	{0,"c2",20,0},
    {0,"n0",18,0},
	{0,"n1",19,0},
	{0,"n2",20,0},
    }
    }
};

/*变量声明*/





/*函数声明*/

#if 1//发送指令

static uint8_t * SendInstruct_page(uint8_t * ID)
{
    strcpy(BuffHMI,Instruct_page);
    strcat(BuffHMI," ");
    strcat(BuffHMI,ID);
    return BuffHMI;
}

static uint8_t * SendInstruct_ref(uint8_t * ID)
{
    strcpy(BuffHMI,Instruct_ref);
    strcat(BuffHMI," ");
    strcat(BuffHMI,ID);
    return BuffHMI;
}

static uint8_t * SendInstruct_click(uint8_t * ID,uint8_t * Event)
{
    strcpy(BuffHMI,Instruct_click);
    strcat(BuffHMI," ");
    strcat(BuffHMI,ID);
    strcat(BuffHMI,",");
    strcat(BuffHMI,Event);
    return BuffHMI;
}

static uint8_t * SendInstruct_get(uint8_t * ValueName)
{
    strcpy(BuffHMI,Instruct_get);
    strcat(BuffHMI," ");
    strcat(BuffHMI,ValueName);
    return BuffHMI;
}

static uint8_t * SendInstruct_print(uint8_t * ValueName)
{
    strcpy(BuffHMI,Instruct_print);
    strcat(BuffHMI," ");
    strcat(BuffHMI,ValueName);
    return BuffHMI;
}

static uint8_t * SendInstruct_printh(uint8_t * ValueName)
{
    strcpy(BuffHMI,Instruct_printh);
    strcat(BuffHMI," ");
    strcat(BuffHMI,ValueName);
    return BuffHMI;
}

static uint8_t * SendInstruct_vis(uint8_t * ID,uint8_t * State)
{
    strcpy(BuffHMI,Instruct_vis);
    strcat(BuffHMI," ");
    strcat(BuffHMI,ID);
    strcat(BuffHMI,",");
    strcat(BuffHMI,State);
    return BuffHMI;
}

static uint8_t * SendInstruct_tsw(uint8_t * ID,uint8_t * State)
{
    strcpy(BuffHMI,Instruct_tsw);
    strcat(BuffHMI," ");
    strcat(BuffHMI,ID);
    strcat(BuffHMI,",");
    strcat(BuffHMI,State);
    return BuffHMI;
}

static uint8_t * SendInstruct_randset(uint8_t * Min,uint8_t * Max)
{
    strcpy(BuffHMI,Instruct_randset);
    strcat(BuffHMI," ");
    strcat(BuffHMI,Min);
    strcat(BuffHMI,",");
    strcat(BuffHMI,Max);
    return BuffHMI;
}

static uint8_t * SendInstruct_add(uint8_t * ID,uint8_t * ch,uint8_t * val)
{
    strcpy(BuffHMI,Instruct_add);
    strcat(BuffHMI," ");
    strcat(BuffHMI,ID);
    strcat(BuffHMI,",");
    strcat(BuffHMI,ch);
    strcat(BuffHMI,",");
    strcat(BuffHMI,val);
    return BuffHMI;
}

static uint8_t * SendInstruct_cle(uint8_t * ID,uint8_t * ch)
{
    strcpy(BuffHMI,Instruct_cle);
    strcat(BuffHMI," ");
    strcat(BuffHMI,ID);
    strcat(BuffHMI,",");
    strcat(BuffHMI,ch);
    return BuffHMI;
}

static uint8_t * SendInstruct_addt(uint8_t * ID,uint8_t * ch,uint8_t * qyt)
{
    strcpy(BuffHMI,Instruct_addt);
    strcat(BuffHMI," ");
    strcat(BuffHMI,ID);
    strcat(BuffHMI,",");
    strcat(BuffHMI,ch);
    strcat(BuffHMI,",");
    strcat(BuffHMI,qyt);
    return BuffHMI;
}

static uint8_t * SendInstruct_dovents(void)
{
    return Instruct_dovents;
}

static uint8_t * SendInstruct_sendme(void)
{
    return Instruct_sendme;
}

static uint8_t * SendInstruct_cov(uint8_t * att1,uint8_t * att2,uint8_t * length)
{
    strcpy(BuffHMI,Instruct_cov);
    strcat(BuffHMI," ");
    strcat(BuffHMI,att1);
    strcat(BuffHMI,",");
    strcat(BuffHMI,att2);
    strcat(BuffHMI,",");
    strcat(BuffHMI,length);
    return BuffHMI;
}

static uint8_t * SendInstruct_strlen(uint8_t * att0,uint8_t * att1)
{
    strcpy(BuffHMI,Instruct_strlen);
    strcat(BuffHMI," ");
    strcat(BuffHMI,att0);
    strcat(BuffHMI,",");
    strcat(BuffHMI,att1);
    return BuffHMI;
}

static uint8_t * SendInstruct_touch_j(void)
{
    return Instruct_touch_j;
}

static uint8_t * SendInstruct_ref_stop(void)
{
    return Instruct_ref_stop;
}

static uint8_t * SendInstruct_ref_start(void)
{
    return Instruct_ref_start;
}

static uint8_t * SendInstruct_com_stop(void)
{
    return Instruct_com_stop;
}

static uint8_t * SendInstruct_com_start(void)
{
    return Instruct_com_start;
}

static uint8_t * SendInstruct_code_c(void)
{
    return Instruct_code_c;
}

static uint8_t * SendInstruct_rest(void)
{
    return Instruct_rest;
}

#endif

static result HMI_SendInstruction(uint8_t * Command)
{
    uint8_t Buf[30] = {0x00},Len = 0;
    
    strcpy(Buf,Command);
    
    Len = strlen(Buf);
    
    Buf[Len + 0] = 0xff;
    Buf[Len + 1] = 0xff;
    Buf[Len + 2] = 0xff;
    
    usart_1.d_puts(0,Buf,Len + 3);
    
    return true;
}

/*

向HMI设置数值，HMI会不会返回触摸热区或者是其他事件？NO

*/

static result SaveHMIValue(uint8_t PageID,uint8_t ItemID,uint8_t NewValue)
{
	uint8_t i = 0,j = 0;
	
	for(i = 0;i < sizeof(PageAll)/sizeof(PageAll[0]);i ++)
	{
		if(PageAll[i].ID == PageID)
		{
			for(j = 0;j < 6; j ++)
			{
				if(PageAll[i].Item[j].ID == ItemID)
				{
					PageAll[i].Item[j].Changed = 0xff;
					PageAll[i].Item[j].Value = NewValue;
				}
			}
			
		}
	}
	
	return true;
	
}

static result SyncPage(uint8_t PageID)
{
	uint8_t i = 0,j = 0,Buf[30] = {0x00},Buf_1[10] = {0x00};
	
	for(i = 0;i < sizeof(PageAll)/sizeof(PageAll[0]);i ++)
	{
		if(PageAll[i].ID == PageID)
		{
			for(j = 0;j < 6;j ++)
			{
                //if(PageAll[i].Item[j].Changed == 0xff)
                {
                    PageAll[i].Item[j].Changed = 0x00;
                    strcpy(Buf,PageAll[i].Item[j].Name);
                    strcat(Buf,".val=");
                    sprintf(Buf_1,"%d",PageAll[i].Item[j].Value);
                    strcat(Buf,Buf_1);
                    HMI_SendInstruction(Buf);
                }

				strcpy(Buf,PageAll[i].Item[j].Name);
				strcat(Buf,".val=");
				sprintf(Buf_1,"%d",PageAll[i].Item[j].Value);
				strcat(Buf,Buf_1);
				usart_1.d_puts(0,Buf,strlen(Buf));
				osDelay(50);
			}
		}
	}
}

result HMI_ExecInstruction(uint8_t * Data,uint8_t Length)
{
	
	if(Data == 0)return false;
	
	switch(*Data)
	{
		case HMI_RETURN_00:{return HMI_ReturnErr;}
		case HMI_RETURN_01:{return HMI_ReturnTrue;}
		case HMI_RETURN_02:{return HMI_ReturnErr;}
		case HMI_RETURN_03:{return HMI_ReturnErr;}
		case HMI_RETURN_04:{return HMI_ReturnErr;}
		case HMI_RETURN_05:{return HMI_ReturnErr;}
		case HMI_RETURN_11:{return HMI_ReturnErr;}
		case HMI_RETURN_12:{return HMI_ReturnErr;}
		case HMI_RETURN_1A:{return HMI_ReturnErr;}
		case HMI_RETURN_1B:{return HMI_ReturnErr;}
		case HMI_RETURN_1C:{return HMI_ReturnErr;}
		case HMI_RETURN_1D:{return HMI_ReturnErr;}
		case HMI_RETURN_1E:{return HMI_ReturnErr;}
		case HMI_RETURN_1F:{return HMI_ReturnErr;}
		case HMI_RETURN_20:{return HMI_ReturnErr;}
		case HMI_RETURN_23:{return HMI_ReturnErr;}
		case HMI_RETURN_HotArea			:{
											HMI_ProcotolHotArea_Typedef *HotArea = (HMI_ProcotolHotArea_Typedef*)Data;
											SaveHMIValue(HotArea->CurPageID,HotArea->ButtonID,HotArea->TouchEvent);
											break;
										}
		case HMI_RETURN_CurPageID		:{
											HMI_ReturnCurPageID_Typedef * PageID = (HMI_ReturnCurPageID_Typedef *)Data;
											SyncPage(PageID->CurPageID);
											break;
										}
		case HMI_RETURN_TouchPos			:{break;}
		case HMI_RETURN_SleepTouch		:{break;}
		case HMI_RETURN_ReturnStr		:{break;}
		case HMI_RETURN_ReturnVal		:{break;}
		case HMI_RETURN_AutoInterSleep	:{break;}
		case HMI_RETURN_AutoAware		:{break;}
		case HMI_RETURN_StartUpSuccess	:{break;}
		case HMI_RETURN_SdUpdate			:{break;}
		case HMI_RETURN_DataTransparentCpt:{break;}
		case HMI_RETURN_DataTransparentRdy:{break;}
		default:break;
	}
}




























