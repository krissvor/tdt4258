#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "ex2.h"


static uint16_t playing = 0;

static uint16_t count = 0;
static uint16_t sample = 0;
const static uint16_t low = 0;
const static uint16_t high = (1<<12) - 1;


/* TIMER1 interrupt handler */
void __attribute__ ((interrupt)) TIMER1_IRQHandler() 
{
  *TIMER1_IFC = 1;
  count++;
  if (count >= 100)
    {
      count = 0;
      sample = (sample == low ? high : low);
    }

  if (playing) 
    {
      *DAC0_CH0DATA = sample;
      *DAC0_CH1DATA = sample;
    }
  else 
    {
      *DAC0_CH0DATA = 0;
      *DAC0_CH1DATA = 0;
    }
}


/* GPIO even pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler() 
{
  ButtonHandler();
}


/* GPIO odd pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler() 
{
  ButtonHandler();
}


void ButtonHandler() 
{
  *GPIO_IFC = 0xFF;
  *GPIO_PA_DOUT = *GPIO_PC_DIN << 8;
  if (~(*GPIO_PC_DIN) & 0xff)
    {
      playing = 1;
    }
  else
    {
      playing = 0;
    }
}


/* if other interrupt handlers are needed, use the following names:
   NMI_Handler
   HardFault_Handler
   MemManage_Handler
   BusFault_Handler
   UsageFault_Handler
   Reserved7_Handler
   Reserved8_Handler
   Reserved9_Handler
   Reserved10_Handler
   SVC_Handler
   DebugMon_Handler
   Reserved13_Handler
   PendSV_Handler
   SysTick_Handler
   DMA_IRQHandler
   GPIO_EVEN_IRQHandler
   TIMER0_IRQHandler
   USART0_RX_IRQHandler
   USART0_TX_IRQHandler
   USB_IRQHandler
   ACMP0_IRQHandler
   ADC0_IRQHandler
   DAC0_IRQHandler
   I2C0_IRQHandler
   I2C1_IRQHandler
   GPIO_ODD_IRQHandler
   TIMER1_IRQHandler
   TIMER2_IRQHandler
   TIMER3_IRQHandler
   USART1_RX_IRQHandler
   USART1_TX_IRQHandler
   LESENSE_IRQHandler
   USART2_RX_IRQHandler
   USART2_TX_IRQHandler
   UART0_RX_IRQHandler
   UART0_TX_IRQHandler
   UART1_RX_IRQHandler
   UART1_TX_IRQHandler
   LEUART0_IRQHandler
   LEUART1_IRQHandler
   LETIMER0_IRQHandler
   PCNT0_IRQHandler
   PCNT1_IRQHandler
   PCNT2_IRQHandler
   RTC_IRQHandler
   BURTC_IRQHandler
   CMU_IRQHandler
   VCMP_IRQHandler
   LCD_IRQHandler
   MSC_IRQHandler
   AES_IRQHandler
   EBI_IRQHandler
   EMU_IRQHandler
*/




