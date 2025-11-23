#include "timer.h"
#include <avr/io.h>
#include <avr/interrupt.h>

static volatile uint32_t gMsTicks = 0;

void TimerInit(void){
    TCCR1A = 0;
    TCCR1B = (1 << WGM12);
    TCCR0A = 0;
    TCCR0B= (1 << CS01);

    OCR1A = 249;

    TIMSK1 |= (1 << OCIE1A);

    TCCR1B |= (1 << CS11) | (1 << CS10);

    sei();
}

ISR(TIMER1_COMPA_vect){
    gMsTicks++;
}

uint32_t Milis(){
    uint32_t copy;

    uint8_t sreg = SREG;
    cli();
    copy = gMsTicks;
    SREG = sreg;

    return copy;
}

void DelayMs(uint16_t ms){
    uint32_t start = Milis();
    while ((Milis() - start) < ms){
        ;; //busy wait
    }
}
void DelayUs(uint16_t us){
    uint8_t start = TCNT0;
    uint16_t ticks = us * 2;

    while((uint8_t)(TCNT0 - start) < ticks){
        ;; //busy
    }
}