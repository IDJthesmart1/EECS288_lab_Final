#include <Arduino.h>
#include "eecs_388_lib.h"

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

uint8_t interrupt_count = 0;
//volatile uint8_t is_red = 1;
//volatile uint8_t led_state = 0;

void setup()
{
    // init GPIO pins
    gpio_mode(GPIO_13, GPIO_OUTPUT); // red
    gpio_mode(GPIO_12, GPIO_OUTPUT); // green
    gpio_mode(GPIO_11, GPIO_OUTPUT); // blue
    
    gpio_write(GPIO_13, OFF);
    gpio_write(GPIO_12, OFF);
    gpio_write(GPIO_11, OFF);

    // setup timer for 100 ms
    enable_timer_interrupt();
    // Task 3.1 - Determine the amount of cycles for a 100ms interrupt timer
    set_cycles(6250); 

    enable_interrupt();
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
 *  INTERRUPT SERVICE ROUTINE. This is the function the ATmega328P jumps to when TCNT1 == OCR1A
 *******************************************************************************/
ISR(TIMER1_COMPA_vect) {
	//Task 3.2 - Write code to blink the red LED every 100ms for 1 second.  Then
	//alternate to the green LED and blink every 100ms for 1 second.  Keep alternating 
	//between the two

    //is_red = interrupt_count < 10;
    if (interrupt_count < 10) {
        toggle(red);
    } else if (interrupt_count < 20) {
        toggle(green);
    }
    interrupt_count = (interrupt_count + 1) % 20;
}

void loop() {
	//Our main loop can run and will only have brief interruptions when the 100ms timer expires
    delay_ms(1000);
    toggle(blue);
}