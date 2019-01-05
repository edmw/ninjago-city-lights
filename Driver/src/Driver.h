#ifndef __DRIVER_H__
#define __DRIVER_H__

///////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2019 Michael Baumgärtner
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <Arduino.h>
#include <map>

#include "global.h"

#include <OS.h>

#include "Blazer.h"
#include "Blinker.h"
#include "Lighter.h"
#include "Pulser.h"
#include "Ringer.h"

// Production mode:
// - Ground Level
// - Street Level (Gallery East)
// - Street Level (Gallery West)
// - Roof
// - Tower
#define MODE_ALL 0

// Development mode:

#define MODE_DEV 1

// Select driver mode:
#define MODE MODE_ALL

// Configure driver
#if MODE == MODE_ALL
#define TLC_COUNT 5
#define TLC_GROUND 4
#define TLC_GALLERY_EAST 3
#define TLC_GALLERY_SOUTH 2
#define TLC_TOWER 1
#define TLC_ROOF 0
#elif MODE == MODE_DEV
#define TLC_COUNT 1
#define TLC_ROOT 0
// #define TLC_GROUND 4
// #define TLC_GALLERY_EAST 3
// #define TLC_GALLERY_SOUTH 2
// #define TLC_TOWER 1
// #define TLC_ROOF 0
#else
#error("illegal mode")
#endif

class Driver final {

public:
    THE_TLC tlc;

    #ifdef TLC_ROOT
    Blinker root_blinker;
    Lighter root_lighter;
    #warning("ROOT enabled")
    #endif

    #ifdef TLC_TOWER
    Blinker obstacle_light;
    TLC_TOWER_LIGHT tower_light_1;
    TLC_TOWER_LIGHT tower_light_2;
    Lighter bath_room;
    Lighter tree_1;
    Lighter tree_2;
    #else
    #warning("TOWER disabled")
    #endif
    #ifdef TLC_ROOF
    Lighter bar_light_1;
    Lighter bar_light_2;
    Lighter bar_light_3;
    Lighter bar_light_4;
    Lighter bar_light_5;
    Lighter bar_light_6;
    Lighter store_light_1;
    Lighter store_light_2;
    Lighter store_light_3;
    Lighter store_light_4;
    #else
    #warning("ROOF disabled")
    #endif
    // upper street level
    #ifdef TLC_GALLERY_EAST
    Lighter decoration_light_1;
    Blazer gallery_light_7;
    Blazer gallery_light_8;
    #else
    #warning("GALLERY EAST disabled")
    #endif
    #ifdef TLC_GALLERY_SOUTH
    Lighter decoration_light_2;
    Blazer gallery_light_9;
    Blazer gallery_light_0;
    Lighter restaurant_light;
    Lighter shop_light;
    #else
    #warning("GALLERY WEST disabled")
    #endif
    // lower street level
    #ifdef TLC_GALLERY_EAST
    Blazer gallery_light_1;
    Blazer gallery_light_2;
    Blazer gallery_light_3;
    Lighter tea_room;
    #endif
    #ifdef TLC_GALLERY_SOUTH
    Blazer gallery_light_4;
    Blazer gallery_light_5;
    Blazer gallery_light_6;
    Lighter bed_room;
    Blinker billboard_light_1;
    Blinker billboard_light_2;
    #endif
    // ground level
    #ifdef TLC_GROUND
    Lighter bridge_light_1;
    Lighter bridge_light_2;
    Lighter marketstall_light;
    Ringer phone_light;
    #else
    #warning("GROUND disabled")
    #endif
    // ground upper level
    #ifdef TLC_GROUND
    Blazer lantern_light_1;
    Blazer lantern_light_2;
    Blazer lantern_light_3;
    #endif

    std::map<String, PWMLED*>leds;

