/******************************************************************************
 *   Header Includes
 *******************************************************************************/

extern "C" {
#include <Arduino.h>
#include <stdint.h>
}

#include "eecs_388_lib.hpp"

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

// Simulated code for the angles from the lab sheet
const int ANGLE_VALUES[] = { 10, 25, 75, 45, 100, 40, 125, 15, 150, 50, 170 };
//------------------------------------------------------------------------------
// Application specific definitions
//------------------------------------------------------------------------------
struct LED {
  uint8_t pin;
  uint8_t state; // 0 = off, 1 = on, 2 = flashing
};

LED red = { GPIO_13, false };
LED green = { GPIO_12, false };
LED blue = { GPIO_11, false };

struct Servo {
  uint8_t pin;
  int angle_state;
};

Servo steering = { GPIO_7, MIN_ANGLE };

struct Timer {
  uint8_t periodic_ISR_vector;
  uint8_t timer_gpio_pin;
};

Timer ctc_timer;

//-------------------------------------------------------------------------------

void toggle(LED& led) {
  if (led.state == 0) {
    turnOn(led);
  } else {
    turnOff(led);
  }
}

void turnOn(LED& led) {
  gpio_write(led.pin, ON);
  led.state = 1;
}

void turnOff(LED& led) {
  gpio_write(led.pin, OFF);
  led.state = 0;
}

void flash(LED& led) {
  enable(ctc_timer.timer_gpio_pin);
  led.state = 2;
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

  // Initialize 100ms CTC Timer for LED flashing
  ctc_timer = ctc_init(100000); // Convert ms to us
}

/******************************************************************************
 *   Function: ISR(USART_RX_vect) - UART Receive Data Interrupt Service Routine
 *      Pre condition:
 *          setup() has been executed and system is initialized
 *      Post condition:
 *          Performs a single iteration of the system's function
 *          Repeates indefinetely unless the board is reset or powered off
 *******************************************************************************/
ISR(USART_RX_vect) {
  // distance is now a value between 0 and 255 where each unit represents 2 cm
  // Valid range of 30 - 540 cm (technically, anything above 200 cm is considered "safe to proceed")
  uint8_t distance_received = ser_read();
  if (distance_received > 200) {
    turnOn(green);
    turnOff(red);
  } else if (distance_received > 100) {
    turnOn(red);
    turnOn(green);
  } else if (distance_received > 60) {
    turnOn(red);
    turnOff(green);
  } else {
    flash(red);
    turnOff(green);
  }

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
  /*
  / 15.9.2 Clear Timer on Compare Match Mode, and 15.9.3 Fast PWM Mode are available
  /
  / Plan:
  /
  / UART communication: LiDAR reading will be triggered by receive complete interrupt,
  / from the LiDAR sensor to Arduino1, and then from Arduino1 to Arduino2.
  /
  / Break control: The LED state will be set in the same ISR that reads the data line,
  / so the state changes as soon as possible and the data never has to be stored in memory.
  / A CTCM Timer will control the red pin directly when flashing,
  / but loses control in any other state.
  / The rest of the time changes will be standard GPIO writes.
  /
  / Servo control: CTC Timer with 20ms period will control the pin directly,
  / an ISR will only trigger at the end of the period,
  / and update the compare match register with the new duty cycle every 20ms.
  /
  / Speed simulation: A global int will track "speed."
  / We need to determine reasonable acceleration and deceleration rates to hardcode.
  / Once that is done a CTCM timer can run a ISR at consistant intervals to update the speed variable,
  / according to the amount of breaking being applied. The speed will also scale the steering speed,
  / so that the servo will not turn as fast at high speeds.
  /
  /
  / LED flashing timer: Mode: CTC, Period: 100ms, Duty cycle: 50%
  / Servo PWM timer: Mode: Fast PWM, Period: 20ms, Duty cycle: set to the calculated angle, ISR: Update the duty cycle every iteration.
  /
  / Speed will scale the servo speed so that the steering will be faster at low speeds and slower at high speeds.
  / The servo angle represents the absolute angle of the car, and it takes longer to turn at high speeds. - Approved by TA
  */


  for (int i : ANGLE_VALUES) {
    goToAngle(steering, i);
  }
}