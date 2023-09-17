#pragma once

#include "process.hpp"
#include "offsets.hpp"
#include "utils.hpp"
#include "key.hpp"
#include <chrono>
#include <thread>
#include <vector>
#include <math.h>

namespace Banners
{
    const void banner()
    {
        std::cout << "[+] PID: " << Process::GetProcessId() << "\n";
        std::cout << "[+] Base Address: " << base << "\n";
        std::cout << "\n";
        std::cout << "       |\\---/|" << "\n";
        std::cout << "       | " << Globals::red << "o" << Globals::white << "_" << Globals::red << "o" << Globals::white << " | " << "\n";
        std::cout << "        \\_^_/ " << "\n";
        std::cout << R"(
  Panic key -> END
)";
    }

    const void menu()
    {
        std::cout << Globals::noRecoil << "\n";
        system("clear");
        Banners::banner();
        std::cout << "\n";
        std::cout << "[F1]  | No Recoil ";
        Globals::noRecoil ? std::cout << " [" << Globals::green << "ON" << Globals::white << "]\n" : std::cout << " [" << Globals::red << "OFF" << Globals::white << "]\n";
        std::cout << "[F2]  | Glow ";
        Globals::glow ? std::cout << "      [" << Globals::green << "ON" << Globals::white << "]\n" : std::cout << "      [" << Globals::red << "OFF" << Globals::white << "]\n";
        std::cout << "[F3]  | Aimbot ";
        Globals::aimbot ? std::cout << "    [" << Globals::green << "ON" << Globals::white << "]\n" : std::cout << "    [" << Globals::red << "OFF" << Globals::white << "]\n";

        if (Globals::aimbot)
        {
            std::cout << "\n[F4-F5] Fov: " << Globals::aimFov << "\n";
            std::cout << "[F6-F7] Smooth: " << Globals::aimSmooth << "\n";
        }
    }
}

namespace Cheats
{           
    const uintptr_t noRecoil()
    {
        Vector3 oldPunch = Vector3{0.f, 0.f, 0.f};

        while (!Globals::END)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(5));

            const uintptr_t localPlayer = Helper::getLocalPlayer();

            if (!Globals::noRecoil)
                continue;

            if (!localPlayer)
                continue;

            Vector3 angles = Process::Read<Vector3>(localPlayer + Offsets::viewAngles);
            Vector3 punch = Process::Read<Vector3>(localPlayer + Offsets::aimPunch);

            const Vector3 newAngles = Vector3{
                angles.x + (oldPunch.x - punch.x), 
                angles.y + (oldPunch.y - punch.y), 
                angles.z + (oldPunch.z - punch.z), 
            };
            Process::Write<float>(localPlayer + Offsets::viewAngles + 0x0, newAngles.x);
            Process::Write<float>(localPlayer + Offsets::viewAngles + 0x4, newAngles.y);
            Process::Write<float>(localPlayer + Offsets::viewAngles + 0x8, newAngles.z);

