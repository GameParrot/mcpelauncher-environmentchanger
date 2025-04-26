#include "environment.h"
#include "color_conversion.h"
#include "moon_phase.h"
#include <time.h>

Environment::Environment() {
    time_t     current_time;
    struct tm* local_time;
    current_time = time(NULL);
    local_time   = localtime(&current_time);

    if (local_time != NULL) {
        timeOffset = local_time->tm_gmtoff;
    }

    loadConfig();
    saveConfig();
}

void Environment::loadConfig() {
    std::string buffer;
    if (auto ec = glz::read_file_json(config, configPath, buffer))
        std::println(stderr, "Failed to load environment config: {}", glz::format_error(ec, buffer));
}

void Environment::saveConfig() {
    std::string buffer;
    if (auto ec = glz::write_file_json<glz::opts{.prettify = true}>(config, configPath, buffer))
        std::println(stderr, "Failed to save environment config: {}", glz::format_error(ec, buffer));
}

int Environment::getTime() {
    int       baseWorldTime = realTime / (24000 * (int)MoonPhase::Count);
    int       timeOfDay     = realTime % 24000;
    MoonPhase moonPhase     = (MoonPhase)((realTime / 24000) % (int)MoonPhase::Count);
    switch (config.moonOptions.moonMode) {
    case MoonMode::Default:
        break;
    case MoonMode::IRL:
        moonPhase = getCurrentMoonPhase();
        break;
    case MoonMode::Custom:
        moonPhase = config.moonOptions.moonPhase;
        break;
    }
    switch (config.timeOptions.timeMode) {
    case TimeMode::Default:
        break;
    case TimeMode::IRL:
        timeOfDay = (int)((((time(NULL) + timeOffset) % 86400) / 3.6) - 6000) % 24000;
        break;
    case TimeMode::Custom:
        timeOfDay = config.timeOptions.customTime;
        break;
    }
    return baseWorldTime + ((int)moonPhase * 24000) + timeOfDay;
}

Color Environment::getFogColor(Color*, float brightness, Color orig) {
    if (config.fogOptions.fogMode == FogMode::Default) {
        return orig;
    }
    Color c = orig;
    switch (config.fogOptions.fogMode) {
    case FogMode::Default:
        break;
    case FogMode::Chroma:
        double h, s, v;
        if (config.fogOptions.fogColor.r == 0 && config.fogOptions.fogColor.g == 0 && config.fogOptions.fogColor.b == 0) {
            h = 0.0;
            s = 1.0;
            v = 1.0;
        } else {
            RGBtoHSV(config.fogOptions.fogColor.r, config.fogOptions.fogColor.g, config.fogOptions.fogColor.b, h, s, v);
        }
        h += (double)(getEpochTime()) / 25.0;
        double r, g, b;
        HSVtoRGB(r, g, b, h, s, v);
        c.r = (float)r;
        c.g = (float)g;
        c.b = (float)b;
        break;
    case FogMode::Custom:
        c = config.fogOptions.fogColor;
        break;
    }
    if (config.fogOptions.applyBrightnessToFogColor) {
        c.r *= brightness;
        c.g *= brightness;
        c.b *= brightness;
    }
    c.a = 1;
    return c;
}

void Environment::onDimensionTick(Dimension* d) {
    if (config.weatherOptions.weatherMode != WeatherMode::Default && d->weather) {
        if (config.weatherOptions.weatherMode == WeatherMode::Clear) {
            d->weather->rainLevel = (float)0;
        } else {
            d->weather->rainLevel = config.weatherOptions.amount;
        }
    }
}

short Environment::getCloudHeight() {
    return config.cloudHeight;
}