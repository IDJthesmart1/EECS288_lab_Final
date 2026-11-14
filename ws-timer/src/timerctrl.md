# `timerctrl.h` Method Reference

Low-level ATmega328P timer register wrappers, grouped by the sections in `timerctrl.h`.

## Timer Waveform Controls

- **`config_wgen(unsigned char timer, unsigned char wgm)`** — Configures the waveform generation mode of the selected timer.

## Timer Output Channel Control

- **`config_chan(unsigned char timer, unsigned char channel, unsigned char ctrl)`** — Configures how an output compare channel controls its hardware output pin.
- **`get_timer_pin(unsigned char timer, unsigned char channel, unsigned char *pin)`** — Retrieves the GPIO pin associated with a timer's output compare channel.

## Timer Clock Control

- **`config_clock(unsigned char timer, unsigned char clk)`** — Selects the timer clock source or prescaler, including stopping the timer.

## Timer Capture Control

- **`config_t1_capture_settings(unsigned char edge, unsigned char filter)`** — Sets Timer1's input capture edge selection and noise cancellation filter.
- **`force_output(unsigned char timer, unsigned char channel)`** — Forces an output compare action on the selected channel in a non-PWM mode.

## Timer Output Compare

- **`config_compare(unsigned char timer, unsigned char channel, uint16_t comp)`** — Writes the output compare register for the selected timer and channel.
- **`get_compare(unsigned char timer, unsigned char channel, uint16_t *comp)`** — Reads the output compare register for the selected timer and channel.

## Timer Counter

- **`config_counter(unsigned char timer, uint16_t count)`** — Sets the current counter value of the selected timer.
- **`get_counter(unsigned char timer, uint16_t *count)`** — Retrieves the current counter value of the selected timer.

## Timer Input Capture

- **`config_t1_capture(uint16_t value)`** — Writes Timer1's input capture register (`ICR1`).
- **`get_t1_capture(uint16_t *value)`** — Reads Timer1's input capture register (`ICR1`).
- **`config_t1_capture_src(unsigned char src)`** — Selects the Timer1 input capture source between the ICP1 pin and analog comparator.

## Timer Interrupt Control

- **`config_interrupt(unsigned char timer, unsigned char interrupt, unsigned char allow)`** — Enables or disables a selected timer interrupt source.
- **`get_interrupted(unsigned char timer, unsigned char interrupt, unsigned char *happened)`** — Checks whether a selected timer interrupt flag is set.
- **`clear_interrupted(unsigned char timer, unsigned char interrupt)`** — Clears a selected timer interrupt flag.

## Timer Sync Control

- **`config_sync(unsigned char enable)`** — Enables or disables timer synchronization mode through the general timer control register.
- **`reset_prescalar(unsigned char timer)`** — Resets the prescaler associated with the selected timer.

## Timer2 Asynchronous Status Control

- **`config_t2_async(unsigned char enable)`** — Enables or disables Timer2 asynchronous clock operation.
- **`config_t2_external(unsigned char enable)`** — Configures Timer2's external clock input option for asynchronous operation.
- **`get_t2_busy(unsigned char flag, unsigned char *busy)`** — Reports whether a selected Timer2 asynchronous register update is pending.

## Timer Power Reduction Control

- **`config_power_reduction(unsigned char feat, unsigned char enabled)`** — Enables or disables power reduction for a selected peripheral.
- **`get_power_reduction(unsigned char feat, unsigned char *enabled)`** — Reads the power reduction state of a selected peripheral.
