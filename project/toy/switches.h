#ifndef SWITCHES_H
#define SWITCHES_H

#include <msp430.h>

// Switch definitions
#define SW1 BIT3       /* switch1 is P1.3 (used for octave shift) */
#define SW2 BIT0       /* switch2 is P2.0 (C4) */
#define SW3 BIT1       /* switch3 is P2.1 (D4) */
#define SW4 BIT2       /* switch4 is P2.2 (E4) */
#define SW5 BIT3       /* switch5 is P2.3 (F4) */

#define SWITCHES (SW1 | SW2 | SW3 | SW4 | SW5) // All 5 switches

// Function declarations
void switch_init(void);  // Initialize switches
void switch_interrupt_handler(void);  // Handle switch interrupts

#endif /* SWITCHES_H */
