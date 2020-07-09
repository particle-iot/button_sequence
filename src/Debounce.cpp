/** 
 * @file Debounce.cpp
 * @author Douglas Xi, edit by Ed Ablan
 * @version 1.0
 * @date 06/17/2020
 *
 * @brief Debounce a sinlge switch with one instance of the debounce class.
 *
 * @details Please read the header file for more details
 *
 * @copyright Copyright (c) 2020 Particle Industries, Inc.  All rights reserved.
 */

#include "Debounce.h"
#include "spark_wiring.h"

#define DEBOUNCE_STATE_DEBOUNCED (0)
#define DEBOUNCE_STATE_UNSTABLE  (1)
#define DEBOUNCE_STATE_CHANGED   (3)
#define _BV(n) (1<<(n))

Debounce::Debounce()
    : _previousMillis(0)
    , _intervalMillis(30)
    , _state(0)
    , _pin(0)
{}

void Debounce::attach(pin_t pin)
{
    _pin = pin;
    start();
}

void Debounce::attach(pin_t pin, PinMode mode)
{
    pinMode(pin, mode);
    attach(pin);
}

void Debounce::attach(pin_t pin, PinMode mode, uint32_t intervalMillis)
{
    attach(pin, mode);
    interval(intervalMillis);
}

void Debounce::attach(std::function<int32_t(void)> read_cb, uint32_t intervalMillis)
{
    interval(intervalMillis);
    _read_cb = read_cb;
    start();
}

void Debounce::interval(uint32_t intervalMillis)
{
    _intervalMillis = intervalMillis;
}

void Debounce::start()
{
    _state = 0;
    if ((_read_cb) ? _read_cb() : digitalRead(_pin)) {
        _state = _BV(DEBOUNCE_STATE_DEBOUNCED) | _BV(DEBOUNCE_STATE_UNSTABLE);
    }
    _previousMillis = millis();
}

bool Debounce::update()
{
    // Read the state of the switch in a temporary variable.
    bool currentState = (_read_cb) ? _read_cb() : digitalRead(_pin);
    return update(currentState);
}

bool Debounce::update(bool currentState)
{
    _state &= ~_BV(DEBOUNCE_STATE_CHANGED);

    // If the read is different from last reading, reset the debounce counter
    if (currentState != (bool)(_state & _BV(DEBOUNCE_STATE_UNSTABLE)) ) {
        _previousMillis = millis();
        _state ^= _BV(DEBOUNCE_STATE_UNSTABLE);
    } else {
        if (millis() - _previousMillis >= _intervalMillis) {
            // We have passed the threshold time, so the input is now stable
            // If it is different from last state, set the 
            //DEBOUNCE_STATE_CHANGED flag
            if ((bool)(_state & _BV(DEBOUNCE_STATE_DEBOUNCED)) != currentState) {
                _previousMillis = millis();
                _state ^= _BV(DEBOUNCE_STATE_DEBOUNCED);
                _state |= _BV(DEBOUNCE_STATE_CHANGED);
            }
        }
    }

    return _state & _BV(DEBOUNCE_STATE_CHANGED);
}

bool Debounce::read()
{
    return _state & _BV(DEBOUNCE_STATE_DEBOUNCED);
}

bool Debounce::updateRead()
{
    update();
    return read();
}

bool Debounce::isHigh()
{
    update();
    return ( read() );
}

bool Debounce::isLow()
{
    update();
    return !( read() );
}
