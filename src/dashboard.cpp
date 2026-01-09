#include "dashboard.h"
double Dashboard::calculateFuelAverage(double fuel, double distance) {
    if (distance < 1e-6) return 0.0;
    return (fuel / (distance / 1000) * 100);
}

void Dashboard::displayInfo(double v, double pos, double t, bool brakePressed, FuelTank &tank, Engine &engine, int gear, bool useAutomaticTransmission) {
    double fuelAverage = calculateFuelAverage(tank.fuelUsed, pos);

    mvprintw(0, 0, "Predkosc: %.0f [km/h], Pozycja rzeczywista: %.0f [m], Throttle: %.0f \n Brake %d, Engine On: %d, Fuel: %.0f [L], Fuel Average: %0.0f [L/100KM], Gear: %d, RPM: %d \nMode: %s", v, pos, t, brakePressed, engine.getEngineState(), tank.f, fuelAverage, gear, engine.getRPM(), useAutomaticTransmission ? "AUTO" : "MANUAL");
    refresh();
    usleep(DT * 1e6);
}

void Dashboard::pressedPedal(bool &useAutomaticTransmission, bool &brakePressed, bool &gasPressed, double &t, FuelTank &tank, Engine &engine, Transmission &trans) {
    int key = getch();

    gasPressed = false;
    brakePressed = false;
  
    bool gearUp = true;
    bool isEngineOn = engine.getEngineState();            
    
    switch (key) {
        case 'w':
            gasPressed = true;
            break;
        case ' ':
            brakePressed = true;
            break;
        case 'e':
            if (isEngineOn) engine.turnOffEngine();
            else engine.turnOnEngine();
            break;
        case 'r':
            tank.refillFuel();
            break;
        case 'a':
            trans.switchGear(gearUp);
            break;
        case 'z':
            trans.switchGear(!gearUp);
            break;
        case 'm':
            if (useAutomaticTransmission)
                useAutomaticTransmission = false;
            else useAutomaticTransmission = true;
        default:
            gasPressed = false;
            brakePressed = false;
            break;
    }

    if (gasPressed && engine.getEngineState() && tank.f > 0)
        t = std::min(1.0, t + 0.05);
    else
        t = std::max(0.0, t - 0.02);
}
