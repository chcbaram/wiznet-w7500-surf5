#include "ap.h"
#include "event.h"



bool eventFunc(event_t *evt)
{
  if (evt->code == EVENT_LED)
  {
    logPrintf("led event : %d\n", evt->data);
  }
  return true;
}

void apInit(void)
{
  cliOpen(HW_UART_CH_CLI, 115200);
  
  eventSub(eventFunc);
}

void apMain(void)
{
  uint32_t pre_time;

  pre_time = millis();
  while(1)
  {
    if (millis()-pre_time >= 500)
    {
      pre_time = millis();
      ledToggle(_DEF_LED1);
      eventPub(EVENT_LED, 0);
    }

    cliMain();   
    eventUpdate();
  }
}

