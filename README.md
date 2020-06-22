##**BUTTON SEQUENCE**
------------------------------
​
###**SUMMARY**
This module reads short and long button sequences. Functions called here setup the debounce, and read of sequences that are either short button presses or long button presses. Long button presses terminate a sequence, short button depresses of greater than 500ms terminate a sequence
​
###**DEPENDENCIES**
Must include the Debounce class. This is what actually does the debounce
​
###**DESCRIPTION**
User passes pin, pin mode, active low, debounce time, long click time, and possibly a callback function to do a custom implementation of reading a pin. The check_button() returns the number of short clicks which are terminated after 500ms of the button being depressed, and the positive number of click counts is returned. The check_button() will also return if the sequence was terminated by a long click by returning the negative of the click count. If no button clicks, or a button sequence is in progress returns 0
​
####EXAMPLE
​look at the test.cpp file to read more
​
```cpp
#include "Particle.h"
#include "ButtonSequence.h"

#define SWITCH_2 D2

ButtonSequence* button;
int32_t click_count;

// setup() runs once, when the device is first turned on.
void setup() {
  // Put initialization like pinMode and begin functions here.
    Serial.begin(9600); 
    button  = new ButtonSequence(SWITCH_2, INPUT, true);
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
    click_count = button->check_button();
    if(click_count) {
        Serial.printf("Number of clicks: %d", click_count);
    }
}
```
​
###**DETAILS**
Uses the debounce.h update() function to know if the state has changed, when it does a the state is checked to see if it is pressed or depressed. If pressed increment the click_count, and setup the long duration timeout. If not pressed (depressed) setup the short click termination timeout. Continued calls to check_button() check to see if one of the termination conditions occur, or keeps incrementing the click_count

###**WARNINGS**
A single instance of this class will debounce one button only.  Multiple buttons will require multiple instances, one instance per button