            oldPunch = punch;
        }

        return 0;
    }

    const uintptr_t getEntity(const size_t i)
    {
        uintptr_t entityList = base + Offsets::entityList;
        uintptr_t baseEntity = Process::Read<uintptr_t>(entityList);

        if (!baseEntity)
            return 0;

        return Process::Read<uintptr_t>(entityList + (i << 5));
    }

    const void writeColor(const uintptr_t entity, const Color color)
    {
        Process::Write<int>(entity + Offsets::glowEnable, 1);
        Process::Write<int>(entity + Offsets::glowWalls, 2);

        Process::Write<float>(entity + Offsets::red, color.r);
        Process::Write<float>(entity + Offsets::green, color.g);
        Process::Write<float>(entity + Offsets::blue, color.b);
    }

    const uintptr_t glow()
    {
        while (!Globals::END)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(5));

            const uintptr_t localPlayer = Helper::getLocalPlayer();

            if (!Globals::glow)
                continue;
            
            if (!localPlayer)
                continue;

            const uintptr_t localTeam = Process::Read<uintptr_t>(localPlayer + Offsets::m_iTeamNum);

            for (size_t i = 0; i < 64; i++)
            {
                const uintptr_t entity = getEntity(i);

                if (!entity)
                    continue;
                
                if (Process::Read<uintptr_t>(entity + Offsets::m_iName) != 125780153691248)
                    continue;

                const uintptr_t entityTeam = Process::Read<uintptr_t>(entity + Offsets::m_iTeamNum);

                if (entityTeam == localTeam)
                    writeColor(entity, {0.f, 255.f, 0.f});
                else if (entityTeam != localTeam && Process::Read<int>(entity + Offsets::m_bleedOutState))
                    writeColor(entity, {255.f, 0.f, 0.f});
                else if (entityTeam != localTeam)
                    writeColor(entity, {0.f, 0.f, 255.f});
            }
        }

        return 0;
    }

    const float getLastVisibleTime(const uintptr_t entity)
    {
        return Process::Read<float>(entity + Offsets::visible);
    }

    const Vector3 getCameraOrigin(const uintptr_t entity)
    {
        return Process::Read<Vector3>(entity + Offsets::cameraOrigin);
    }

    const uintptr_t getBoneMatrix(const uintptr_t entity)
    {
        return Process::Read<uintptr_t>(entity + Offsets::boneMatrix);
    }

    const Vector3 getOrigin(const uintptr_t entity)
    {
        return Process::Read<Vector3>(entity + Offsets::origin);
    }

    const Vector3 getBonePos(const int id, const std::uintptr_t boneMatrix, const Vector3 basePos)
    {
        return Vector3{
            Process::Read<float>(boneMatrix + 0xCC + (id * 0x30)) + basePos.x,
            Process::Read<float>(boneMatrix + 0xDC + (id * 0x30)) + basePos.y,
            Process::Read<float>(boneMatrix + 0xEC + (id * 0x30)) + basePos.z,
        };
    }

    const bool zero(const Vector3 angle)
    {
        return angle.x == 0.f && angle.y == 0.f && angle.z == 0.f;
    }

    const void setViewAngles(const std::uintptr_t entity, const Vector3 angles)
    {
        Process::Write<float>(entity + 0x2598, angles.x);
        Process::Write<float>(entity + 0x259c, angles.y);
        Process::Write<float>(entity + 0x25a0, angles.z);
    }

    const int isInGame()
    {
        const uintptr_t localPlayer = Helper::getLocalPlayer();
        return Process::Read<int>(localPlayer + Offsets::health);
    }

    const uintptr_t aimbot()
    {
        while (!Globals::END)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(5));

            if (GetAsyncKeyState(XK_F4))
            {
                Globals::aimFov -= 1;
                Banners::menu();
            } else  if (GetAsyncKeyState(XK_F5))
            {
                Globals::aimFov += 1;
                Banners::menu();
            }

            if (GetAsyncKeyState(XK_F6))
            {
                Globals::aimSmooth -= 1;
                Banners::menu();
            } else  if (GetAsyncKeyState(XK_F7))
            {
                Globals::aimSmooth += 1;
                Banners::menu();
            }
            
            const uintptr_t localPlayer = Helper::getLocalPlayer();

            if (!Globals::aimbot)
                continue;
            
            if (!localPlayer)
                continue;
            
            if (!Process::Read<int>(base + 0x0bcc8a28))
                continue;
    
            const uintptr_t localTeam = Process::Read<uintptr_t>(localPlayer + Offsets::m_iTeamNum);
    
            const Vector3 viewAngles = Process::Read<Vector3>(localPlayer + Offsets::viewAngles);
            const auto cameraPosition = getCameraOrigin(localPlayer);
    
            const Vector3 punchAngle = Process::Read<Vector3>(localPlayer + Offsets::aimPunch);
    
            auto bestFov = Globals::aimFov;
            auto bestAngle = Vector3{};
            auto visible = false;
    
            const auto boneMatrix = getBoneMatrix(localPlayer);
            const auto basePosition = getOrigin(localPlayer);
    
            for (size_t i = 0; i < 64; i++)
            {
                const uintptr_t entity = getEntity(i);
    
                if (!entity)
                    continue;
                
                if (Process::Read<uintptr_t>(entity + Offsets::m_iName) != 125780153691248)
                    continue;
                
                if (!Process::Read<int>(entity + Offsets::health))
                    continue;
            
                if (Process::Read<int>(entity + Offsets::m_bleedOutState))
                    continue;
    
                const auto visibleTime = getLastVisibleTime(entity);

                visible = visibleTime > 0.f && visibleTime > Globals::entityVisible[i];
    
                if (!visible)
                    continue;
    
                const auto entityTeam = Process::Read<uintptr_t>(entity + Offsets::m_iTeamNum);
    
                if (entityTeam == localTeam)
                    continue;
    
                const auto boneMatrix = getBoneMatrix(entity);
                const auto basePosition = getOrigin(entity);
    
                auto playerBonePosition = Vector3{};
    
                const std::vector<int> bonePositions = { 0, 5, 7, 8 };
    
                playerBonePosition = getBonePos(bonePositions[std::rand() % bonePositions.size()], boneMatrix, basePosition);
    
                const auto relativeOrigin = Vector3{
                    playerBonePosition.x - cameraPosition.x,
                    playerBonePosition.y - cameraPosition.y,
                    playerBonePosition.z - cameraPosition.z, 
                };
    
                const auto magnitude = std::sqrt(relativeOrigin.x * relativeOrigin.x + relativeOrigin.y * relativeOrigin.y + relativeOrigin.z * relativeOrigin.z);
    
                const auto calcAngle = Vector3{
                    asinf(-relativeOrigin.z / magnitude) * (180.0f / PI),
                    std::atan2(relativeOrigin.y, relativeOrigin.x) * (180.0f / PI),
                    0.0f };
    
                const auto angle = Vector3{
                    calcAngle.x - (viewAngles.x + punchAngle.x),
                    calcAngle.y - (viewAngles.y + punchAngle.y),
                    calcAngle.z - (viewAngles.z + punchAngle.z) };
    
                const auto currentFov = std::hypot(angle.x, angle.y);
    
                if (currentFov < bestFov)
                {
                    bestFov = currentFov;
                    bestAngle = angle;
                }
    
                Globals::entityVisible[i] = visibleTime;
            }
    
            if (!zero(bestAngle)) 
                setViewAngles(localPlayer, { viewAngles.x + bestAngle.x / Globals::aimSmooth, viewAngles.y + bestAngle.y / Globals::aimSmooth, viewAngles.z + bestAngle.z / Globals::aimSmooth });
        }
    }
}