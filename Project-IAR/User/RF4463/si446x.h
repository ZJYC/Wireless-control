#ifndef _SI446X_H_
#define _SI446X_H_

#include "mytypedef.h"

#include "SI446X_defs.h"
#include "board.h"

/*===========================================================================
-----------------------------External IMPORT functions---------------------------
============================================================================*/

#include "stm32f1xx_hal.h"  //BSP里面包含了Si446X所用的相关函数。

/*===========================================================================
----------------------------INTERNAL EXPORT APIs-----------------------------
============================================================================*/

#define  PACKET_LENGTH      64 //0-64, if = 0: variable mode, else: fixed mode

#define PORT_SI_CSN     GPIOB
#define PIN_SI_CSN      GPIO_PIN_12

#define PORT_SI_SDN     GPIOD
#define PIN_SI_SDN      GPIO_PIN_7

//#define PORT_SI_GIO0    GPIOC
//#define PIN_SI_GIO0     GPIO_PIN_0

//#define PORT_SI_GIO1    GPIOC
//#define PIN_SI_GIO1     GPIO_PIN_1

#define SI_CSN_LOW()   HAL_GPIO_WritePin(PORT_SI_CSN, PIN_SI_CSN,GPIO_PIN_RESET);SI4463_Delay(50);
#define SI_CSN_HIGH()  HAL_GPIO_WritePin(PORT_SI_CSN, PIN_SI_CSN,GPIO_PIN_SET);SI4463_Delay(50);

#define SI_SDN_LOW()   HAL_GPIO_WritePin(PORT_SI_SDN, PIN_SI_SDN,GPIO_PIN_RESET);
#define SI_SDN_HIGH()  HAL_GPIO_WritePin(PORT_SI_SDN, PIN_SI_SDN,GPIO_PIN_SET);

/*===========================================================================
----------------------------INTERNAL EXPORT APIs-----------------------------
============================================================================*/

/*Read the PART_INFO of the device, 8 bytes needed*/
INT8U SI446X_PART_INFO(INT8U *buffer);

/*Read the FUNC_INFO of the device, 7 bytes needed*/
INT8U SI446X_FUNC_INFO(INT8U *buffer);

/*Send a command to the device*/
INT8U SI446X_CMD(INT8U *cmd, INT8U cmdsize);

/*Read the INT status of the device, 9 bytes needed*/
INT8U SI446X_INT_STATUS(INT8U *buffer);

/*Read the PROPERTY of the device*/
INT8U SI446X_GET_PROPERTY_X(SI446X_PROPERTY GROUP_NUM, INT8U NUM_PROPS, INT8U *buffer);

/*configuration the device*/
INT8U SI446X_CONFIG_INIT(void);

/*reset the SI446x device*/
INT8U SI446X_RESET(void);

/*write data to TX fifo*/
INT8U SI446X_W_TX_FIFO(INT8U *txbuffer, INT8U size);

/*start TX command*/
INT8U SI446X_START_TX(INT8U channel, INT8U condition, INT16U tx_len);

/*read RX fifo*/
INT8U SI446X_READ_PACKET(INT8U *buffer);

/*start RX state*/
INT8U SI446X_START_RX(INT8U channel, INT8U condition, INT16U rx_len,
                      INT8U n_state1, INT8U n_state2, INT8U n_state3);

/*read packet information*/
INT8U SI446X_PKT_INFO(INT8U *buffer, INT8U FIELD, INT16U length, INT16U diff_len);

/*read fifo information*/
INT8U SI446X_FIFO_INFO(INT8U *buffer);

/*Power up the device*/
INT8U SI446X_POWER_UP(INT32U f_xtal);

/*send a packet*/
INT8U SI446X_SEND_PACKET(INT8U *txbuffer, INT8U size, INT8U channel, INT8U condition);

/*Set the PROPERTY of the device*/
INT8U SI446X_SET_PROPERTY_X(SI446X_PROPERTY GROUP_NUM, INT8U NUM_PROPS, INT8U *PAR_BUFF);

