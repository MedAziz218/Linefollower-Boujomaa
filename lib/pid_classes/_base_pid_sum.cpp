#include "pid_classes.h"

int _base_pid_sum::_calculate_error_with_sum_method()
{
    int error = 0;
    for (int i = 0; i < 8; i++)
    {
        error += s[i] * values[i];
    }
    return error;
};