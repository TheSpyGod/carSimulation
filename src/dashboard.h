#pragma once
#include <cmath>
#include "curses.h"
#include "transmission.h"
#include "brake.h"
#include "fueltank.h"
#include "engine.h"
#include "constants.h"
#include <unistd.h>

class Dashboard {
    double calculateFuelAverage(double fuel, double distance);
        
public: 
    void displayInfo(double v, double pos, double t, bool brakePressed, FuelTank &tank, Engine &engine, int gear, bool useAutomaticTransmission);
    void pressedPedal(bool &useAutomaticTransmission, bool &brakePressed, bool &gasPressed, double &t, FuelTank &tank, Engine &engine, Transmission &trans);
};
