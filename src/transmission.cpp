#include "transmission.h"
    void Transmission::setPolicy(ShiftPolicy* p) {
        policy = p;
    }

    int Transmission::getGear() const {
        return gear;
    }

    double Transmission::getFinalDrive() const {
        return final_drive;
    }

    double Transmission::getGearRatio() const {
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

    void Transmission::switchGear(bool incrementGear){
        if (incrementGear && gear < 5) gear++;
        else if (!incrementGear && gear > 1) gear--;
    }

    void Transmission::useAutomatic(int rpm, bool engineState) {
        if (policy)
            policy->shift(rpm, engineState, gear);
    }
