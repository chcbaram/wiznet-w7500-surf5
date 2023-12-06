#ifndef HW_DEF_H_
#define HW_DEF_H_



#include "bsp.h"


#define _DEF_FIRMWATRE_VERSION    "V231202R1"
#define _DEF_BOARD_NAME           "WIZNET-SURF5-FW"



#define _USE_HW_LED
#define      HW_LED_MAX_CH          1
#define      HW_LED_CH_DEBUG        _DEF_LED1

#define _USE_HW_UART
#define      HW_UART_MAX_CH         1
#define      HW_UART_CH_DBG         _DEF_UART1

#define _USE_HW_LOG
#define      HW_LOG_CH              HW_UART_CH_DBG


#endif