#pragma once

namespace commonConsts {
static const int MAIN_ENGINE_THRUST{34'020'000}; // NETWONS
static const int SIDE_THRUSTERS_THRUST{50'000 *
                                       1'000}; // NETWONS - elaborated guess
static const int MOON_DISTANCE{384'400'000};   // METERS
static const int ROCKET_MASS{3'038'500};       // KG
static const int ROCKET_HEIGHT{110};           // METERS
static const float ROCKET_MOMENT_OF_INERTIA{0.5 * ROCKET_MASS *
                                            (0.5 * ROCKET_HEIGHT) *
                                            (0.5 * ROCKET_HEIGHT)}; // KG * m^2

// INDEPENDANT VARIABLES
static const float OXYGEN_USAGE_PER_SECOND{1};
static const float MAIN_ENGINE_FUEL_USAGE_PER_SECOND{1};
static const float SIDE_THRUSTER_FUEL_USAGE_PER_SECOND{0.5};

// ACCORDING TO EQUATION FOR CILINDER INERTIA MOMENT

} // namespace commonConsts
