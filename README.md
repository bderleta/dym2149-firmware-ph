# dym2149-firmware-ph
Very basic firmware for [Dual YM2149 synth board](https://github.com/bderleta/dym2149-board)

Listens on UART (500000bps, 1 stop bit, no parity) for 2 byte packets.
Every 2 byte packet contains:
- address byte
- register value byte

Higher 4 bits of address byte indicate a device address:
- `0b0000xxxx` - set value of register `0b0000xxxx` on both PSGs
- `0b0001xxxx` - set value of register `0b0000xxxx` on first PSG
- `0b0010xxxx` - set value of register `0b0000xxxx` on second PSG

Returns `ACK` byte for every byte received, or `NAK`, if the buffer is full.

Contains some stubs for board peripherals (timers, shift register, etc.), but they are not being used at the moment.
