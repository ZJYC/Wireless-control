/*
****************************************************
*  文件名             : HMI.h
*  作者               : -5A4A5943-
*  版本               : V1.0.0
*  编写日期           : 2016--10--29--13
*  简介               : 
*  函数列表           : 
*  历史版本           : 
*****************************************************
*/


/*头文件  */





/*宏定义  */

#if 1 //instruction

#define Instruct_page       "page"
#define Instruct_ref        "ref"
#define Instruct_click      "click"
#define Instruct_get        "get"
#define Instruct_print      "print"
#define Instruct_printh     "printh"
#define Instruct_vis        "vis"
#define Instruct_tsw        "tsw"
#define Instruct_randset    "randset"
#define Instruct_add        "add"
#define Instruct_cle        "cle"
#define Instruct_addt       "addt"
#define Instruct_dovents    "dovents"
#define Instruct_sendme     "sendme"
#define Instruct_cov        "cov"
#define Instruct_strlen     "strlen"
#define Instruct_touch_j    "touch_j"
#define Instruct_ref_stop   "ref_stop"
#define Instruct_ref_start  "ref_start"
#define Instruct_com_stop   "com_stop"
#define Instruct_com_start  "com_start"
#define Instruct_code_c     "code_c"
#define Instruct_rest       "rest"

#endif

#if 1//

#define HMI_RETURN_00                   0x00//无效指令
#define HMI_RETURN_01                   0x01//指令成功执行
#define HMI_RETURN_02                   0x02//控件 ID 无效
#define HMI_RETURN_03                   0x03//页面 ID 无效
#define HMI_RETURN_04                   0x04//图片 ID 无效
#define HMI_RETURN_05                   0x05//字库 ID 无效
#define HMI_RETURN_11                   0x11//波特率设置无效
#define HMI_RETURN_12                   0x12//曲线控件 ID 号或通道号无效
#define HMI_RETURN_1A                   0x1A//变量名称无效
#define HMI_RETURN_1B                   0x1B//变量运算无效
#define HMI_RETURN_1C                   0x1C//赋值操作失败
#define HMI_RETURN_1D                   0x1D//EEPROM 操作失败
#define HMI_RETURN_1E                   0x1E//参数数量无效
#define HMI_RETURN_1F                   0x1F//IO 操作失败
#define HMI_RETURN_20                   0x20//转义字符使用错误
#define HMI_RETURN_23                   0x23//变量名称太长

#define HMI_RETURN_HotArea              0x65//触摸热区事件返回
#define HMI_RETURN_CurPageID            0x66//当前页面的 ID 号返回
#define HMI_RETURN_TouchPos             0x67//触摸坐标数据返回
#define HMI_RETURN_SleepTouch           0x68//睡眠模式触摸事件
#define HMI_RETURN_ReturnStr            0x70//字符串变量数据返回
#define HMI_RETURN_ReturnVal            0x71//数值变量数据返回
#define HMI_RETURN_AutoInterSleep       0x86//设备自动进入睡眠模式
#define HMI_RETURN_AutoAware            0x87//设备自动唤醒
#define HMI_RETURN_StartUpSuccess       0x88//系统启动成功
#define HMI_RETURN_SdUpdate             0x89//开始 SD 卡升级
#define HMI_RETURN_DataTransparentCpt   0xFD//透传数据完成
#define HMI_RETURN_DataTransparentRdy   0xFE//数据透传就绪

#define HMI_RETURN_TouchEventPress      0x01    
#define HMI_RETURN_TouchEventRelease    0x00            

__packed typedef struct HMI_ProcotolEnd_Typedef_
{
    //three 0xFF
    uint8_t Data[3];
}HMI_ProcotolEnd_Typedef;

//触摸热区事件返回
__packed typedef struct HMI_ReturnHotArea_Typedef_
{
    
    uint8_t Header;
    uint8_t CurPageID;
    uint8_t ButtonID;
    uint8_t TouchEvent;
    HMI_ProcotolEnd_Typedef ProcotolEnd;
    
}HMI_ProcotolHotArea_Typedef;

