/******************************************************************************
 *   Header Includes
 *******************************************************************************/

#include <Arduino.h>
#include "timer.h"

/******************************************************************************
 *   Function: setup() - Initializes the Arduino System
 *      Pre condition:
 *          Hardware must be properly connected (sensors, LEDS, etc.)
 *      Post condition:
 *          Runs initialization calls one time on power up
 *          Input/Output (IO) pins are configured
 *******************************************************************************/
void setup() {
  uart_init();

  unsigned char pin;

  get_timer_pin(1, CHANNEL_A, &pin);

  ser_printf("Setup blinking on pin: %d", pin);

  //enable_interrupt();

  gpio_mode(pin, GPIO_OUTPUT);
  gpio_write(pin, OFF);

  DDRB |= (1 << PB1);
  config_interrupt(1, T1_INPUT_CAPTURE, ALLOW_INTERRUPT);

  if (setup_period_20ms_pwm(CHANNEL_A, to_duty_cycle(200, 1)))
    ser_printf("Error!");

  return;
}

int angles[] = {0, 10, 20, 30, 40, 50};

/******************************************************************************
 *   Function: loop() - Main execution loop
 *      Pre condition:
 *          setup() has been executed and system is initialized
 *      Post condition:
 *          Performs a single iteration of the system's function
 *          Repeates indefinetely unless the board is reset or powered off
 *******************************************************************************/
void loop() {

}

volatile uint16_t time_idx = 0;
volatile uint16_t i = 1;

ISR(TIMER1_CAPT_vect)
{
  time_idx ++;
  ser_printf("after 20ms!!, %d", time_idx);

  if (time_idx == 50)
  {
    ser_printf("after 1s im changing.");
    time_idx = 0;
    if (setup_period_20ms_pwm(CHANNEL_A, to_duty_cycle(200, i++)))
      ser_printf("Error!");

    if (i >= 19)
    {
      i = 1;
    }
  }
}