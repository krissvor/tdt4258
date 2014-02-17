#ifndef _EX2_H
#define _EX2_H


/* Declaration of peripheral setup functions */
void setupNVIC();
void setupGPIO();
void setupTimer(uint16_t period);
void setupDAC();


/* Declaration of helper functions */
void ButtonHandler();


#endif
