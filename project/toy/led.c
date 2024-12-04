#include <msp430.h>
#include "led.h"
#include "switches.h"

void led_init() {
  P1DIR |= LEDS;
  P1OUT &= ~LEDS; /* LEDs initially off */
}

void led_update(char buttonStates, char octaveShift) {
  /* Turn off LEDs initially */
  P1OUT &= ~LEDS;

  // Update LEDs based on switches
  if (buttonStates & SW2) {
    P1OUT |= LED_GREEN; // Green LED for SW2 (C4 or C5)
  }
  if ((buttonStates & SW2) && (buttonStates & SW3)) {
    P1OUT |= LED_RED; // Red LED for SW2 + SW3 (D4 or D5)
  }
  if (buttonStates & SW3) {
    P1OUT |= LED_GREEN; // Green LED for SW3 (E4 or E5)
  }
  if ((buttonStates & SW3) && (buttonStates & SW4)) {
    P1OUT |= LED_RED; // Red LED for SW3 + SW4 (F4 or F5)
  }
  if (buttonStates & SW4) {
    P1OUT |= LED_GREEN; // Green LED for SW4 (G4 or G5)
  }
  if ((buttonStates & SW4) && (buttonStates & SW5)) {
    P1OUT |= LED_RED; // Red LED for SW4 + SW5 (A4 or A5)
  }
  if (buttonStates & SW5) {
    P1OUT |= LED_GREEN; // Green LED for SW5 (B4 or B5)
  }
}
