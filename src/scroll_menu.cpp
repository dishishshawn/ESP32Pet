#include <Arduino.h>
#include "scroll_menu.h"
#include <TFT_eSPI.h>

#ifndef GFX
extern TFT_eSprite canvas;
#define GFX canvas
#endif

ScrollMenu::ScrollMenu(int x, int y, int menuSize) {
    this->menuSize = menuSize;
    this->x = x;
    this->y = y;
}

// void ScrollMenu::moveRight(){
//     if (nextSection == menuSize) {
//         nextSection = 0;
//         activeSection ++;
//         prevSection ++;
//     } else if (activeSection == menuSize) {
//         activeSection = 0;
//         nextSection ++;
//         prevSection ++;
//     } else if (prevSection == menuSize) {
//         prevSection = 0;
//         activeSection ++;
//         nextSection ++;
//     } else {
//         activeSection ++;
//         nextSection ++;
//         prevSection ++;
//     }
// }

// void ScrollMenu::moveLeft(){
//     if (prevSection == 0) {
//         prevSection = menuSize;
//         activeSection --;
//         nextSection --;
//     } else if (activeSection == 0) {
//         activeSection = menuSize;
//         prevSection --;
//         nextSection --;
//     } else if (nextSection == 0) {
//         nextSection = menuSize;
//         activeSection --;
//         prevSection --;
//     } else {
//         activeSection --;
//         prevSection --;
//         nextSection --;
//     }
// }