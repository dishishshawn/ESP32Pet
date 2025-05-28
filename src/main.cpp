#include <Arduino.h>
#include <TFT_eSPI.h>
#include <LittleFS.h>

TFT_eSPI tft = TFT_eSPI();
#define TFT_WIDTH 128
#define TFT_HEIGHT 160

void setup() {
    tft.init();
    tft.setRotation(0); // Portrait mode
    tft.fillScreen(TFT_BLACK);
    tft.setSwapBytes(true);

    while(!Serial && !LittleFS.begin(true)) {
        tft.setCursor(0, 0);
        tft.print("Initializing...");
        Serial.begin(115200);
        LittleFS.begin(true);
        delay(100);
    }

    tft.fillScreen(TFT_WHITE);
    tft.setCursor(0, 0);
    tft.print("Ready...");
}

void loop() {
}
