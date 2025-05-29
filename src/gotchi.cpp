#include <Arduino.h>
#include <EEPROM.h>
#include <TFT_eSPI.h>
#include "gotchi.h"

#ifndef GFX
extern TFT_eSprite canvas;
#define GFX canvas
#endif

Gotchi::Gotchi(bool sleeping, int sleep, int hunger, int happiness, int age, int beardLength, int expression, int clothing) {
    this->sleeping = sleeping;
    this->sleep = sleep;
    this->hunger = hunger;
    this->happiness = happiness;
    this->age = age;
    this->beardLength = beardLength;
    this->expression = expression;
    this->clothing = clothing;
}

// Sleeping and Waking up function
void Gotchi::updateSleeping() {
    bool old = sleeping;
    sleeping = !sleeping; // change sleep state
    if (sleeping != old) {
        EEPROM.write(0, sleeping);
        EEPROM.commit();
    }
}

// Functions to update stats

void Gotchi::updateSleep(int sleepVal) {
    int old = sleep;
    sleep += sleepVal;
    sleep = constrain(sleep, 0, 24);
    if (sleep != old) {
        EEPROM.write(1, sleep);
        EEPROM.commit();
    }
}

void Gotchi::updateHappiness(int happinessVal) {
    int old = happiness;
    happiness += happinessVal;
    happiness = constrain(happiness, 0, 24);
    if (happiness != old) {
        EEPROM.write(2, happiness);
        EEPROM.commit();
    }
}

void Gotchi::updateHunger(int hungerVal) {
    int old = hunger;
    hunger += hungerVal;
    hunger = constrain(hunger, 0, 24);
    if (hunger != old) {
        EEPROM.write(3, hunger);
        EEPROM.commit();
    }
}

void Gotchi::updateAge() {
    int old = age;
    age++;
    if (age != old) {
        EEPROM.put(10, age);
        EEPROM.commit();
    }
}

void Gotchi::updateBeardLength(int newBeardLength) {
    int old = beardLength;
    beardLength = constrain(newBeardLength, 0, 24);
    if (beardLength != old) {
        EEPROM.write(5, beardLength);
        EEPROM.commit();
    }
}

void Gotchi::updateExpression(int newExpression) {
    int old = expression;
    expression = newExpression;
    if (expression != old) {
        EEPROM.write(6, expression);
        EEPROM.commit();
    }
}

void Gotchi::updateClothing(int newClothing) {
    int old = clothing;
    clothing = newClothing;
    if (clothing != old) {
        EEPROM.write(7, clothing);
        EEPROM.commit();
    }
}

// Values to pixels

int Gotchi::mapValue(int val, int barLength) {
    int statLength = (val * barLength - 2) / 24;
    return statLength;
}