
  .syntax unified
	.cpu cortex-m0
	.fpu softvfp
	.thumb

.global	g_pfnVectors
.global	Default_Handler

/* start address for the initialization values of the .data section.
defined in linker script */
.word _sidata
/* start address for the .data section. defined in linker script */
.word _sdata
/* end address for the .data section. defined in linker script */
.word _edata
/* start address for the .bss section. defined in linker script */
.word _sbss
/* end address for the .bss section. defined in linker script */
.word _ebss

  .section .text.Reset_Handler
  .weak Reset_Handler
  .type Reset_Handler, %function
Reset_Handler:
  ldr   r0, =_estack
  mov   sp, r0          /* set stack pointer */

/* Copy the data segment initializers from flash to SRAM */
  movs r1, #0
  b LoopCopyDataInit

CopyDataInit:
  ldr r3, =_sidata
  ldr r3, [r3, r1]
  str r3, [r0, r1]
  adds r1, r1, #4

LoopCopyDataInit:
  ldr r0, =_sdata
  ldr r3, =_edata
  adds r2, r0, r1
  cmp r2, r3
  bcc CopyDataInit
  ldr r2, =_sbss
  b LoopFillZerobss
/* Zero fill the bss segment. */
FillZerobss:
  movs r3, #0
  str  r3, [r2]
  adds r2, r2, #4


LoopFillZerobss:
  ldr r3, = _ebss
  cmp r2, r3
  bcc FillZerobss

/* Call the clock system initialization function.*/
  bl  SystemInit
/* Call static constructors */
  bl __libc_init_array
/* Call the application's entry point.*/
  bl main

LoopForever:
    b LoopForever


.size Reset_Handler, .-Reset_Handler

/**
 * @brief  This is the code that gets called when the processor receives an
 *         unexpected interrupt.  This simply enters an infinite loop, preserving
 *         the system state for examination by a debugger.
 *
 * @param  None
 * @retval : None
*/
    .section .text.Default_Handler,"ax",%progbits
Default_Handler:
Infinite_Loop:
  b Infinite_Loop
  .size Default_Handler, .-Default_Handler
/******************************************************************************
*
* The minimal vector table for a Cortex M0.  Note that the proper constructs
* must be placed on this to ensure that it ends up at physical address
* 0x0000.0000.
*
******************************************************************************/
   .section .isr_vector,"a",%progbits
  .type g_pfnVectors, %object
  .size g_pfnVectors, .-g_pfnVectors


g_pfnVectors:
  .word  _estack
  .word  Reset_Handler
  .word  NMI_Handler
  .word  HardFault_Handler
  .word  0
  .word  0
  .word  0
  .word  0
  .word  0
  .word  0
  .word  0
  .word  SVC_Handler
  .word  0
  .word  0
  .word  PendSV_Handler
  .word  SysTick_Handler

  /* External Interrupts */  
  .word  SSP0_Handler                       /* 16+ 0: SSP 0 Handler                   */
  .word  SSP1_Handler                       /* 16+ 1: SSP 1 Handler                   */
  .word  UART0_Handler                      /* 16+ 2: UART 0 Handler                  */
  .word  UART1_Handler                      /* 16+ 3: UART 1 Handler                  */
  .word  UART2_Handler                      /* 16+ 4: UART 2 Handler                  */
  .word  I2C0_Handler                       /* 16+ 5: I2C 0 Handler                   */
  .word  I2C1_Handler                       /* 16+ 6: I2C 1 Handler                   */
  .word  PORT0_Handler                      /* 16+ 7: GPIO Port 0 Combined Handler    */
  .word  PORT1_Handler                      /* 16+ 8: GPIO Port 1 Combined Handler    */
  .word  PORT2_Handler                      /* 16+ 9: GPIO Port 2 Combined Handler    */
  .word  PORT3_Handler                      /* 16+10: GPIO Port 3 Combined Handler    */
  .word  DMA_Handler		                    /* 16+11: DMA Combined Handler            */
  .word  DUALTIMER0_Handler                 /* 16+12: Dual timer 0 handler            */ 
  .word  DUALTIMER1_Handler		              /* 16+ 13: Dual timer 1 Handler	          */
  .word  PWM0_Handler		                    /* 16+ 14: PWM0 Handler		                */
  .word  PWM1_Handler		                    /* 16+ 15: PWM1 Handler		                */
  .word  PWM2_Handler		                    /* 16+ 16: PWM2 Handler		                */
  .word  PWM3_Handler		                    /* 16+ 17: PWM3 Handler		                */
  .word  PWM4_Handler		                    /* 16+ 18: PWM4 Handler		                */
  .word  PWM5_Handler		                    /* 16+ 19: PWM5 Handler		                */
  .word  PWM6_Handler		                    /* 16+ 20: PWM6 Handler		                */
  .word  PWM7_Handler		                    /* 16+ 21: PWM7 Handler		                */
  .word  RTC_Handler		                    /* 16+ 22: RTC Handler			              */
  .word  ADC_Handler		                    /* 16+ 23: ADC Handler			              */
  .word  WZTOE_Handler                      /* 16+ 24: WZTOE Handler		              */
  .word  EXTI_Handler                       /* 16+ 25: EXTI Handler                   */


