#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "assets.h"
#include <Fonts/Picopixel.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SSD1306_ADDRESS 0x3C

#define FONT Picopixel
#define FONT_HEIGHT 6
#define FONT_WIDTH 4

#define DEBUG Serial