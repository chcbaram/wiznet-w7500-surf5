#include "uart.h"
#include "qbuffer.h"
#include "cli.h"
#ifdef _USE_HW_USB
#include "cdc.h"
#endif

#ifdef _USE_HW_UART


#define UART_RX_BUF_LENGTH        256



typedef struct
{
  bool is_open;
  uint32_t baud;

  uint8_t  rx_buf[UART_RX_BUF_LENGTH];
  qbuffer_t qbuffer;
  UART_TypeDef   *p_huart;
  UART_InitTypeDef *p_huart_init;
  uint32_t rx_cnt;
  uint32_t tx_cnt;
} uart_tbl_t;

typedef struct
{
  const char         *p_msg;

  UART_TypeDef       *p_huart;
  UART_InitTypeDef   *p_huart_init;
} uart_hw_t;



#ifdef _USE_HW_CLI
static void cliUart(cli_args_t *args);
#endif


static bool is_init = false;

static uart_tbl_t uart_tbl[UART_MAX_CH];

static const uart_hw_t uart_hw_tbl[UART_MAX_CH] = 
  {
    {"UART2 DBG   ", NULL, NULL},
  };





bool uartInit(void)
{
  for (int i=0; i<UART_MAX_CH; i++)
  {
    uart_tbl[i].is_open = false;
    uart_tbl[i].baud = 57600;
    uart_tbl[i].rx_cnt = 0;
    uart_tbl[i].tx_cnt = 0;    
  }

  is_init = true;

#ifdef _USE_HW_CLI
  cliAdd("uart", cliUart);
#endif
  return true;
}

bool uartDeInit(void)
{
  return true;
}

bool uartIsInit(void)
{
  return is_init;
}

bool uartOpen(uint8_t ch, uint32_t baud)
{
  bool ret = false;
  NVIC_InitTypeDef NVIC_InitStructure;


  if (ch >= UART_MAX_CH) return false;

  if (uart_tbl[ch].is_open == true && uart_tbl[ch].baud == baud)
  {
    return true;
  }


  switch(ch)
  {
    case _DEF_UART1:
      uart_tbl[ch].baud      = baud;

      
      uart_tbl[ch].p_huart   = uart_hw_tbl[ch].p_huart;
      uart_tbl[ch].p_huart_init = uart_hw_tbl[ch].p_huart_init;

      qbufferCreate(&uart_tbl[ch].qbuffer, &uart_tbl[ch].rx_buf[0], UART_RX_BUF_LENGTH);

      S_UART_DeInit();
      S_UART_Init(baud);

      S_UART_Cmd(ENABLE);
      S_UART_ITConfig(S_UART_CTRL_FLAG_RXI, ENABLE);

      NVIC_InitStructure.NVIC_IRQChannel = UART2_IRQn;
      NVIC_InitStructure.NVIC_IRQChannelPriority = 0x0;
      NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
      NVIC_Init(&NVIC_InitStructure);

      ret = true;
      uart_tbl[ch].is_open = true;
      break;

    case _DEF_UART2:
      uart_tbl[ch].baud    = baud;
      uart_tbl[ch].is_open = true;
      ret = true;
      break;      
  }

  return ret;
}

bool uartClose(uint8_t ch)
{
  if (ch >= UART_MAX_CH) return false;

  uart_tbl[ch].is_open = false;

  return true;
}

uint32_t uartAvailable(uint8_t ch)
{
  uint32_t ret = 0;

  ret = qbufferAvailable(&uart_tbl[ch].qbuffer);      

  return ret;
}

bool uartFlush(uint8_t ch)
{
  uint32_t pre_time;


  pre_time = millis();
  while(uartAvailable(ch))
  {
    if (millis()-pre_time >= 10)
    {
      break;
    }
    uartRead(ch);
  }

  return true;
}

uint8_t uartRead(uint8_t ch)
{
  uint8_t ret = 0;

  qbufferRead(&uart_tbl[ch].qbuffer, &ret, 1);
  uart_tbl[ch].rx_cnt++;

  return ret;
}

uint32_t uartWrite(uint8_t ch, uint8_t *p_data, uint32_t length)
{
  uint32_t tx_cnt = 0;
  uint32_t pre_time;
  UART_TypeDef *p_huart = uart_tbl[ch].p_huart;


  // p_huart가 NULL이면 S_UART를 사용해야 한다. 레지스터가 
  // 일반 UART하고 같지 않아서 별도 함수를 이용해야함.
  //
  if (p_huart == NULL)
  {
    pre_time = millis();
    while(tx_cnt < length)
    {
      if (S_UART_GetFlagStatus(S_UART_FLAG_TXF) == RESET)
      {
        UART2->DR = p_data[tx_cnt];
        tx_cnt++;
      }

      if (millis()-pre_time >= 100)
      {
        break;
      }
    }
  }
  else
  {
    pre_time = millis();
    while(tx_cnt < length)
    {
      if (UART_GetFlagStatus(p_huart, UART_FLAG_TXFF) == RESET)
      {
        p_huart->DR = p_data[tx_cnt];
        tx_cnt++;
      }

      if (millis()-pre_time >= 100)
      {
        break;
      }
    }
  }

  uart_tbl[ch].tx_cnt += tx_cnt;

  return tx_cnt;
}

