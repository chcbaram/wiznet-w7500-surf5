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
#define      HW_UART_CH_CLI         _DEF_UART1

#define _USE_HW_LOG
#define      HW_LOG_CH              HW_UART_CH_DBG

#define _USE_HW_CLI
#define      HW_CLI_CMD_LIST_MAX    16
#define      HW_CLI_CMD_NAME_MAX    16
#define      HW_CLI_LINE_HIS_MAX    8
#define      HW_CLI_LINE_BUF_MAX    64

#define _USE_HW_SWTIMER
#define      HW_SWTIMER_MAX_CH      8

#define _USE_HW_WIZNET
#define      HW_WIZNET_SOCKET_CMD   0
#define      HW_WIZNET_SOCKET_DHCP  1
#define      HW_WIZNET_SOCKET_SNTP  2

#define _USE_HW_EVENT
#define      HW_EVENT_Q_MAX         8
#define      HW_EVENT_NODE_MAX      16        



//-- CLI
//
#define _USE_CLI_HW_LED             1
#define _USE_CLI_HW_EVENT           1
#define _USE_CLI_HW_WIZNET          1

#endif