# Gameboy Emulator

A Gameboy (DMG) emulator written in C.

## Requirements

- libsdl2-dev
- libsdl2-ttf-dev
- build-essential
- cmake
- check

# Setup

Run CMake and generate build_commands.json for clangd to not show errors in the
editor.

> For neovim, use the `astrocommunity.pack.cpp` plugin.

```bash
mkdir build
cd build && cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
```
