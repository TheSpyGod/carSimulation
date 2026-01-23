#pragma once
#include "engine.h"
#include "transmission.h"
#include "fueltank.h"
#include "brake.h"
#include "constants.h"
#include "environment.h"
class Physics {
    double getResistanceForce(double speed);
public:
    bool absOn = false;
    bool tcsOn = false;
    void physicsStep(double &v, double &a, double t, double &pos, FuelTank &tank, Engine &engine, Transmission &trans, Brake &brake, bool brakePressed, const Environment &env);
};
