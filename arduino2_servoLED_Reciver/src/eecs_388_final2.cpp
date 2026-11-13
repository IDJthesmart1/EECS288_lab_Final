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
#define SERVO_PULSE_MAX (2400)    /* 2400 us (2.4ms) */
#define SERVO_PULSE_MIN (544)     /* 544 us (0.544ms) */
#define SERVO_PERIOD    (20000)   /* 20000 us (20ms) */
#define MIN_ANGLE       (0)       /* degrees */
#define MAX_ANGLE       (180)     /* degrees */
// Speed assumes no load and slowest case of 4.8V
#define SERVO_SPEED     (2000)    /* 2000 us/deg (0.12 seconds per 60 degrees) */

#define SERVO_1         (GPIO_6)
int angle_state_1;

//------------------------------------------------------------------------------
// Application specific definitions
//------------------------------------------------------------------------------
struct LED {
  uint8_t pin;
  bool isOn;
};

LED red = { GPIO_13, false };
LED green = { GPIO_12, false };
LED blue = { GPIO_11, false };

struct Servo {
  uint8_t pin;
  int angle_state;
};

Servo steering = { GPIO_6, MIN_ANGLE };
//-------------------------------------------------------------------------------

void toggle(LED& led) {
  if (led.isOn) {
    gpio_write(led.pin, OFF);
    led.isOn = false;
  } else {
    gpio_write(led.pin, ON);
    led.isOn = true;
  }
}

void turnOn(LED& led) {
  gpio_write(led.pin, ON);
  led.isOn = true;
}

void turnOff(LED& led) {
  gpio_write(led.pin, OFF);
  led.isOn = false;
}

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


const unsigned long pulse_range = SERVO_PULSE_MAX - SERVO_PULSE_MIN;
const unsigned long angle_range = MAX_ANGLE - MIN_ANGLE;
/******************************************************************************
 *   Function: goToAngle() - Servo Motor Pulse
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
static void goToAngle(Servo servo, int angle) {
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

  int travel_degrees = abs(angle - servo.angle_state);
  int loops = (travel_degrees * SERVO_SPEED) / SERVO_PERIOD;
  for (int i = 0; i <= loops; i++) {
    gpio_write(servo.pin, 1);
    delay_ms(on_time_us / 1000);
    delay_us(on_time_us % 1000);
    gpio_write(servo.pin, 0);
    delay_ms(off_time_us / 1000);
    delay_us(off_time_us % 1000);
  }
  servo.angle_state = angle;

  return;
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
  int angle[] = { 10, 25, 75, 45, 100, 40, 125, 15, 150, 50, 170 };

  // Task-1&2:
  // Receive data from arudino1 for the distance and setup the LEDs
  auto_brake();

  //  Task-4:
  for (int i : angle) {
    goToAngle(steering, angle[i]);
  }
}