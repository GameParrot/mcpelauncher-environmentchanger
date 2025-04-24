#include "menu.hpp"
#include <cmath>
#include <cstddef>
#include <dlfcn.h>
#include <functional>
#include <glaze/json.hpp>
#include <libhat.hpp>
#include <link.h>
#include <memory>
#include <print>
#include <span>
#include <stdlib.h>
#include <thread>

struct Color {
    float r;
    float g;
    float b;
    float a;
};

struct Config {
    Color fogColor                  = Color{};
    bool  fogColorEnabled           = false;
    bool  applyBrightnessToFogColor = true;

    int  time        = 0;
    bool timeEnabled = false;

    short cloudHeight = 192;
};

static Config config;

static glz::sv configPath = "/data/data/com.mojang.minecraftpe/environment_config.json";

static void loadConfig() {
    std::string buffer;
    if (auto ec = glz::read_file_json(config, configPath, buffer))
        std::println(stderr, "Failed to load environment config: {}", glz::format_error(ec, buffer));
}

static void saveConfig() {
    std::string buffer;
    if (auto ec = glz::write_file_json<glz::opts{.prettify = true}>(config, configPath, buffer))
        std::println(stderr, "Failed to save environment config: {}", glz::format_error(ec, buffer));
}

extern "C" [[gnu::visibility("default")]] void mod_preinit() {
    auto menuLib = dlopen("libmcpelauncher_menu.so", 0);

    addMenu     = reinterpret_cast<decltype(addMenu)>(dlsym(menuLib, "mcpelauncher_addmenu"));
    showWindow  = reinterpret_cast<decltype(showWindow)>(dlsym(menuLib, "mcpelauncher_show_window"));
    closeWindow = reinterpret_cast<decltype(closeWindow)>(dlsym(menuLib, "mcpelauncher_close_window"));

    loadConfig();
    saveConfig();

    MenuEntryABI fogEntry{
        .name  = "Change fog color",
        .click = [](void*) {
            ControlABI controls[3];
            controls[0].type             = 2;
            controls[0].data.sliderfloat = {
                .label    = "Red",
                .min      = 0,
                .def      = config.fogColor.r * 255,
                .max      = 255,
                .user     = nullptr,
                .onChange = [](void*, float value) { config.fogColor.r = value / 255; },
            };

            controls[1].type             = 2;
            controls[1].data.sliderfloat = {
                .label    = "Green",
                .min      = 0,
                .def      = config.fogColor.g * 255,
                .max      = 255,
                .user     = nullptr,
                .onChange = [](void*, float value) { config.fogColor.g = value / 255; },
            };

            controls[2].type             = 2;
            controls[2].data.sliderfloat = {
                .label    = "Blue",
                .min      = 0,
                .def      = config.fogColor.b * 255,
                .max      = 255,
                .user     = nullptr,
                .onChange = [](void*, float value) { config.fogColor.b = value / 255; },
            };

            showWindow("Fog color", false, nullptr, [](void*) { saveConfig(); }, std::size(controls), controls);
        },
    };

    MenuEntryABI timeEntry{
        .name  = "Change time",
        .click = [](void*) {
            ControlABI controls[1];
            controls[0].type           = 1;
            controls[0].data.sliderint = {
                .label    = "Time",
                .min      = 0,
                .def      = config.time,
                .max      = 24000,
                .user     = nullptr,
                .onChange = [](void*, int value) { config.time = value; },
            };

            showWindow("Time", false, nullptr, [](void*) { saveConfig(); }, std::size(controls), controls);
        },
    };

    MenuEntryABI cloudEntry{
        .name  = "Change cloud height",
        .click = [](void*) {
            ControlABI controls[1];
            controls[0].type           = 1;
            controls[0].data.sliderint = {
                .label    = "Cloud height",
                .min      = 0,
                .def      = config.cloudHeight,
                .max      = 512,
                .user     = nullptr,
                .onChange = [](void*, int value) { config.cloudHeight = (short)value; },
            };

            showWindow("Cloud height", false, nullptr, [](void*) { saveConfig(); }, std::size(controls), controls);
        },
    };

    MenuEntryABI menuSubEntries[6];
    menuSubEntries[0] = {
        .name     = "Custom time enabled",
        .selected = [](void*) -> bool {
            return config.timeEnabled;
        },
        .click = [](void*) {
            config.timeEnabled ^= true;
            saveConfig(); },
    };
    menuSubEntries[1] = timeEntry;
    menuSubEntries[2] = {
        .name     = "Custom fog color enabled",
        .selected = [](void*) -> bool {
            return config.fogColorEnabled;
        },
        .click = [](void*) {
            config.fogColorEnabled ^= true;
            saveConfig(); },
    };
    menuSubEntries[3] = {
        .name     = "Apply brightness to custom fog color",
        .selected = [](void*) -> bool {
            return config.applyBrightnessToFogColor;
        },
        .click = [](void*) {
            config.applyBrightnessToFogColor ^= true;
            saveConfig(); },
    };
    menuSubEntries[4] = fogEntry;
    menuSubEntries[5] = cloudEntry;

    MenuEntryABI menuEntry{
        .name       = "Environment changer",
        .length     = std::size(menuSubEntries),
        .subentries = menuSubEntries,
    };
    addMenu(1, &menuEntry);
}

