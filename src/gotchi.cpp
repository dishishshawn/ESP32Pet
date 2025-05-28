#include <Arduino.h>
#include <EEPROM.h>
#include "gotchi.h"

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
    sleeping = !sleeping; // change sleep state
    EEPROM.write(0, sleeping);
}

// Functions to update stats

void Gotchi::updateSleep(int sleepVal) {
    sleep += sleepVal;
    if(sleep < 0){
        sleep = 0;
    } else if(sleep > 24) {
        sleep = 24;
    }
    EEPROM.write(1, sleep);
}

void Gotchi::updateHappiness(int happinessVal) {
    happiness += happinessVal;
    if(happiness < 0){
        happiness = 0;
    } else if(happiness > 24) {
        happiness = 24;
    }
    EEPROM.write(2, happiness);
}

void Gotchi::updateHunger(int hungerVal) {
    hunger += hungerVal;
    if(hunger < 0){
        hunger = 0;
    } else if(hunger > 24) {
        hunger = 24;
    }
    EEPROM.write(3, hunger);
}

void Gotchi::updateAge() {
    age++;
    EEPROM.put(10, age);
}

void Gotchi::updateBeardLength(int newBeardLength) {
    beardLength = newBeardLength;
    EEPROM.write(5, beardLength);
}

void Gotchi::updateExpression(int newExpression) {
    expression = newExpression;
    EEPROM.write(6, expression);
}

void Gotchi::updateClothing(int newClothing) {
    clothing = newClothing;
    EEPROM.write(7, clothing);
}

// Values to pixels

int Gotchi::mapValue(int val, int barLength) {
    int statLength = (val * barLength - 2) / 24;
    return statLength;
}