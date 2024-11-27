#include <msp430.h>
#include "switches.h"
#include "led.h"
#include "buzzer.h" // Include buzzer functionality

char switch_state_down, switch_state_changed; /* effectively boolean */

static char
switch_update_interrupt_sense()
{
  char p1val = P1IN; // Read the current state of switches on P1.3 (SW1)
  char p2val = P2IN; // Read the current state of switches on P2.0 - P2.3 (SW2, SW3, SW4, SW5)

  /* Update switch interrupt to detect changes from current buttons */
  // For P1.3 (SW1), set the interrupt edge sense
  P1IES |= (p1val & SW1);   // if switch up, sense down
  P1IES &= (p1val | ~SW1);  // if switch down, sense up

  // For P2.0 - P2.3 (SW2, SW3, SW4, SW5), set the interrupt edge sense
  P2IES |= (p2val & (SW2 | SW3 | SW4 | SW5));   // if switch up, sense down
  P2IES &= (p2val | ~(SW2 | SW3 | SW4 | SW5));  // if switch down, sense up

  return (p1val | p2val); // Return the combined input value
}

void
switch_init()          /* setup switch */
{
  P1REN |= SW1;        // enable resistor for SW1 on P1.3
  P2REN |= (SW2 | SW3 | SW4 | SW5);  // enable resistors for SW2, SW3, SW4, SW5 on P2.0 - P2.3
  P1IE |= SW1;         // enable interrupt for SW1 on P1.3
  P2IE |= (SW2 | SW3 | SW4 | SW5); // enable interrupts for SW2, SW3, SW4, SW5 on P2.0 - P2.3
  P1OUT |= SW1;        // pull-up for SW1 on P1.3
  P2OUT |= (SW2 | SW3 | SW4 | SW5);  // pull-ups for SW2, SW3, SW4, SW5 on P2.0 - P2.3
  P1DIR &= ~SW1;       // set SW1 as input (P1.3)
  P2DIR &= ~(SW2 | SW3 | SW4 | SW5);  // set SW2, SW3, SW4, SW5 as input (P2.0 - P2.3)
  switch_update_interrupt_sense();  // update interrupt sense based on current switch states
  led_update();         // Initialize the LEDs state
}

void
switch_interrupt_handler()
{
  char p1val = P1IN; // Read the current state of switches on P1.3 (SW1)
  char p2val = P2IN; // Read the current state of switches on P2.0 - P2.3 (SW2, SW3, SW4, SW5)

  // Update switch_state_down based on which switches are pressed
  switch_state_down = 0; // Reset switch_state_down

  if (!(p1val & SW1)) switch_state_down |= 0x01;  // SW1 pressed (P1.3)
  if (!(p2val & SW2)) switch_state_down |= 0x02;  // SW2 pressed (P2.0)
  if (!(p2val & SW3)) switch_state_down |= 0x04;  // SW3 pressed (P2.1)
  if (!(p2val & SW4)) switch_state_down |= 0x08;  // SW4 pressed (P2.2)
  if (!(p2val & SW5)) switch_state_down |= 0x10;  // SW5 pressed (P2.3)

  switch_state_changed = 1;  // Mark that the state has changed

  // Call led_update() to handle LED state change based on switch input
  led_update();

  // Play the corresponding note for each button press
  if (!(p2val & SW2) && !(p2val & SW3)) {     // SW2 && SW3 pressed (P2.0 && P2.1)
    buzzer_set_period(7393); // C#4 (Db4)
  } else if (!(p2val & SW3) && !(p2val & SW4)) { // SW3 && SW4 pressed (P2.1 && P2.2)
    buzzer_set_period(6223); // D#4 (Eb4)
  } else if (!(p2val & SW4) && !(p2val & SW5)) { // SW4 && SW5 pressed (P2.2 && P2.3)
    buzzer_set_period(5444); // F#4 (Gb4)
  } else if (!(p1val & SW1)) {     // SW2 pressed (P2.0)
    buzzer_set_period(7635); // C4
  } else if (!(p2val & SW3)) { // SW3 pressed (P2.1)
    buzzer_set_period(6841); // D4
  } else if (!(p2val & SW4)) { // SW4 pressed (P2.2)
    buzzer_set_period(6079); // E4
  } else if (!(p2val & SW5)) { // SW5 pressed (P2.3)
    buzzer_set_period(5745); // F4
  } else {
    buzzer_set_period(0);    // No note if no button is pressed
  }

  // Call led_update() again to ensure the LEDs are updated after any buzzer activity
  led_update();
}
