/************************************************************************/
/*                                                                      */
/* NAME:    timerctrl.h                                                 */
/* AUTHOR:  William Sierra                                              */
/* DESC:    Low-level register wrapper methods and constants.           */
/*                                                                      */
/************************************************************************/

#ifndef TIMER_CTRL_API
#define TIMER_CTRL_API

#include <avr/io.h>
#include <eecs_388_lib.h>

/************************************************************************/
/* API TIMER CODES                                                      */
/************************************************************************/

#define SUCCESS       (0)
#define INVALID_ARG   (1 << 0)
#define NO_OPERATION  (1 << 1)
#define RESERVED_2    (1 << 2)
#define RESERVED_3    (1 << 3)
#define RESERVED_4    (1 << 4)
#define RESERVED_5    (1 << 5)
#define RESERVED_6    (1 << 6)
#define RESERVED_7    (1 << 7)

/************************************************************************/
/* TIMER WAVEFORM CONTROLS                                              */
/************************************************************************/

#define T0_NORMAL       0
#define T0_PWM_PC       1
#define T0_CTC_OUT      2
#define T0_PWM_F        3
#define T0_PWM_PC_OUT   5
#define T0_PWM_F_OUT    7

#define T1_NORMAL       0
#define T1_PWM_PC8      1
#define T1_PWM_PC9      2
#define T1_PWM_PC10     3
#define T1_CTC_OUT      4
#define T1_PWM_F8       5
#define T1_PWM_F9       6
#define T1_PWM_F10      7
#define T1_PWM_PCF_IN   8
#define T1_PWM_PCF_OUT  9
#define T1_PWM_PC_IN    10
#define T1_PWM_PC_OUT   11
#define T1_CTC_IN       12
#define T1_PWM_F_IN     14
#define T1_PWM_F_OUT    15

#define T2_NORMAL       0
#define T2_PWM_PC       1
#define T2_CTC_OUT      2
#define T2_PWM_F        3
#define T2_PWM_PC_OUT   5
#define T2_PWM_F_OUT    7

// Configures the waveform generation mode of the selected timer.
uint8_t config_wgen(unsigned char timer, unsigned char wgm);

/************************************************************************/
/* TIMER OUTPUT CHANNEL CONTROL                                         */
/************************************************************************/

#define CTCNORM_DISCONNECT  (0b00)
#define CTCNORM_TOGGLE_OUT  (0b01)
#define CTCNORM_CLEAR_OUT   (0b10)
#define CTCNORM_SET_OUT     (0b11)

#define PWM_F_DISCONNECT    (0b00)
#define PWM_F_TOGGLE        (0b01)
#define PWM_F_NINVERT       (0b10)
#define PWM_F_INVERT        (0b11)

#define CHANNEL_A   (0b0)
#define CHANNEL_B   (0b1)

/************************************************************************/
/* CHANNEL PINS                                                         */
/* - Timer 0/A: D6                                                      */
/* - Timer 0/B: D5                                                      */
/* - Timer 1/A: D9                                                      */
/* - Timer 1/B: D10                                                     */
/* - Timer 2/A: D11                                                     */
/* - Timer 2/B: D3                                                      */
/************************************************************************/

// Configures how an output compare channel controls its hardware output pin.
uint8_t config_chan(unsigned char timer, unsigned char channel, unsigned char ctrl);

// Retrives the GPIO pin associated with a timer's output compare channel (see CHANNEL PINS).
uint8_t get_timer_pin(unsigned char timer, unsigned char channel, unsigned char* pin);

/************************************************************************/
/* TIMER CLOCK CONTROL                                                  */
/************************************************************************/

#define T0_STOPPED          (0b000)
#define T0_PRESCALER_1      (0b001)
#define T0_PRESCALER_8      (0b010)
#define T0_PRESCALER_64     (0b011)
#define T0_PRESCALER_256    (0b100)
#define T0_PRESCALER_1024   (0b101)
#define T0_EXT_FALL         (0b110)
#define T0_EXT_RISE         (0b111)

#define T1_STOPPED          (0b000)
#define T1_PRESCALER_1      (0b001)
#define T1_PRESCALER_8      (0b010)
#define T1_PRESCALER_64     (0b011)
#define T1_PRESCALER_256    (0b100)
#define T1_PRESCALER_1024   (0b101)
#define T1_EXT_FALL         (0b110)
#define T1_EXT_RISE         (0b111)

#define T2_STOPPED          (0b000)
#define T2_PRESCALER_1      (0b001)
#define T2_PRESCALER_8      (0b010)
#define T2_PRESCALER_32     (0b011)
#define T2_PRESCALER_64     (0b100)
#define T2_PRESCALER_128    (0b101)
#define T2_PRESCALER_256    (0b110)
#define T2_PRESCALER_1024   (0b111)

// Selects the timer clock source or prescalar, including stopping the timer.
uint8_t config_clock(unsigned char timer, unsigned char clk);

