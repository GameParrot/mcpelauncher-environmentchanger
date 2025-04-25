#pragma once

#define MOON_SYNODIC_PERIOD 29.530588853 // Period of moon cycle in days.
#define MOON_SYNODIC_OFFSET 2451550.26   // Reference cycle offset in days.

enum class MoonPhase : int {
    FullMoon       = 0,
    WaningGibbous  = 1,
    ThirdQuarter   = 2,
    WaningCrescent = 3,
    NewMoon        = 4,
    WaxingCrescent = 5,
    FirstQuarter   = 6,
    WaxingGibbous  = 7,
    Count          = 8,
};

MoonPhase getCurrentMoonPhase();