/*Read the PROPERTY of the device*/
INT8U SI446X_GET_PROPERTY_X(SI446X_PROPERTY GROUP_NUM, INT8U NUM_PROPS, INT8U *buffer );

/*config the CRC, PROPERTY 0x1200*/
INT8U SI446X_CRC_CONFIG(INT8U PKT_CRC_CONFIG);

/*Get the PROPERTY of the device, only 1 byte*/
INT8U SI446X_GET_PROPERTY_1(SI446X_PROPERTY GROUP_NUM);

/*Set the PROPERTY of the device, only 1 byte*/
INT8U SI446X_SET_PROPERTY_1(SI446X_PROPERTY GROUP_NUM, INT8U proirity);

/*config the GPIOs, IRQ, SDO*/
INT8U SI446X_GPIO_CONFIG(INT8U G0, INT8U G1, INT8U G2, INT8U G3,
                         INT8U IRQ, INT8U SDO, INT8U GEN_CONFIG);

/*reset the RX FIFO of the device*/
INT8U SI446X_RX_FIFO_RESET(void);

/*reset the TX FIFO of the device*/
INT8U SI446X_TX_FIFO_RESET(void);

/*change a new state*/
INT8U SI446X_CHANGE_STATE(INT8U NewState);

/*Get the chip status*/
INT8U SI446X_CHIP_STATUS(INT8U* buffer_8);

/*Set the SYNC bytes of the device*/
INT8U SI446X_SET_SYNC(INT32U sync);

/*Set the PA Power of the device*/
INT8U SI446X_SET_POWER(INT8U Power_Level);

INT8U SI446X_GET_DEVICE_STATE(void);
/*===========================================================================
--------------------------PROPERTY fast setting macros-----------------------
============================================================================*/
// GOLBAL(0x00)
#define GLOBAL_XO_TUNE(x)                 SI446X_SET_PROPERTY_1(0x0000, x)
#define GLOBAL_CLK_CFG(x)                 SI446X_SET_PROPERTY_1(0x0001, x)
#define GLOBAL_LOW_BATT_THRESH(x)         SI446X_SET_PROPERTY_1(0x0002, x)
#define GLOBAL_CONFIG(x)                  SI446X_SET_PROPERTY_1(0x0003, x)
#define GLOBAL_WUT_CONFIG(x)              SI446X_SET_PROPERTY_1(0x0004, x)
#define GLOBAL_WUT_M_15_8(x)              SI446X_SET_PROPERTY_1(0x0005, x)
#define GLOBAL_WUT_M_7_0(x)               SI446X_SET_PROPERTY_1(0x0006, x)
#define GLOBAL_WUT_R(x)                   SI446X_SET_PROPERTY_1(0x0007, x)
#define GLOBAL_WUT_LDC(x)                 SI446X_SET_PROPERTY_1(0x0008, x)
#define GLOBAL_WUT_CAL(x)                 SI446X_SET_PROPERTY_1(0x0009, x)

// INT_CTL(0x01)
#define INT_CTL_ENABLE(x)                 SI446X_SET_PROPERTY_1(0x0100, x)
#define INT_CTL_PH_ENABLE(x)              SI446X_SET_PROPERTY_1(0x0101, x)
#define INT_CTL_MODEM_ENABLE(x)           SI446X_SET_PROPERTY_1(0x0102, x)
#define INT_CTL_CHIP_ENABLE(x)            SI446X_SET_PROPERTY_1(0x0103, x)

//group 0x02, FRR_CTL
#define FRR_CTL_A_MODE(x)                 SI446X_SET_PROPERTY_1(0x0200, x)
#define FRR_CTL_B_MODE(x)                 SI446X_SET_PROPERTY_1(0x0201, x)
#define FRR_CTL_C_MODE(x)                 SI446X_SET_PROPERTY_1(0x0202, x)
#define FRR_CTL_D_MODE(x)                 SI446X_SET_PROPERTY_1(0x0203, x)

// PREAMBLE (0x10)

#endif //_SI446X_H_

/*===========================================================================
----------------------------------End of FILE--------------------------------
============================================================================*/
