#ifndef _G2LIB_H
#define _G2LIB_H

#include "efm32gg.h"


/* --------
 * | NVIC |
 * --------
 *
 *
 */

/* Interrupt line number definition */
typedef enum {
	IRQ_DMA = (1<<0), // line 0
	IRQ_GPIO_EVEN = (1<<1), // line 1
	IRQ_TIMER0 = (1<<2), // line 2
	IRQ_GPIO_ODD = (1<<11), // line 11
	IRQ_TIMER1 = (1<<12), // line 12
	IRQ_TIMER2 = (1<<13), // line 13
	IRQ_TIMER3 = (1<<14) // line 14
} InterruptLine;

/* Enable or disable an interrupt line to trigger the interrupt vector in the NVIC.
 *
 * Parameters:
 *  irqline:	The interrupt line to enable or disable
 *  enable:		true to enable, false to disable */
void NVIC_IRQenable(InterruptLine irqLine, bool enable);


/* -------
 * | CMU |
 * -------
 *
 *
 */

/* Devices that can be clock managed */
typedef enum {
	CMU_TIMER1 = CMU2_HFPERCLKEN0_TIMER1,
	CMU_GPIO = CMU2_HFPERCLKEN0_GPIO,
	CMU_PRS = CMU2_HFPERCLKEN0_PRS,
	CMU_DAC0 = CMU2_HFPERCLKEN0_DAC0
} PeriferalClock;
typedef enum {
	CMU_DMA = CMU_HFCORECLKEN0_DMA
} CoreClock;

/* Manage periferal clock for the given device
 *
 * Parameters:
 *  device:		The device to manage
 *  enable:		true to enable, false to disable */
void CMU_periClockEnable(PeriferalClock device, bool enable);

/* Manage core clock for the given device
 *
 * Parameters:
 *  device:		The device to manage
 *  enable:		true to enable, false to disable */
void CMU_coreClockEnable(CoreClock device, bool enable);


/* --------
 * | GPIO |
 * --------
 *
 *
 */

/* Port number definition */
typedef enum {
	GPIO_portA = 0,
	GPIO_portB = 1,
	GPIO_portC = 2,
	GPIO_portD = 3,
	GPIO_portE = 4,
	GPIO_portF = 5
} GPIO_Port;

/* Different IO modes for the GPIO ports */
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

/* Drive strengths for GPIO ports in out mode */
typedef enum {
	GPIO_STANDARD = 0, // 6 mA drive current
	GPIO_LOWEST = 1, // 0.5 mA drive current
	GPIO_HIGH = 2, // 20 mA drive current
	GPIO_LOW = 3 // 2 mA drive current
} GPIO_driveMode;

/* Configure interrupt for a single pin on a port in the GPIO controller
 *
 * Parameters:
 *  portnr:		The GPIO port to configure
 *  pinnr:		The pin to configure
 *  rising:		Trigger interrupt on the rising edge
 *  falling:	Trigger interrupt on the falling edge
 *  enable:		true to enable, false to disable */
void GPIO_IRQsetup(GPIO_Port portnr, unsigned int pinnr, bool rising, bool falling, bool enable);

/* Configure interrupt for a range of pins on a port in the GPIO controller
 *
 * Parameters:
 *  portnr:		The GPIO port to configure
 *  fromPinnr:	The first pin to configure
 *  toPinnr:	The last pin to configure
 *  rising:		Trigger interrupt on the rising edge
 *  falling:	Trigger interrupt on the falling edge
 *  enable:		true to enable, false to disable */
void GPIO_IRQsetupRange(GPIO_Port portnr, unsigned int fromPinnr, unsigned int toPinnr, bool rising, bool falling, bool enable);

/* Configure a GPIO pin with the selected mode
 *
 * Parameters:
 *  portnr:		The GPIO port to configure
 *  pinnr:		The pin to configure
 *  mode:		The IO mode to set */
void GPIO_portSetup(GPIO_Port portnr, unsigned int pinnr, GPIO_portMode mode);

/* Configure a GPIO pin range with the selected mode
 *
 * Parameters:
 *  portnr:		The GPIO port to configure
 *  fromPinnr:	The first pin to configure
 *  toPinnr:	The last pin to configure
 *  mode:		The IO mode to set */
void GPIO_portSetupRange(GPIO_Port portnr, unsigned int fromPinnr, unsigned int toPinnr, GPIO_portMode mode);

/* Set drive strength for the selected port
 *
 * Parameters:
 *  portnr:		The GPIO port to configure
 *  mode:		The drive mode to set */
void GPIO_driveStrength(GPIO_Port portnr, GPIO_driveMode mode);

/* Clear GPIO interrupt flag for a pin
 *
 * Parameters:
 *  pinnr:		The pin number to clear interrupt for */
void GPIO_clearInterrupt(unsigned int pinnr);

/* Clear GPIO interrupt flags for all pins */
void GPIO_clearAllInterrupts();

/* Return the DIN register contents for the given port
 *
 * Parameters:
 *  portnr:		The GPIO port to poll
 *
 * Return value:
 *  A 32 bit unsigned integer with the contents of the DIN register */
uint32_t GPIO_pollPort(GPIO_Port portnr);

/* Check if the specified pin is active.
 * This function assumes the pin is active low
 *
 * Parameters:
 *  portnr:		The GPIO port to check
 *  pinnr:		The pin number of the port
 *
 * Return value:
 * 	true if the pin is active (electric low), false otherwise */
bool GPIO_pollPin(GPIO_Port portnr, unsigned int pinnr);

/*

 */


#endif
