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

#include "PWMLED.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

PWMLED::PWMLED(THE_TLC& tlc, String label, uint16_t channel) :
    label(label), tlc(tlc), channel(channel) {
}

bool PWMLED::begin() {
    this->current = next();
    return true;
}

bool PWMLED::begin(float dim) {
    set_dim(dim);
    this->current = next();
    return true;
}

void PWMLED::set_dim(float dim) {
    this->dim = std::max(std::min(dim, float(1)), float(0));
}

void PWMLED::activate() {
    drive = true;
}

void PWMLED::deactivate() {
    drive = false;
}

bool PWMLED::is_active() {
    return drive;
}

bool PWMLED::loop() {
    uint8_t prior = current;

    current = drive ? next() : 0;

    if (current == prior) return false;

    tlc.THE_TLC_SET_CHANNEL(channel, current * dim);

    return true;
}

uint16_t PWMLED::next() {
    return 0;
}