extern "C" [[gnu::visibility("default")]] void mod_init() {
    auto mcLib = dlopen("libminecraftpe.so", 0);

    std::span<std::byte> range1, range2;

    auto callback = [&](const dl_phdr_info& info) {
        if (auto h = dlopen(info.dlpi_name, RTLD_NOLOAD); dlclose(h), h != mcLib)
            return 0;
        range1 = {reinterpret_cast<std::byte*>(info.dlpi_addr + info.dlpi_phdr[1].p_vaddr), info.dlpi_phdr[1].p_memsz};
        range2 = {reinterpret_cast<std::byte*>(info.dlpi_addr + info.dlpi_phdr[2].p_vaddr), info.dlpi_phdr[2].p_memsz};
        return 1;
    };

    dl_iterate_phdr(
        [](dl_phdr_info* info, size_t, void* data) {
            return (*static_cast<decltype(callback)*>(data))(*info);
        },
        &callback);

    auto Level_typeinfo_name = hat::find_pattern(range1, hat::object_to_signature("16MultiPlayerLevel")).get();
    auto Level_typeinfo      = hat::find_pattern(range2, hat::object_to_signature(Level_typeinfo_name)).get() - sizeof(void*);
    auto Level_vtable        = hat::find_pattern(range2, hat::object_to_signature(Level_typeinfo)).get() + sizeof(void*);
    auto Level_getTime       = reinterpret_cast<int (**)(void*)>(Level_vtable) + 122; // 1.21.80 - 123

    static auto Level_getTime_orig = *Level_getTime;

    *Level_getTime = [](void* self) -> int {
        if (!config.timeEnabled) {
            return Level_getTime_orig(self);
        }
        return config.time;
    };

    auto OverworldDimension_typeinfo_name                  = hat::find_pattern(range1, hat::object_to_signature("18OverworldDimension")).get();
    auto OverworldDimension_typeinfo                       = hat::find_pattern(range2, hat::object_to_signature(OverworldDimension_typeinfo_name)).get() - sizeof(void*);
    auto OverworldDimension_vtable                         = hat::find_pattern(range2, hat::object_to_signature(OverworldDimension_typeinfo)).get() + sizeof(void*);
    auto OverworldDimension_getBrightnessDependentFogColor = reinterpret_cast<Color (**)(void*, Color*, float)>(OverworldDimension_vtable) + 23;
    auto OverworldDimension_getCloudHeight                 = reinterpret_cast<short (**)(void*)>(OverworldDimension_vtable) + 24;

    static auto OverworldDimension_getBrightnessDependentFogColor_orig = *OverworldDimension_getBrightnessDependentFogColor;

    *OverworldDimension_getBrightnessDependentFogColor = [](void* self, Color* baseColor, float brightness) -> Color {
        if (!config.fogColorEnabled) {
            return OverworldDimension_getBrightnessDependentFogColor_orig(self, baseColor, brightness);
        }
        Color c = config.fogColor;
        if (config.applyBrightnessToFogColor) {
            c.r *= brightness;
            c.g *= brightness;
            c.b *= brightness;
        }
        c.a = 1;
        return c;
    };

    *OverworldDimension_getCloudHeight = [](void*) -> short {
        return config.cloudHeight;
    };
}
