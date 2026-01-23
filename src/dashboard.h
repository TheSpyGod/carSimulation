#pragma once
#include <cmath>
#include "curses.h"
#include "transmission.h"
#include "brake.h"
#include "fueltank.h"
#include "engine.h"
#include "constants.h"
#include <unistd.h>
#include "environment.h"
#include <set>

class Dashboard {
    double calculateFuelAverage(double fuel, double distance);
    std::set<int> pressedKeys;
        
public: 
    void displayInfo(double v, double pos, double t, bool brakePressed, FuelTank &tank, Engine &engine, int gear, bool useAutomaticTransmission, bool absOn, bool tcsOn, const Environment &env, bool paused);
    void pressedPedal(bool &useAutomaticTransmission, bool &brakePressed, bool &gasPressed, double &t, FuelTank &tank, Engine &engine, Transmission &trans, Environment &env, bool &paused, bool &reset, bool &captureFrame);

    void end();
};
