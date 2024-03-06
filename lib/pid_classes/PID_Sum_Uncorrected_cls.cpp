#include "pid_classes.h"

void PID_Sum_Uncorrected_cls::Compute() {
    int error = _calculate_error_with_sum_method();
    _compute_pid_and_setmotors(error);
}