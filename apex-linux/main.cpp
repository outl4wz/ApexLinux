#include "cheats.hpp"

const void toggle(bool &boolName, KeySym key)
{
    if (GetAsyncKeyState(key))
    {
        boolName = !boolName;
        Banners::menu();
    }
}

int main() 
{
    XInitThreads();

    Process::GetProcessId();

    base = Process::GetModuleBaseAddress("r5apex.exe");

    if (!base)
    {
        std::cout << "[!] Failed to obtain base address" << "\n";
        return 0;
    }

    display = XOpenDisplay(NULL);

    printf("PS1='\\[\e]0;Linux Apex External\a\\]\\u@\\h:\\w\\$'");
    Banners::banner();
    Banners::menu();

    std::thread t1(Cheats::noRecoil);
    std::thread t2(Cheats::glow);
    std::thread t3(Cheats::aimbot);
    
    t1.detach();
    t2.detach();
    t3.detach();

    while (!Globals::END)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));

        if (GetAsyncKeyState(XK_End))
        {
            Globals::END = true;
            system("clear");
            break;
        }

        if (!Cheats::isInGame())
            std::fill(Globals::entityVisible.begin(), Globals::entityVisible.end(), 0);

        toggle(Globals::noRecoil, XK_F1);
        toggle(Globals::glow, XK_F2);
        toggle(Globals::aimbot, XK_F3);
    }

    XCloseDisplay(display);
    return 0;
}
