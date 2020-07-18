/*
 * Project button_sequence
 * Description:
 * Author:
 * Date:
 */

#include "Particle.h"
#include "ButtonSequence.h"

#define SWITCH_2 D2

ButtonSequence* button;
int32_t click_count;

// setup() runs once, when the device is first turned on.
void setup() {
  // Put initialization like pinMode and begin functions here.
    Serial.begin(9600); 
    button  = new ButtonSequence(SWITCH_2, INPUT, ActiveLevel::LOW);
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
    click_count = button->check_button();
    if(click_count) {
        Serial.printf("Number of clicks: %d", click_count);
    }
}