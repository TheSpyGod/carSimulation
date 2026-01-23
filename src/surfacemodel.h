#pragma once
#include <string>
#include "constants.h"

class SurfaceModel {
public:
    double mu = grip_dry;   // default grip
    std::string name = "asphalt";

    void setSurface(int key); // 1=asphalt,2=gravel,3=ice
};
