#include "brake.h"
double Brake::getBrakeForce(bool brakeIsPressed) {
    if (!brakeIsPressed) return 0.0;
    
    const double k_brake = 8000;
    return -k_brake;
}
