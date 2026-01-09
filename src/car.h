#pragma once
#include <iostream>
#include "curses.h"
#include <unistd.h>
#include <cmath>
#include <algorithm>

#include "brake.h"
#include "dashboard.h"
#include "fueltank.h"
#include "engine.h"
#include "transmission.h"
#include "physics.h"
#include "shiftpolicy.h"

class Car {
private: 

    Brake brake;
    Dashboard dash;
    FuelTank tank;        
    Engine engine;
    Transmission trans;
    Physics p;
    ShiftPolicy policy;

    double v, a, t, pos = 0;
    bool gasPressed, brakePressed, useAutoTransmission = false;
    
public:
    Car();
    void tick();
};
