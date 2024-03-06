#include "pid_classes.h"

void PID_SwitchCase_Uncorrected_cls::Compute() {
    int error = _calculate_error_with_switchcase_method();
    _compute_pid_and_setmotors(error);
}