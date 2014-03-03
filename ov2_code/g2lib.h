#ifndef _G2LIB_H
#define _G2LIB_H

#include "efm32gg.h"


// --------
// | NVIC |
// --------

typedef enum {
	IRQ_DMA = (1<<0), // line 0
	IRQ_GPIO_EVEN = (1<<1), // line 1
	IRQ_TIMER0 = (1<<2), // line 2
	IRQ_GPIO_ODD = (1<<11), // line 11
	IRQ_TIMER1 = (1<<12), // line 12
	IRQ_TIMER2 = (1<<13), // line 13
	IRQ_TIMER3 = (1<<14) // line 14
} InterruptLine;

void NVIC_IRQenable(InterruptLine irqLine, bool enable);


// -------
// | CMU |
// -------

typedef enum {
	CMU_TIMER1 = CMU2_HFPERCLKEN0_TIMER1,
	CMU_GPIO = CMU2_HFPERCLKEN0_GPIO,
	CMU_PRS = CMU2_HFPERCLKEN0_PRS,
	CMU_DAC0 = CMU2_HFPERCLKEN0_DAC0
} PeriferalClock;

typedef enum {
	CMU_DMA = CMU_HFCORECLKEN0_DMA
} CoreClock;

void CMU_periClockEnable(PeriferalClock device, bool enable);
void CMU_coreClockEnable(CoreClock device, bool enable);


// --------
// | GPIO |
// --------

typedef enum {
	GPIO_portA = 0,
	GPIO_portB = 1,
	GPIO_portC = 2,
	GPIO_portD = 3,
	GPIO_portE = 4,
	GPIO_portF = 5
} GPIO_Port;

typedef enum {
	GPIO_DISABLED = 0, // Input disabled. Pullup if DOUT is set.
	GPIO_INPUT = 1, // Input enabled. Filter if DOUT is set
	GPIO_INPUTPULL = 2, // Input enabled. DOUT determines pull direction
	GPIO_INPUTPULLFILTER = 3, // Input enabled with filter. DOUT determines pull direction
	GPIO_PUSHPULL = 4, // Push-pull output
	GPIO_PUSHPULLDRIVE = 5, // Push-pull output with drive-strength set by DRIVEMODE
	GPIO_WIREDOR = 6, // Wired-or output
	GPIO_WIREDORPULLDOWN = 7, // Wired-or output with pull-down
	GPIO_WIREDAND = 8, // Open-drain output
	GPIO_WIREDANDFILTER = 9, // Open-drain output with filter
	GPIO_WIREDANDPULLUP = 10, // Open-drain output with pullup
	GPIO_WIREDANDPULLUPFILTER = 11, // Open-drain output with filter and pullup
	GPIO_WIREDANDDRIVE = 12, // Open-drain output with drive-strength set by DRIVEMODE
	GPIO_WIREDANDDRIVEFILTER = 13, // Open-drain output with filter and drive-strength set by DRIVEMODE
	GPIO_WIREDANDDRIVEPULLUP = 14, // Open-drain output with pullup and drive-strength set by DRIVEMODE
	GPIO_WIREDANDDRIVEPULLUPFILTER = 15, // Open-drain output with filter, pullup and drive-strength set by DRIVEMODE
} GPIO_portMode;

typedef enum {
	GPIO_STANDARD = 0, // 6 mA drive current
	GPIO_LOWEST = 1, // 0.5 mA drive current
	GPIO_HIGH = 2, // 20 mA drive current
	GPIO_LOW = 3 // 2 mA drive current
} GPIO_driveMode;

void GPIO_IRQsetup(GPIO_Port portnr, unsigned int pinnr, bool rising, bool falling, bool enable);
void GPIO_IRQsetupRange(GPIO_Port portnr, unsigned int fromPinnr, unsigned int toPinnr, bool rising, bool falling, bool enable);
void GPIO_portSetup(GPIO_Port portnr, unsigned int pinnr, GPIO_portMode mode);
void GPIO_portSetupRange(GPIO_Port portnr, unsigned int fromPinnr, unsigned int toPinnr, GPIO_portMode mode);
void GPIO_driveStrength(GPIO_Port portnr, GPIO_driveMode mode);
void GPIO_clearInterrupt(unsigned int pinnr);
void GPIO_clearAllInterrupts();
uint32_t GPIO_pollPort(GPIO_Port portnr);
bool GPIO_pollPin(GPIO_Port portnr, unsigned int pinnr);
/*

 */


#endif
