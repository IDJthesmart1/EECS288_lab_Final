/******************************************************************************
 *   Header Includes
 *******************************************************************************/
#include <Arduino.h>
#include <stdint.h>

#include "eecs_388_lib.h"

//------------------------------------------------------------------------------
// Application specific definitions
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
 *          Hardware must be properly connected (sensors, LEDS, etc.)
 *      Post condition: 
 *          Runs initialization calls one time on power up
 *          Input/Output (IO) pins are configured
 *******************************************************************************/
void setup() 
{
gpio_mode(red.pin, GPIO_OUTPUT);
gpio_mode(green.pin, GPIO_OUTPUT);
gpio_mode(blue.pin, GPIO_OUTPUT);

uart_init();

return;
}

//------------------------------------------------------------------------------
// Custom functions
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
    gpio_write(led.pin, ON);
    led.isOn = true;
}

void turnOff(LED &led) {
    gpio_write(led.pin, OFF);
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
    turnOff(red);
    turnOff(green);
    turnOff(blue);

    ser_printline("Setup completed.");
    ser_write( '\n' );

    while( 1 ) { 
        /* 
            Task 1.2: 
            - read a data frame from the TFmini sensor
            
            - from Table 6, we know each LIDAR measurement sends 9 bytes
            - the function `ser_read()` can process 1 byte per function call

            - This if-statement below will read (and check) the first two bytes of a valid data frame
            - You will need to write code to read the subsequent bytes to get the distance data
            - You will need to process the data received from the sensor to get the correct distance value
        */
        if ('Y' == ser_read() && 'Y' == ser_read()) {
            uint8_t Dist_L = ser_read();
            uint8_t Dist_H = ser_read();
            uint16_t distance = Dist_L | (Dist_H << 8);              /* LIDAR distance data is 16 bits. */
            uint8_t Strength_L = ser_read();
            uint8_t Strength_H = ser_read();
            uint16_t strength = Strength_L | (Strength_H << 8);
            uint8_t reserved = ser_read();
            uint8_t quality = ser_read();
            // Checksum parity bit is a low 8-bit, Checksum = Byte1 + Byte2 + ... + Byte8,
            // Checksum is the sum of the first 8 bytes of actual data; here is only a low 8-bit
            uint8_t calculated_checksum = (uint8_t)('Y' + 'Y' + Dist_L + Dist_H + Strength_L + Strength_H + reserved + quality);
            uint8_t checksum = ser_read();

            if (checksum == calculated_checksum) {
            /* 
                Task 1.3: 
                - turn on the red LED if the distance measured is less than 50 cm. 
                - otherwise turn on the green LED 

                - print the measured value to the console (i.e. the serial monitor) 
                - You can use ser_printf() or ser_printline() or ser_write()
                
                - Helpful guide for printing with variadic function similiar to printf()
                - https://en.cppreference.com/w/cpp/io/c/fprintf
            */
                if (distance <= 30) {
                    turnOff(red);
                    turnOff(green);
                    turnOn(blue);
                } else if (distance < 50) {
                  turnOff(blue);
                  turnOff(green);
                  turnOn(red);
                } else if (distance < 1200) {
                  turnOff(red);
                  turnOff(blue);
                  turnOn(green);
                } else {
                  turnOff(red);
                  turnOff(green);
                  turnOn(blue);
                }

                ser_printf("Distance: %u cm", distance);
                ser_printf("Strength: %u", strength);
            }
        }
    }

    return;
}