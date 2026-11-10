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
    uint16_t distance =
        Dist_L | (Dist_H << 8); /* LIDAR distance data is 16 bits. */
    uint8_t Strength_L = ser_read();
    uint8_t Strength_H = ser_read();
    uint16_t strength = Strength_L | (Strength_H << 8);
    uint8_t reserved = ser_read();
    uint8_t quality = ser_read();
    // Checksum parity bit is a low 8-bit, Checksum = Byte1 + Byte2 + ... +
    // Byte8, Checksum is the sum of the first 8 bytes of actual data; here is
    // only a low 8-bit
    uint8_t calculated_checksum =
        (uint8_t)('Y' + 'Y' + Dist_L + Dist_H + Strength_L + Strength_H +
                  reserved + quality);
    uint8_t checksum = ser_read();
  }
  return;
}