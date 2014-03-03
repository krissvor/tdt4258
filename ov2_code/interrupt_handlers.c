#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "ex2.h"
#include "synth.h"


static uint16_t playing = 0;

static uint16_t sample_idx = 0;
static uint16_t note_idx = 0;
static uint16_t sample = 0;
static uint16_t amp;

/* TIMER1 interrupt handler */
void __attribute__ ((interrupt)) TIMER1_IRQHandler() 
{
  *TIMER1_IFC = 1;

  uint16_t coin[] = {0, 2*NOTE_Bb, NOTE_Eb, NOTE_Eb, NOTE_Eb, NOTE_Eb, NOTE_Eb, NOTE_Eb, NOTE_Eb, NOTE_Eb, NOTE_Eb};

  if (playing) 
    {
      if (sample_idx % 4410 == 0)
        {
          sample_idx = 0;
          note_idx++;
	  if (note_idx < 11)
            {
	      square1_play_note((square_note_t) {.period_begin = coin[note_idx], .period_end = coin[note_idx], .octave = 5, .amp_begin = 0, .amp_end = 0, .duty_cycle = 50, .duration = 4410});
              square2_play_note((square_note_t){.period_begin = coin[note_idx], .period_end = coin[note_idx], .octave = 5, .amp_begin = 0, .amp_end = 0, .duty_cycle = 50, .duration = 4410});
              triangle_play_note((triangle_note_t) {.amp_begin = 0, .amp_end = 0});
              noise_play((noise_note_t) {.amp_begin = amp, .multiplier = 4});
              amp /= 1.4;
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
      note_idx = 0;
      amp = MAX_AMPLITUDE_PER_CHANNEL;
      sample_idx = 0;
    }
  else
    {
      playing = 1;
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




