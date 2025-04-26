#include "dimension.h"
#include "imgui_config.h"
#include "menu.hpp"
#include <cmath>
#include <cstddef>
#include <dlfcn.h>
#include <functional>
#include <libhat.hpp>
#include <link.h>
#include <memory>
#include <print>
#include <span>
#include <stdlib.h>

extern "C" [[gnu::visibility("default")]] void mod_preinit() {
    auto menuLib = dlopen("libmcpelauncher_menu.so", 0);
    addMenu      = reinterpret_cast<decltype(addMenu)>(dlsym(menuLib, "mcpelauncher_addmenu"));
    showWindow   = reinterpret_cast<decltype(showWindow)>(dlsym(menuLib, "mcpelauncher_show_window"));
    closeWindow  = reinterpret_cast<decltype(closeWindow)>(dlsym(menuLib, "mcpelauncher_close_window"));
}

static Environment* env;
static ImguiConfig* conf;

extern "C" [[gnu::visibility("default")]] void mod_init() {
    env = new Environment();

    conf = new ImguiConfig(env);
    conf->addEntries();

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
    auto Level_getTime       = reinterpret_cast<int (**)(void*)>(Level_vtable) + 122;       // 1.21.80 - 123
    auto Level_setTime       = reinterpret_cast<void (**)(void*, int)>(Level_vtable) + 123; // 1.21.80 - 124

    static auto Level_getTime_orig = *Level_getTime;
    static auto Level_setTime_orig = *Level_setTime;

    *Level_getTime = [](void* self) -> int {
        return env->getTime(Level_getTime_orig(self));
    };

    *Level_setTime = [](void* self, int time) {
        Level_setTime_orig(self, env->getTime(time));
    };

    auto OverworldDimension_typeinfo_name                  = hat::find_pattern(range1, hat::object_to_signature("18OverworldDimension")).get();
    auto OverworldDimension_typeinfo                       = hat::find_pattern(range2, hat::object_to_signature(OverworldDimension_typeinfo_name)).get() - sizeof(void*);
    auto OverworldDimension_vtable                         = hat::find_pattern(range2, hat::object_to_signature(OverworldDimension_typeinfo)).get() + sizeof(void*);
    auto OverworldDimension_tick                           = reinterpret_cast<void (**)(Dimension*)>(OverworldDimension_vtable) + 16;
    auto OverworldDimension_getBrightnessDependentFogColor = reinterpret_cast<Color (**)(void*, Color*, float)>(OverworldDimension_vtable) + 23;
    auto OverworldDimension_getCloudHeight                 = reinterpret_cast<short (**)(Dimension*)>(OverworldDimension_vtable) + 24;

    static auto OverworldDimension_tick_orig                           = *OverworldDimension_tick;
    static auto OverworldDimension_getBrightnessDependentFogColor_orig = *OverworldDimension_getBrightnessDependentFogColor;

    *OverworldDimension_tick = [](Dimension* self) {
        env->onDimensionTick(self);
        OverworldDimension_tick_orig(self);
    };

    *OverworldDimension_getBrightnessDependentFogColor = [](void* self, Color* baseColor, float brightness) -> Color {
        return env->getFogColor(baseColor, brightness, OverworldDimension_getBrightnessDependentFogColor_orig(self, baseColor, brightness));
    };

    *OverworldDimension_getCloudHeight = [](Dimension*) -> short {
        return env->getCloudHeight();
    };
}
