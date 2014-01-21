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
	ldr R1, [port_a, #CMU_HFPERCLKEN0]
	mov R2, #1
	lsl R2, R2, #CMU_HFPERCLKEN0_GPIO
	orr R2, R1, R2
	str R2, [R0, #CMU_HFPERCLKEN0]

	//setup NVIC for GPIO odd & even
	ldr R0, =ISER0
	ldr R1, [R0]
	movw R2, #0x802
	orr R2, R2, R1
	str R2, [R0]


	//Setup GPIO
	//Never ever ever change these
	ldr R0, =GPIO_PA_BASE
	ldr R1, =GPIO_PC_BASE
	port_a .req R0
	port_c .req R1

	//set high drive strength for LEDs
	mov R0, #0x2
	str R0, [port_a, #GPIO_CTRL]

	//set port A pin 8-15 as output
	ldr R2, =0x55555555
	str R2, [port_a, #GPIO_MODEH]

	//set port C pin 0-7 as input
	ldr R2, =0x33333333
	str R2, [port_c, #GPIO_MODEL]

	//enable pull-up resistors
	mov R2, 0xff
	str R2, [port_c, #GPIO_DOUT]

loop:
	ldr R2, [port_c, #GPIO_DIN]
	lsl R2, R2, #8
	str R2, [port_a, #GPIO_DOUT]
	b loop




	/////////////////////////////////////////////////////////////////////////////
	//
  // GPIO handler
  // The CPU will jump here when there is a GPIO interrupt
	//
	/////////////////////////////////////////////////////////////////////////////

        .thumb_func
gpio_handler:

	      b .  // do nothing

	/////////////////////////////////////////////////////////////////////////////

        .thumb_func
dummy_handler:
        b .  // do nothing

