
#ifndef __KEY_H__
#define __KEY_H__

/*
****************************************************
*  File name          : Key.h
*  Author             : 5A4A5943
*  Version            : V1.0
*  Date               : 2016--08--27--11--03--51
*  Description        : 按键(ADC按键)检测
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
	/* 按键采样值缓冲区 */
	uint16_t KeyADC_Value[ADCBufDeep];
	/* 按键采样滤波值 */
	uint16_t KeyADC_ValueAverage;
	/* 按键采样值缓冲区指针 */
	uint8_t  KeyADC_ValuePoint;
	/* 按键边界 */
	uint16_t KeyADC_Boundary[NumOfKey + 1];
	
	KeyValueEnum NewKeyValue;
	KeyValueEnum OldKeyValue;
	
}ADC_KeyStruct,*p_ADC_KeyStruct;

extern deviceModule d_ADC_Key;








#endif






























