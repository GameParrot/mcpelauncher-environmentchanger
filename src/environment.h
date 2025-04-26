#pragma once
#include "config.h"
#include "dimension.h"

class Environment {
  private:
    void   loadConfig();
    long   timeOffset;
    Config config;
    int    realTime;

  public:
    Environment();

    int   getTime();
    Color getFogColor(Color* baseColor, float brightness, Color orig);
    void  onDimensionTick(Dimension* d);
    short getCloudHeight();

    void saveConfig();

    Config* getConfig() {
        return &config;
    }

    void updateRealTime(int newTime) {
        realTime = newTime;
    }

    bool changingTime() {
        return config.timeOptions.timeMode != TimeMode::Default || config.moonOptions.moonMode != MoonMode::Default;
    }
};