//当前页面的 ID 号返回
__packed typedef struct HMI_ReturnCurPageID_Typedef_
{
    
    uint8_t Header;
    uint8_t CurPageID;
    HMI_ProcotolEnd_Typedef ProcotolEnd;
    
}HMI_ReturnCurPageID_Typedef;

//触摸坐标数据返回
__packed typedef struct HMI_ReturnTouchPos_Typedef_
{
    
    uint8_t Header;
    uint8_t PosX_H;
    uint8_t PosX_L;
    uint8_t PosY_H;
    uint8_t PosY_L;
    uint8_t TouchEvent;
    HMI_ProcotolEnd_Typedef ProcotolEnd;
}HMI_ReturnTouchPos_Typedef;

//睡眠模式触摸事件
__packed typedef struct HMI_ReturnSleepTouch_Typedef_
{
    
    uint8_t Header;
    uint8_t PosX_H;
    uint8_t PosX_L;
    uint8_t PosY_H;
    uint8_t PosY_L;
    uint8_t TouchEvent;
    HMI_ProcotolEnd_Typedef ProcotolEnd;
    
}HMI_ReturnSleepTouch_Typedef;

//字符串变量数据返回
__packed typedef struct HMI_ReturnStr_Typedef_
{
    
    uint8_t Header;
    uint8_t Valirabal;//!!!!!!!!!
    HMI_ProcotolEnd_Typedef ProcotolEnd;
    
}HMI_ReturnStr_Typedef;

//数值变量数据返回
__packed typedef struct HMI_ReturnVal_Typedef_
{
    
    uint8_t Header;
    uint8_t Val[4];
    HMI_ProcotolEnd_Typedef ProcotolEnd;
    
}HMI_ReturnVal_Typedef;

//设备自动进入睡眠模式
__packed typedef struct HMI_AutoInterSleep_Typedef_
{
    
    uint8_t Header;
    HMI_ProcotolEnd_Typedef ProcotolEnd;
    
}HMI_AutoInterSleep_Typedef;

//设备自动唤醒
__packed typedef struct HMI_AutoAware_Typedef_
{
    
    uint8_t Header;
    HMI_ProcotolEnd_Typedef ProcotolEnd;
    
}HMI_AutoAware_Typedef;

//系统启动成功
__packed typedef struct HMI_StartUpSuccess_Typedef_
{
    
    uint8_t Header;
    HMI_ProcotolEnd_Typedef ProcotolEnd;
    
}HMI_StartUpSuccess_Typedef;

//开始 SD 卡升级
__packed typedef struct HMI_SdUpdate_Typedef_
{
    
    uint8_t Header;
    HMI_ProcotolEnd_Typedef ProcotolEnd;
    
}HMI_SdUpdate_Typedef;

//透传数据完成
__packed typedef struct HMI_DataTransparentCpt_Typedef_
{
    
    uint8_t Header;
    HMI_ProcotolEnd_Typedef ProcotolEnd;
    
}HMI_DataTransparentCpt_Typedef;

//数据透传就绪
__packed typedef struct HMI_DataTransparentRdy_Typedef_
{
    
    uint8_t Header;
    HMI_ProcotolEnd_Typedef ProcotolEnd;
    
}HMI_DataTransparentRdy_Typedef;
#endif

/*变量定义*/

// to represent one Item
__packed typedef struct ItemTypedef_
{
    uint8_t Changed;
    uint8_t Name[4];
    uint8_t ID;
    uint16_t Value;
    uint8_t DeviceName[16];
    uint8_t Board;
    uint8_t Param[16];
}ItemTypedef;

//to represent one page
__packed typedef struct PageInfTypedef_
{
    uint8_t Name[8];
    uint8_t ID;
    ItemTypedef Item[6];
}PageInfTypedef;

/*变量声明*/

extern deviceModule HMI;



/*函数声明*/

result HMI_Task(uint32_t Type,uint32_t Param);




