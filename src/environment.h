#pragma once
#include "config.h"
#include "dimension.h"

class Environment {
  private:
    void   loadConfig();
    long   timeOffset;
    Config config;

  public:
    Environment();

    int   getTime(int orig);
    Color getFogColor(Color* baseColor, float brightness, Color orig);
    void  onDimensionTick(Dimension* d);
    short getCloudHeight();

    void saveConfig();

    Config* getConfig() {
        return &config;
    }
};