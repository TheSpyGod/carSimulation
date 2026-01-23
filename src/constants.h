#pragma once
#include <cmath>

extern double grip;
extern double angle;
extern double grade_percent;
extern double F_grade;
extern double F_max;

constexpr int MAX_SPEED_KMH = 180;
constexpr int MASS_KG = 2400;
constexpr double gravity = 9.81;

constexpr double grip_dry = 0.9;
constexpr double grip_wet = 0.4;
constexpr double grip_ice = 0.15;

constexpr double DT = 0.02;
constexpr double PI = 3.141592653589793;

