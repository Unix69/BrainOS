/*
 * Copyright (C) 2022 (Unix69@github)
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */


#include "timer.h"



void sys_timer_boot();
void sys_timer_init();

//set and get system timer control/status
void sys_timer_stat_set(uint8_t sys_timer, uint32_t sys_timer_status);

void sys_timer_stat_clear(uint8_t sys_timer){
    unsigned int sys_timer_status = 0;

    
    sys_timer_status = *SYSTEM_TIMER_CS;
    if (IS_TIMER(sys_timer)){
        sys_timer_status &= sys_timer;
        if(sys_timer_status == ST_MATCH){
            sys_timer_status |= (1 << sys_timer);
            *SYSTEM_TIMER_CS = sys_timer_status; 
        }
    }
    
    return (sys_timer_status);
}


uint32_t sys_timer_stat_get(uint8_t sys_timer){
    unsigned int sys_timer_status = 0;
    
    sys_timer_status = *SYSTEM_TIMER_CS;
    if (IS_TIMER(sys_timer)){
        sys_timer_status &= sys_timer;
    }

    return (sys_timer_status);

}

//get system timer counter value
uint64_t sys_timer_cnt_get(uint8_t sys_timer);
uint32_t sys_timer_cnt_lo_get(uint8_t sys_timer);
uint32_t sys_timer_cnt_hi_get(uint8_t sys_timer);

//set and get system timer compare value
uint32_t sys_timer_cmp_get(uint8_t sys_timer);
void sys_timer_cmp_set(uint8_t sys_timer, uint32_t sys_timer_cmp_val);

