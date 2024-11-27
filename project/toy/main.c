#include <msp430.h>

#include "libTimer.h"

#include "buzzer.h"

#include "led.h"

#include "switches.h"



void wdt_init() {

  configureClocks();/* setup master oscillator, CPU & peripheral clocks */

  enableWDTInterrupts();/* enable periodic interrupt */

}



void main(void)

{

  switch_init();

  led_init();

  buzzer_init();

  wdt_init();



  or_sr(0x18);  // CPU off, GIE on

}



static int buttonDown;



void

switch_interrupt_handler()

{

  char p1val = P1IN;/* switch is in P1 */



  /* update switch interrupt sense to detect changes from current buttons */

  P1IES |= (p1val & SWITCHES);/* if switch up, sense down */

  P1IES &= (p1val | ~SWITCHES);/* if switch down, sense up */



  if (p1val & SW1) {/* button up */

    P1OUT &= ~LED_GREEN;

    buttonDown = 0;

    buzzer_set_period(0);/* start buzzing!!! 2MHz/1000 = 2kHz*/

  } else {/* button down */

    P1OUT |= LED_GREEN;

    buttonDown = 1;buzzer_set_period(1000);/* start buzzing!!! 2MHz/1000 = 2kHz*/

  }

}




/* Switch on P1 (S2) */

void

__interrupt_vec(PORT1_VECTOR) Port_1(){

  if (P1IFG & SWITCHES) {      /* did a button cause this interrupt? */

    P1IFG &= ~SWITCHES;      /* clear pending sw interrupts */

    switch_interrupt_handler();/* single handler for all switches */

  }

}



void

__interrupt_vec(WDT_VECTOR) WDT()/* 250 interrupts/sec */

{

  static int blink_count = 0;

  switch (blink_count) {

  case 6:

    blink_count = 0;

    P1OUT |= LED_RED;

    break;

  default:

    blink_count ++;

    if (!buttonDown) P1OUT &= ~LED_RED; /* don't blink off if button is down */

  }

}

