#include <msp430.h>
#include "libTimer.h"
#include "switches.h"
#include "led.h"
#include "buzzer.h"

void wdt_init() {
  configureClocks();         /* Setup master oscillator, CPU & peripheral clocks */
}

void main(void) {
  switch_init();
  led_init();
  buzzer_init();  // Initialize the buzzer
  wdt_init();

  or_sr(0x18); /* CPU off, GIE on */
}
