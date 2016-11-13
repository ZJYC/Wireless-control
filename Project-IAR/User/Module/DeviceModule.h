/*$T indentinput.cpp GC 1.140 07/23/16 10:26:06 */


/*$6
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */


#ifndef _DEVICEMODULE_H_
#define _DEVICEMODULE_H_

#include "stm32f1xx_hal.h"

#define STATE_OPEN				(1 << 0)
#define STATE_CLOSE				(1 << 2)
#define STATE_ERROR				(1 << 3)
#define STATE_TX				(1 << 4)
#define STATE_RX				(1 << 5)

#define CHECK_STATE(Var, Bit)	((Var) & (Bit))
#define SET_STATE(Var, Bit)		{ (Var) |= (Bit); }
#define RESET_STATE(Var, Bit)	{ (Var) &= ~(Bit); }

typedef enum _result 
{ 
    false = 0,
    HMI_ReturnErr = 1,
    HMI_ReturnTrue = 2,
    true = 0xff//不可大于0xff，
} result;

typedef enum _Command
{
    Set_AddPrivateData      = 0,//添加私有数据
    Task_CalledPeriod       = 1 //调用周期
}Command;

typedef enum DeviceAttri_
{
	Private = 0,
	Normal = 10,
	Public = 100
}DeviceAttri;

typedef struct _peripherals
{
	/* 外设名称 */
	uint8_t		name[16];
	/* 主设备号 */
	uint8_t		major;
	/* 从设备号 */
	uint8_t		mnor;
	/* 状态 */
	uint16_t	state;
	/* 打开外设-初始化 */
	result (*p_open) (void);
	/* 关闭外设 */
	result (*p_close) (void);
	/* 外设控制函数 */
	result (*p_ioctl) (uint32_t type, uint32_t param);
	/* 从外设获取连续的数据 */
	result (*p_gets) (uint8_t * start, uint32_t length);
	/* 想外设发送连续的数据 */
	result (*p_puts) (uint8_t * start, uint32_t length);
	/* 发送数据同时获得数据 */
	uint8_t (*p_get_put) (uint8_t temp);
	/* 获得单个数据 */
	uint8_t (*p_getc) (uint8_t temp);
	/* 发送耽搁数据 */
	uint8_t (*p_putc) (uint8_t temp);
}peripherals, *p_peripherals;

typedef struct _deviceModule
{
	/* 字符串名字 */
	uint8_t					name[16];
	/* 设备状态 */
	uint16_t				state;
	/* 节点 */
	struct _deviceModule	*next;
    
    uint8_t                 *data;
    uint16_t                DataLen;
    
	//设备属性
	DeviceAttri				DA;
	/* 打开 */
	result (*d_open) (void);
	/* 关闭 */
	result (*d_close) (void);
	/* 检测是否存在 */
	result (*d_detect) (void);
	/* 执行命令 */
	result (*d_command) (uint8_t *, uint32_t);
	/* 设置 */
	result (*d_set) (uint32_t, uint32_t);
	/* 输出 */
	result (*d_puts) (uint32_t, uint8_t *, uint32_t);
	/* 获取 */
	result (*d_gets) (uint32_t, uint8_t *, uint32_t);
	/* 定时执行部分 */
	result (*d_timing_proceee)(uint32_t, uint32_t, uint32_t);
	/* 中断执行部分 */
	result (*d_process_it)(uint32_t, uint32_t, uint32_t);
    /* 任务 */
    result (*Task)(uint32_t Type,uint32_t Param);
}deviceModule, *p_deviceModule;

void device_add(p_deviceModule new_device);
void device_delete(p_deviceModule del_device);
result AddPrivateBuf(p_deviceModule Device,uint8_t * Buf,uint32_t BufLen);
p_deviceModule device_find_local(uint8_t * name);
extern peripherals spi_1;
extern peripherals II2C_1;
#endif
