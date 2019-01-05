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

#include "Blinker.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

bool Blinker::begin(unsigned int on_time, unsigned int off_time, bool on) {
    this->on_time = on_time;
    this->off_time = off_time;
    this->timer = on ? 0 : on_time;

    return PWMLED::begin();
}

void Blinker::on() {
    this->timer = 0;
}

void Blinker::off() {
    this->timer = on_time;
}

uint16_t Blinker::next() {
    timer = timer % (on_time + off_time);
    return (timer <= on_time) ? PWMLED::Brightness::Full : PWMLED::Brightness::Off;
}
