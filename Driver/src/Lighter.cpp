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

#include "Lighter.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

bool Lighter::begin(float cap, bool full) {
    this->full_brightness = PWMLED::Brightness::Full * std::max(std::min(cap, float(1)), float(0));

    this->current_brightness = 0;
    this->brightness = full ? full_brightness : 0;

    return PWMLED::begin();
}

uint16_t Lighter::next() {
    if (brightness != current_brightness) {
        current_brightness = std::min(brightness, full_brightness);
    }
    return current_brightness;
}

void Lighter::off() {
    brightness = 0;
}

void Lighter::full() {
    brightness = full_brightness;
}
