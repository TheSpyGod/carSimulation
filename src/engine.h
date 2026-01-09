#pragma once
#include <algorithm>
#include "constants.h"

class Engine {
private:
    bool engineState = false;
    int rpm = 800;
    const int idleRPM = 800;
    const int redline = 6500;

public:
    int getRPM() const;
    void updateRPM(double speed, double throttle, double gearRatio, double finaldrive, double tireDiameter);
    bool getEngineState() const;
    void turnOnEngine();
    void turnOffEngine();
    double getEngineForce(double throttle, double gearRatio, double finalDrive, int rpm);
};
