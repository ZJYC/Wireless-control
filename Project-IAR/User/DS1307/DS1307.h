#ifndef __DS1307_H__
#define __DS1307_H__

#ifdef __cplusplus
 extern "C" {
#endif
   
#define DS1307_I2C_PORT          GPIOB
#define DS1307_SCL_PIN           GPIO_PIN_10
#define DS1307_SDA_PIN           GPIO_PIN_11
     
#define DS1307_SCL_HIGH()        HAL_GPIO_WritePin(DS1307_I2C_PORT, DS1307_SCL_PIN,GPIO_PIN_SET);
#define DS1307_SCL_LOW()         HAL_GPIO_WritePin(DS1307_I2C_PORT, DS1307_SCL_PIN,GPIO_PIN_RESET);

#define DS1307_SDA_HIGH() 	    HAL_GPIO_WritePin(DS1307_I2C_PORT, DS1307_SDA_PIN,GPIO_PIN_SET);
#define DS1307_SDA_LOW()         HAL_GPIO_WritePin(DS1307_I2C_PORT, DS1307_SDA_PIN,GPIO_PIN_RESET);

#define DS1307_SCL_STATE()       HAL_GPIO_ReadPin(DS1307_I2C_PORT, DS1307_SCL_PIN)
#define DS1307_SDA_STATE()       HAL_GPIO_ReadPin(DS1307_I2C_PORT, DS1307_SDA_PIN)

#define ADDR_SEC		0x00
#define ADDR_MIN		0x01
#define ADDR_HOUR		0x02
#define	ADDR_WDAY		0x03
#define ADDR_DATE		0x04
#define ADDR_MON		0x05
#define ADDR_YEAR		0x06
#define ADDR_ALL        0x07

typedef struct
{
	 uint8_t sec;
	 uint8_t min;
	 uint8_t hour;
	 uint8_t wday;
	 uint8_t day;
	 uint8_t mon;
	 uint8_t year;
}DS1307_Time;

extern deviceModule DS1307;
extern DS1307_Time Ds1307Time;
#ifdef __cplusplus
}
#endif

#endif
