#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "ex2.h"


void setupNVIC()
{
  /* TODO use the NVIC ISERx registers to enable handling of interrupt(s)
     remember two things are necessary for interrupt handling:
      - the peripheral must generate an interrupt signal
      - the NVIC must be configured to make the CPU handle the signal
     You will need TIMER1, GPIO odd and GPIO even interrupt handling for this
     assignment.
  */
  *ISER0 |= (1<<1) | (1<<11) | (1<<12);
}


/* function to set up GPIO mode and interrupts*/
void setupGPIO()
{
  /* TODO set input and output pins for the joystick */

  /* Example of HW access from C code: turn on joystick LEDs D4-D8
     check efm32gg.h for other useful register definitions
  */
  *CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_GPIO; /* enable GPIO clock*/
  *GPIO_PA_CTRL = 2;  /* set high drive strength */
  *GPIO_PA_MODEH = 0x55555555; /* set pins A8-15 as output */
  *GPIO_PA_DOUT = 0x0700; /* turn on LEDs D4-D8 (LEDs are active low) */

  *GPIO_PC_MODEL = 0x33333333;
  *GPIO_PC_DOUT = 0xFF;

  *GPIO_EXTIPSELL = 0x22222222;
  *GPIO_EXTIFALL = 0xff;
  *GPIO_EXTIRISE = 0xff;
  *GPIO_IEN = 0xff;
}


#define CLOCK_PERIOD_44KHZ 317

/* function to setup the timer */
void setupTimer(uint16_t period)
{
  *CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_TIMER1;
  *TIMER1_TOP = CLOCK_PERIOD_44KHZ;
  *TIMER1_IEN = 1;
  *ISER0 |= (1 << 12);
  *TIMER1_CMD = 1;
}


void setupDAC()
{
  /*
    TODO enable and set up the Digital-Analog Converter

    1. Enable the DAC clock by setting bit 17 in CMU_HFPERCLKEN0
    2. Prescale DAC clock by writing 0x50010 to DAC0_CTRL
    3. Enable left and right audio channels by writing 1 to DAC0_CH0CTRL and DAC0_CH1CTRL
    4. Write a continuous stream of samples to the DAC data registers, DAC0_CH0DATA and DAC0_CH1DATA, for example from a timer interrupt
  */

    *CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_DAC0;
    *DAC0_CTRL = 0x50010;
    *DAC0_CH0CTRL = 1;
    *DAC0_CH1CTRL = 1;
}





