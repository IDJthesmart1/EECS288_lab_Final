/******************************************************************************
 *   Header Includes
 *******************************************************************************/
#include <Arduino.h>

#include "eecs_388_lib2.h"

//------------------------------------------------------------------------------
// Application specific definitions - From Lab 4
//------------------------------------------------------------------------------
struct LED {
  uint8_t pin;
  bool isOn;
};

LED red = {GPIO_13, false};
LED green = {GPIO_12, false};
LED blue = {GPIO_11, false};
//-------------------------------------------------------------------------------

/******************************************************************************
 *   Function: setup() - Initializes the Arduino System
 *      Pre condition:
 *          Hardware must be properly connected (LEDS, RX/TX from arduino1)
 *      Post condition:
 *          Runs initialization calls one time on power up
 *			UART is initialized
 *          Input/Output (IO) pins are configured for LEDs
 *******************************************************************************/
void setup() {
  uart_init();
  gpio_mode(GPIO_13, GPIO_OUTPUT);
  gpio_mode(GPIO_12, GPIO_OUTPUT);
  gpio_mode(GPIO_11, GPIO_OUTPUT);

  return;
}

//------------------------------------------------------------------------------
// Custom functions - From Lab 4
//------------------------------------------------------------------------------

void toggle(LED &led) {
  if (led.isOn) {
    gpio_write(led.pin, OFF);
    led.isOn = false;
  } else {
    gpio_write(led.pin, ON);
    led.isOn = true;
  }
}

void turnOn(LED &led) {
  gpio_write(led.pin, ON); // Active high
  led.isOn = true;
}

void turnOff(LED &led) {
  gpio_write(led.pin, OFF); // Active high
  led.isOn = false;
}
//------------------------------------------------------------------------------

/******************************************************************************
 *   Function: loop() - Main execution loop
 *      Pre condition:
 *          setup() has been executed and system is initialized
 *      Post condition:
 *          Performs a single iteration of the system's function
 *          Repeates indefinetely unless the board is reset or powered off
 *******************************************************************************/
void loop() {
  char char_rcvd;

  // Task 1.2 - Receive a character from the arduino1_sender, light up the
  // appropriate LED, and send the acknowldegment 'A' character back.
  char_rcvd = ser_read();
  if (char_rcvd == 'r') {
    turnOn(red);
    turnOff(green);
    turnOff(blue);
    ser_write('A');

  } else if (char_rcvd == 'g') {
    turnOn(green);
    turnOff(red);
    turnOff(blue);
    ser_write('A');

  } else if (char_rcvd == 'b') {
    turnOn(blue);
    turnOff(red);
    turnOff(green);
    ser_write('A');

  } else {
    turnOff(red);
    turnOff(green);
    turnOff(blue);
    ser_write('F');
  }
  return;
}