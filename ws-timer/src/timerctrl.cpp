/************************************************************************/
/*                                                                      */
/* NAME:    timerctrl.cpp                                               */
/* AUTHOR:  William Sierra                                              */
/* DESC:    Low-level register wrapper methods and constants.           */
/*                                                                      */
/************************************************************************/

#include <timerctrl.h>

/************************************************************************/
/* TIMER WAVEFORM CONTROLS                                              */
/************************************************************************/

uint8_t config_wgen(unsigned char timer, unsigned char wgm)
{
    if (timer > 2)
        return INVALID_ARG;
    
    if (timer == 0)
    {
        if (wgm > 7 || wgm == 4 || wgm == 6)
            return INVALID_ARG;

        TCCR0A = (TCCR0A & ~0b11) | (wgm & 0b11);
        TCCR0B = (TCCR0B & ~0b1000) | ((wgm & 0b100) << 1);
    }

    if (timer == 1)
    {
        if (wgm > 15 || wgm == 13)
            return INVALID_ARG;

        TCCR1A = (TCCR1A & ~0b11) | (wgm & 0b11);
        TCCR1B = (TCCR1B & ~0b11000) | ((wgm & 0b1100) << 1);
    }

    if (timer == 2)
    {
        if (wgm > 7 || wgm == 4 || wgm == 6)
            return INVALID_ARG;
        TCCR2A = (TCCR2A & ~0b11) | (wgm & 0b11);
        TCCR2B = (TCCR2B & ~0b1000) | ((wgm & 0b100) << 1);
    }

    return SUCCESS;
}

/************************************************************************/
/* TIMER OUTPUT CHANNEL CONTROL                                         */
/************************************************************************/

uint8_t config_chan(unsigned char timer, unsigned char channel, unsigned char ctrl)
{
    if (ctrl > 0b11 || timer > 2 || channel > 1)
        return INVALID_ARG;
    
    volatile uint8_t *reg;

    if (timer == 0) reg = &TCCR0A;
    if (timer == 1) reg = &TCCR1A;
    if (timer == 2) reg = &TCCR2A;

    if (channel == CHANNEL_A)
    {
        *reg = (*reg & ~0b11000000) | (ctrl << 6);
    }
    if (channel == CHANNEL_B)
    {
        *reg = (*reg & ~0b00110000) | (ctrl << 4);
    }

    return SUCCESS;
}

uint8_t get_timer_pin(unsigned char timer, unsigned char channel, unsigned char *pin)
{
    if (channel > 1 || timer > 2)
        return INVALID_ARG;

    if (timer == 0) *pin = channel == CHANNEL_A ? GPIO_6 : GPIO_5;
    if (timer == 1) *pin = channel == CHANNEL_A ? GPIO_9 : GPIO_10;
    if (timer == 2) *pin = channel == CHANNEL_A ? GPIO_11 : GPIO_3;

    return SUCCESS;
}

/************************************************************************/
/* TIMER CLOCK CONTROL                                                  */
/************************************************************************/

uint8_t config_clock(unsigned char timer, unsigned char clk)
{
    if (clk > 0b111 || timer > 2)
        return INVALID_ARG;
    
    volatile uint8_t *reg;

    if (timer == 0) reg = &TCCR0B;
    if (timer == 1) reg = &TCCR1B;
    if (timer == 2) reg = &TCCR2B;

    *reg = (*reg & ~0b111) | (clk);

    return SUCCESS;
}

/************************************************************************/
/* TIMER CAPTURE CONTROL                                                */
/************************************************************************/

uint8_t config_t1_capture_settings(unsigned char edge, unsigned char filter)
{
    if (edge > 1 || filter > 1)
        return INVALID_ARG;

    TCCR1B = (TCCR1B & ~((0x1 << ICES1) | (0x1 << ICNC1))) | (edge << ICES1) | (filter << ICNC1);
    return SUCCESS;
}

