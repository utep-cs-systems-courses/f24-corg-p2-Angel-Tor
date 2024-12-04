#include <msp430.h>
#include "switches.h"
#include "led.h"

void __interrupt_vec(PORT1_VECTOR) Port_1() {
  if (P1IFG & SW1) { /* did SW1 cause this interrupt? */
    P1IFG &= ~SW1; /* clear pending SW1 interrupt */
    switch_interrupt_handler();
  }
}

void __interrupt_vec(PORT2_VECTOR) Port_2() {
  if (P2IFG & (SW2 | SW3 | SW4 | SW5)) { /* did SW2-SW5 cause this interrupt? */
    P2IFG &= ~(SW2 | SW3 | SW4 | SW5); /* clear pending SW2-SW5 interrupts */
    switch_interrupt_handler();
  }
}
