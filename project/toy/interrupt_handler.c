#include <msp430.h>
#include "switches.h"  // Include the switches header file to get the switch-related functions

/* Interrupt handler for Port 1 (P1) for switch SW1 on P1.3 */
#pragma vector=PORT1_VECTOR
__interrupt void Port_1_ISR(void)
{
  if (P1IFG & SW1) {               // Check if SW1 caused the interrupt (P1.3)
    P1IFG &= ~SW1;                 // Clear interrupt flag for SW1
    switch_interrupt_handler();    // Handle the switch press (update states, etc.)
  }
}

/* Interrupt handler for Port 2 (P2) for switches SW2-SW5 on P2.0 to P2.3 */
#pragma vector=PORT2_VECTOR
__interrupt void Port_2_ISR(void)
{
  if (P2IFG & (SW2 | SW3 | SW4 | SW5)) { // Check if any of SW2, SW3, SW4, or SW5 caused the interrupt
    if (P2IFG & SW2) {              // If SW2 (P2.0) caused the interrupt
      P2IFG &= ~SW2;                // Clear interrupt flag for SW2
    }
    if (P2IFG & SW3) {              // If SW3 (P2.1) caused the interrupt
      P2IFG &= ~SW3;                // Clear interrupt flag for SW3
    }
    if (P2IFG & SW4) {              // If SW4 (P2.2) caused the interrupt
      P2IFG &= ~SW4;                // Clear interrupt flag for SW4
    }
    if (P2IFG & SW5) {              // If SW5 (P2.3) caused the interrupt
      P2IFG &= ~SW5;                // Clear interrupt flag for SW5
    }
    switch_interrupt_handler();     // Handle the switch press (update states, etc.)
  }
}
