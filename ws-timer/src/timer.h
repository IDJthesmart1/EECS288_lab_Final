/************************************************************************/
/*                                                                      */
/* NAME:    timer.h                                                     */
/* AUTHOR:  William Sierra                                              */
/* DESC:    High-level helper methods and timer control                 */
/*                                                                      */
/************************************************************************/

#ifndef TIMER_HL_API
#define TIMER_HL_API

#include <timerctrl.h>

/************************************************************************/
/* GLOBAL TIMER CONTROL                                                 */
/************************************************************************/

void enable_interrupt();
void disable_interrupt();

/************************************************************************/
/* MATH HELPERS                                                         */
/************************************************************************/

uint32_t pow(uint32_t n, uint32_t e);

/************************************************************************/
/* TIMER CONTROLS                                                       */
/************************************************************************/

/************************************************************************/
/* Timer 0: 8bits, 	maximum time 0.016384s or 16384us                   */
/* Timer 1: 16bits, maximum time 4.194304s or 4194304us                 */
/* Timer 2: 8bits, 	maximum time 0.016384s or 16384us                   */
/************************************************************************/
uint8_t setup_period(unsigned char timer, uint32_t period_us);

/************************************************************************/
/* Timer 0: Fast PWM, fixed TOP=255, max frequency 62.5kHz              */
/* Timer 1: Fast PWM, fixed TOP=1, max frequency 8MHz                   */
/* Timer 2: Fast PWM, fixed TOP=255, max frequency 62.5kHz              */
/* Timer 0/2: Fast PWM, fixed TOP=OCRnA, max frequency 8MHz             */
/************************************************************************/
uint8_t setup_pwm(
  unsigned char timer,
  unsigned char channel,
  uint16_t frequency_Hz,
  uint16_t duty_cycle_pct
);


#endif