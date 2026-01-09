#pragma once
class FuelTank {
public:
    double f, fuelUsed = 0;
    void refillFuel();
 //   void setFuel(double &f);
 //   double getFuel() const;
    void useFuel(double usage);
 //   double getFuelUsed() const;
};
