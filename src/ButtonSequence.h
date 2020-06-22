/** 
 * @file ButtonSequence.h
 * @author Ed Ablan
 * @version 1.0
 * @date 06/16/2020
 *
 * @brief Interface for button sequence. Functions called here setup the 
 * debounce, and read of sequences that are either short button presses or 
 * long button presses. Long button presses terminate a sequence, short button
 * depresses of greater than 500ms terminate a sequence
 *
 * @details A single instance of this class will debounce one button only. 
 * Multiple buttons will require multiple instances. Read funtion comments and 
 * accompanying README.md for more details
 *
 * @copyright Copyright (c) 2020 Particle Industries, Inc.  All rights reserved.
 */
#pragma once

#include "Debounce.h"

#define DEFAULT_DEBOUNCE_MS 50
#define DEFAULT_LONG_CLICK_MS 5000

class ButtonSequence {
public:

    /**
     * @brief Basic constructor
     *
     * @details Create an instance of ButtonSequence. Calls the 
     * debounce.attach() with debounce interval, pin, and pin mode
     *
     * @param[in] button_pin - pin to debounce
     * @param[in] mode - mode of the pin (i.e INPUT, PULLUP, PULLDOWN)
     * @param[in] active_low - pin logic high on or logic low on
     * @param[in] debounce_interval - milli sec debounce time
     * @param[in] long_duration_interval - milli sec long click time
     */
    ButtonSequence(pin_t button_pin, PinMode mode, bool active_low = false, 
                system_tick_t debounce_interval = DEFAULT_DEBOUNCE_MS, 
                system_tick_t long_duration_interval = DEFAULT_LONG_CLICK_MS);

    /**
     * @brief Full constructor with callback to read a signal to debounce
     *
     * @details Create an instance of ButtonSequence. Calls the 
     * debounce.attach() with debounce interval, pin, pin mode, and read_cb
     *
     * @param[in] read_cb - callback to do custom read of signal
     * @param[in] button_pin - pin to debounce
     * @param[in] mode - mode of the pin (i.e INPUT, PULLUP, PULLDOWN)
     * @param[in] active_low - pin logic high on or logic low on
     * @param[in] debounce_interval - milli sec debounce time
     * @param[in] long_duration_interval - milli sec long click time
     */
    ButtonSequence(std::function<int32_t(pin_t)> read_cb, pin_t button_pin, 
                PinMode mode, bool active_low = false,
                system_tick_t debounce_interval = DEFAULT_DEBOUNCE_MS, 
                system_tick_t long_duration_interval = DEFAULT_LONG_CLICK_MS);

    /**
     * @brief Update the debounce counters and check if that state was 
     * debounced. If so determine if it was a press or depress, increment the 
     * click count if pressed, and setup the appropriate sequence termination
     * timers
     *
     * @details Function calls debounce.update(), determines if there is a 
     * debounced state change. Determines pressed or depressed state, and checks
     * to see if the sequence has been terminated due to a long click, or short
     * click finished
     *
     * @return 0 if no button click or sequence in progress, positive click 
     * count if short click sequence detected, negative click count if long 
     * click terminates the short click sequence or a single long click detected
     */
    int32_t check_button();

    /**
     * @brief Set the _long_duration_interval
     *
     * @details Sets the long duration interval used to determine if a long
     * click occured
     *
     * @param[in] long_duration_interval - milli secs for interval of long click
     */
    void set_long_interval(system_tick_t long_duration_interval);

    /**
     * @brief Get the _long_duration_interval
     *
     * @details Get the milli second interval for a long click
     *
     * @return the _long_duration_interval in milliseconds
     */
    uint32_t get_long_interval();

private:

    Debounce  debounce_button;
    int32_t button_click_count;
    system_tick_t _long_duration_interval;
    bool _active_low;
};