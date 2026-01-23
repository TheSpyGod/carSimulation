#include "brake.h"
double Brake::getBrakeForce(bool brakeIsPressed, double velocity) {
    if (!brakeIsPressed) return 0.0;

    const double k_brake = 12000;
    return (velocity > 0 ? -k_brake : k_brake);
}
