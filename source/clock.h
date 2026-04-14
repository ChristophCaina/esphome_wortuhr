#pragma once
#include "esphome/components/light/addressable_light.h"
using namespace esphome::light;

namespace WordClock {

// ── LED-Indices (identisch zu deinem Original) ────────────
static const uint8_t ES_IST[]     = {0,1,3,4,5};
static const uint8_t UHR[]        = {101,100,99};
static const uint8_t FUENF[]      = {7,8,9,10};
static const uint8_t ZEHN[]       = {18,19,20,21};
static const uint8_t ZWANZIG[]    = {11,12,13,14,15,16,17};
static const uint8_t VIERTEL[]    = {26,27,28,29,30,31,32};
static const uint8_t VOR[]        = {41,42,43};
static const uint8_t NACH[]       = {33,34,35,36};
static const uint8_t HALB[]       = {44,45,46,47};
static const uint8_t DREIVIERTEL[]= {22,23,24,25,26,27,28,29,30,31,32};
static const uint8_t DREI[]       = {66,67,68,69};
static const uint8_t VIER_H[]     = {73,74,75,76};
static const uint8_t FUENF_H[]    = {51,52,53,54};
static const uint8_t EINS[]       = {62,63,64,65};
static const uint8_t EIN[]        = {63,64,65};
static const uint8_t ZWEI[]       = {55,56,57,58};
static const uint8_t SECHS[]      = {83,84,85,86,87};
static const uint8_t ACHT[]       = {77,78,79,80};
static const uint8_t SIEBEN[]     = {88,89,90,91,92,93};
static const uint8_t ZWOELF[]     = {94,95,96,97,98};
static const uint8_t ZEHN_H[]     = {106,107,108,109};
static const uint8_t NEUN[]       = {103,104,105,106};
static const uint8_t ELF[]        = {49,50,51};
static const uint8_t MIN_LEDS[]   = {110,111,112,113};

// ── Hilfsfunktionen ───────────────────────────────────────
static void setWord(AddressableLight &it,
                    const uint8_t *arr, uint8_t len,
                    Color col) {
  for (uint8_t i = 0; i < len; i++)
    it[arr[i]] = col;
}

static bool isNight(uint8_t hour, uint8_t ns, uint8_t ne) {
  if (ns == ne) return false;
  if (ns < ne)  return hour >= ns && hour < ne;
  return hour >= ns || hour < ne;
}

static void showHour(AddressableLight &it, uint8_t h, Color col) {
  switch(h) {
    case 1:  setWord(it, EINS,    4, col); break;
    case 2:  setWord(it, ZWEI,    4, col); break;
    case 3:  setWord(it, DREI,    4, col); break;
    case 4:  setWord(it, VIER_H,  4, col); break;
    case 5:  setWord(it, FUENF_H, 4, col); break;
    case 6:  setWord(it, SECHS,   5, col); break;
    case 7:  setWord(it, SIEBEN,  6, col); break;
    case 8:  setWord(it, ACHT,    4, col); break;
    case 9:  setWord(it, NEUN,    4, col); break;
    case 10: setWord(it, ZEHN_H,  4, col); break;
    case 11: setWord(it, ELF,     3, col); break;
    case 12: setWord(it, ZWOELF,  5, col); break;
  }
}

// ── Haupt-Update ──────────────────────────────────────────
static void update(AddressableLight &it,
                   int hour, int minute,
                   float r, float g, float b,
                   float dayBri, float nightBri,
                   float ns, float ne,
                   const std::string &dialect) {

  // Helligkeit
  float bri = isNight(hour, (uint8_t)ns, (uint8_t)ne) ? nightBri : dayBri;
  uint8_t brightness = (uint8_t)(bri / 100.0f * 255.0f);
  Color col = Color((uint8_t)r, (uint8_t)g, (uint8_t)b);
  // Helligkeit in Farbe einrechnen
  col.r = (uint8_t)(col.r * brightness / 255);
  col.g = (uint8_t)(col.g * brightness / 255);
  col.b = (uint8_t)(col.b * brightness / 255);

  // Alles löschen
  it.all() = Color::BLACK;

  uint8_t rounded = (minute / 5) * 5;
  uint8_t extra   = minute % 5;
  uint8_t h = hour % 12;
  if (h == 0) h = 12;
  if (rounded >= 25) h = (h % 12) + 1;

  setWord(it, ES_IST, 5, col);

  bool ost = (dialect == "Ost");

  if (ost) {
    switch (rounded) {
      case 0:
        if (h == 1) setWord(it, EIN, 3, col);
        else        showHour(it, h, col);
        setWord(it, UHR, 3, col);
        break;
      case 5:  setWord(it,FUENF,4,col); setWord(it,NACH,4,col); showHour(it,h,col); break;
      case 10: setWord(it,ZEHN,4,col);  setWord(it,NACH,4,col); showHour(it,h,col); break;
      case 15: { uint8_t nx=(h%12)+1; setWord(it,VIERTEL,7,col); showHour(it,nx,col); } break;
      case 20: setWord(it,ZWANZIG,7,col); setWord(it,NACH,4,col); showHour(it,h,col); break;
      case 25: setWord(it,FUENF,4,col); setWord(it,VOR,3,col); setWord(it,HALB,4,col); showHour(it,h,col); break;
      case 30: setWord(it,HALB,4,col); showHour(it,h,col); break;
      case 35: setWord(it,FUENF,4,col); setWord(it,NACH,4,col); setWord(it,HALB,4,col); showHour(it,h,col); break;
      case 40: setWord(it,ZWANZIG,7,col); setWord(it,VOR,3,col); showHour(it,h,col); break;
      case 45: setWord(it,DREIVIERTEL,11,col); showHour(it,h,col); break;
      case 50: setWord(it,ZEHN,4,col);  setWord(it,VOR,3,col); showHour(it,h,col); break;
      case 55: setWord(it,FUENF,4,col); setWord(it,VOR,3,col); showHour(it,h,col); break;
    }
  } else {
    switch (rounded) {
      case 0:
        if (h == 1) setWord(it, EIN, 3, col);
        else        showHour(it, h, col);
        setWord(it, UHR, 3, col);
        break;
      case 5:  setWord(it,FUENF,4,col);   setWord(it,NACH,4,col); showHour(it,h,col); break;
      case 10: setWord(it,ZEHN,4,col);    setWord(it,NACH,4,col); showHour(it,h,col); break;
      case 15: setWord(it,VIERTEL,7,col); setWord(it,NACH,4,col); showHour(it,h,col); break;
      case 20: setWord(it,ZWANZIG,7,col); setWord(it,NACH,4,col); showHour(it,h,col); break;
      case 25: setWord(it,FUENF,4,col); setWord(it,VOR,3,col); setWord(it,HALB,4,col); showHour(it,h,col); break;
      case 30: setWord(it,HALB,4,col); showHour(it,h,col); break;
      case 35: setWord(it,FUENF,4,col); setWord(it,NACH,4,col); setWord(it,HALB,4,col); showHour(it,h,col); break;
      case 40: setWord(it,ZWANZIG,7,col); setWord(it,VOR,3,col); showHour(it,h,col); break;
      case 45: setWord(it,VIERTEL,7,col); setWord(it,VOR,3,col); showHour(it,h,col); break;
      case 50: setWord(it,ZEHN,4,col);    setWord(it,VOR,3,col); showHour(it,h,col); break;
      case 55: setWord(it,FUENF,4,col);   setWord(it,VOR,3,col); showHour(it,h,col); break;
    }
  }

  // Minutenpunkte
  for (uint8_t i = 0; i < 4; i++)
    it[MIN_LEDS[i]] = (i < extra) ? col : Color::BLACK;
}

} // namespace WordClock
