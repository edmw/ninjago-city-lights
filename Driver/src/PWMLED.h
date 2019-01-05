#ifndef __PWMLED_H__
#define __PWMLED_H__

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

#include "global.h"

class PWMLED {
public:
    PWMLED(THE_TLC& tlc, String label, uint16_t index);

    bool begin();
    bool begin(float dim);

    void set_dim(float dim);

    void activate();
    void deactivate();
    bool is_active();

    bool loop(void);

    enum Brightness : uint16_t {
        Off = 0x0,
        Fourth = 0x3FFF,
        ThreeFourths = 0xBFFF,
        Full = 0xffff
    };

    String label;

protected:
    // PWM
    uint16_t current;
    virtual uint16_t next(void) = 0;

private:
    THE_TLC &tlc;
    uint16_t channel;

    float dim = 1.0;

    bool drive = true;
};

#endif
