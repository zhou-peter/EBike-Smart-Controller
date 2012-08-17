/*
 * EBike Smart Controller
 *
 * Copyright (C) Jorge Pinto aka Casainho, 2012.
 *
 *   casainho [at] gmail [dot] com
 *     www.casainho.net
 *
 * Released under the GPL Licence, Version 3
 */

#include "lpc210x.h"
#include "main.h"
#include "pwm.h"
#include "ios.h"

volatile unsigned int wTime=0;

void timer0_int_handler (void) __attribute__ ((interrupt("IRQ")));

void timer0_init (void)
{
  /* Initialize VIC */
  VICINTSEL &= ~(1 << 4); /* Timer 0 selected as IRQ */
  VICINTEN |= (1 << 4); /* Timer 0 interrupt enabled */
  VICVECTCNTL1 = 0x24; /* Assign Timer0; IRQ */
  VICVECTADDR1 = (unsigned long) timer0_int_handler; /* Address of the ISR */

  /* Timer/Counter 0 power/clock enable */
  PCONP |= (1 << 1);

  /* Initialize Timer 0 */
  TIMER0_TCR = 0;
  TIMER0_TC = 0; /* Counter register: Clear counter */
  TIMER0_PR = 47; /* Prescaler register: Timer0 Counter increments each 1us; 1us/(48MHz-1) */
  TIMER0_PC = 0; /* Prescaler counter register: Clear prescaler counter */

  /* Clear the interrupt flag */
  TIMER0_IR = 1;
  VICVECTADDR = 0xff;

  TIMER0_MCR = 3; /* Reset and interrupt on match */
}

void timer0_start (void)
{
  /* Start timer */
  TIMER0_TCR = 1;
}

void timer0_stop(void)
{
  /* Stop timer */
  TIMER0_TCR = 0;
}

void timer0_set_us (unsigned long us)
{
  /* Match register 0:
   * Fclk = 48000000Hz; 48MHz/48 = 1MHz -> 1us.
   * x us * 1us = x us */
  TIMER0_MR0 = us;
}

//just use it to increase the time
void timer0_int_handler (void)
{
  /* Clear the interrupt flag */
  TIMER0_IR = 1;
  VICVECTADDR = 0xff;

	wTime++;
}

int Timer1_wReadTimer ()
{
	return wTime;
}