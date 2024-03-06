#include "pid_classes.h"

int _base_pid_switchcase::_calculate_error_with_switchcase_method(int shift)
{
    // clang-format off
    int error = 0;
    int dataSensorLocal = dataSensor;
    if (shift != 0){dataSensorLocal = dataSensorLocal >> shift;}

    switch (dataSensorLocal) {
        case 0b00011000: error = 0;    break;
        case 0b00110000: error = 1;    break;
        case 0b00100000: error = 2;    break;
        case 0b01100000: error = 3;    break;
        case 0b01000000: error = 4;    break;
        case 0b11000000: error = 5;    break;
        case 0b10000000: error = 6;    break;

        case 0b00001100: error = -1;   break;
        case 0b00000100: error = -2;   break;
        case 0b00000110: error = -3;   break;
        case 0b00000010: error = -4;   break;
        case 0b00000011: error = -5;   break;
        case 0b00000001: error = -6;   break;
    }
    // clang-format on
    return error;
};