uint8_t force_output(unsigned char timer, unsigned char channel)
{
    if (timer > 2 || channel > 1)
        return INVALID_ARG;

    if (timer == 0) TCCR0B |= (1 << (channel == CHANNEL_A ? FOC0A : FOC0B));
    if (timer == 1) TCCR1C |= (1 << (channel == CHANNEL_A ? FOC1A : FOC1B));
    if (timer == 2) TCCR2B |= (1 << (channel == CHANNEL_A ? FOC2A : FOC2B));

    return SUCCESS;
}

/************************************************************************/
/* TIMER OUTPUT COMPARE                                                 */
/************************************************************************/

uint8_t config_compare(unsigned char timer, unsigned char channel, uint16_t comp)
{
    if (timer > 2 || channel > 1)
        return INVALID_ARG;

    if (timer == 0)
    {
        if (comp > 0xFF)
            return INVALID_ARG;
        volatile uint8_t *reg = channel == CHANNEL_A ? &OCR0A : &OCR0B;
        *reg = comp;
    }

    if (timer == 1)
    {
        volatile uint16_t *reg = channel == CHANNEL_A ? &OCR1A : &OCR1B;
        *reg = comp;
    }

    if (timer == 2)
    {
        if (comp > 0xFF)
            return INVALID_ARG;
        volatile uint8_t *reg = channel == CHANNEL_A ? &OCR2A : &OCR2B;
        *reg = comp;
    }

    return SUCCESS;
}

uint8_t get_compare(unsigned char timer, unsigned char channel, uint16_t *comp)
{
    if (timer > 2 || channel > 1)
        return INVALID_ARG;

    if (timer == 0) *comp = channel == CHANNEL_A ? OCR0A : OCR0B;
    if (timer == 1) *comp = channel == CHANNEL_A ? OCR1A : OCR1B;
    if (timer == 2) *comp = channel == CHANNEL_A ? OCR2A : OCR2B;

    return SUCCESS;
}

/************************************************************************/
/* TIMER COUNTER                                                        */
/************************************************************************/

uint8_t config_counter(unsigned char timer, uint16_t count)
{
    if (timer > 2)
        return INVALID_ARG;

    if (timer == 0)
    {
        if (count > 0xFF)
            return INVALID_ARG;
        TCNT0 = count;
    }

    if (timer == 1)
    {
        TCNT1 = count;
    }

    if (timer == 2)
    {
        if (count > 0xFF)
            return INVALID_ARG;
        TCNT2 = count;
    }

    return SUCCESS;
}

uint8_t get_counter(unsigned char timer, uint16_t *count)
{
    if (timer > 2)
        return INVALID_ARG;

    if (timer == 0) *count = TCNT0;
    if (timer == 1) *count = TCNT1;
    if (timer == 2) *count = TCNT2;

    return SUCCESS;
}

/************************************************************************/
/* TIMER INPUT CAPTURE                                                  */
/************************************************************************/

uint8_t config_t1_capture(uint16_t value)
{
    ICR1 = value;
    return SUCCESS;
}

uint8_t get_t1_capture(uint16_t *value)
{
    *value = ICR1;
    return SUCCESS;
}

uint8_t config_t1_capture_src(unsigned char src)
{
    if (src > 1)
        return INVALID_ARG;

    ACSR = (ACSR & ~(0x1 << ACIC)) | (src << ACIC);

    return SUCCESS;
}

/************************************************************************/
/* TIMER INTERRUPT CONTROL                                              */
/************************************************************************/

uint8_t config_interrupt(unsigned char timer, unsigned char interrupt, unsigned char allow)
{
    if (timer > 2 || allow > 1)
        return INVALID_ARG;

    if (interrupt > 2 && interrupt != ICIE1)
        return INVALID_ARG;

    if (timer != 1 && interrupt == T1_INPUT_CAPTURE)
        return INVALID_ARG;

    if (timer == 0) TIMSK0 = (TIMSK0 & ~(0x1 << interrupt)) | (allow << interrupt);
    if (timer == 1) TIMSK1 = (TIMSK1 & ~(0x1 << interrupt)) | (allow << interrupt);
    if (timer == 2) TIMSK2 = (TIMSK2 & ~(0x1 << interrupt)) | (allow << interrupt);

    return SUCCESS;
}

