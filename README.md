# mcpelauncher-environmentchanger

A mod for [mcpelauncher](https://minecraft-linux.github.io) that lets you change the game's environment.


## Installation

Create a `mods` directory in the directory of your mcpelauncher profile (e.g. `~/.local/share/mcpelauncher`) if you have not already done so.
Download the mod from [releases](https://github.com/GameParrot/mcpelauncher-environment/releases) and move the file into the `mods` directory.


## Usage

- Change the environment via the menu bar. You can change time of day, fog color, and cloud height.


## Building

Prerequisites:

- Android NDK r27 or later. [Download](https://developer.android.com/ndk/downloads)
- CMake 3.21 or later

Replace `/path/to/ndk` with the actual path to the Android NDK:

```
cmake -DCMAKE_TOOLCHAIN_FILE=/path/to/ndk/build/cmake/android.toolchain.cmake -DANDROID_ABI=x86_64 -DCMAKE_BUILD_TYPE=Release -B build
cmake --build build
```

This will create a `build` directory containing the mod file. Install it as described in [Installation](#installation).
