.syntax unified

.include "efm32gg.s"

	/////////////////////////////////////////////////////////////////////////////
	//
	// Exception vector table
	// This table contains addresses for all exception handlers
	//
	/////////////////////////////////////////////////////////////////////////////

.section .vectors

	.long   stack_top               /* Top of Stack                 */
	.long   _reset                  /* Reset Handler                */
	.long   dummy_handler           /* NMI Handler                  */
	.long   dummy_handler           /* Hard Fault Handler           */
	.long   dummy_handler           /* MPU Fault Handler            */
	.long   dummy_handler           /* Bus Fault Handler            */
	.long   dummy_handler           /* Usage Fault Handler          */
	.long   dummy_handler           /* Reserved                     */
	.long   dummy_handler           /* Reserved                     */
	.long   dummy_handler           /* Reserved                     */
	.long   dummy_handler           /* Reserved                     */
	.long   dummy_handler           /* SVCall Handler               */
	.long   dummy_handler           /* Debug Monitor Handler        */
	.long   dummy_handler           /* Reserved                     */
	.long   dummy_handler           /* PendSV Handler               */
	.long   dummy_handler           /* SysTick Handler              */

	/* External Interrupts */
	.long   dummy_handler
	.long   gpio_handler            /* GPIO even handler */
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   gpio_handler            /* GPIO odd handler */
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler

.section .text

	/////////////////////////////////////////////////////////////////////////////
	//
	// Reset handler
	// The CPU will start executing here after a reset
	//
	/////////////////////////////////////////////////////////////////////////////

.globl  _reset
.type   _reset, %function
.thumb_func
_reset:
	//setup GPIO clock in CMU
	ldr R0, =CMU_BASE
	ldr R1, [R0, #CMU_HFPERCLKEN0] // load any existing enabled clocks
	mov R2, #1
	lsl R2, R2, #CMU_HFPERCLKEN0_GPIO // left shift 13 times. Bit 13 is for GPIO
	orr R2, R1, R2 // or with any existing high bits
	str R2, [R0, #CMU_HFPERCLKEN0]

	//setup NVIC for GPIO odd & even
	ldr R0, =ISER0
	ldr R1, [R0] // load any existing enabled interrupts
	movw R2, #0x802 // Pin 1 and 11 for even and odd
	orr R2, R2, R1 // or with existing value
	str R2, [R0]

	//Setup GPIO
	//Port base constants. Never ever ever change these
	ldr R0, =GPIO_PA_BASE
	ldr R1, =GPIO_PC_BASE
	port_a .req R0
	port_c .req R1

	//set high drive strength for LEDs
	mov R2, #0x2
	str R2, [port_a, #GPIO_CTRL]

	//set port A pin 8-15 as output (leds)
	ldr R2, =0x55555555 // 0101 Output in push-pull configuration with drive strength specified in the control register
	str R2, [port_a, #GPIO_MODEH]

	//set port C pin 0-7 as input (buttons)
	ldr R2, =0x33333333 // 0011 Input enabled with glitch filter and pull-down/pull-up controlled in the dout register
	str R2, [port_c, #GPIO_MODEL]

	//enable pull-up resistors for buttons
	mov R2, 0xFF
	str R2, [port_c, #GPIO_DOUT]

	//setup interrupts
	ldr R4, =GPIO_BASE
	ldr R5, =0x22222222 // 0010 (port c)
	str R5, [R4, #GPIO_EXTIPSELL] // set port c pin 0-7 (low pins) as interrupt source
	mov R5, #0xFF // 0-7 high
	str R5, [R4, #GPIO_EXTIFALL] // interrupt on falling edge (button pushed)
//	str R5, [R4, #GPIO_EXTIRISE]
	str R5, [R4, #GPIO_IEN] // enable interrupt

	//Turn on a LED
	ldr R2, =0xFEFEFEFE // 1111 1110 is repeated four times to make rotating lit led easier
	str R2, [port_a, #GPIO_DOUT]

	//Set up sleep mode
	ldr R3, =SCR
	mov R4, #6 // enable deep sleep and return to sleep after interrupt
	str R4, [R3]

	//Sleep and wait for interrupt
	wfi

	/////////////////////////////////////////////////////////////////////////////
	//
	// GPIO handler
	// The CPU will jump here when there is a GPIO interrupt
	//
	/////////////////////////////////////////////////////////////////////////////

.thumb_func
gpio_handler:
	// Clear interruptflags
	ldr R3, =GPIO_BASE
	mov R4, 0xff
	str R4, [R3, #GPIO_IFC]

	// load button status
	ldr R5, [port_c, #GPIO_DIN]

	ldr R7, =0xFFFFFFFE // bit 0 low (button 1)
	orr R6, R5, R7 // or to get all 1's if button 1 is pressed
	mvn R6, R6 // negate so we can check if zero
	cbz R6, not_button_1 // goto button 3 check routine if button 1 is not pressed
	ror R2, R2, #1 // The order of the leds is opposite to the bit order so we rotate right by 1 to rotate leds left
	b end_ih // goto exit

not_button_1:
	ldr R7, =0xFFFFFFFB // bit 2 low (button 3)
	orr R6, R5, R7 // or to get all 1's if button 3 is pressed
	mvn R6, R6 // negate so we can check if zero
	cbz R6, end_ih // branch if button 3 is not pressed
	ror R2, R2, #7 // rotate right by 7 to rotate leds right by 1

end_ih:
	str R2, [port_a, #GPIO_DOUT] // store new led configuration value
	bx LR // return

	/////////////////////////////////////////////////////////////////////////////

.thumb_func
dummy_handler:
	b .  // do nothing
