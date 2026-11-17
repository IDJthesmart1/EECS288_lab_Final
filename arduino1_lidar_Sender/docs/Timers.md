### ATmega328P Timer Specifications (Internal RC Oscillator Setup)

| Specification / Attribute    | Timer/Counter0                    | Timer/Counter1                     | Timer/Counter2                     |
| ---------------------------- | --------------------------------- | ---------------------------------- | ---------------------------------- |
| **Bit Resolution**           | 8-bit                             | 16-bit                             | 8-bit                              |
| **Maximum Count Value**      | 255 (0xFF)                        | 65,535 (0xFFFF)                    | 255 (0xFF)                         |
| **Timer Control Register**   | `TCCR0A`                          | `TCCR1A`                           | `TCCR2A`                           |
| **Count-to Register**        | `TCNT0`                           | `TCNT1H` / `TCNT1L` (High/Low bit) | `TCNT2`                            |
| **Channel A Pin Connection** | **`OC0A`** (Pin PD6 / Arduino D6) | **`OC1A`** (Pin PB1 / Arduino D9)  | **`OC2A`** (Pin PB3 / Arduino D11) |
| **Channel B Pin Connection** | **`OC0B`** (Pin PD5 / Arduino D5) | **`OC1B`** (Pin PB2 / Arduino D10) | **`OC2B`** (Pin PD3 / Arduino D3)  |
| **Output Compare Registers** | `OCR0A`, `OCR0B`                  | `OCR1AH`/`L`, `OCR1BH`/`L`         | `OCR2A`, `OCR2B`                   |
| **Prescaler Options**        | 1, 8, 64, 256, 1024               | 1, 8, 64, 256, 1024                | 1, 8, 32, 64, 128, 256, 1024       |
| **Special Features**         | None                              | **Input Capture Mode**             | **Asyncronous Mode**               |
|                              |                                   | Times an input signal precisely    | Runs on an External Crystal timer  |