#include "fueltank.h"
void FuelTank::refillFuel() {
    if (f < 40)
        f+=5;
    else
        f=45;
}

void FuelTank::useFuel(double usage) {
    f -= usage;
    fuelUsed += usage;
}
