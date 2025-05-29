#include <Arduino.h>
#include <TFT_eSPI.h>
#include "esp_task_wdt.h"
#include <EEPROM.h>

#include "gotchi.h"
#include "scroll_menu.h"
#include "sprites.h"

TFT_eSPI tft = TFT_eSPI();

#define LEFT 13
#define RIGHT 14
#define SELECT 12
#define WAKE_UP_PIN 34

void stateSelection();
void homeFunc();
void sleepFunc();
void dressFunc();
void statsFunc();
void changeState(const char* section);
void displayMenuFunc();
void displayGotchi();
void displaySleep();
void displayEat();
void displayPlay();
void displayCut();
void displayAge();
void displayMenuDress();
void displayStats();

#define BAR_W 16
inline void clearOutside() {
    tft.resetViewport();                       // turn clipping OFF
    tft.fillRect(0, 0, BAR_W, 128, TFT_BLACK);           // left bar
    tft.fillRect(160-BAR_W, 0, BAR_W, 128, TFT_BLACK);   // right bar
    tft.setViewport(16, 0, 143, 127, true);    // clipping ON again
}

void setup() {
    Serial.begin(115200);
    Serial.println("Boot");
    tft.init(INITR_GREENTAB);
    tft.fillScreen(TFT_BLACK);
    tft.setRotation(1);

    bool flip = true;
    int x0 = 16;
    int y0 = 0;
    int x1 = x0 + 128 - 1;
    int y1 = y0 + 128 - 1;
    tft.setViewport(x0, y0, x1, y1, flip); // Set the viewport to the full screen

    tft.fillScreen(TFT_BLACK);

    pinMode(LEFT, INPUT_PULLDOWN);
    pinMode(RIGHT, INPUT_PULLDOWN);
    pinMode(SELECT, INPUT_PULLDOWN);
    pinMode(WAKE_UP_PIN, INPUT_PULLUP);

    esp_task_wdt_init(40, true); 
    esp_task_wdt_add(NULL); 

    Serial.println("Ready to start");
}

// States
int state = 0;
/*
    0 - home
    1 - sleep
    2 - dress
    3 - stats
*/

char menuSections[][20] = {
    "Cut",
    "Dress",
    "Eat",
    "Stats",
    "Play",
    "Sleep",
};

const unsigned char* menuIcons[] = {
    beard_icon,
    dress_icon,
    eat_icon,
    stats_icon,
    play_icon,
    sleep_icon,
};

const unsigned char* bodyArray[] = {
    myBitmapbody_01,
    myBitmapbody_02,
    myBitmapbody_03,
    myBitmapbody_04
};

const unsigned char* menuDressIcons[] = {
    backHome,
    myBitmapbody_01,
    myBitmapbody_02,
    myBitmapbody_03,
    myBitmapbody_04
};

ScrollMenu menu(58, 32, 6);
ScrollMenu menuDress(58, 32, 5);
char activeState[20] = "Home";

bool sleeping = EEPROM.read(0);

int sleepLevel = EEPROM.read(1);
int happiness = EEPROM.read(2);
int hunger = EEPROM.read(3);

int var_type = 0; // Data type to be retrieved from EEPROM (ints)
int age = EEPROM.get(10, var_type);
int beardLength = EEPROM.read(5);
int expression = EEPROM.read(6);
int clothing = EEPROM.read(7);

Gotchi gotchi(sleeping, sleepLevel, happiness, hunger, age, beardLength, expression, clothing);

int sec = 0;
bool pressed = false;
int frameCount = 0;
int spriteOffset = -1;

void loop() {
    stateSelection();
    delay(10);
}

void stateSelection() {
    if(state == 0){
        homeFunc();
    } else if (state == 1) {
        sleepFunc();
    } else if (state == 2) {
        dressFunc();
    } else if (state == 3) {
        statsFunc();
    }
}

// State functions