/*******************************************************************************
*
* Provide weak aliases for each Exception handler to the Default_Handler.
* As they are weak aliases, any function with the same name will override
* this definition.
*
*******************************************************************************/

  .weak      NMI_Handler
  .thumb_set NMI_Handler,Default_Handler

  .weak      HardFault_Handler
  .thumb_set HardFault_Handler,Default_Handler

  .weak      SVC_Handler
  .thumb_set SVC_Handler,Default_Handler

  .weak      PendSV_Handler
  .thumb_set PendSV_Handler,Default_Handler

  .weak      SysTick_Handler
  .thumb_set SysTick_Handler,Default_Handler

  /* External Interrupts */  
  .weak      SSP0_Handler
  .thumb_set SSP0_Handler,Default_Handler

  .weak      SSP1_Handler
  .thumb_set SSP1_Handler,Default_Handler

  .weak      UART0_Handler
  .thumb_set UART0_Handler,Default_Handler

  .weak      UART1_Handler
  .thumb_set UART1_Handler,Default_Handler

  .weak      UART2_Handler
  .thumb_set UART2_Handler,Default_Handler

  .weak      I2C0_Handler
  .thumb_set I2C0_Handler,Default_Handler

  .weak      I2C1_Handler
  .thumb_set I2C1_Handler,Default_Handler

  .weak      PORT0_Handler
  .thumb_set PORT0_Handler,Default_Handler

  .weak      PORT1_Handler
  .thumb_set PORT1_Handler,Default_Handler

  .weak      PORT2_Handler
  .thumb_set PORT2_Handler,Default_Handler

  .weak      PORT3_Handler
  .thumb_set PORT3_Handler,Default_Handler

  .weak      DMA_Handler
  .thumb_set DMA_Handler,Default_Handler

  .weak      DUALTIMER0_Handler
  .thumb_set DUALTIMER0_Handler,Default_Handler

  .weak      DUALTIMER1_Handler
  .thumb_set DUALTIMER1_Handler,Default_Handler

  .weak      PWM0_Handler
  .thumb_set PWM0_Handler,Default_Handler

  .weak      PWM1_Handler
  .thumb_set PWM1_Handler,Default_Handler

  .weak      PWM2_Handler
  .thumb_set PWM2_Handler,Default_Handler

  .weak      PWM3_Handler
  .thumb_set PWM3_Handler,Default_Handler

  .weak      PWM4_Handler
  .thumb_set PWM4_Handler,Default_Handler

  .weak      PWM5_Handler
  .thumb_set PWM5_Handler,Default_Handler

  .weak      PWM6_Handler
  .thumb_set PWM6_Handler,Default_Handler

  .weak      PWM7_Handler
  .thumb_set PWM7_Handler,Default_Handler  

  .weak      RTC_Handler
  .thumb_set RTC_Handler,Default_Handler  

  .weak      ADC_Handler
  .thumb_set ADC_Handler,Default_Handler    

  .weak      WZTOE_Handler
  .thumb_set WZTOE_Handler,Default_Handler    

  .weak      EXTI_Handler
  .thumb_set EXTI_Handler,Default_Handler    
