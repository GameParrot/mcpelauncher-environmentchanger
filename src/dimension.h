#pragma once

#include "util.h"
#include "weather.h"

class Dimension {
  public:
    CLASS_FIELD(class Weather*, weather, 0x1A0);
};