/******************************************************************************
 *   Header Includes
 *******************************************************************************/
#include <avr/io.h>
#include <stdint.h>

#include "eecs_388_lib.h"
#include "wiring_private.h"
#include "pins_arduino.h"
#include "timer.h"

/******************************************************************************
 *  Timer Interrupt Functions
 *******************************************************************************/

uint16_t get_cycles(uint8_t timer) {
	if (timer == 0)
		return TCNT0;
	if (timer == 1)
		return TCNT1;
	if (timer == 2)
		return TCNT2;
	return 0;
}

int set_cycles(uint16_t cycle, uint8_t timer) {
	if (timer == 0)
	{
		if (cycle > 255) // ensure cycles do not exceed 8 bits for this timer.
			return -1;
		TCNT0 = 0;
		OCR0A = cycle;
		return 0;
	}
	if (timer == 1)
	{
		TCNT1 = 0;
		OCR1A = cycle;
		return 0;
	}
	if (timer == 2)
	{
		if (cycle > 255)
			return -1;
		TCNT2 = 0;
		OCR2A = cycle;
		return 0;
	}
	return -1;
}

int stop_timer(uint8_t timer)
{
    if (timer == 0) {
        TCCR0B &= ~((1 << CS02) | (1 << CS01) | (1 << CS00));
        return 0;
    }
    if (timer == 1) {
        TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
        return 0;
    }
    if (timer == 2) {
        TCCR2B &= ~((1 << CS22) | (1 << CS21) | (1 << CS20));
        return 0;
    }
    return -1;
}

void enable_interrupt(void) {
	sei(); // set SREG I-bit
}

void disable_interrupt(void) {
	cli(); // clear SREG I-bit
}

int enable_timer_interrupt(uint8_t shift, uint8_t timer)
{
	// Setup CTC mode and prescalar value
	if (timer == 0)
	{
		TIFR0 = (0x1 << OCF0A);
		TCCR0A = (0x1 << WGM01);
		TCCR0B = shift;
		TIMSK0 |= (0x1 << OCIE0A);
		return 0;
	}
	if (timer == 1)
	{
		TIFR1 = (0x1 << OCF1A);
		TCCR1A = 0;
		TCCR1B = (0x1 << WGM12) | shift;
		TIMSK1 |= (0x1 << OCIE1A);
		return 0;
	}
	if (timer == 2)
	{
		TIFR2 = (0x1 << OCF2A);
		TCCR2A = (0x1 << WGM21);
		TCCR2B = shift;
		TIMSK2 |= (0x1 << OCIE2A);
		return 0;
	}
	return -1;
}

int disable_timer_interrupt(int timer) {
	if (timer == 0)
	{
		TIMSK0 &= ~(0x1 << OCIE0A);
		return 0;
	}
	if (timer == 1)
	{
		TIMSK1 &= ~(0x1 << OCIE1A);
		return 0;
	}
	if (timer == 2)
	{
		TIMSK2 &= ~(0x1 << OCIE2A);
		return 0;
	}
	return -1;
}

uint32_t pow(uint32_t n, uint32_t e)
{
	uint32_t out = 1;

	for (uint32_t i = 0; i < e; i++)
	{
		out *= n;
	}

	return out;
}

int _set_interrupt_timer_0(uint32_t period_us, uint32_t *cycles, uint8_t *shift)
{
	const uint32_t prescalar_values[5] = {1, 8, 64, 256, 1024};
	const uint8_t prescalar_shifts[5] = {
		(0x1 << CS00), 
		(0x1 << CS01), 
		(0x1 << CS00) | (0x1 << CS01),
		(0x1 << CS02),
		(0x1 << CS02) | (0x1 << CS00)
	};
	static const uint32_t timer_width_bits = 8;

	for (int i = 0; i < 5; i++)
	{
		uint32_t p = prescalar_values[i];
		uint64_t max_cycles = pow(2, timer_width_bits);
		// +p/2 -> rounds up to the nearest int
		uint64_t request_cycles = ((uint64_t) period_us * 16 + p/2) / p; // 16MHz * 1us -> 16
		if (request_cycles > max_cycles)
			continue;
		if (request_cycles == 0)
			return -1;
		*cycles = request_cycles - 1;
		*shift = prescalar_shifts[i];
		return 0;
	}

	return -1;
}

int _set_interrupt_timer_1(uint32_t period_us, uint32_t *cycles, uint8_t *shift)
{
	const uint32_t prescalar_values[5] = {1, 8, 64, 256, 1024};
	const uint8_t prescalar_shifts[5] = {
		(0x1 << CS10), 
		(0x1 << CS11), 
		(0x1 << CS10) | (0x1 << CS11),
		(0x1 << CS12),
		(0x1 << CS12) | (0x1 << CS10)
	};
	static const uint32_t timer_width_bits = 16;

	for (int i = 0; i < 5; i++)
	{
		uint32_t p = prescalar_values[i];
		uint64_t max_cycles = pow(2, timer_width_bits);
		// +p/2 -> rounds up to the nearest int
		uint64_t request_cycles = ((uint64_t) period_us * 16 + p/2) / p; // 16MHz * 1us -> 16
		if (request_cycles > max_cycles)
			continue;
		if (request_cycles == 0)
			return -1;
		*cycles = request_cycles - 1;
		*shift = prescalar_shifts[i];
		return 0;
	}

	return -1;
}

int _set_interrupt_timer_2(uint32_t period_us, uint32_t *cycles, uint8_t *shift)
{
	static const uint32_t prescalar_values[7] = {1, 8, 32, 64, 128, 256, 1024};
	static const uint8_t prescalar_shifts[7] = {
		(0x1 << CS20), 									// 1 
		(0x1 << CS21),  								// 8
		(0x1 << CS20) | (0x1 << CS21),  				// 32
		(0x1 << CS22),									// 64
		(0x1 << CS22) | (0x1 << CS20),					// 128
		(0x1 << CS22) | (0x1 << CS21),  				// 256
		(0x1 << CS22) | (0x1 << CS21) | (0x1 << CS20) 	// 1024
	};
	static const uint32_t timer_width_bits = 8;

	for (int i = 0; i < 7; i++)
	{
		uint32_t p = prescalar_values[i];
		uint64_t max_cycles = pow(2, timer_width_bits);
		// +p/2 -> rounds up to the nearest int
		uint64_t request_cycles = ((uint64_t) period_us * 16 + p/2) / p; // 16MHz * 1us -> 16
		if (request_cycles > max_cycles)
			continue;
		if (request_cycles == 0) 
			return -1;
		*cycles = request_cycles - 1;
		*shift = prescalar_shifts[i];
		return 0;
	}

	return -1;
}

/* Timer 0: 8bits, 	maximum time 0.016384s or 16.384ms
 * Timer 1: 16bits, maximum time 4.194304s or 4194.304ms
 * Timer 2: 8bits, 	maximum time 0.016384s or 16.384ms
 *
 * Returns 0 if successful, or -1 if bad arguments.
 */
int set_interrupt_timer(uint32_t period_us, uint8_t timer)
{
	if (timer > 2) return -1;

	uint32_t cycles;
	uint8_t shift;

	if (timer == 0 && _set_interrupt_timer_0(period_us, &cycles, &shift))
		return -1;
	if (timer == 1 && _set_interrupt_timer_1(period_us, &cycles, &shift))
		return -1;
	if (timer == 2 && _set_interrupt_timer_2(period_us, &cycles, &shift))
		return -1;

	if (stop_timer(timer))
		return -1;

	enable_timer_interrupt(shift, timer);
	set_cycles(cycles, timer);
	return 0;
}