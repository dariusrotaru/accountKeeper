#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

/**
 * @brief Initializes Timer1 for 1ms ticks and Timer0 for 0.5 us ticks.
 * 
 */
void TimerInit(void);
/**
 * @brief 
 * 
 * @return uint32_t 
 */
uint32_t Milis(void);
void DelayMs(uint16_t ms);
void DelayUs(uint16_t us);

#endif