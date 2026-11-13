/******************************************************************************
 *   Header Includes
 *******************************************************************************/
#include <Arduino.h>
#include <stdint.h>
#include <stdio.h>
#include "timer.h"
#include "eecs_388_lib.h"

/******************************************************************************
 *   Function: setup() - Initializes the Arduino System
 *      Pre condition: 
 *          Hardware must be properly connected (sensors, LEDS, etc.)
 *      Post condition: 
 *          Runs initialization calls one time on power up
 *          Input/Output (IO) pins are configured
 *******************************************************************************/
void setup() 
{
    uart_init();
    enable_interrupt();
    set_interrupt_timer(1e6, 1);
    gpio_mode(GPIO_13, GPIO_OUTPUT);
    gpio_write(GPIO_13, OFF);
    return;
}

volatile uint8_t on = 0;

ISR(TIMER1_COMPA_vect) {
    gpio_write(GPIO_13, on ? ON : OFF);
    on = on ? 0 : 1;
}

/******************************************************************************
 *   Function: loop() - Main execution loop
 *      Pre condition: 
 *          setup() has been executed and system is initialized
 *      Post condition: 
 *          Performs a single iteration of the system's function
 *          Repeates indefinetely unless the board is reset or powered off
 *******************************************************************************/
void loop() 
{

}