void changeState(const char* section){
    tft.drawRoundRect(0, 0, 128, 160, 3, TFT_WHITE); // quick outline
    delay(40);
    tft.drawRoundRect(0, 0, 128, 160, 3, TFT_BLACK); // erase it  

    Serial.println(section);
    if(strcmp(section, "Sleep") == 0){
        gotchi.updateSleeping();
        gotchi.updateHappiness(0);
        gotchi.updateSleep(0);
        tft.fillRect(0, 16, 128, 160, TFT_BLACK);
        state = 1;
    } else if(strcmp(section, "Play") == 0){
        gotchi.updateHappiness(4);
        gotchi.updateSleep(-2);
        gotchi.updateHunger(-2);
        displayPlay();
    } else if(strcmp(section, "Stats") == 0){
        displayAge();
        state = 3;
    } else if(strcmp(section, "Eat") == 0){
        gotchi.updateHunger(5);
        gotchi.updateHappiness(1);
        displayEat();
    } else if(strcmp(section, "Cut") == 0){
        gotchi.updateBeardLength(0);
        if(gotchi.beardLength >= 24){
        gotchi.updateHappiness(10);
        }
        displayCut();
    } else if(strcmp(section, "Dress") == 0){
        tft.fillRect(0, 16, 128, 48, TFT_BLACK);
        gotchi.updateHappiness(2);
        displayMenuDress();
        state = 2;
    } else if(strcmp(section, "Home") == 0){
        frameCount = 50;
        tft.fillRect(0, 16, 128, 48, TFT_BLACK);
        displayMenuFunc();
        displayGotchi();
        state = 0;
    }
    for (int i = 0; i < 20; i++){
    activeState[i] = section[i]; 
    }
    pressed = false;
    displayStats();
}

void homeFunc() {
    if(pressed == false) {
        if(digitalRead(SELECT) == HIGH) {
            pressed = true;
            changeState(menuSections[menu.activeSection]);
    } else if(digitalRead(LEFT) == HIGH) {
            pressed = true;
            menu.moveLeft();
            displayMenuFunc();
        } else if(digitalRead(RIGHT) == HIGH) {
            pressed = true;
            menu.moveRight();
            displayMenuFunc();
        }
    } else if(digitalRead(SELECT) == LOW && digitalRead(LEFT) == LOW && digitalRead(RIGHT) == LOW) {
            pressed = false;
        }
    displayGotchi();
}

void sleepFunc() {
    if(!gotchi.sleeping) {
        changeState("Home");
    }

    if(!pressed) {
        if(digitalRead(SELECT) == HIGH) {
            pressed = true;
            gotchi.updateSleeping();
            gotchi.updateHappiness(-3);
            changeState("Home");
        } else if(digitalRead(SELECT) == LOW && digitalRead(LEFT) == LOW && digitalRead(RIGHT) == LOW) {
        pressed = false;
    }
    displaySleep();
    }
}

void dressFunc() {

    if (!pressed) {

        if (digitalRead(SELECT) == HIGH) {          // SELECT
            pressed = true;

            if (menuDress.activeSection == 0) {     // “← Home”
                changeState("Home");
            } else {                                // one of the outfits
                gotchi.updateClothing(menuDress.activeSection - 1);
                changeState("Home");
            }

        } else if (digitalRead(LEFT) == HIGH) {     // LEFT
            pressed = true;
            menuDress.moveLeft();
            displayMenuDress();

        } else if (digitalRead(RIGHT) == HIGH) {    // RIGHT
            pressed = true;
            menuDress.moveRight();
            displayMenuDress();
        }
    }

    else if (digitalRead(SELECT) == LOW &&
             digitalRead(LEFT)   == LOW &&
             digitalRead(RIGHT)  == LOW) {
        pressed = false;
    }
}

void statsFunc(){
  if(pressed == false){
    if(digitalRead(SELECT) == HIGH){
      pressed = true;
      changeState("Home");    
    }
  }else if(digitalRead(SELECT) == LOW && digitalRead(LEFT) == LOW && digitalRead(RIGHT) == LOW){
    pressed = false;
  }  
}

