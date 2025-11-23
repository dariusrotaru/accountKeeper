#include "button.h"
#include <avr/io.h>

#define BTN_L (1 << PC0)
#define BTN_R (1 << PC1)

#define LOW 0
#define HIGH 1


void BtnInit(void){
    DDRC &= ~(BTN_L | BTN_R);
}
uint8_t BtnLGetState(void){
    static uint8_t lastState    = LOW;
    uint8_t currentState;
    currentState = (PINC & BTN_L) ? HIGH : LOW;
    if(currentState == HIGH && lastState == LOW){
        return 1; // btn pressed
    }
    lastState = currentState;
    return 0; //btn not pressed
}
uint8_t BtnRGetState(void){
    static uint8_t lastState    = LOW;
    uint8_t currentState;
    currentState = (PINC & BTN_R) ? HIGH : LOW;
    if(currentState == HIGH && lastState == LOW){
        return 1; // btn pressed
    }
    lastState = currentState;
    return 0; //btn not pressed
}

