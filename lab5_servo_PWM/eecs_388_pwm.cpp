/******************************************************************************
 *   Header Includes
 *******************************************************************************/
#include <Arduino.h>
#include <stdint.h>
#include <stdio.h>
//#include <stdlib.h>

#include "eecs_388_lib.h"

#define SERVO_PULSE_MAX (2400)    /* 2400 us (2.4ms) */
#define SERVO_PULSE_MIN (544)     /* 544 us (0.544ms) */
#define SERVO_PERIOD    (20000)   /* 20000 us (20ms) */
// Speed assumes no load and worst case of 4.8V
#define SERVO_SPEED     (2000)    /* 2000 us/deg (0.12 seconds per 60 degrees) */

#define MIN_ANGLE       (0)       /* degrees */
#define MAX_ANGLE       (180)     /* degrees */
#define DEGREE_STEP     (30)      /* degrees */

#define SERVO_1         (GPIO_6)

int angle_state;

const unsigned long pulse_range = SERVO_PULSE_MAX - SERVO_PULSE_MIN;
const unsigned long angle_range = MAX_ANGLE - MIN_ANGLE;
/******************************************************************************
 *   Function: servo() - Servo Motor Pulse
 *      Pre condition:
 *          Assumes GPIO pin used has been configured for output
 *          Assumes GPIO pin is off at call time
 *      Post condition:
 *          Calculates a duty cycle in microseconds for the amount of on time
 *          needed to achieve the angle passed in
 *      Input:
 *          gpio - GPIO pin number to use for the servo
 *          angle - Angle to set the servo to (0 to 180 degrees)
 *******************************************************************************/
static void servo(int gpio, int angle) {
  unsigned long int on_time_us;
  unsigned long int off_time_us;
  if (angle < MIN_ANGLE) {
    on_time_us = SERVO_PULSE_MIN;
  } else if (angle > MAX_ANGLE) {
    on_time_us = SERVO_PULSE_MAX;
  } else {
    on_time_us = SERVO_PULSE_MIN + ((unsigned long)(angle - MIN_ANGLE) * pulse_range) / angle_range;
  }
  off_time_us = SERVO_PERIOD - on_time_us;

  //int travel_degrees = abs(angle - angle_state);
  //int loops = (travel_degrees * SERVO_SPEED) / SERVO_PERIOD;
  //for (int i = 0; i <= loops; i++) {
  gpio_write(gpio, 1);
  delay_ms(on_time_us / 1000);
  delay_us(on_time_us % 1000);
  gpio_write(gpio, 0);
  delay_ms(off_time_us / 1000);
  delay_us(off_time_us % 1000);
  //}
  //angle_state = angle;

  return;
}

/******************************************************************************
 *   Function: setup() - Initializes the Arduino System
 *      Pre condition:
 *          Hardware must be properly connected (sensors, LEDS, etc.)
 *      Post condition:
 *          Runs initialization calls one time on power up
 *          Input/Output (IO) pins are configured
 *******************************************************************************/
void setup() {
  gpio_mode(SERVO_1, GPIO_OUTPUT);

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
    /**
     * Sweeps through angles 0 -> 180 by 30 to test the 'servo' function
     *
     * The inner loop calls the function 50 times, causing each angle
     * to be held on the servo for ~1 second.
     */
  for (int angle = MIN_ANGLE; angle <= MAX_ANGLE; angle += DEGREE_STEP) {
      /* control the servo for 1 sec duration */
    for (int i = 0; i < 50; i++) {
      servo(SERVO_1, angle);
    }
  }

  // Sweeps through angles 180 -> 0 by 30 to test the 'servo' function
  for (int angle = MAX_ANGLE; angle >= MIN_ANGLE; angle -= DEGREE_STEP) {
      /* control the servo for 1 sec duration */
    for (int i = 0; i < 50; i++) {
      servo(SERVO_1, angle);
    }
  }

  return;
}