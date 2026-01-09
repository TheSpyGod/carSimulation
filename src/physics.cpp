#include "physics.h"
    double Physics::getResistanceForce(double speed) {
        const double c_roll = 300;
        const double c_drag = 0.25;

        return -c_roll - c_drag * speed * speed;
    }

    void Physics::physicsStep(double &v, double &a, double t, double &pos, FuelTank &tank, Engine &engine, Transmission &trans, Brake &brake, bool brakePressed) {
        double driveForce = engine.getEngineForce(t,trans.getGearRatio(), trans.getFinalDrive(), engine.getRPM());
        double brakeForce = brake.getBrakeForce(brakePressed);
        double resistance = getResistanceForce(v);

        double totalForce = driveForce + brakeForce + resistance;

        a = totalForce / MASS_KG;
        v += a * DT * 3.6;

        if (v < 0) v = 0;
        if (v > MAX_SPEED_KMH) v = MAX_SPEED_KMH;

        pos += (v / 3.6) * DT;

        engine.updateRPM(v, t, trans.getGearRatio(), trans.getFinalDrive(), 0.64);

        if (driveForce > 0) tank.useFuel(t * v * 0.00005 * DT);
    }
