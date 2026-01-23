#include "surfacemodel.h"

void SurfaceModel::setSurface(int key) {
    switch(key) {
        case 1:
            name = "asphalt";
            mu = grip_dry;
            break;
        case 2:
            name = "gravel";
            mu = grip_wet;
            break;
        case 3:
            name = "ice";
            mu = grip_ice;
            break;
        default:
            name = "asphalt";
            mu = grip_dry;
            break;
    }
}

