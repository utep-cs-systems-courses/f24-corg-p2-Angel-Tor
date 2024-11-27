#include <msp430.h>
#include "led.h"
#include "switches.h"

void led_init()
{
  P1DIR |= LEDS;// Set the bits attached to LEDs as output
  switch_state_changed = 1;  // Flag to update LEDs on initialization
  led_update();             // Initialize the LED state
}

void led_update(){
  if (switch_state_changed) {
    char ledFlags = 0; /* By default, no LEDs are on */

    // Turn on the green LED when any of the individual switches (SW2, SW3, SW4, SW5) is pressed
    if (switch_state_down & (SW2 | SW3 | SW4 | SW5)) {
      ledFlags |= LED_GREEN;  // Set the green LED on
    }

    // Turn on the red LED for specific switch combinations
    if ((switch_state_down & SW2) && (switch_state_down & SW3)) { // SW2 && SW3 pressed
      ledFlags |= LED_RED;  // Set the red LED on
    } else if ((switch_state_down & SW3) && (switch_state_down & SW4)) { // SW3 && SW4 pressed
      ledFlags |= LED_RED;  // Set the red LED on
    } else if ((switch_state_down & SW4) && (switch_state_down & SW5)) { // SW4 && SW5 pressed
      ledFlags |= LED_RED;  // Set the red LED on
    }

    // If neither of the combinations or individual switches are pressed, both LEDs should be off
    if (switch_state_down == 0) {
      ledFlags = 0;  // Turn off all LEDs
    }

    // Update LEDs on the output register
    P1OUT &= (0xff - LEDS) | ledFlags; // Clear bits for off LEDs
    P1OUT |= ledFlags;                 // Set bits for on LEDs
  }
  switch_state_changed = 0; // Reset the change flag
}
