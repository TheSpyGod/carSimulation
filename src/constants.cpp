#include "constants.h"
#include "environment.h"
#include <cmath>

// Initialize mutable globals here
double grip = grip_dry;
double angle = 0;
double grade_percent = std::tan(angle);
double F_grade = MASS_KG * gravity * angle;
double F_max = grip * MASS_KG * gravity;

