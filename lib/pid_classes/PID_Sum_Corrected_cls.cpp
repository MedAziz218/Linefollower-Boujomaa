#include "pid_classes.h"

void PID_Sum_Corrected_cls::Compute() {
    int error = _calculate_error_with_sum_method();
    
    if (dataSensor == 0b00000000)
    {
      error = lastOnLineSomme < 3.5 ? correctionValue : -correctionValue;
    }
    _compute_pid_and_setmotors(error);
}