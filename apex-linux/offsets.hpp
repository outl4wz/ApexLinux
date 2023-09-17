#pragma once

#include <iostream>

namespace Offsets
{
    constexpr uintptr_t health = 0x43c;
    constexpr uintptr_t aimPunch = 0x24b0;
    constexpr uintptr_t viewAngles = 0x25ac - 0x14;
    constexpr uintptr_t localEntity = 0x01ee8d50 + 0x8;
    constexpr uintptr_t glowEnable = 0x3c8;
    constexpr uintptr_t glowWalls = 0x3d0;
    constexpr uintptr_t entityList= 0x1b37b18;
    constexpr uintptr_t m_iName = 0x0589;
    constexpr uintptr_t m_iTeamNum = 0x044c;
    constexpr uintptr_t m_bleedOutState = 0x2740;
    constexpr uintptr_t visible = 0x1a78;
    constexpr uintptr_t cameraOrigin = 0x1f48;
    constexpr uintptr_t boneMatrix = 0x0e98 + 0x48;
    constexpr std::uintptr_t origin = 0x014c;
    constexpr uintptr_t red = 0x1D0;
    constexpr uintptr_t green = 0x1D4;
    constexpr uintptr_t blue = 0x1D8;
    constexpr uintptr_t in_attack = 0x07678368;
} 