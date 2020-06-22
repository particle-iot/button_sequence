/** 
 * @file ButtonSequence.cpp
 * @author Ed Ablan
 * @version 1.0
 * @date 06/16/2020
 *
 * @brief A single instance of this class debounces a single button. Returns 
 * number of button presses, whether those were short presses, or short presses 
 * terminated by a long press, or just a long press. Long presses can terminate
 * the sequence, or n short presses with a defined settle period of no further 
 * presses.
 *
 * @details This implements the button debounce to debounce and count button 
 * presses that are terminated by either n short presses and a long hold, or 
 * just n short presses. Read the header file for more details on the interface
 *
 * @copyright Copyright (c) 2020 Particle Industries, Inc.  All rights reserved.
 */

#include "ButtonSequence.h"
#include "spark_wiring_ticks.h"

#define SHORT_CLICK_TIMEOUT_MS 500

ButtonSequence::ButtonSequence(pin_t button_pin, PinMode mode, bool active_low, 
        system_tick_t debounce_interval, system_tick_t long_duration_interval) :
        _long_duration_interval(long_duration_interval), 
        _active_low(active_low)
{
    debounce_button.attach(button_pin, mode, debounce_interval);
}

ButtonSequence::ButtonSequence(std::function<int32_t(pin_t)> read_cb, 
        pin_t button_pin, PinMode mode, bool active_low, 
        system_tick_t debounce_interval, system_tick_t long_duration_interval) :
        _long_duration_interval(long_duration_interval),
        _active_low(active_low)
{
    debounce_button.attach(button_pin, mode, debounce_interval, read_cb);
}

int32_t ButtonSequence::check_button()
{
    static system_tick_t long_press_timeout = 0, short_depress_timeout = 0;
    static bool pressed = false;
    static int32_t click_count = 0;
    int32_t returnval = 0;

    if(debounce_button.update()) {
        auto switch_state = debounce_button.read();
        pressed = (_active_low) ?  !switch_state : switch_state;

        if(pressed){click_count++;}

        if(pressed) {long_press_timeout = millis() + _long_duration_interval;}
        else {short_depress_timeout = millis() + SHORT_CLICK_TIMEOUT_MS;}   
    }
    //state didn't change, check sequence termination
    else {
        //only if a sequence is in progress
        if(click_count) {
            if(pressed) {
                //check if long press was used to terminate the sequence
                if(millis() > long_press_timeout) {
                    returnval = (-1*click_count);
                    click_count = 0;
                }
            }
            else {
                //check if short depress terminates the sequence
                if(millis() > short_depress_timeout) {
                    returnval = click_count;
                    click_count = 0;
                }
            }
        }
    }

    return returnval;
}

void ButtonSequence::set_long_interval(system_tick_t long_duration_interval)
{
    _long_duration_interval = long_duration_interval;
}

system_tick_t ButtonSequence::get_long_interval()
{
    return _long_duration_interval;
}