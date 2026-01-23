#include "physics.h"
#include "constants.h"
    double Physics::getResistanceForce(double speed) {
        const double c_roll = 150;
        const double c_drag = 0.05;

        return -(c_roll + c_drag * speed * speed);
    }
    void Physics::physicsStep(double &v, double &a, double t, double &pos, FuelTank &tank, Engine &engine, Transmission &trans, Brake &brake, bool brakePressed, const Environment &env) {
 
        double driveForce = engine.getEngineForce(t,trans.getGearRatio(), trans.getFinalDrive(), engine.getRPM(), 0.64);
        double brakeForce = brake.getBrakeForce(brakePressed, v);
        double resistance = getResistanceForce(v);

        tcsOn = false;
        absOn = false;

        double frictionLimit = env.surface.mu * MASS_KG * gravity;
        double slopeForce = env.getSlopeForce();

        if (driveForce > frictionLimit) {
            driveForce = frictionLimit;
            tcsOn = true;
        } else {
            tcsOn = false;
        }

        double totalForce = driveForce + brakeForce + resistance + slopeForce;

        if (totalForce > frictionLimit) totalForce = frictionLimit;
        if (totalForce < -frictionLimit) totalForce = -frictionLimit;

        if (brakeForce < 0 && std::abs(totalForce) < -F_max) {
            totalForce = -F_max;
            absOn = true;
        }

        a = totalForce / MASS_KG;
        v += a * DT * 3.6;

        if (v < 0) v = 0;
        if (v > MAX_SPEED_KMH) v = MAX_SPEED_KMH;

        pos += (v / 3.6) * DT;

        engine.updateRPM(v, t, trans.getGearRatio(), trans.getFinalDrive(), 0.64);

        if (driveForce > 0) tank.useFuel(t * v * 0.00005 * DT);
  
}
