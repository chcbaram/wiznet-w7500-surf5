#include "led.h"


#ifdef _USE_HW_LED
#include "cli.h"


const typedef struct 
{
  GPIO_TypeDef *port;
  uint32_t      pin;
  BitAction     on_state;
  BitAction     off_state;
} led_tbl_t;


#ifdef _USE_HW_CLI
static void cliLed(cli_args_t *args);
#endif


static const led_tbl_t led_tbl[LED_MAX_CH] = 
{
  {GPIOC, GPIO_Pin_15,  Bit_RESET, Bit_SET},      // 0. DEBUG
};

#ifdef _USE_HW_CLI
static const char *led_name[LED_MAX_CH+1] = 
{
  "0_DEBUG",   
  "Unknown",
};
#endif


bool ledInit(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;


  GPIO_InitStructure.GPIO_Direction = GPIO_Direction_OUT;
  GPIO_InitStructure.GPIO_Pad = GPIO_OpenDrainDisable | GPIO_HighDrivingStrength | GPIO_PuPd_NOPULL;  
  GPIO_InitStructure.GPIO_AF = PAD_AF1;

  for (int i=0; i<LED_MAX_CH; i++)
  {
    GPIO_InitStructure.GPIO_Pin = led_tbl[i].pin;
    GPIO_Init(led_tbl[i].port, &GPIO_InitStructure);
    ledOff(i);
  }

#ifdef _USE_HW_CLI
  cliAdd("led", cliLed);
#endif
  return true;
}

void ledOn(uint8_t ch)
{
  if (ch >= LED_MAX_CH) return;

  GPIO_WriteBit(led_tbl[ch].port, led_tbl[ch].pin, led_tbl[ch].on_state);
}

void ledOff(uint8_t ch)
{
  if (ch >= LED_MAX_CH) return;

  GPIO_WriteBit(led_tbl[ch].port, led_tbl[ch].pin, led_tbl[ch].off_state);
}

void ledToggle(uint8_t ch)
{
  if (ch >= LED_MAX_CH) return;

  GPIO_ToggleBits(led_tbl[ch].port, led_tbl[ch].pin);
}

#ifdef _USE_HW_CLI
void cliLed(cli_args_t *args)
{
  bool ret = false;


  if (args->argc == 1 && args->isStr(0, "info"))
  {
    for (int i=0; i<LED_MAX_CH; i++)
    {
      cliPrintf("%-12s \n", led_name[i]);
    }
    ret = true;
  }

  if (args->argc == 3 && args->isStr(0, "toggle"))
  {
    uint8_t  ch;
    uint32_t toggle_time;
    uint32_t pre_time;

    ch = (uint8_t)args->getData(1);
    ch = constrain(ch, 0, LED_MAX_CH-1);
    toggle_time = (uint32_t)args->getData(2);

    pre_time = millis();
    while(cliKeepLoop())
    {
      if (millis()-pre_time >= toggle_time)
      {
        pre_time = millis();
        ledToggle(ch);
      }
    }
    ledOff(ch);
    ret = true;
  }

  if (ret == false)
  {
    cliPrintf("led info\n");
    cliPrintf("led toggle 0~%d ms\n", LED_MAX_CH);
  }
}
#endif

#endif