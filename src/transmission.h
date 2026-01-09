#pragma once
#include "shiftpolicy.h"
class Transmission {
private:
    int gear = 1;
    const double final_drive = 3.9;
    ShiftPolicy* policy = nullptr;
public:
    void setPolicy(ShiftPolicy* p);
    int getGear() const;
    double getFinalDrive() const;
    double getGearRatio() const;
    void switchGear(bool incrementGear);
    void useAutomatic(int rpm, bool engineState);
};
