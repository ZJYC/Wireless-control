
#ifndef __KEY_H__
#define __KEY_H__

/*
****************************************************
*  File name          : Key.h
*  Author             : 5A4A5943
*  Version            : V1.0
*  Date               : 2016--08--27--11--03--51
*  Description        : ����(ADC����)���
*  Function List      : 
						ADC_KeyInit();
						ADC_KeyProcess();
						ADC_KeyGetKey();
						
*  History            : 
*****************************************************
*/

#include "Board.h"

#define NumOfKey	4
#define ADCPort		GPIOx
#define ADCPin		PINx
#define ADCBufDeep	12
#define ADC_DX_MAX	1000

typedef enum KeyValueEnum_
{
	N = 0,
	B = 1,
	A = 2,
	D = 3,
	C = 4
}KeyValueEnum;	

typedef struct ADC_KeyStruction
{
	/* ��������ֵ������ */
	uint16_t KeyADC_Value[ADCBufDeep];
	/* ���������˲�ֵ */
	uint16_t KeyADC_ValueAverage;
	/* ��������ֵ������ָ�� */
	uint8_t  KeyADC_ValuePoint;
	/* �����߽� */
	uint16_t KeyADC_Boundary[NumOfKey + 1];
	
	KeyValueEnum NewKeyValue;
	KeyValueEnum OldKeyValue;
	
}ADC_KeyStruct,*p_ADC_KeyStruct;

extern deviceModule d_ADC_Key;








#endif






























