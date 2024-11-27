#include <msp430.h>
#include "libTimer.h"
#include "buzzer.h"

#define LED_RED BIT0               // P1.0
#define LED_GREEN BIT6             // P1.6
#define LEDS (LED_RED | LED_GREEN)

#define SW1 BIT3        /* switch1 is p1.3 */
#define SW2 BIT0        /* switch2 is p2.0 */
#define SW3 BIT1        /* switch3 is p2.1 */
#define SW4 BIT2        /* switch4 is p2.2 */
#define SW5 BIT3        /* switch5 is p2.3 */
#define SWITCHES (SW1 | SW2 | SW3 | SW4 | SW5)   /* all switches */

static int buttonDown = 0;  // to track button state

void switch_init() {
  // Setup Port 1 (SW1)
  P1REN |= SW1;         // Enable pull-up resistors
  P1IE |= SW1;          // Enable interrupts for SW1
  P1OUT |= SW1;         // Enable pull-ups for SW1
  P1DIR &= ~SW1;        // Set SW1 as input

  // Setup Port 2 (SW2, SW3, SW4, SW5)
  P2REN |= (SW2 | SW3 | SW4 | SW5);   // Enable pull-up resistors for SW2-SW5
  P2IE |= (SW2 | SW3 | SW4 | SW5);    // Enable interrupts for SW2-SW5
  P2OUT |= (SW2 | SW3 | SW4 | SW5);   // Enable pull-ups for SW2-SW5
  P2DIR &= ~(SW2 | SW3 | SW4 | SW5);  // Set SW2-SW5 as input
}

void led_init() {
  P1DIR |= LEDS;         // Set LED pins as output
  P1OUT &= ~LEDS;        // Initially turn off LEDs
}

void wdt_init() {
  configureClocks();        // Set up the master oscillator, CPU & peripheral clocks
  enableWDTInterrupts();    // Enable periodic interrupt from the Watchdog Timer
}

void main(void) {
  switch_init();    // Initialize the switches
  led_init();       // Initialize the LEDs
  buzzer_init();    // Initialize the buzzer
  wdt_init();       // Initialize the Watchdog Timer

  or_sr(0x18);      // CPU off, GIE on (enable interrupts)
}

// Update the switch interrupt sense to detect changes
void switch_interrupt_handler() {
  char p1val = P1IN;   // Read Port 1 input
  char p2val = P2IN;   // Read Port 2 input

  // Update switch interrupt sense to detect changes from the current buttons
  P1IES |= (p1val & SW1);    // If switch is up, sense down
  P1IES &= (p1val | ~SW1);   // If switch is down, sense up

  P2IES |= (p2val & (SW2 | SW3 | SW4 | SW5));    // If switch is up, sense down
  P2IES &= (p2val | ~(SW2 | SW3 | SW4 | SW5));   // If switch is down, sense up

  // Handle SW1 (P1.3)
  if (p1val & SW1) {    // Button is up
    P1OUT &= ~LED_GREEN;  // Turn off Green LED
    buttonDown = 0;
    buzzer_set_period(0); // No buzzer sound
  } else {               // Button is down
    P1OUT |= LED_GREEN;   // Turn on Green LED
    buttonDown = 1;
    buzzer_set_period(1000);  // Buzzer at 2kHz
  }

  // Handle SW2, SW3, SW4, SW5 (P2.0 - P2.3)
  if (!(p2val & SW2)) {    // SW2 pressed
    buzzer_set_period(7635); // C4
  } else if (!(p2val & SW3)) {   // SW3 pressed
    buzzer_set_period(6841); // D4
  } else if (!(p2val & SW4)) {   // SW4 pressed
    buzzer_set_period(6079); // E4
  } else if (!(p2val & SW5)) {   // SW5 pressed
    buzzer_set_period(5745); // F4
  } else if (!(p2val & (SW2 | SW3))) {    // SW2 && SW3 pressed
    buzzer_set_period(7393); // C#4 (Db4)
  } else if (!(p2val & (SW3 | SW4))) {    // SW3 && SW4 pressed
    buzzer_set_period(6223); // D#4 (Eb4)
  } else if (!(p2val & (SW4 | SW5))) {    // SW4 && SW5 pressed
    buzzer_set_period(5444); // F#4 (Gb4)
  } else {
    buzzer_set_period(0);    // No sound if no button pressed
  }
}

void __interrupt_vec(PORT1_VECTOR) Port_1() {
  if (P1IFG & SWITCHES) {  // Check if switch on Port 1 triggered the interrupt
    P1IFG &= ~SWITCHES;     // Clear the interrupt flags for Port 1
    switch_interrupt_handler();  // Handle the interrupt
  }
}

void __interrupt_vec(PORT2_VECTOR) Port_2() {
  if (P2IFG & (SW2 | SW3 | SW4 | SW5)) {  // Check if any switch on Port 2 triggered the interrupt
    P2IFG &= ~(SW2 | SW3 | SW4 | SW5);   // Clear the interrupt flags for Port 2
    switch_interrupt_handler();  // Handle the interrupt
  }
}

void __interrupt_vec(WDT_VECTOR) WDT() {
  static int blink_count = 0;
  switch (blink_count) {
  case 6:
    blink_count = 0;
    P1OUT |= LED_RED;  // Turn on Red LED
    break;
  default:
    blink_count++;
    if (!buttonDown) P1OUT &= ~LED_RED; // Don't blink off if button is down
  }
}
