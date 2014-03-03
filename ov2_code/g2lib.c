#include <stdbool.h>

#include "g2lib.h"
#include "efm32gg.h"



/* --------
 * | NVIC |
 * --------
 *
 *
 */

/* Enable interrupt line for the given device */
void NVIC_IRQenable(InterruptLine irqLine, bool enable) {
	if (enable) {
		*ISER0 |= irqLine;
	} else {
		*ISER0 &= ~irqLine;
	}
}



/* -------
 * | CMU |
 * -------
 *
 *
 */

/* Enable periferal clock for the given device */
void CMU_periClockEnable(PeriferalClock device, bool enable) {
	if (enable) {
		*CMU_HFPERCLKEN0 |= device;
	} else {
		*CMU_HFPERCLKEN0 &= ~device;
	}
}


/* Enable core clock for the given device */
void CMU_coreClockEnable(CoreClock device, bool enable) {
	if (enable) {
		*CMU_HFCORECLKEN0 |= device;
	} else {
		*CMU_HFCORECLKEN0 &= ~device;
	}
}



/* --------
 * | GPIO |
 * --------
 *
 *
 */

/* Enable interrupt for the given port and pin with rising and/or falling edge triggers */
void GPIO_IRQsetup(GPIO_Port portnr, unsigned int pinnr, bool rising, bool falling, bool enable) {
	if (enable) {
		if (pinnr < 8) {
			*GPIO_EXTIPSELL |= portnr << pinnr * 4;
		} else if (pinnr < 16) {
			*GPIO_EXTIPSELH |= portnr << (pinnr - 8) * 4;
		} else {
			return; // wrong pin number
		}

		if (rising) *GPIO_EXTIRISE |= 1 << pinnr;
		if (falling) *GPIO_EXTIFALL |= 1 << pinnr;

		*GPIO_IEN |= 1 << pinnr;

	} else {
		if (pinnr < 8) {
			*GPIO_EXTIPSELL &= ~(portnr << pinnr * 4);
		} else if (pinnr < 16) {
			*GPIO_EXTIPSELH &= ~(portnr << (pinnr - 8) * 4);
		} else {
			return; // wrong pin number
		}

		if (rising) *GPIO_EXTIRISE &= ~(1 << pinnr);
		if (falling) *GPIO_EXTIFALL &= ~(1 << pinnr);

		*GPIO_IEN &= ~(1 << pinnr);
	}
}


/* Enable interrupt for the given port and pin range with rising and/or falling edge triggers */
void GPIO_IRQsetupRange(GPIO_Port portnr, unsigned int fromPinnr, unsigned int toPinnr, bool rising, bool falling, bool enable) {
	for (unsigned int pin = fromPinnr; pin <= toPinnr; pin++) {
		GPIO_IRQsetup(portnr, pin, rising, falling, enable);
	}
}


/* Setup GPIO pin on port with the selected mode */
void GPIO_portSetup(GPIO_Port portnr, unsigned int pinnr, GPIO_portMode mode) {
	if (pinnr < 8) {
		volatile uint32_t* regAddress = (volatile uint32_t*)((GPIO_BASE + portnr * GPIO_PORT_SIZE) + GPIO_MODEL_OFFSET);
		*regAddress |= mode << pinnr * 4;

	} else if (pinnr < 16) {
		volatile uint32_t* regAddress = (volatile uint32_t*)((GPIO_BASE + portnr * GPIO_PORT_SIZE) + GPIO_MODEH_OFFSET);
		*regAddress |= mode << (pinnr - 8) * 4;

	} else {
		return; // wrong pin number
	}
}


/* Setup GPIO pin range on port with the selected mode */
void GPIO_portSetupRange(GPIO_Port portnr, unsigned int fromPinnr, unsigned int toPinnr, GPIO_portMode mode) {
	for (unsigned int pin = fromPinnr; pin <= toPinnr; pin++) {
		GPIO_portSetup(portnr, pin, mode);
	}
}


/* Set drive strength for the selected port */
void GPIO_driveStrength(GPIO_Port portnr, GPIO_driveMode mode) {
	volatile uint32_t* portBase = (volatile uint32_t*)(GPIO_BASE + portnr * GPIO_PORT_SIZE);
	*portBase = mode;
}


/* Clear interrupt flag */
void GPIO_clearInterrupt(unsigned int pinnr) {
	*GPIO_IFC = 1 << pinnr;
}


/* Clear interrupt flags for all pins */
void GPIO_clearAllInterrupts() {
	*GPIO_IFC = 0xFF;
}


/* Return the DIN register contents for the given port */
uint32_t GPIO_pollPort(GPIO_Port portnr) {
	volatile uint32_t* adressDIN = (volatile uint32_t*)((GPIO_BASE + portnr * GPIO_PORT_SIZE) + GPIO_DIN_OFFSET);
	return *adressDIN;
}


/* Check if the specified pin is active. Assuming active low */
bool GPIO_pollPin(GPIO_Port portnr, unsigned int pinnr) {
	volatile uint32_t* adressDIN = (volatile uint32_t*)((GPIO_BASE + portnr * GPIO_PORT_SIZE) + GPIO_DIN_OFFSET);

	return ~(*adressDIN | ~(1 << pinnr)) != 0;
}





