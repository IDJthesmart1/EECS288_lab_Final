/************************************************************************/
/*                                                                      */
/* NAME:    timer.cpp                                                   */
/* AUTHOR:  William Sierra                                              */
/* DESC:    High-level helper methods and timer control                 */
/*                                                                      */
/************************************************************************/
extern "C" {
  #include <avr/interrupt.h>

  #include <stdint.h>
}

#include <timer.hpp>
#include <timerctrl.hpp>

/************************************************************************/
/* GLOBAL TIMER CONTROL                                                 */
/************************************************************************/

void enable_interrupt() {
  sei(); // set SREG I-bit
}

void disable_interrupt() {
  cli(); // clear SREG I-bit
}

/************************************************************************/
/* MATH HELPERS                                                         */
/************************************************************************/

uint32_t pow(uint32_t n, uint32_t e) {
  uint32_t out = 1;

  for (uint32_t i = 0; i < e; i++) {
    out *= n;
  }

  return out;
}

/************************************************************************/
/* TIMER CONTROLS                                                       */
/************************************************************************/

uint8_t _setup_t0period(uint32_t period_us, uint32_t* cycles, uint8_t* shift) {
  const uint32_t prescalar_values[5] = { 1, 8, 64, 256, 1024 };
  const uint8_t prescaler_codes[5] = {
      T0_PRESCALER_1,
      T0_PRESCALER_8,
      T0_PRESCALER_64,
      T0_PRESCALER_256,
      T0_PRESCALER_1024
  };
  static const uint32_t timer_width_bits = 8;

  for (int i = 0; i < 5; i++) {
    uint32_t p = prescalar_values[i];
    uint64_t max_cycles = pow(2, timer_width_bits);
    // +p/2 -> rounds up to the nearest int
    uint64_t request_cycles = ((uint64_t)period_us * 16 + p / 2) / p; // 16MHz * 1us -> 16
    if (request_cycles > max_cycles)
      continue;
    if (request_cycles == 0)
      return -1;
    *cycles = request_cycles - 1;
    *shift = prescaler_codes[i];
    return SUCCESS;
  }

  return INVALID_ARG;
}

uint8_t _setup_t1period(uint32_t period_us, uint32_t* cycles, uint8_t* shift) {
  const uint32_t prescalar_values[5] = { 1, 8, 64, 256, 1024 };
  const uint8_t prescaler_codes[5] = {
      T1_PRESCALER_1,
      T1_PRESCALER_8,
      T1_PRESCALER_64,
      T1_PRESCALER_256,
      T1_PRESCALER_1024
  };
  static const uint32_t timer_width_bits = 16;

  for (int i = 0; i < 5; i++) {
    uint32_t p = prescalar_values[i];
    uint64_t max_cycles = pow(2, timer_width_bits);
    // +p/2 -> rounds up to the nearest int
    uint64_t request_cycles = ((uint64_t)period_us * 16 + p / 2) / p; // 16MHz * 1us -> 16
    if (request_cycles > max_cycles)
      continue;
    if (request_cycles == 0)
      return -1;
    *cycles = request_cycles - 1;
    *shift = prescaler_codes[i];
    return 0;
  }

  return INVALID_ARG;
}

uint8_t _setup_t2period(uint32_t period_us, uint32_t* cycles, uint8_t* shift) {
  static const uint32_t prescalar_values[7] = { 1, 8, 32, 64, 128, 256, 1024 };
  const uint8_t prescaler_codes[7] = {
      T2_PRESCALER_1,
      T2_PRESCALER_8,
      T2_PRESCALER_32,
      T2_PRESCALER_64,
      T2_PRESCALER_128,
      T2_PRESCALER_256,
      T2_PRESCALER_1024
  };
  static const uint32_t timer_width_bits = 8;

  for (int i = 0; i < 7; i++) {
    uint32_t p = prescalar_values[i];
    uint64_t max_cycles = pow(2, timer_width_bits);
    // +p/2 -> rounds up to the nearest int
    uint64_t request_cycles = ((uint64_t)period_us * 16 + p / 2) / p; // 16MHz * 1us -> 16
    if (request_cycles > max_cycles)
      continue;
    if (request_cycles == 0)
      return -1;
    *cycles = request_cycles - 1;
    *shift = prescaler_codes[i];
    return SUCCESS;
  }

  return INVALID_ARG;
}

uint8_t setup_period(unsigned char timer, uint32_t period_us) {
  if (timer > 2)
    return INVALID_ARG;

  uint32_t cycles;
  uint8_t shift;

  if (timer == 0) {
    if (_setup_t0period(period_us, &cycles, &shift))
      return INVALID_ARG;

    config_clock(timer, 0);
    config_wgen(timer, T0_CTC_OUT);
  }

  if (timer == 1) {
    if (_setup_t1period(period_us, &cycles, &shift))
      return INVALID_ARG;

    config_clock(timer, 0);
    config_wgen(timer, T1_CTC_OUT);
  }

  if (timer == 2) {
    if (_setup_t2period(period_us, &cycles, &shift))
      return INVALID_ARG;

    config_clock(timer, 0);
    config_wgen(timer, T2_CTC_OUT);
  }

  config_compare(timer, CHANNEL_A, cycles);
  config_counter(timer, 0);
  config_clock(timer, shift);

  return SUCCESS;
}

// TODO: Finish method for other timers other than 1.
// Each have different features and are configured differently.
uint8_t setup_pwm(
  unsigned char timer,
  unsigned char channel,
  uint32_t frequency_hz,
  uint16_t duty_cycle
) {
  if (timer != 1 || channel > CHANNEL_B ||
    frequency_hz == 0 || duty_cycle > 1000)
    return INVALID_ARG;

  const uint16_t prescalers[5] = { 1, 8, 64, 256, 1024 };
  const uint8_t codes[5] = {
      T1_PRESCALER_1,
      T1_PRESCALER_8,
      T1_PRESCALER_64,
      T1_PRESCALER_256,
      T1_PRESCALER_1024
  };

  uint32_t top = 0;
  uint8_t clock = 0;

  for (uint8_t i = 0; i < 5; i++) {
    uint64_t counts = 16000000ULL /
      ((uint64_t)prescalers[i] * frequency_hz);

    if (counts < 2 || counts > 65536)
      continue;

    top = counts - 1;
    clock = codes[i];
    break;
  }

  if (clock == 0)
    return INVALID_ARG;

// Duty cycle expressed in tenths of a percent (0.0% – 100.0%).
  uint32_t compare = ((uint64_t)(top + 1) * duty_cycle) / 1000;

  config_clock(1, T1_STOPPED);
  config_wgen(1, T1_PWM_F_IN);
  config_t1_capture(top);
  config_compare(1, channel, compare);
  config_chan(1, channel, PWM_F_NINVERT);
  config_counter(1, 0);
  config_clock(1, clock);

  return SUCCESS;
}