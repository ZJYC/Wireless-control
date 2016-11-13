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
    true = 0xff//���ɴ���0xff��
} result;

typedef enum _Command
{
    Set_AddPrivateData      = 0,//���˽������
    Task_CalledPeriod       = 1 //��������
}Command;

typedef enum DeviceAttri_
{
	Private = 0,
	Normal = 10,
	Public = 100
}DeviceAttri;

typedef struct _peripherals
{
	/* �������� */
	uint8_t		name[16];
	/* ���豸�� */
	uint8_t		major;
	/* ���豸�� */
	uint8_t		mnor;
	/* ״̬ */
	uint16_t	state;
	/* ������-��ʼ�� */
	result (*p_open) (void);
	/* �ر����� */
	result (*p_close) (void);
	/* ������ƺ��� */
	result (*p_ioctl) (uint32_t type, uint32_t param);
	/* �������ȡ���������� */
	result (*p_gets) (uint8_t * start, uint32_t length);
	/* �����跢������������ */
	result (*p_puts) (uint8_t * start, uint32_t length);
	/* ��������ͬʱ������� */
	uint8_t (*p_get_put) (uint8_t temp);
	/* ��õ������� */
	uint8_t (*p_getc) (uint8_t temp);
	/* ���͵������� */
	uint8_t (*p_putc) (uint8_t temp);
}peripherals, *p_peripherals;

typedef struct _deviceModule
{
	/* �ַ������� */
	uint8_t					name[16];
	/* �豸״̬ */
	uint16_t				state;
	/* �ڵ� */
	struct _deviceModule	*next;
    
    uint8_t                 *data;
    uint16_t                DataLen;
    
	//�豸����
	DeviceAttri				DA;
	/* �� */
	result (*d_open) (void);
	/* �ر� */
	result (*d_close) (void);
	/* ����Ƿ���� */
	result (*d_detect) (void);
	/* ִ������ */
	result (*d_command) (uint8_t *, uint32_t);
	/* ���� */
	result (*d_set) (uint32_t, uint32_t);
	/* ��� */
	result (*d_puts) (uint32_t, uint8_t *, uint32_t);
	/* ��ȡ */
	result (*d_gets) (uint32_t, uint8_t *, uint32_t);
	/* ��ʱִ�в��� */
	result (*d_timing_proceee)(uint32_t, uint32_t, uint32_t);
	/* �ж�ִ�в��� */
	result (*d_process_it)(uint32_t, uint32_t, uint32_t);
    /* ���� */
    result (*Task)(uint32_t Type,uint32_t Param);
}deviceModule, *p_deviceModule;

void device_add(p_deviceModule new_device);
void device_delete(p_deviceModule del_device);
result AddPrivateBuf(p_deviceModule Device,uint8_t * Buf,uint32_t BufLen);
p_deviceModule device_find_local(uint8_t * name);
extern peripherals spi_1;
extern peripherals II2C_1;
#endif
