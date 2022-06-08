#pragma once

#define INPUT_ANY 0x3

#define digitalReadF(pin) (*(volatile uint8_t*)port_to_input_PGM[digital_pin_to_port_PGM[pin]] & digital_pin_to_bit_mask_PGM[pin])
#define digitalWriteF(pin, v) {\
    if (v) {*(volatile uint8_t*)port_to_output_PGM[digital_pin_to_port_PGM[pin]] |= digital_pin_to_bit_mask_PGM[pin];} \
    else   {*(volatile uint8_t*)port_to_output_PGM[digital_pin_to_port_PGM[pin]] &= ~digital_pin_to_bit_mask_PGM[pin];}\
}
#define digitalToggleF(pin) (*(volatile uint8_t*)port_to_input_PGM[digital_pin_to_port_PGM[pin]] = digital_pin_to_bit_mask_PGM[pin])
#define pinModeF(pin, pm) {switch (pm){\
    case INPUT:        *(volatile uint8_t*)port_to_mode_PGM[digital_pin_to_port_PGM[pin]] &= ~digital_pin_to_bit_mask_PGM[pin]; digitalWriteF(pin, 0);break;\
    case INPUT_PULLUP: *(volatile uint8_t*)port_to_mode_PGM[digital_pin_to_port_PGM[pin]] &= ~digital_pin_to_bit_mask_PGM[pin]; digitalWriteF(pin, 1); break;\
    case INPUT_ANY:    *(volatile uint8_t*)port_to_mode_PGM[digital_pin_to_port_PGM[pin]] &= ~digital_pin_to_bit_mask_PGM[pin];  break;\
    case OUTPUT:       *(volatile uint8_t*)port_to_mode_PGM[digital_pin_to_port_PGM[pin]] |=  digital_pin_to_bit_mask_PGM[pin];  break;}}
