#pragma once

#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include <X11/keysym.h>
#include <X11/extensions/XKB.h>
#include <chrono>

Display* display;

bool toggleKey(unsigned int keycode)
{
    static bool prevState = false;
    static bool curState = false;
    static auto lastPress = std::chrono::steady_clock::now();
    const int debounceTime = 150;

    char keys[32];
    XQueryKeymap(display, keys);
    curState = !!(keys[keycode / 8] & (1 << (keycode % 8)));

    auto curTime = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(curTime - lastPress).count();

    if (curState && !prevState && elapsed > debounceTime) {
        prevState = true;
        lastPress = curTime;
        return true;
    }
    prevState = false;

    return false;
}

bool GetAsyncKeyState(KeySym key)
{
    return toggleKey(XKeysymToKeycode(display, key));
}
