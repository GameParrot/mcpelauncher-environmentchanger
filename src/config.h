#pragma once
#include "color.h"
#include "moon_phase.h"
#include <glaze/json.hpp>

enum class TimeMode : int {
    Default = 0,
    IRL     = 1,
    Custom  = 2,
};

enum class MoonMode : int {
    Default = 0,
    IRL     = 1,
    Custom  = 2,
};

enum class FogMode : int {
    Default = 0,
    Chroma  = 1,
    Custom  = 2,
};

enum class WeatherMode : int {
    Default = 0,
    Clear   = 1,
    Rain    = 2,
};

struct TimeOptions {
    int      customTime = 0;
    TimeMode timeMode   = TimeMode::Default;
};

struct MoonOptions {
    MoonMode  moonMode  = MoonMode::Default;
    MoonPhase moonPhase = MoonPhase::NewMoon;
};

struct FogOptions {
    Color   fogColor                  = Color{};
    FogMode fogMode                   = FogMode::Default;
    bool    applyBrightnessToFogColor = false;
};

struct WeatherOptions {
    WeatherMode weatherMode = WeatherMode::Default;
    float       amount      = 1.0;
};

struct Config {
    FogOptions     fogOptions     = FogOptions{};
    TimeOptions    timeOptions    = TimeOptions{};
    MoonOptions    moonOptions    = MoonOptions{};
    WeatherOptions weatherOptions = WeatherOptions{};
    short          cloudHeight    = 192;
};

static glz::sv configPath = "/data/data/com.mojang.minecraftpe/environment_config.json";
