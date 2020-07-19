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

ButtonSequence::ButtonSequence(pin_t button_pin, PinMode mode, 
        ActiveLevel active_level, system_tick_t debounce_interval, 
        system_tick_t long_duration_interval) :
        _long_duration_interval(long_duration_interval)
{
    _active_low = (active_level == ActiveLevel::LOW) ? true : false;
    debounce_button.attach(button_pin, mode, debounce_interval);
}

ButtonSequence::ButtonSequence(std::function<int32_t(void)> read_cb, 
                    ActiveLevel active_level, system_tick_t debounce_interval, 
                    system_tick_t long_duration_interval) :
        _long_duration_interval(long_duration_interval)
{
    _active_low = (active_level == ActiveLevel::LOW) ? true : false;
    debounce_button.attach(read_cb, debounce_interval);
}

int32_t ButtonSequence::update_sequence(bool state_changed)
{
    static system_tick_t long_press_timeout = 0, short_depress_timeout = 0,
    start_time = 0;
    static bool pressed = false;
    static int32_t click_count = 0;
    int32_t returnval = 0;

    if(state_changed) {
        auto switch_state = debounce_button.read();
        pressed = (_active_low) ?  !switch_state : switch_state;

        if(pressed){click_count++;}

        start_time = millis();
        if(pressed) {long_press_timeout = _long_duration_interval;}
        else {short_depress_timeout = SHORT_CLICK_TIMEOUT_MS;}   
    }
    //state didn't change, check sequence termination
    else {
        //only if a sequence is in progress
        if(click_count) {
            if(pressed) {
                //check if long press was used to terminate the sequence
                if(millis() - start_time > long_press_timeout) {
                    returnval = (-1*click_count);
                    click_count = 0;
                }
            }
            else {
                //check if short depress terminates the sequence
                if(millis() - start_time > short_depress_timeout) {
                    returnval = click_count;
                    click_count = 0;
                }
            }
        }
    }

    return returnval;
}

int32_t ButtonSequence::check_button()
{
    bool state_changed = debounce_button.update();
    return update_sequence(state_changed);
}

int32_t ButtonSequence::check_button(bool current_state)
{
    bool state_changed = debounce_button.update(current_state);
    return update_sequence(state_changed);
}

void ButtonSequence::set_long_interval(system_tick_t long_duration_interval)
{
    _long_duration_interval = long_duration_interval;
}

system_tick_t ButtonSequence::get_long_interval()
{
    return _long_duration_interval;
}