void displayMenuFunc()
{
    int y = 17;
    const int rowH = 16;

    tft.fillRect(51, 17, 77, 47, TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(1);

    /* ── previous ─────────────────────────── */
    tft.setCursor(menu.x + 19, y + 4);
    tft.println(menuSections[ menu.prev() ]);
    tft.drawBitmap(menu.x, y + 3, menuIcons[ menu.prev() ], 10, 10, 1);

    /* ── active ───────────────────────────── */
    y += rowH;
    tft.drawRoundRect(menu.x - 5, y, 73, rowH, 3, TFT_WHITE);
    tft.setCursor(menu.x + 19, y + 4);
    tft.println(menuSections[ menu.activeSection ]);
    tft.drawBitmap(menu.x, y + 3, menuIcons[ menu.activeSection ], 10, 10, 1);

    /* ── next ─────────────────────────────── */
    y += rowH;
    tft.setCursor(menu.x + 19, y + 4);
    tft.println(menuSections[ menu.next() ]);
    tft.drawBitmap(menu.x, y + 3, menuIcons[ menu.next() ], 10, 10, 1);
}

// void displayMenuFunc(){
//   int y = 17;
//   int height = 16;

//   tft.fillRect(51, 17, 77, 47, TFT_BLACK);
//   tft.setTextColor(TFT_WHITE, TFT_BLACK);
  
//   tft.setTextSize(1);

//   //PREV SECTION
//   tft.setCursor(menu.x + 19, y + 4);
//   tft.println(menuSections[menu.prev()]);
//   tft.drawBitmap(menu.x, y + 3, menuIcons[menu.prev()], 10, 10, 1);

//   //ACTIVE SECTION
//   y = y + height;
//   tft.drawRoundRect(menu.x - 5, y, 73, 16, 3, TFT_WHITE);
//   tft.setCursor(menu.x + 19, y + 4);
//   tft.println(menuSections[menu.next()]);
//   tft.drawBitmap(menu.x, y + 3, menuIcons[menu.next()], 10, 10, 1);

//   //NEXT SECTION
//   y = y + height;
//   tft.setCursor(menu.x + 19, y + 4);
//   tft.println(menuSections[menu.nextSection]);
//   tft.drawBitmap(menu.x, y + 3, menuIcons[menu.nextSection], 10, 10, 1);
// }

void displayMenuDress(){
  tft.fillRect(1, 17, 127, 47, TFT_BLACK);

  // PREV CLOTHING
  tft.drawBitmap(3, 17, menuDressIcons[menuDress.prevSection], 32, 46, TFT_WHITE);

  // DISPLAY ALE WITH CHANGING CLOTHING
  tft.drawRoundRect(39, 16, 50, 48, 5, TFT_WHITE);
  
  int spriteX = 39 + 10;
  
  if(menuDress.activeSection != 0){
    tft.fillRect(spriteX, 18, 32, 46, TFT_BLACK);
    tft.drawBitmap(spriteX, 18, menuDressIcons[menuDress.activeSection], 32, 46, TFT_WHITE);
  
    tft.drawBitmap(spriteX, 18, feet_bg, 32, 46, TFT_BLACK);
    tft.drawBitmap(spriteX, 18, feet, 32, 46, TFT_WHITE);
    
    tft.drawBitmap(spriteX, 18, head_bg, 32, 46, TFT_BLACK);
    tft.drawBitmap(spriteX, 18, head, 32, 46, TFT_WHITE);
    
    if(gotchi.beardLength >= 24){
      tft.drawBitmap(10, 18 + spriteOffset, beard_bg, 32, 46, TFT_BLACK);
      tft.drawBitmap(10, 18 + spriteOffset, beard, 32, 46, TFT_WHITE);
    }
    
    tft.drawBitmap(spriteX, 18, eyes, 32, 46, TFT_BLACK);
  }else{
    tft.fillRect(spriteX, 18, 32, 46, TFT_BLACK);
    tft.drawBitmap(spriteX, 18, menuDressIcons[menuDress.activeSection], 32, 46, TFT_WHITE);
  }
  
  
  // NEXT CLOTHING
  tft.drawBitmap(93, 17, menuDressIcons[menuDress.nextSection], 32, 46, TFT_WHITE);
}

void displayGotchi(){

  if(frameCount >= 50){
    tft.drawRoundRect(0, 16, 50, 48, 5, TFT_WHITE);
    
    tft.fillRect(10, 18, 32, 46, TFT_BLACK);
    
    tft.drawBitmap(10, 18 + spriteOffset, bodyArray[gotchi.clothing], 32, 46, TFT_WHITE);
  
    tft.drawBitmap(10, 18, feet_bg, 32, 46, TFT_BLACK);
    tft.drawBitmap(10, 18, feet, 32, 46, TFT_WHITE);

    tft.drawBitmap(10, 18 + spriteOffset, head_bg, 32, 46, TFT_BLACK);
    tft.drawBitmap(10, 18 + spriteOffset, head, 32, 46, TFT_WHITE);
    
    if(gotchi.beardLength >= 24){
      tft.drawBitmap(10, 18 + spriteOffset, beard_bg, 32, 46, TFT_BLACK);
      tft.drawBitmap(10, 18 + spriteOffset, beard, 32, 46, TFT_WHITE);
    }

    tft.drawBitmap(10, 18 + spriteOffset, eyes, 32, 46, TFT_BLACK);
      
    
    if(spriteOffset == -1){
      spriteOffset = -2;
    }else{   
      spriteOffset = -1;
    }
    frameCount = 0;
  }
  frameCount++;
    
}

void displaySleep(){

  if(frameCount >= 50){
    tft.fillRect(0, 16, 128, 48, TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(1);
    tft.setCursor(64 - spriteOffset, 33 - spriteOffset);
    tft.println("Z");
    tft.setCursor(59 - !spriteOffset, 46 - !spriteOffset);
    tft.println("Z");
    tft.setTextSize(2);
    tft.setCursor(47 - spriteOffset, 26 - !spriteOffset);
    tft.println("Z");
      
    
    spriteOffset = !spriteOffset;
    frameCount = 0;
  }
  frameCount++;    
}

void selectEatFrame(int x, int y, int n){
  if(gotchi.beardLength >= 24){
    if(n == 1){
      tft.drawBitmap(x, y, eat_anim_head_b_1, 45, 40, TFT_WHITE);
    }else{
      tft.drawBitmap(x, y, eat_anim_head_b_2, 45, 40, TFT_WHITE);
    }  
  }else{
    if(n == 1){
      tft.drawBitmap(x, y, eat_anim_head_n_1, 45, 40, TFT_WHITE);
    }else{
      tft.drawBitmap(x, y, eat_anim_head_n_2, 45, 40, TFT_WHITE);
    }    
  }
  
}

void displayEat(){

  int startX = 30;
  int startY = 20;

  tft.fillRect(0, 16, 128, 48, TFT_BLACK); // Pulisco lo schermo

  // Frame 1
  selectEatFrame(startX,startY,1);
  tft.drawBitmap(startX + 50, startY + 15, eat_anim_food_1, 15, 15, TFT_WHITE);
  

  delay(500);

  // Frame 2
  tft.fillRect(0, 16, 45 + startX, 48, TFT_BLACK);
  selectEatFrame(startX,startY,2); 
  

  delay(500);

  // Frame 3
  tft.fillRect(0, 16, 45 + startX, 48, TFT_BLACK);
  tft.fillRect(45 + startX, 16, 55 + startX, 48, TFT_BLACK);
  selectEatFrame(startX,startY,1);
  tft.drawBitmap(startX + 50, startY + 15, eat_anim_food_2, 15, 15, TFT_WHITE);
  

  delay(500);

  // Frame 4
  tft.fillRect(0, 16, 45 + startX, 48, TFT_BLACK);
  selectEatFrame(startX,startY,2);
  

  delay(500);

  // Frame 5
  tft.fillRect(0, 17, 128, 48, TFT_BLACK);
  tft.fillRect(45 + startX, 16, 55 + startX, 48, TFT_BLACK);
  selectEatFrame(startX,startY,1);
  tft.drawBitmap(startX + 50, startY + 15, eat_anim_food_3, 15, 15, TFT_WHITE);
  

  delay(500);

  // Frame 6
  tft.fillRect(0, 16, 45 + startX, 48, TFT_BLACK);
  selectEatFrame(startX,startY,2);
  

  delay(500);

  // Frame 7
  tft.fillRect(0, 17, 128, 48, TFT_BLACK);
  tft.fillRect(45 + startX, 16, 55 + startX, 48, TFT_BLACK);
  selectEatFrame(startX,startY,1);
  
  esp_task_wdt_reset();

  delay(500);
  delay(500);

  changeState("Home");
  
}

//__________________________________________________ PLAY ANIMATION

void displayPlay(){
  int startX = 50;
  int startY = 17;
  int loop = 2;
  
  for (int i = 0; i <= loop; i++) {
    // Frame 1
    tft.fillRect(0, 16, 128, 48, TFT_BLACK); // Pulisco lo schermo 
    tft.drawBitmap(startX, startY, play_anim_1, 27, 48, TFT_WHITE);
     
    delay(200);

    // Frame 2
    tft.fillRect(0, 16, 128, 48, TFT_BLACK);
    tft.drawBitmap(startX, startY, play_anim_2, 27, 48, TFT_WHITE);
    
    delay(200);

    // Frame 3
    tft.fillRect(0, 16, 128, 48, TFT_BLACK);
    tft.drawBitmap(startX, startY, play_anim_3, 27, 48, TFT_WHITE);
    
    delay(200);

    // Frame 4
    tft.fillRect(0, 16, 128, 48, TFT_BLACK);
    tft.drawBitmap(startX, startY, play_anim_4, 27, 48, TFT_WHITE);
    
    delay(200);

    // Frame 5
    tft.fillRect(0, 16, 128, 48, TFT_BLACK);
    tft.drawBitmap(startX, startY, play_anim_5, 27, 48, TFT_WHITE);
    
    delay(200);
  }
  
  delay(500);

  changeState("Home");
  
}

//__________________________________________________ CUT ANIMATION

void displayCut(){
  int startX = 45;
  int startY = 24;
  int loop = 2;
  
  for (int i = 0; i <= loop; i++) {
    // Frame 1
    tft.fillRect(0, 16, 128, 48, TFT_BLACK); // Pulisco lo schermo 
    tft.drawBitmap(startX, startY, cut_anim_1, 37, 28, TFT_WHITE);
     
    delay(500);

    // Frame 2
    tft.fillRect(0, 16, 128, 48, TFT_BLACK);
    tft.drawBitmap(startX, startY, cut_anim_2, 37, 28, TFT_WHITE);
    
    delay(500);

  }
  
  delay(500);

  changeState("Home");
  
}

//__________________________________________________ DISPLAY AGE

void displayAge(){
  tft.fillRect(0, 16, 128, 48, TFT_BLACK);

  tft.setTextSize(1);
  
  tft.setCursor(10,20);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  tft.print("Name");
  tft.setTextColor(TFT_WHITE);
  tft.println(" Shawn");
  
  tft.setCursor(10,32);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  tft.print("Age");
  tft.setTextColor(TFT_WHITE);
  tft.print(" ");
  tft.print(gotchi.age/24);
  tft.println(" days");

  tft.setCursor(10,44);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  tft.print("Weight");
  tft.setTextColor(TFT_WHITE);
  tft.println(" 88 kg");
}


//__________________________________________________ STATS DISPLAY

void displayStats(){

  tft.fillRect(1, 1, 127, 15, TFT_BLACK);
  tft.setTextSize(1);

  // HAPPINESS STATS
  tft.setCursor(2,5);
  tft.println("H");
  tft.drawRoundRect(9, 6, 27, 6, 1, TFT_WHITE);
  int H = gotchi.mapValue(gotchi.happiness, 27);
  tft.fillRect(10, 7, H, 4, TFT_WHITE);

  // SLEEP STATS
  tft.setCursor(43,5);
  tft.println("S");
  tft.drawRoundRect(50, 6, 27, 6, 1, TFT_WHITE);
  int S = gotchi.mapValue(gotchi.sleep, 27);
  tft.fillRect(51, 7, S, 4, TFT_WHITE);

  // HUNGER/FOOD STATS
  tft.setCursor(85,5);
  tft.println("F");
  tft.drawRoundRect(92, 6, 27, 6, 1, TFT_WHITE);
  int F = gotchi.mapValue(gotchi.hunger, 27);
  tft.fillRect(93, 7, F, 4, TFT_WHITE);
  
  
}

// Timer
int hour = 3600; // 1 hour in seconds
void IRAM_ATTR watchdogCallback() {
    sec++;
    Serial.print("sec: ");
    Serial.println(sec);

    if (sec > hour) {
        sec = 0;

        gotchi.updateAge();
        Serial.println(gotchi.age);

        gotchi.updateHappiness(-1);
        gotchi.updateHunger(-1);
        gotchi.updateBeardLength(gotchi.beardLength + 1);
        if (gotchi.sleeping) {
            gotchi.updateSleep(3);
        } else {
            gotchi.updateSleep(-1);
        }
    }
}


void turnOn() {
  Serial.println("TURN ON");
  delay(2000);
}

void turnOff() {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(20, 30);
    tft.println("Goodbye!");

    delay(2000);

    // Save data to EEPROM
    EEPROM.write(0, gotchi.sleeping);
    EEPROM.write(1, gotchi.sleep);
    EEPROM.write(2, gotchi.happiness);
    EEPROM.write(3, gotchi.hunger);
    EEPROM.put(10, gotchi.age);
    EEPROM.write(5, gotchi.beardLength);
    EEPROM.write(6, gotchi.expression);
    EEPROM.write(7, gotchi.clothing);

    Serial.println("Entering deep sleep...");
    // esp_sleep_enable_ext0_wakeup(static_cast<gpio_num_t>(WAKE_UP_PIN), LOW); // Cast WAKE_UP_PIN to gpio_num_t
    // esp_deep_sleep_start();
}