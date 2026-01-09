#pragma once
#include "engine.h"
#include "transmission.h"
#include "fueltank.h"
#include "brake.h"
#include "constants.h"
class Physics {
    double getResistanceForce(double speed);
public:
    void physicsStep(double &v, double &a, double t, double &pos, FuelTank &tank, Engine &engine, Transmission &trans, Brake &brake, bool brakePressed);
};
