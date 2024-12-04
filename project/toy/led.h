#ifndef LED_H
#define LED_H

#define LED_RED BIT0   /* P1.0 */
#define LED_GREEN BIT6 /* P1.6 */
#define LEDS (LED_RED | LED_GREEN)

void led_init();
void led_update(char buttonStates, char octaveShift);

#endif // LED_H
