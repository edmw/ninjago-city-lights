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

#include "Pulser.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

bool Pulser::begin(unsigned long pulse_time, unsigned long start_time) {
    this->pulse_time = pulse_time;
    this->timer = start_time;

    return PWMLED::begin();
}

uint16_t Pulser::next() {
    if (timer > pulse_time) timer = 0;
    float f = float(timer) / float(pulse_time);
    float t = (f < 0.5) ? 2*f : 2*(1-f);
    float p = (t < 0.5) ? 4*t*t*t : (t-1)*(2*t-2)*(2*t-2)+1;
    return uint16_t(p * PWMLED::Brightness::Full);
}
