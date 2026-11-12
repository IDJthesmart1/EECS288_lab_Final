/******************************************************************************
 *   Header Includes
 *******************************************************************************/
#include <Arduino.h>
#include <stdint.h>
#include <stdio.h>

#include "eecs_388_lib.h"

/******************************************************************************
 *   Constant definitions
 *******************************************************************************/

/*ServoMotor constants*/
#define SERVO_PULSE_MAX (2400)    /* 2400 us */
#define SERVO_PULSE_MIN (544)     /* 544 us */
#define SERVO_PERIOD    (20000)   /* 20000 us (20ms) */
#define MIN_ANGLE       (0)       /* degrees */
#define MAX_ANGLE       (180)     /* degrees */


/******************************************************************************
 *   Local function prototypes
 *******************************************************************************/
static void auto_brake();
static void steering(int pos);

/******************************************************************************
 *   Function: auto_brake() - Auto Brake
 *      Pre condition:
 *          None
 *      Post condition:
 *          Checks the LiDAR distance and configures LEDs
 *******************************************************************************/
static void auto_brake() {
    // Task-2: 
    // Your code goes here (Use Lab 2 & 4 for reference)
    // Check the project document to understand the task
}


/******************************************************************************
 *   Function: steering() - Steering
 *      Pre condition:
 *          None
 *      Post condition:
 *          Control the servomotor with GPIO_6
 *******************************************************************************/
static void steering(int pos) {
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
}

/******************************************************************************
 *   Function: setup() - Initializes the Arduino System
 *      Pre condition:
 *          Hardware must be properly connected (BMP180 sensors, etc.)
 *      Post condition:
 *          Runs initialization calls one time on power up
 *          UART is initialized for ser_printf()
 *          I2C is initialized for communication with BMP180
 *          calib_data is filled with calibration data from BMP180
 *******************************************************************************/
void setup() {
  uart_init();     // Initialize UART for serial output

  //Setup Auto-break LEDs for Distance
  gpio_mode(GPIO_13, GPIO_OUTPUT); //RED
  gpio_mode(GPIO_12, GPIO_OUTPUT); //GREEN
  gpio_mode(GPIO_11, GPIO_OUTPUT); //BLUE

  //Setup GPIO_6 for PWM output
  gpio_mode(GPIO_6, GPIO_OUTPUT);

  ser_printf("System Initialized");
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
// Task-4: 
// Setup simulated code for the angles from the lab sheet
  int angle;

  // Task-1&2:
  // Receive data from arudino1 for the distance and setup the LEDs
  auto_brake();

  //  Task-4:
  for (int i = 0; i < 10; i++) {
    // Here, we set the angle to 180 if the prediction from the DNN is a positive angle
    // and 0 if the prediction is a negative angle.
    // This is so that it is easier to see the movement of the servo.
    // You are welcome to pass the angle values directly to the steering function.
    // If the servo function is written correctly, it should still work,
    // only the movements of the servo will be more subtle
    if (angle > 0) {
      steering(180);
    } else {
      steering(0);
    }

  // Uncomment the line below to see the actual angles on the servo.
  // Remember to comment out the if-else statement above!
  // steering(angle);
  }
}
