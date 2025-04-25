#include "moon_phase.h"
#include "time.h"
#include <cmath>

double julianDate(time_t t) {
    return ((double)t / 86400.0 + 2440587.5);
}

MoonPhase getCurrentMoonPhase() {
    double jDate = julianDate(time(NULL));

    // Calculate illumination (synodic) phase.
    // From number of days since new moon on Julian date MOON_SYNODIC_OFFSET
    // (1815UTC January 6, 2000), determine remainder of incomplete cycle.
    int phase = (int)((jDate - MOON_SYNODIC_OFFSET) / MOON_SYNODIC_PERIOD);

    return (MoonPhase)((phase + 4) % (int)MoonPhase::Count);
}