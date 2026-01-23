/*
#include "dashboard.h"
#include "constants.h"
double Dashboard::calculateFuelAverage(double fuel, double distance) {
    if (distance < 1e-6) return 0.0;
    return (fuel / (distance / 1000) * 100);
}

void Dashboard::displayInfo(double v, double pos, double t, bool brakePressed, FuelTank &tank, Engine &engine, int gear, bool useAutomaticTransmission, bool absOn, bool tcsOn, const Environment &env) {
    double fuelAverage = calculateFuelAverage(tank.fuelUsed, pos);

    mvprintw(0, 0, "Predkosc: %.0f [km/h], Pozycja rzeczywista: %.0f [m], Throttle: %.0f \n Brake %d, Engine On: %d, Fuel: %.0f [L], Fuel Average: %0.0f [L/100KM], Gear: %d, RPM: %d \nMode: %s\nABS: %s TCS: %s", v, pos, t, brakePressed, engine.getEngineState(), tank.f, fuelAverage, gear, engine.getRPM(), useAutomaticTransmission ? "AUTO" : "MANUAL", absOn ? "ON" : "OFF", tcsOn ? "ON" : "OFF");
    refresh();
    usleep(DT * 1e6);
}

void Dashboard::pressedPedal(bool &useAutomaticTransmission, bool &brakePressed, bool &gasPressed, double &t, FuelTank &tank, Engine &engine, Transmission &trans, Environment &env) {
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
        case '1': env.surface.setSurface(1); break;
        case '2': env.surface.setSurface(2); break;
        case '3': env.surface.setSurface(3); break;
        case 'i': grade_percent += 0.01; break;
        case 'k': grade_percent -= 0.01; break;
        default:
            gasPressed = false;
            brakePressed = false;
            break;
    }

    if (gasPressed && engine.getEngineState() && tank.f > 0)
        t = std::min(1.0, t + 0.05);
    else
        t = std::max(0.0, t - 0.02);
}*/ 


#include "dashboard.h"
#include "constants.h"
#include <unistd.h>
#include <algorithm>
#include <string>
#include <cmath>

double Dashboard::calculateFuelAverage(double fuel, double distance) {
    if (distance < 1e-6) return 0.0;
    return (fuel / (distance / 1000) * 100);
}

static void drawBar(int row, const std::string &label, double fraction, int width, const std::string &unit, double value) {
    fraction = std::clamp(fraction, 0.0, 1.0);
    int fill = static_cast<int>(fraction * width);
    std::string bar = std::string(fill, '=') + std::string(width - fill, ' ');
    mvprintw(row, 0, "%s [%s] %.1f %s", label.c_str(), bar.c_str(), value, unit.c_str());
}

void Dashboard::displayInfo(double v, double pos, double t, bool brakePressed,
                            FuelTank &tank, Engine &engine, int gear,
                            bool useAutomaticTransmission, bool absOn, bool tcsOn,
                            const Environment &env, bool paused) {

    clear();

    double fuelAvg = calculateFuelAverage(tank.fuelUsed, pos);

    drawBar(0, "Speed ", v / MAX_SPEED_KMH, 50, "km/h", v);
    drawBar(1, "RPM   ", engine.getRPM() / 7000.0, 50, "RPM", engine.getRPM());
    drawBar(2, "Fuel  ", tank.f / 45.0, 20, "L", tank.f);
    drawBar(3, "Throttle", t, 20, "", t * 100);

    mvprintw(5, 0, "Gear: %d  Mode: %s  ABS: %s  TCS: %s",
             gear,
             useAutomaticTransmission ? "AUTO" : "MANUAL",
             absOn ? "ON" : "OFF",
             tcsOn ? "ON" : "OFF");

    mvprintw(6, 0, "Distance: %.1f m  Fuel Avg: %.1f L/100KM  Brake: %d", pos, fuelAvg, brakePressed);

    refresh();
    usleep(DT * 1e6);
}

void Dashboard::pressedPedal(bool &useAutomaticTransmission, bool &brakePressed,
                             bool &gasPressed, double &t, FuelTank &tank,
                             Engine &engine, Transmission &trans, Environment &env,
                             bool &paused, bool &reset, bool &captureFrame) {

    int key = getch();

    // Debounce: only act on key once per frame
    if (pressedKeys.find(key) != pressedKeys.end()) return;
    if (key != ERR) pressedKeys.insert(key);

    gasPressed = false;
    brakePressed = false;

    bool gearUp = true;
    bool isEngineOn = engine.getEngineState();

    switch (key) {
        case 'w': gasPressed = true; break;
        case ' ': brakePressed = true; break;
        case 'e': isEngineOn ? engine.turnOffEngine() : engine.turnOnEngine(); break;
        case 'r': tank.refillFuel(); break;
        case 'a': trans.switchGear(gearUp); break;
        case 'z': trans.switchGear(!gearUp); break;
        case 'm': useAutomaticTransmission = !useAutomaticTransmission; break;
        case '1': env.surface.setSurface(1); break;
        case '2': env.surface.setSurface(2); break;
        case '3': env.surface.setSurface(3); break;
        case 'i': grade_percent += 0.01; break;
        case 'k': grade_percent -= 0.01; break;

        // **New features**
        case 'p': paused = !paused; break;        // Pause
        case KEY_BACKSPACE: reset = true; break;  // Reset
        case KEY_F(12): captureFrame = true; break; // Screenshot CSV
        default: break;
    }

    // Throttle ramp
    if (gasPressed && engine.getEngineState() && tank.f > 0)
        t = std::min(1.0, t + 0.05);
    else
        t = std::max(0.0, t - 0.02);

    // Clear debounce set if no key pressed
    if (key == ERR) pressedKeys.clear();
}

void Dashboard::end() {
    endwin();
}
