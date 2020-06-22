/** 
 * @file Debounce.h
 * @author Douglas Xi, edit by Ed Ablan
 * @version 1.0
 * @date 06/17/2020
 *
 * @brief Used to debounce a single switch or button
 *
 * @details Uses a state and counter to read a switch or button, and tell when
 * the state is stable and if the state changed from the previous read.
 *
 * @copyright Copyright (c) 2020 Particle Industries, Inc.  All rights reserved.
 */


#pragma once

#include <functional>
#include "Particle.h"

class Debounce {
public:
    
    /**
     * @brief Constructor for class
     */
    Debounce();

    /**
     * @brief Attach to a pin (and also sets initial state)
     *
     * @details called to attach the neccessary parameters for debouncing. 
     *
     * @param[in] pin - signal pin to debounce signal
     */
    void attach(pin_t pin);

    /**
     * @brief Attach to a pin (and also sets initial state) and sets pin to mode 
     * (INPUT/INPUT_PULLUP/OUTPUT)
     *
     * @details called to attach the neccessary parameters for debouncing. 
     *
     * @param[in] pin - signal pin to debounce signal
     * @param[in] mode - pin mode (i.e PULL UP, PULL DOWN, etc)
     */
    void attach(pin_t pin, PinMode mode);

    /**
     * @brief Attach to a pin, sets pin to mode and debounce interval in 
     * milliseconds
     *
     * @details called to attach the neccessary parameters for debouncing. 
     *
     * @param[in] pin - signal pin to debounce signal
     * @param[in] mode - pin mode (i.e PULL UP, PULL DOWN, etc)
     * @param[in] intervalMillis - debounce interval
     */
    void attach(pin_t pin, PinMode mode, uint32_t intervalMillis);

    /**
     * @brief Attach to pin, sets pin mode, debounce time, and attach the 
     * callback function
     *
     * @details called to attach the neccessary parameters for debouncing. The 
     * callback function is used so the programmer can write their own custom
     * function for reading a signal if something more complicated that 
     * digitalRead() is needed. If a basic digitalRead() is sufficient to read
     * the pin, just use one of the above attach() functions.
     *
     * @param[in] pin - signal pin to debounce signal
     * @param[in] mode - pin mode (i.e PULL UP, PULL DOWN, etc)
     * @param[in] intervalMillis - debounce interval
     * @param[in] read_cb - callback function to do the reading of the signal
     */
    void attach(pin_t pin, PinMode mode, 
                uint32_t intervalMillis, std::function<int32_t(pin_t)> read_cb);

    /**
     * @brief Sets the debounce interval
     *
     * @details Set the _intervalMillis which is the debounce time
     *
     * @param[in] intervalMillis -  debounce interval time
     */
    void interval(uint32_t intervalMillis);

    /**
     * @brief Update the debouce counters, and check for a stable signal
     *
     * @details updates the bit states for the pin, and check against the 
     * debounce time, if it is stable return that the state changed
     *
     * @return 1 if the state changed, 0 if the state did not change
     */
    bool update();

    /**
     * @brief Get the updated pin state
     * 
     * @return Returns the updated pin state. True for logic high, 
     * false for logic low
     */
    bool read();
    
    /**
     * @brief Updates the pin and returns the debounced state
     *
     * @details Calls update() first, then calls read()
     *
     * @return Returns the updated pin state. True for logic high, 
     * false for logic low
     */
    bool updateRead();

    /**
     * @brief Updates the pin and returns true if the pin is debounced HIGH
     *
     * @details Calls update() and then tests the output of read()
     *
     * @return true if the pin is debounced HIGH, false if not
     */
    bool isHigh();

    /**
     * @brief Updates the pin and returns true if the pin is debounced LOW
     *
     * @details Calls update() and then tests the output of read()
     *
     * @return true if the pin is debounced LOW, false if not
     */
    bool isLow();

protected:
    std::function<int32_t(pin_t)> _read_cb;
    uint32_t _previousMillis;
    uint32_t _intervalMillis;
    uint8_t _state;
    pin_t _pin;
};