    Driver() :
        tlc(TLC_COUNT),
        // root
        #ifdef TLC_ROOT
        root_blinker(tlc, "BLINKER", TLC_ROOT * 12 + 0),
        root_lighter(tlc, "LIGHTER", TLC_ROOT * 12 + 1),
        #endif
        // tower
        #ifdef TLC_TOWER
        obstacle_light(tlc, "ObstacleLight", TLC_TOWER * 12 + 0),
        tower_light_1(tlc, "Tower1", TLC_TOWER * 12 + 2),
        tower_light_2(tlc, "Tower2", TLC_TOWER * 12 + 3),
        bath_room(tlc, "BathRoom", TLC_TOWER * 12 + 1),
        tree_1(tlc, "Tree1", TLC_TOWER * 12 + 9),
        tree_2(tlc, "Tree2", TLC_TOWER * 12 + 11),
        #endif
        // roof
        #ifdef TLC_ROOF
        bar_light_1(tlc, "Bar1", TLC_ROOF * 12 + 0),
        bar_light_2(tlc, "Bar2", TLC_ROOF * 12 + 1),
        bar_light_3(tlc, "Bar3", TLC_ROOF * 12 + 2),
        bar_light_4(tlc, "Bar4", TLC_ROOF * 12 + 3),
        bar_light_5(tlc, "Bar5", TLC_ROOF * 12 + 4),
        bar_light_6(tlc, "Bar6", TLC_ROOF * 12 + 5),
        store_light_1(tlc, "Store1", TLC_ROOF * 12 + 6),
        store_light_2(tlc, "Store2", TLC_ROOF * 12 + 7),
        store_light_3(tlc, "Store3", TLC_ROOF * 12 + 8),
        store_light_4(tlc, "Store4", TLC_ROOF * 12 + 9),
        #endif
        // upper street level
        #ifdef TLC_GALLERY_EAST
        decoration_light_1(tlc, "Decoration1", TLC_GALLERY_EAST * 12 + 6),
        gallery_light_7(tlc, "Gallery7", TLC_GALLERY_EAST * 12 + 7),
        gallery_light_8(tlc, "Gallery8", TLC_GALLERY_EAST * 12 + 8),
        #endif
        #ifdef TLC_GALLERY_SOUTH
        decoration_light_2(tlc, "Decoration2", TLC_GALLERY_SOUTH * 12 + 6),
        gallery_light_9(tlc, "Gallery9", TLC_GALLERY_SOUTH * 12 + 7),
        gallery_light_0(tlc, "Gallery0", TLC_GALLERY_SOUTH * 12 + 8),
        restaurant_light(tlc, "Restaurant", TLC_GALLERY_SOUTH * 12 + 9),
        shop_light(tlc, "Shop", TLC_GALLERY_SOUTH * 12 + 10),
        #endif
        // lower street level
        #ifdef TLC_GALLERY_EAST
        gallery_light_1(tlc, "Gallery1", TLC_GALLERY_EAST * 12 + 0),
        gallery_light_2(tlc, "Gallery2", TLC_GALLERY_EAST * 12 + 1),
        gallery_light_3(tlc, "Gallery3", TLC_GALLERY_EAST * 12 + 2),
        tea_room(tlc, "TeaRoom", TLC_GALLERY_EAST * 12 + 3),
        #endif
        #ifdef TLC_GALLERY_SOUTH
        gallery_light_4(tlc, "Gallery4", TLC_GALLERY_SOUTH * 12 + 0),
        gallery_light_5(tlc, "Gallery5", TLC_GALLERY_SOUTH * 12 + 1),
        gallery_light_6(tlc, "Gallery6", TLC_GALLERY_SOUTH * 12 + 2),
        bed_room(tlc, "BedRoom", TLC_GALLERY_SOUTH * 12 + 3),
        billboard_light_1(tlc, "Billboard1", TLC_GALLERY_SOUTH * 12 + 4),
        billboard_light_2(tlc, "Billboard2", TLC_GALLERY_SOUTH * 12 + 5),
        #endif
        #ifdef TLC_GROUND
        bridge_light_1(tlc, "Bridge1", TLC_GROUND * 12 + 0),
        bridge_light_2(tlc, "Bridge2", TLC_GROUND * 12 + 1),
        marketstall_light(tlc, "Marketstall", TLC_GROUND * 12 + 3),
        phone_light(tlc, "Phone", TLC_GROUND * 12 + 4),
        #endif
        #ifdef TLC_GROUND
        lantern_light_1(tlc, "Lantern1", TLC_GROUND * 12 + 6),
        lantern_light_2(tlc, "Lantern2", TLC_GROUND * 12 + 7),
        lantern_light_3(tlc, "Lantern3", TLC_GROUND * 12 + 8),
        #endif

        leds()

    {}

};

#endif
