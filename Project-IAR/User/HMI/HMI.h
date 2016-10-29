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

/*变量定义*/

typedef struct ItemTypedef_
{
    uint8_t Changed;
    uint8_t Name[4];
    uint8_t ID;
    uint16_t Value;
}ItemTypedef;

typedef struct PageInfTypedef_
{
    ItemTypedef ItemCheckBox[3];
    ItemTypedef ItemValue[3];
}PageInfTypedef;

/*变量声明*/

extern deviceModule HMI;



/*函数声明*/






