#pragma once

#include <iostream>
#include "process.hpp"

uintptr_t base;

#define PI 3.14159265358979323846

struct Vector3
{
    float x, y, z;
};

struct Color
{
    float r, g, b;
};

namespace Globals
{
    std::string white = "\033[1;37m";
    std::string green = "\033[1;32m";
    std::string red = "\033[1;31m";
    bool END = false;
    bool noRecoil = false;
    bool glow = false;
    bool aimbot = false;
    float aimFov = 5.f;
    float aimSmooth = 5.f;
    std::array<float, 64>entityVisible = {};
}

namespace Helper
{
    uintptr_t getLocalPlayer()
    {
        uintptr_t localPlayer = Process::Read<uintptr_t>(base + 0x1ee8d58);
    
        if (!localPlayer)
            return 0;
    
        return localPlayer;
    }
}