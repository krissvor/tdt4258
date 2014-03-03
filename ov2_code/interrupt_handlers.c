#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "ex2.h"
#include "synth.h"
#include "sounds.h"
#include "g2lib.h"

static uint16_t playing = 0;

static uint16_t sample_idx = 0;
static int16_t note_idx = -1;
static uint16_t sample = 0;

static sound_t current_sound;

/* TIMER1 interrupt handler */
void __attribute__ ((interrupt)) TIMER1_IRQHandler() 
{
  *TIMER1_IFC = 1;

  if (playing) 
    {
      if (sample_idx % current_sound.note_duration == 0)
        {
          sample_idx = 0;
          note_idx++;
	  if (note_idx < current_sound.note_count)
            {
	      square1_play_note(current_sound.sq1_notes[note_idx]);
	      square2_play_note(current_sound.sq2_notes[note_idx]);
	      triangle_play_note(current_sound.tri_notes[note_idx]);
	      noise_play(current_sound.noise_notes[note_idx]);
            } 
          else
            {
              playing = 0;
            }
       }
      sample = get_sample();
      sample_idx++;
      *DAC0_CH0DATA = sample;
      *DAC0_CH1DATA = sample;
    }
  else 
    {
      *GPIO_PA_DOUT = 0xFF00;
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
  if (GPIO_pollPin(GPIO_portC, 6))
    {
      current_sound = cannon;
      playing = 1;
      note_idx = -1;
      sample_idx = 0;
    }
  else if (GPIO_pollPin(GPIO_portC, 7))
    {
      current_sound = coin;
      playing = 1;
      note_idx = -1;
      sample_idx = 0;
    }
  else
    {

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




