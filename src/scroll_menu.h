/*
    Scroll_menu.h - Library for Scroll Menu instance.
    Created by Shawn Agarwal, May 2025
    This library is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*/

#ifndef SCROLL_MENU_H
#define SCROLL_MENU_H
#include <Arduino.h>

class ScrollMenu {
    public:
    ScrollMenu(int x, int y, int menuSize);

    int x;
    int y;
    int menuSize;
    int prevSection = 0;
    int activeSection = 1;
    int nextSection = 2;

    int prev() const { return (activeSection + menuSize - 1) % menuSize; }
    int next() const { return (activeSection + 1) % menuSize; }

    void moveRight() { activeSection = next(); }
    void moveLeft() { activeSection = prev(); }
};

#endif