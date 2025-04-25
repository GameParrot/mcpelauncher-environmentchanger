#include "imgui_config.h"
#include "menu.hpp"

ImguiConfig::ImguiConfig(Environment* env) {
    this->env = env;
}

void ImguiConfig::addEntries() {
    MenuEntryABI timeEntries[3];
    timeEntries[0] = {
        .name     = "Default",
        .user     = (void*)this,
        .selected = [](void* user) -> bool {
            return ((ImguiConfig*)user)->env->getConfig()->timeOptions.timeMode == TimeMode::Default;
        },
        .click = [](void* user) {
            ((ImguiConfig*)user)->env->getConfig()->timeOptions.timeMode = TimeMode::Default;
            ((ImguiConfig*)user)->env->saveConfig(); },
    };
    timeEntries[1] = {
        .name     = "IRL",
        .user     = (void*)this,
        .selected = [](void* user) -> bool {
            return ((ImguiConfig*)user)->env->getConfig()->timeOptions.timeMode == TimeMode::IRL;
        },
        .click = [](void* user) {
            ((ImguiConfig*)user)->env->getConfig()->timeOptions.timeMode = TimeMode::IRL;
            ((ImguiConfig*)user)->env->saveConfig(); },
    };
    timeEntries[2] = {
        .name     = "Custom...",
        .user     = (void*)this,
        .selected = [](void* user) -> bool {
            return ((ImguiConfig*)user)->env->getConfig()->timeOptions.timeMode == TimeMode::Custom;
        },
        .click = [](void* user) {
            ((ImguiConfig*)user)->env->getConfig()->timeOptions.timeMode = TimeMode::Custom;
            ControlABI controls[1];
            controls[0].type           = 1;
            controls[0].data.sliderint = {
                .label    = "Time",
                .min      = 0,
                .def      = ((ImguiConfig*)user)->env->getConfig()->timeOptions.customTime,
                .max      = 24000,
                .user     = user,
                .onChange = [](void* user, int value) { ((ImguiConfig*)user)->env->getConfig()->timeOptions.customTime = value; },
            };

            showWindow("Time", false, user, [](void* user) { ((ImguiConfig*)user)->env->saveConfig(); }, std::size(controls), controls); },
    };

    MenuEntryABI moonPhaseEntries[8];
    moonPhaseEntries[0] = {
        .name     = "New",
        .user     = (void*)this,
        .selected = [](void* user) -> bool {
            MoonOptions opts = ((ImguiConfig*)user)->env->getConfig()->moonOptions;
            return opts.moonPhase == MoonPhase::NewMoon && opts.moonMode == MoonMode::Custom;
        },
        .click = [](void* user) {
            Config* conf = ((ImguiConfig*)user)->env->getConfig();
            conf->moonOptions.moonPhase = MoonPhase::NewMoon;
            conf->moonOptions.moonMode = MoonMode::Custom;
            ((ImguiConfig*)user)->env->saveConfig(); },
    };
    moonPhaseEntries[1] = {
        .name     = "Waxing Crescent",
        .user     = (void*)this,
        .selected = [](void* user) -> bool {
            MoonOptions opts = ((ImguiConfig*)user)->env->getConfig()->moonOptions;
            return opts.moonPhase == MoonPhase::WaxingCrescent && opts.moonMode == MoonMode::Custom;
        },
        .click = [](void* user) {
            Config* conf = ((ImguiConfig*)user)->env->getConfig();
            conf->moonOptions.moonPhase = MoonPhase::WaxingCrescent;
            conf->moonOptions.moonMode = MoonMode::Custom;
            ((ImguiConfig*)user)->env->saveConfig(); },
    };
    moonPhaseEntries[2] = {
        .name     = "First Quarter",
        .user     = (void*)this,
        .selected = [](void* user) -> bool {
            MoonOptions opts = ((ImguiConfig*)user)->env->getConfig()->moonOptions;
            return opts.moonPhase == MoonPhase::FirstQuarter && opts.moonMode == MoonMode::Custom;
        },
        .click = [](void* user) {
            Config* conf = ((ImguiConfig*)user)->env->getConfig();
            conf->moonOptions.moonPhase = MoonPhase::FirstQuarter;
            conf->moonOptions.moonMode = MoonMode::Custom;
            ((ImguiConfig*)user)->env->saveConfig(); },
    };
    moonPhaseEntries[3] = {
        .name     = "Waxing Gibbous",
        .user     = (void*)this,
        .selected = [](void* user) -> bool {
            MoonOptions opts = ((ImguiConfig*)user)->env->getConfig()->moonOptions;
            return opts.moonPhase == MoonPhase::WaxingGibbous && opts.moonMode == MoonMode::Custom;
        },
        .click = [](void* user) {
            Config* conf = ((ImguiConfig*)user)->env->getConfig();
            conf->moonOptions.moonPhase = MoonPhase::WaxingGibbous;
            conf->moonOptions.moonMode = MoonMode::Custom;
            ((ImguiConfig*)user)->env->saveConfig(); },
    };
    moonPhaseEntries[4] = {
        .name     = "Full",
        .user     = (void*)this,
        .selected = [](void* user) -> bool {
            MoonOptions opts = ((ImguiConfig*)user)->env->getConfig()->moonOptions;
            return opts.moonPhase == MoonPhase::FullMoon && opts.moonMode == MoonMode::Custom;
        },
        .click = [](void* user) {
            Config* conf = ((ImguiConfig*)user)->env->getConfig();
            conf->moonOptions.moonPhase = MoonPhase::FullMoon;
            conf->moonOptions.moonMode = MoonMode::Custom;
            ((ImguiConfig*)user)->env->saveConfig(); },
    };
    moonPhaseEntries[5] = {
        .name     = "Waning Gibbous",
        .user     = (void*)this,
        .selected = [](void* user) -> bool {
            MoonOptions opts = ((ImguiConfig*)user)->env->getConfig()->moonOptions;
            return opts.moonPhase == MoonPhase::WaningGibbous && opts.moonMode == MoonMode::Custom;
        },
        .click = [](void* user) {
            Config* conf = ((ImguiConfig*)user)->env->getConfig();
            conf->moonOptions.moonPhase = MoonPhase::WaningGibbous;
            conf->moonOptions.moonMode = MoonMode::Custom;
            ((ImguiConfig*)user)->env->saveConfig(); },
    };
    moonPhaseEntries[6] = {
        .name     = "Third Quarter",
        .user     = (void*)this,
        .selected = [](void* user) -> bool {
            MoonOptions opts = ((ImguiConfig*)user)->env->getConfig()->moonOptions;
            return opts.moonPhase == MoonPhase::ThirdQuarter && opts.moonMode == MoonMode::Custom;
        },
        .click = [](void* user) {
            Config* conf = ((ImguiConfig*)user)->env->getConfig();
            conf->moonOptions.moonPhase = MoonPhase::ThirdQuarter;
            conf->moonOptions.moonMode = MoonMode::Custom;
            ((ImguiConfig*)user)->env->saveConfig(); },
    };
    moonPhaseEntries[7] = {
        .name     = "Waning Crescent",
        .user     = (void*)this,
        .selected = [](void* user) -> bool {
            MoonOptions opts = ((ImguiConfig*)user)->env->getConfig()->moonOptions;
            return opts.moonPhase == MoonPhase::WaningCrescent && opts.moonMode == MoonMode::Custom;
        },
        .click = [](void* user) {
            Config* conf = ((ImguiConfig*)user)->env->getConfig();
            conf->moonOptions.moonPhase = MoonPhase::WaningCrescent;
            conf->moonOptions.moonMode = MoonMode::Custom;
            ((ImguiConfig*)user)->env->saveConfig(); },
    };

    MenuEntryABI moonEntries[3];
    moonEntries[0] = {
        .name     = "Default",
        .user     = (void*)this,
        .selected = [](void* user) -> bool {
            return ((ImguiConfig*)user)->env->getConfig()->moonOptions.moonMode == MoonMode::Default;
        },
        .click = [](void* user) {
            ((ImguiConfig*)user)->env->getConfig()->moonOptions.moonMode = MoonMode::Default;
            ((ImguiConfig*)user)->env->saveConfig(); },
    };
    moonEntries[1] = {
        .name     = "IRL",
        .user     = (void*)this,
        .selected = [](void* user) -> bool {
            return ((ImguiConfig*)user)->env->getConfig()->moonOptions.moonMode == MoonMode::IRL;
        },
        .click = [](void* user) {
            ((ImguiConfig*)user)->env->getConfig()->moonOptions.moonMode = MoonMode::IRL;
            ((ImguiConfig*)user)->env->saveConfig(); },
    };

    moonEntries[2] = {
        .name       = "Custom",
        .user       = (void*)this,
        .length     = std::size(moonPhaseEntries),
        .subentries = moonPhaseEntries,
    };

    MenuEntryABI fogEntries[5];
    fogEntries[0] = {
        .name     = "Default",
        .user     = (void*)this,
        .selected = [](void* user) -> bool {
            return ((ImguiConfig*)user)->env->getConfig()->fogOptions.fogMode == FogMode::Default;
        },
        .click = [](void* user) {
            ((ImguiConfig*)user)->env->getConfig()->fogOptions.fogMode = FogMode::Default;
            ((ImguiConfig*)user)->env->saveConfig(); },
    };
    fogEntries[1] = {
        .name     = "Chroma",
        .user     = (void*)this,
        .selected = [](void* user) -> bool {
            return ((ImguiConfig*)user)->env->getConfig()->fogOptions.fogMode == FogMode::Chroma;
        },
        .click = [](void* user) {
            ((ImguiConfig*)user)->env->getConfig()->fogOptions.fogMode = FogMode::Chroma;
            ((ImguiConfig*)user)->env->saveConfig(); },
    };
    fogEntries[2] = {
        .name     = "Custom",
        .user     = (void*)this,
        .selected = [](void* user) -> bool {
            return ((ImguiConfig*)user)->env->getConfig()->fogOptions.fogMode == FogMode::Custom;
        },
        .click = [](void* user) {
            ((ImguiConfig*)user)->env->getConfig()->fogOptions.fogMode = FogMode::Custom;
            ((ImguiConfig*)user)->env->saveConfig(); },
    };
    fogEntries[3] = {
        .name     = "Apply brightness to custom color",
        .user     = (void*)this,
        .selected = [](void* user) -> bool {
            return ((ImguiConfig*)user)->env->getConfig()->fogOptions.applyBrightnessToFogColor;
        },
        .click = [](void* user) {
            ((ImguiConfig*)user)->env->getConfig()->fogOptions.applyBrightnessToFogColor ^= true;
            ((ImguiConfig*)user)->env->saveConfig(); },
    };
    fogEntries[4] = {
        .name  = "Change color",
        .user  = (void*)this,
        .click = [](void* user) {
            ControlABI controls[3];
            controls[0].type             = 2;
            controls[0].data.sliderfloat = {
                .label    = "Red",
                .min      = 0,
                .def      = ((ImguiConfig*)user)->env->getConfig()->fogOptions.fogColor.r * 255,
                .max      = 255,
                .user     = user,
                .onChange = [](void* user, float value) { ((ImguiConfig*)user)->env->getConfig()->fogOptions.fogColor.r = value / 255; },
            };

            controls[1].type             = 2;
            controls[1].data.sliderfloat = {
                .label    = "Green",
                .min      = 0,
                .def      = ((ImguiConfig*)user)->env->getConfig()->fogOptions.fogColor.g * 255,
                .max      = 255,
                .user     = user,
                .onChange = [](void* user, float value) { ((ImguiConfig*)user)->env->getConfig()->fogOptions.fogColor.g = value / 255; },
            };

            controls[2].type             = 2;
            controls[2].data.sliderfloat = {
                .label    = "Blue",
                .min      = 0,
                .def      = ((ImguiConfig*)user)->env->getConfig()->fogOptions.fogColor.b * 255,
                .max      = 255,
                .user     = user,
                .onChange = [](void* user, float value) { ((ImguiConfig*)user)->env->getConfig()->fogOptions.fogColor.b = value / 255; },
            };

            showWindow("Fog color", false, user, [](void* user) { ((ImguiConfig*)user)->env->saveConfig(); }, std::size(controls), controls);
        },
    };

    MenuEntryABI weatherEntries[4];
    weatherEntries[0] = {
        .name     = "Default",
        .user     = (void*)this,
        .selected = [](void* user) -> bool {
            return ((ImguiConfig*)user)->env->getConfig()->weatherOptions.weatherMode == WeatherMode::Default;
        },
        .click = [](void* user) {
            ((ImguiConfig*)user)->env->getConfig()->weatherOptions.weatherMode = WeatherMode::Default;
            ((ImguiConfig*)user)->env->saveConfig(); },
    };
    weatherEntries[1] = {
        .name     = "Clear",
        .user     = (void*)this,
        .selected = [](void* user) -> bool {
            return ((ImguiConfig*)user)->env->getConfig()->weatherOptions.weatherMode == WeatherMode::Clear;
        },
        .click = [](void* user) {
            ((ImguiConfig*)user)->env->getConfig()->weatherOptions.weatherMode = WeatherMode::Clear;
            ((ImguiConfig*)user)->env->saveConfig(); },
    };
    weatherEntries[2] = {
        .name     = "Rain",
        .user     = (void*)this,
        .selected = [](void* user) -> bool {
            return ((ImguiConfig*)user)->env->getConfig()->weatherOptions.weatherMode == WeatherMode::Rain;
        },
        .click = [](void* user) {
            ((ImguiConfig*)user)->env->getConfig()->weatherOptions.weatherMode = WeatherMode::Rain;
            ((ImguiConfig*)user)->env->saveConfig(); },
    };
    weatherEntries[3] = {
        .name  = "Change rain amount",
        .user  = (void*)this,
        .click = [](void* user) {
            ControlABI controls[1];
            controls[0].type             = 2;
            controls[0].data.sliderfloat = {
                .label    = "Rain amount",
                .min      = 0,
                .def      = ((ImguiConfig*)user)->env->getConfig()->weatherOptions.amount,
                .max      = 1,
                .user     = user,
                .onChange = [](void* user, float value) { ((ImguiConfig*)user)->env->getConfig()->weatherOptions.amount = value; },
            };

            showWindow("Rain amount", false, user, [](void* user) { ((ImguiConfig*)user)->env->saveConfig(); }, std::size(controls), controls);
        },
    };

    MenuEntryABI cloudEntry{
        .name  = "Change cloud height",
        .user  = (void*)this,
        .click = [](void* user) {
            ControlABI controls[1];
            controls[0].type           = 1;
            controls[0].data.sliderint = {
                .label    = "Cloud height",
                .min      = 0,
                .def      = ((ImguiConfig*)user)->env->getConfig()->cloudHeight,
                .max      = 512,
                .user     = user,
                .onChange = [](void* user, int value) { ((ImguiConfig*)user)->env->getConfig()->cloudHeight = (short)value; },
            };

            showWindow("Cloud height", false, user, [](void* user) { ((ImguiConfig*)user)->env->saveConfig(); }, std::size(controls), controls);
        },
    };
    MenuEntryABI menuSubEntries[5];

    menuSubEntries[0] = {
        .name       = "Time",
        .length     = std::size(timeEntries),
        .subentries = timeEntries,
    };

    menuSubEntries[1] = {
        .name       = "Moon phase",
        .length     = std::size(moonEntries),
        .subentries = moonEntries,
    };

    menuSubEntries[2] = {
        .name       = "Fog",
        .length     = std::size(fogEntries),
        .subentries = fogEntries,
    };

    menuSubEntries[3] = {
        .name       = "Weather",
        .length     = std::size(weatherEntries),
        .subentries = weatherEntries,
    };

    menuSubEntries[4] = cloudEntry;

    MenuEntryABI menuEntry{
        .name       = "Environment changer",
        .length     = std::size(menuSubEntries),
        .subentries = menuSubEntries,
    };
    addMenu(1, &menuEntry);
}