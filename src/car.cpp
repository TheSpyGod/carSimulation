#include "car.h"
Car::Car() {
    trans.setPolicy(&policy);
}

void Car::tick() {
    /*dash.pressedPedal(useAutoTransmission, brakePressed, gasPressed, t, tank, engine, trans, env);

    if (!paused) p.physicsStep(v, a, t, pos, tank, engine, trans, brake, brakePressed, env);

    engine.updateRPM(v, t, trans.getGearRatio(), trans.getFinalDrive(), 0.64);

    if (useAutoTransmission) trans.useAutomatic(engine.getRPM(), engine.getEngineState());

    dash.displayInfo(v, pos, t, brakePressed, tank, engine, trans.getGear(), useAutoTransmission, p.absOn, p.tcsOn, env);
    */ 

    dash.pressedPedal(useAutoTransmission, brakePressed, gasPressed, t,
                      tank, engine, trans, env, paused, reset, captureFrame);

    // 2. Physics update if not paused
    if (!paused) {
        p.physicsStep(v, a, t, pos, tank, engine, trans, brake, brakePressed, env);
    }

    // 3. Update engine RPM (must be BEFORE automatic transmission!)
    engine.updateRPM(v, t, trans.getGearRatio(), trans.getFinalDrive(), 0.64);

    // 4. Automatic transmission
    if (useAutoTransmission) {
        trans.useAutomatic(engine.getRPM(), engine.getEngineState());
    }

    // 5. Update dashboard display
    dash.displayInfo(v, pos, t, brakePressed, tank, engine, trans.getGear(),
                     useAutoTransmission, p.absOn, p.tcsOn, env, paused);

    // 6. Reset simulation
    if (reset) {
        v = 0; a = 0; pos = 0; t = 0;
        tank.f = 45; tank.fuelUsed = 0;
        engine.turnOffEngine();
        engine.updateRPM(0, 0, 0, 0, 0.64);
        trans.switchGear(false); // Reset to gear 1
        paused = false;
        reset = false;
    }

    // 7. Capture frame (CSV)
    if (captureFrame) {
        std::ofstream file("frame.csv");
        file << v << "," << a << "," << pos << "," << tank.f << "," << engine.getRPM() << "\n";
        file.close();
        captureFrame = false;
    }
}