/************************************************************************/
/* TIMER CAPTURE CONTROL                                                */
/************************************************************************/

#define T1_FILTER_NOISE (1)
#define T1_NO_FILTER    (0)
#define T1_EDGE_RISE    (1)
#define T1_EDGE_FALL    (0)

// Sets Timer 1's input capture edge selection and noise cancellation filter.
uint8_t config_t1_capture_settings(unsigned char edge, unsigned char filter);

// Forces an output compare action on the selected channel (in non-PWM mode).
uint8_t force_output(unsigned char timer, unsigned char channel);

/************************************************************************/
/* TIMER OUTPUT COMPARE                                                 */
/************************************************************************/

// Writes to the output compare register for the selected timer and channel.
uint8_t config_compare(unsigned char timer, unsigned char channel, uint16_t comp);

// Retrieves the current counter compare value of the selected timer.
uint8_t get_compare(unsigned char timer, unsigned char channel, uint16_t* comp);

/************************************************************************/
/* TIMER COUNTER                                                        */
/************************************************************************/

// Sets the current counter value of the selected timer.
uint8_t config_counter(unsigned char timer, uint16_t count);

// Retrieves the current counter value of the selected timer.
uint8_t get_counter(unsigned char timer, uint16_t* count);

/************************************************************************/
/* TIMER INPUT CAPTURE                                                  */
/************************************************************************/

// Writes to Timer 1's input capture register (ICR1).
uint8_t config_t1_capture(uint16_t value);

// Retrieves the value in Timer 1's input capture register (ICR1).
uint8_t get_t1_capture(uint16_t* value);

#define T1_SRC_ICP1         (0)
#define T1_SRC_ANALOG_COMP  (1)

// Selects Timer 1's input capture source.
uint8_t config_t1_capture_src(unsigned char src);

/************************************************************************/
/* TIMER INTERRUPT CONTROL                                              */
/************************************************************************/

#define ALLOW_INTERRUPT (1)
#define BLOCK_INTERRUPT (0)

#define COUNTER_OVERFLOW    (0)
#define CHANNEL_A_COMPARE   (1)
#define CHANNEL_B_COMPARE   (2)
#define T1_INPUT_CAPTURE    (ICIE1)

// Enables or disables interrupts for the selected timer.
uint8_t config_interrupt(unsigned char timer, unsigned char interrupt, unsigned char allow);

#define CHANNEL_A_INTERRUPTED   (1)
#define CHANNEL_B_INTERRUPTED   (2)
#define T1_INPUT_CAPTURED       (ICF1)

#define OCCURRED    (1)
#define NO_OCCUR    (0)

// Checks whether the interrupt flag is set for the selected timer.
uint8_t get_interrupted(unsigned char timer, unsigned char interrupt, unsigned char* happened);

// Clears the interrupt flag for the selected timer.
uint8_t clear_interrupted(unsigned char timer, unsigned char interrupt);

/************************************************************************/
/* TIMER SYNC CONTROL                                                   */
/************************************************************************/

#define SYNC_ENABLE  (1)
#define SYNC_DISABLE (0)

// Enables or disables timer synchronization mode through the general timer control register.
uint8_t config_sync(unsigned char enable);

// Resets the prescaler on the selected timer.
uint8_t reset_prescalar(unsigned char timer);

/************************************************************************/
/* TIMER 2 ASYNC STATUS CONTROL                                         */
/************************************************************************/

#define T2_OP_ENABLE     (1)
#define T2_OP_DISABLE    (0)

// Enables or disables Timer 2's asynchronous clock operation.
uint8_t config_t2_async(unsigned char enable);

// Configures Timer 2's external clock input option for asynchronous operation.
uint8_t config_t2_external(unsigned char enable);

#define T2_BUSY  (1)
#define T2_READY (0)

#define T2_COUNTER_BUSY    (TCN2UB)
#define T2_COMPARE_A_BUSY  (OCR2AUB)
#define T2_COMPARE_B_BUSY  (OCR2BUB)
#define T2_CONTROL_A_BUSY  (TCR2AUB)
#define T2_CONTROL_B_BUSY  (TCR2BUB)

// Reports whether a selected Timer 2 asynchronous register update is pending.
uint8_t get_t2_busy(unsigned char flag, unsigned char* busy);

/************************************************************************/
/* TIMER POWER REDUCTION CONTROL                                        */
/************************************************************************/

#define PR_ENABLED     (1)
#define PR_DISABLED    (0)

#define PR_T0       (PRTIM0)
#define PR_T1       (PRTIM1)
#define PR_T2       (PRTIM2)
#define PR_SPI      (PRSPI)
#define PR_USART    (PRUSART0)
#define PR_ADC      (PRADC)
#define PR_I2C      (PRTWI)

// Enables or disables power reduction for a selected peripheral.
uint8_t config_power_reduction(unsigned char feat, unsigned char enabled);

// Retrieves current power reduction state for the selected peripheral.
uint8_t get_power_reduction(unsigned char feat, unsigned char* enabled);

#endif