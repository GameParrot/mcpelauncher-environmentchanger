#pragma once
#include "environment.h"

class ImguiConfig {
  private:
    Environment* env;

  public:
    ImguiConfig(Environment* env);
    void addEntries();
};