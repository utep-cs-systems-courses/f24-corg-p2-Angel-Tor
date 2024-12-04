#include <msp430.h>
#include "switches.h"
#include "led.h"
#include "buzzer.h"

static char buttonStates = 0;
static char octaveShift = 0;  // To track octave shift (0 = normal, 1 = shifted)

void switch_init() {
  // Initialize P1.3 (SW1) and P2.0 - P2.3 (SW2 - SW5)
  P1REN |= SW1;             /* Enable resistor for SW1 */
  P2REN |= (SW2 | SW3 | SW4 | SW5);  /* Enable resistors for SW2-SW5 */
  P1IE |= SW1;              /* Enable interrupts for SW1 */
  P2IE |= (SW2 | SW3 | SW4 | SW5); /* Enable interrupts for SW2-SW5 */
  P1OUT |= SW1;             /* Pull-up resistor for SW1 */
  P2OUT |= (SW2 | SW3 | SW4 | SW5); /* Pull-up resistors for SW2-SW5 */
  P1DIR &= ~SW1;            /* Set SW1 as input */
  P2DIR &= ~(SW2 | SW3 | SW4 | SW5); /* Set SW2-SW5 as input */
}

void switch_interrupt_handler() {
  char p1val = P1IN;
  char p2val = P2IN;

  /* Update P1.3 (SW1) interrupt sense */
  P1IES |= (p1val & SW1); /* If SW1 is up, sense down */
  P1IES &= (p1val | ~SW1); /* If SW1 is down, sense up*/

  /* Update P2.0 - P2.3 (SW2 - SW5) interrupt sense */
  P2IES |= (p2val & (SW2 | SW3 | SW4 | SW5)); /* If switches are up, sense down */
  P2IES &= (p2val | ~(SW2 | SW3 | SW4 | SW5)); /* If switches are down, sense up */

  /* Read button states */
  buttonStates = 0;
  if (!(p2val & SW2)) buttonStates |= SW2;
  if (!(p2val & SW3)) buttonStates |= SW3;
  if (!(p2val & SW4)) buttonStates |= SW4;
  if (!(p2val & SW5)) buttonStates |= SW5;
  if (!(p1val & SW1)) {  // If SW1 is pressed, shift octave
    octaveShift = 1;
  } else {
    octaveShift = 0;
  }

  /* Update LEDs and buzzer based on button states */
  led_update(buttonStates, octaveShift);

  // Handle buzzer frequencies based on switch presses
  if (buttonStates & SW2) {
    buzzer_set_period(octaveShift ? 955 : 1911);  // C4 (C5 when octaveShift is on)
  } else if ((buttonStates & SW2) && (buttonStates & SW3)) {
    buzzer_set_period(octaveShift ? 851 : 1703);  // D4 (D5 when octaveShift is on)
  } else if (buttonStates & SW3) {
    buzzer_set_period(octaveShift ? 758 : 1517);  // E4 (E5 when octaveShift is on)
  } else if ((buttonStates & SW3) && (buttonStates & SW4)) {
    buzzer_set_period(octaveShift ? 716 : 1432);  // F4 (F5 when octaveShift is on)
  } else if (buttonStates & SW4) {
    buzzer_set_period(octaveShift ? 638 : 1276);  // G4 (G5 when octaveShift is on)
  } else if ((buttonStates & SW4) && (buttonStates & SW5)) {
    buzzer_set_period(octaveShift ? 568 : 1136);  // A4 (A5 when octaveShift is on)
  } else if (buttonStates & SW5) {
    buzzer_set_period(octaveShift ? 506 : 1012);  // B4 (B5 when octaveShift is on)
  } else {
    buzzer_set_period(0); // Stop buzzer if no switch is pressed
  }
}
