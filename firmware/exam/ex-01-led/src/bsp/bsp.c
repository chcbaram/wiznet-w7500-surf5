#include "bsp.h"



static volatile uint32_t systick_counter = 0;

void SysTick_Handler(void)
{
  systick_counter++;
}


bool bspInit(void)
{
  volatile uint32_t system_core_clock;


  system_core_clock = GetSystemClock();

  SysTick_Config(system_core_clock/1000);  
  NVIC_SetPriority(SysTick_IRQn, 0);

  return true;
}

void delay(uint32_t time_ms)
{
  uint32_t pre_time = systick_counter;

  while(systick_counter-pre_time < time_ms);  
}

uint32_t millis(void)
{
  return systick_counter;
}