uint8_t get_interrupted(unsigned char timer, unsigned char interrupt, unsigned char *happened)
{
    if (timer > 2)
        return INVALID_ARG;

    if (interrupt > 2 && interrupt != T1_INPUT_CAPTURED)
        return INVALID_ARG;

    if (timer != 1 && interrupt == T1_INPUT_CAPTURED)
        return INVALID_ARG;
    
    if (timer == 0) *happened = (TIFR0 & (0x1 << interrupt)) >> interrupt;
    if (timer == 1) *happened = (TIFR1 & (0x1 << interrupt)) >> interrupt;
    if (timer == 2) *happened = (TIFR2 & (0x1 << interrupt)) >> interrupt;

    return SUCCESS;
}

uint8_t clear_interrupted(unsigned char timer, unsigned char interrupt)
{
    if (timer > 2)
        return INVALID_ARG;

    if (interrupt > 2 && interrupt != T1_INPUT_CAPTURED)
        return INVALID_ARG;

    if (timer != 1 && interrupt == T1_INPUT_CAPTURED)
        return INVALID_ARG;

    // TIFRn flags are cleared by writing 1 to the corresponding bit.
    // Writing 0 leaves the flag unchanged.
    if (timer == 0) TIFR0 = (0x1 << interrupt);
    if (timer == 1) TIFR1 = (0x1 << interrupt);
    if (timer == 2) TIFR2 = (0x1 << interrupt);

    return SUCCESS;
}

/************************************************************************/
/* TIMER SYNC CONTROL                                                   */
/************************************************************************/

uint8_t config_sync(unsigned char enable)
{
    if (enable > 1)
        return INVALID_ARG;

    GTCCR = (GTCCR & ~(1 << TSM)) | (enable << TSM);
    return SUCCESS;
}

uint8_t reset_prescalar(unsigned char timer)
{
    if (timer > 2)
        return INVALID_ARG;
    
    if (timer == 0 || timer == 1)
        GTCCR |= (0x1 << PSRSYNC);
    
    if (timer == 2)
        GTCCR |= (0x1 << PSRASY);

    return SUCCESS;
}

/************************************************************************/
/* TIMER 2 ASYNC STATUS CONTROL                                         */
/************************************************************************/

uint8_t config_t2_async(unsigned char enable)
{
    if (enable > 1)
        return INVALID_ARG;

    ASSR = (ASSR & ~(0x1 << AS2)) | (enable << AS2);
    return SUCCESS;
}

uint8_t config_t2_external(unsigned char enable)
{
    if (enable > 1)
        return INVALID_ARG;

    ASSR = (ASSR & ~(0x1 << EXCLK)) | (enable << EXCLK);
    return SUCCESS;
}

uint8_t get_t2_busy(unsigned char flag, unsigned char *busy)
{
    if (flag > 4)
        return INVALID_ARG;

    *busy = (ASSR >> flag) & T2_BUSY;

    return SUCCESS;
}

/************************************************************************/
/* TIMER POWER REDUCTION CONTROL                                        */
/************************************************************************/

uint8_t config_power_reduction(unsigned char feat, unsigned char enabled)
{
    if (feat == 4 || feat > 7 || enabled > 1)
        return INVALID_ARG;

    PRR = (PRR & ~(0x1 << feat)) | (enabled << feat);

    return SUCCESS;
}

uint8_t get_power_reduction(unsigned char feat, unsigned char *enabled)
{
    if (feat == 4 || feat > 7)
        return INVALID_ARG;

    *enabled = (PRR >> feat) & PR_ENABLED;

    return SUCCESS;
}