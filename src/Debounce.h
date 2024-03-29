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
     * @brief Attach the callback function, and interval in milliseconds. This
     * is intended for the callback function to return the signal state.
     *
     * @details called to attach the neccessary parameters for debouncing. The 
     * callback function is used so the programmer can write their own custom
     * function for reading a signal if something more complicated that 
     * digitalRead() is needed. If a basic digitalRead() is sufficient to read
     * the pin, just use one of the above attach() functions.
     *
     * @param[in] read_cb - callback function to do the reading of the signal
     * @param[in] intervalMillis - debounce interval
     */
    void attach(std::function<int32_t(void)> read_cb, uint32_t intervalMillis);

    /**
     * @brief Sets the debounce interval
     *
     * @details Set the _intervalMillis which is the debounce time
     *
     * @param[in] intervalMillis -  debounce interval time
     */
    void interval(uint32_t intervalMillis);

    /**
     * @brief Update the debouce counters, and check for a stable signal. This
     * version of update will read the digial pin or use the callback to get
     * the signal value
     *
     * @details updates the bit states for the pin, and check against the 
     * debounce time, if it is stable return that the state changed
     *
     * @return 1 if the state changed, 0 if the state did not change
     */
    bool update();

    /**
     * @brief Pass the signal value, Update the debouce counters, and check 
     * for a stable signal. This version of update has to have the value passed
     * to it
     *
     * @details updates the bit states for the signal, and check against the 
     * debounce time, if it is stable return that the state changed
     * 
     * @param[in] value - signal value that will be debounced
     *
     * @return 1 if the state changed, 0 if the state did not change
     */
    bool update(bool value);

    /**
     * @brief Get the updated signal state
     * 
     * @return Returns the updated signal state. True for logic high, 
     * false for logic low
     */
    bool read();
    
    /**
     * @brief Updates the state and returns the debounced state
     *
     * @details Calls update() first, then calls read()
     *
     * @return Returns the updated signal state. True for logic high, 
     * false for logic low
     */
    bool updateRead();

    /**
     * @brief Updates the state and returns true if the signal is debounced HIGH
     *
     * @details Calls update() and then tests the output of read()
     *
     * @return true if the signal is debounced HIGH, false if not
     */
    bool isHigh();

    /**
     * @brief Updates the state and returns true if the signal is debounced LOW
     *
     * @details Calls update() and then tests the output of read()
     *
     * @return true if the signal is debounced LOW, false if not
     */
    bool isLow();

private:
    /**
     * @brief Starts up the debounce counters and time
     *
     * @details Sets the state to zero, starts the timer, and sets up the 
     * counters
     */
    void start();


protected:
    std::function<int32_t(void)> _read_cb;
    uint32_t _previousMillis;
    uint32_t _intervalMillis;
    uint8_t _state;
    pin_t _pin;
};
