/******************************************************************************
 *   Header Includes
 *******************************************************************************/

#include "timer.h"

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

    unsigned char pin;

    setup_period(1, 1e6);
    config_chan(1, CHANNEL_A, CTCNORM_TOGGLE_OUT);
    get_timer_pin(1, CHANNEL_A, &pin);

    ser_printf("Setup blinking on pin: %d", pin);

    gpio_mode(pin, GPIO_OUTPUT);
    return;
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