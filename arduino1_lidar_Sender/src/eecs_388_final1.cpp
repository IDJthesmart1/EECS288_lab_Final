/******************************************************************************
 *   Header Includes
 *******************************************************************************/
#include <Arduino.h>
#include <stdint.h>
#include <stdio.h>

#include "eecs_388_lib.h"

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
void loop() {
  if ('Y' == ser_read() && 'Y' == ser_read()) {
    uint8_t Dist_L = ser_read();
    uint8_t Dist_H = ser_read();
    uint16_t distance = Dist_L | (Dist_H << 8); /* LIDAR distance data is 16 bits. */
    uint8_t Strength_L = ser_read();
    uint8_t Strength_H = ser_read();
    uint16_t strength = Strength_L | (Strength_H << 8);
    uint8_t reserved = ser_read();
    uint8_t quality = ser_read();
    // Checksum is the lower 8 bits of the sum of the first 8 bytes of actual data
    uint8_t calculated_checksum = (uint8_t)('Y' + 'Y' + Dist_L + Dist_H + Strength_L + Strength_H + reserved + quality);
    uint8_t checksum = ser_read();
    // distance is a 16-bit value, with a valid range of 30 to 1200 cm

    if (checksum == calculated_checksum) { // Transmission is valid, process it
      distance -= 30;
      distance /= 2;
      if (distance > 255) { distance = 255; }
      // distance is now a value between 0 and 255 where each unit represents 2 cm
      // Valid range of 30 - 540 cm (technically, anything above 200 cm is considered "safe to proceed")
      ser_write((uint8_t)distance);
    }
  }
  return;
}