#include "engine.h"

int Engine::getRPM() const {
    return rpm;
}

void Engine::updateRPM(double speed, double throttle, double gearRatio, double finaldrive, double tireDiameter) {
    if (!engineState) {
        rpm = 0;
        return;
    }

    if (gearRatio == 0) {
        rpm = idleRPM + throttle * 2000;
        return;
    }

    double speed_mps = speed / 3.6;
    double tireCircumferance = PI * tireDiameter;
    double wheelRPM = (speed_mps / tireCircumferance) * 60.0;

    rpm = static_cast<int>(wheelRPM * gearRatio * finaldrive);

    rpm = std::clamp(rpm, idleRPM, redline);
}

bool Engine::getEngineState() const {
    return engineState;
}

double Engine::getEngineForce(double throttle, double gearRatio, double finaldrive, int rpm) {
        if (!engineState || throttle <= 0 || gearRatio == 0) return 0.0;

        double torque;
        if (rpm < 1500) torque = 500;
        else if (rpm < 3500) torque = 700;
        else if (rpm < 5500) torque = 650;
        else torque = 400;

        return throttle * torque * gearRatio * finaldrive;
}

void Engine::turnOffEngine() {
    if (engineState) engineState = false;
}

void Engine::turnOnEngine() {
    if (!engineState) engineState = true;
}
