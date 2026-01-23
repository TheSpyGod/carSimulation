#pragma once
#include "surfacemodel.h"
#include "constants.h"

struct Environment {
    SurfaceModel surface;

    double getSlopeForce() const {
        return MASS_KG * gravity * grade_percent;
    }
};
