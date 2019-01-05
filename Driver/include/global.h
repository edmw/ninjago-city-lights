#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#define PRODUCTION true

#include <SPI.h>

// #include <Tlc59711.h>
// #define THE_TLC Tlc59711
// #define THE_TLC_BEGIN beginFast
// #define THE_TLC_WRITE write
// #define THE_TLC_SET_CHANNEL setChannel

#include "Adafruit_TLC59711.h"
#define THE_TLC Adafruit_TLC59711
#define THE_TLC_BEGIN begin
#define THE_TLC_WRITE write
#define THE_TLC_SET_CHANNEL setPWM

// #define TLC_TOWER_LIGHT Lighter
// #define TLC_TOWER_LIGHT_BEGIN begin(1.0, true)
#define TLC_TOWER_LIGHT Pulser
#define TLC_TOWER_LIGHT_BEGIN begin(5000)

#endif