uint32_t uartVPrintf(uint8_t ch, const char *fmt, va_list arg)
{
  uint32_t ret = 0;
  char print_buf[256];


  int len;
  len = vsnprintf(print_buf, 256, fmt, arg);

  if (len > 0)
  {
    ret = uartWrite(ch, (uint8_t *)print_buf, len);
  }

  return ret;
}

uint32_t uartPrintf(uint8_t ch, const char *fmt, ...)
{
  char buf[256];
  va_list args;
  int len;
  uint32_t ret;

  va_start(args, fmt);
  len = vsnprintf(buf, 256, fmt, args);

  ret = uartWrite(ch, (uint8_t *)buf, len);

  va_end(args);


  return ret;
}

uint32_t uartGetBaud(uint8_t ch)
{
  uint32_t ret = 0;


  if (ch >= UART_MAX_CH) return 0;

  #ifdef _USE_HW_USB
  if (ch == HW_UART_CH_USB)
    ret = cdcGetBaud();
  else
    ret = uart_tbl[ch].baud;
  #else
  ret = uart_tbl[ch].baud;
  #endif
  
  return ret;
}

uint32_t uartGetRxCnt(uint8_t ch)
{
  if (ch >= UART_MAX_CH) return 0;

  return uart_tbl[ch].rx_cnt;
}

uint32_t uartGetTxCnt(uint8_t ch)
{
  if (ch >= UART_MAX_CH) return 0;

  return uart_tbl[ch].tx_cnt;
}

void uartReceiveISR(const uint8_t ch)
{
  if (uart_tbl[ch].p_huart == NULL)
  {
    if (S_UART_GetITStatus(S_UART_ISR_RXI) == SET) 
    {
      S_UART_ClearITPendingBit(S_UART_ISR_RXI);

      uint8_t rx_data;
      rx_data = S_UART_ReceiveData();
      qbufferWrite(&uart_tbl[ch].qbuffer, &rx_data, 1);    
    }
  }
  else
  {
    if (UART_GetITStatus(uart_tbl[ch].p_huart, UART_IT_RXIM) == SET) 
    {
      UART_ClearITPendingBit(uart_tbl[ch].p_huart, UART_IT_RXIM);

      uint8_t rx_data;
      rx_data = UART_ReceiveData(UART0);
      qbufferWrite(&uart_tbl[ch].qbuffer, &rx_data, 1);    
    }
  }
}

void UART2_Handler(void)
{
  uartReceiveISR(_DEF_UART1);
}

#ifdef _USE_HW_CLI
void cliUart(cli_args_t *args)
{
  bool ret = false;


  if (args->argc == 1 && args->isStr(0, "info"))
  {
    for (int i=0; i<UART_MAX_CH; i++)
    {
      cliPrintf("_DEF_UART%d : %s, %d bps\n", i+1, uart_hw_tbl[i].p_msg, uartGetBaud(i));
    }
    ret = true;
  }

  if (args->argc == 2 && args->isStr(0, "test"))
  {
    uint8_t uart_ch;

    uart_ch = constrain(args->getData(1), 1, UART_MAX_CH) - 1;

    if (uart_ch != cliGetPort())
    {
      uint8_t rx_data;

      while(1)
      {
        if (uartAvailable(uart_ch) > 0)
        {
          rx_data = uartRead(uart_ch);
          cliPrintf("<- _DEF_UART%d RX : 0x%X\n", uart_ch + 1, rx_data);
        }

        if (cliAvailable() > 0)
        {
          rx_data = cliRead();
          if (rx_data == 'q')
          {
            break;
          }
          else
          {
            uartWrite(uart_ch, &rx_data, 1);
            cliPrintf("-> _DEF_UART%d TX : 0x%X\n", uart_ch + 1, rx_data);            
          }
        }
      }
    }
    else
    {
      cliPrintf("This is cliPort\n");
    }
    ret = true;
  }

  if (ret == false)
  {
    cliPrintf("uart info\n");
    cliPrintf("uart test ch[1~%d]\n", HW_UART_MAX_CH);
  }
}
#endif


#endif