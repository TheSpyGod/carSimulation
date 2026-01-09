#include "car.h"
Car::Car() {
    trans.setPolicy(&policy);
}

void Car::tick() {
    dash.pressedPedal(useAutoTransmission, brakePressed, gasPressed, t, tank, engine, trans);

    p.physicsStep(v, a, t, pos, tank, engine, trans, brake, brakePressed);

    engine.updateRPM(v, t, trans.getGearRatio(), trans.getFinalDrive(), 0.64);

    if (useAutoTransmission) trans.useAutomatic(engine.getRPM(), engine.getEngineState());

    dash.displayInfo(v, pos, t, brakePressed, tank, engine, trans.getGear(), useAutoTransmission);
}
