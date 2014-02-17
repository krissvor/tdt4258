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
  if (~(*GPIO_PC_DIN))
    {
      playing = 1;
    }
  else
    {
      playing = 0;
    }
}
