/*
    Gotchi.h - Library for Gotchi instance.
    Created by Shawn Agarwal, May 2025
    This library is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*/


#ifndef GOTCHI_H
#define GOTCHI_H

#include <Arduino.h>

class Gotchi {
    public:
        Gotchi(bool sleeping,
               int sleep,
               int hunger,
               int happiness,
               int age,
               int beardLength,
               int expression,
               int clothing);

        bool sleeping; // 0

        int sleep; // 1
        int happiness; // 2
        int hunger; // 3
        int age; // 4
        int beardLength; // 5
        int expression; // 6
        int clothing; // 7

        void updateSleeping();

        void updateSleep(int sleepVal);
        void updateHappiness(int happinessVal);
        void updateHunger(int hungerVal);

        void updateAge();

        void updateBeardLength(int newBeardLength);
        void updateExpression(int newExpression);
        void updateClothing(int newClothing);

        int mapValue(int val, int barLength);
};

#endif
