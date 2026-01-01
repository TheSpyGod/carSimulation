#include <iostream>
#include "curses.h"
#include <unistd.h>
#include <cmath>
#include <algorithm>

const int MAX_SPEED_KMH = 180;
const int MASS_KG = 2400;
const double DT = 0.02;
constexpr double PI = 3.141592653589793;

class Car {

    class FuelTank {
        double fuel = 0;     
        double fuelUsed = 0;   

        public:
        void refillFuel() {
            if (fuel < 40)
                fuel+=5;
            else
                fuel=45;
        }
        
        void setFuel(double &f) {
            fuel = f;
        }
        
        double getFuel() {
            return fuel;
        }

        void useFuel(double usage) {
            fuel -= usage;
            fuelUsed += usage;
        }
        
        double getFuelUsed() {
            return fuelUsed;
        }

    };
    
    class Engine {
        private:
        bool engineState = false;
        int rpm = 800;
        const int idleRPM = 800;
        const int redline = 6500;

        public:
        int getRPM() const {
            return rpm;
        }

        void updateRPM(double speed, double throttle, double gearRatio, double finaldrive, double tireDiameter) {
            if (!engineState) {
                rpm = 0;
                return;
            }

            if (gearRatio == 0) {
                rpm = idleRPM + throttle * 2000;
                return;
            }

            double speed_mps = speed / 3.6;
            double tireCircumferance = PI * tireDiameter;
            double wheelRPM = (speed_mps / tireCircumferance) * 60.0;

            rpm = static_cast<int>(wheelRPM * gearRatio * finaldrive);

            rpm = std::clamp(rpm, idleRPM, redline);
        }

        bool getEngineState() {
            return engineState;
        }

        void turnOnEngine() {
            engineState = true;
        }

        void turnOffEngine() {
           engineState = false;
        }

        double getEngineForce(double throttle, double gearRatio, double finalDrive, int rpm) {
            if (!engineState || throttle <= 0 || gearRatio == 0) return 0.0;

            double torque;
            if (rpm < 1500) torque = 500;
            else if (rpm < 3500) torque = 700;
            else if (rpm < 5500) torque = 650;
            else torque = 400;

            return throttle * torque * gearRatio * finalDrive;
        }
    };

    class Command {
        public:
        int keyPressed() {
            int ch = getch();
            return ch;
        }

    };

    class Brake {
        public:

        double getBrakeForce(bool brakeIsPressed) {
            if (!brakeIsPressed) return 0.0;
            
            const double k_brake = 8000;
            return -k_brake;
        }
    };

    class ShiftPolicy {
    public:
        virtual void shift(int rpm, bool engineState, int& gear) = 0;
        virtual ~ShiftPolicy() = default;
    };

    class SimpleShiftPolicy : public ShiftPolicy {
    public:
        void shift(int rpm, bool engineState, int& gear) override {
            if (!engineState) return;
            if (rpm >= 3000 && gear < 5) gear++;
            else if (rpm <= 2000 && gear > 1) gear--;
        }
    };

    class Transmission {
        private:
        int gear = 1;
        const double final_drive = 3.9;
        ShiftPolicy* policy = nullptr;

        public:
        void setPolicy(ShiftPolicy* p) {
            policy = p;
        }

        int getGear() {
            return gear;
        }

        double getFinalDrive() const {
            return final_drive;
        }

        double getGearRatio() const {
            switch (gear) {
                case 0:
                    //Neutral
                    return 0.0;
                    break;
                case 1:
                    return 3.6;
                    break;
                case 2:
                    return 2.1;
                    break;
                case 3:
                    return 1.4;
                    break;
                case 4:
                    return 1.0;
                    break;
                case 5:
                    return 0.8;
                    break;
            }
            return 0;
        }

        void switchGear(bool incrementGear){
            if (incrementGear && gear < 5) gear++;
            else if (!incrementGear && gear > 1) gear--;
        }

        void useAutomatic(int rpm, bool engineState) {
            if (policy)
                policy->shift(rpm, engineState, gear);
        }
    };
    
    class Physics {
        double getResistanceForce(double speed) {
            const double c_roll = 300;
            const double c_drag = 0.25;

            return -c_roll - c_drag * speed * speed;
        }
        public:
        void physicsStep(double &v, double &a, double t, double &pos, FuelTank &tank, Engine &engine, Transmission &trans, Brake &brake, bool brakePressed) {
            double driveForce = engine.getEngineForce(t/*, v, */,trans.getGearRatio(), trans.getFinalDrive(), engine.getRPM());
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
    };

    class Dashboard {
        Command cmd;
        
        double calculateFuelAverage(double fuel, double distance) {
            if (distance < 1e-6) return 0.0;
            return (fuel / (distance / 1000) * 100);
        } 
            
        public: 
        void displayInfo(double v, double pos, double t, bool brakePressed, FuelTank &tank, Engine &engine, int gear, bool useAutomaticTransmission) {
    
            double fuelAverage = calculateFuelAverage(tank.getFuelUsed(), pos);
    
            mvprintw(0, 0, "Predkosc: %.0f [km/h], Pozycja rzeczywista: %.0f [m], Throttle: %.0f \n Brake %d, Engine On: %d, Fuel: %.0f [L], Fuel Average: %0.0f [L/100KM], Gear: %d, RPM: %d \nMode: %s", v, pos, t, brakePressed, engine.getEngineState(), tank.getFuel(), fuelAverage, gear, engine.getRPM(), useAutomaticTransmission ? "AUTO" : "MANUAL");
            refresh();
            usleep(DT * 1e6);
        }

        void pressedPedal(bool &useAutomaticTransmission, bool &brakePressed, bool &gasPressed, double &t, FuelTank &tank, Engine &engine, Transmission &trans) {        
            int key = cmd.keyPressed();
            
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

            if (gasPressed && engine.getEngineState() && tank.getFuel() > 0)
                t = std::min(1.0, t + 0.05);
            else
                t = std::max(0.0, t - 0.02);
        }
    };

        Brake brake;
        Dashboard dash;
        FuelTank tank;        
        Engine engine;
        Transmission trans;
        Physics p;
        SimpleShiftPolicy autoPolicy;

        double v = 0;
        double a = 0;
        double t = 0;
        double pos = 0;
        double fuel = 45;
        double fuelUsed = 0;

        bool gasPressed = false;
        bool brakePressed = false;
        bool useAutomaticTransmission = false;

        public:
        Car() {
            tank.setFuel(fuel);
            trans.setPolicy(&autoPolicy);
        }

        void tick() {
        dash.pressedPedal(useAutomaticTransmission, brakePressed, gasPressed, t, tank, engine, trans);
        
        p.physicsStep(v, a, t, pos, tank, engine, trans, brake, brakePressed);
        
        engine.updateRPM(v, t, trans.getGearRatio(), trans.getFinalDrive(), 0.64);

        if (useAutomaticTransmission) trans.useAutomatic(engine.getRPM(), engine.getEngineState());

        dash.displayInfo(v, pos, t, brakePressed, tank, engine, trans.getGear(), useAutomaticTransmission);
        }
};
