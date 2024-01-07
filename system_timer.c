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


#include "types.h"

struct sys_timer {
    uint32_t st_id;
    uint8_t st_owner;
    uint8_t st_free;
    uint8_t st_en;
    uint8_t st_running;
    volatile unsigned long int* st_cmp;
    volatile unsigned long int* st_cnt_lo;
    volatile unsigned long int* st_cnt_hi;
    volatile unsigned long int* st_stat;
    void *st_irq_handler;
    void *st_fiq_handler;
    void *st_err_handler;
    void *st_sync_handler;
};

#include "system_timer.h"


//Defines
#define N_SYSTEM_TIMERS 4


//Identify system timer status match
#define ST_MATCH 1
//Identify system timer status no match
#define ST_NOMATCH 0
//Identify system timer status clear
#define ST_STATUS_CLEAR 1
//Identify system timer status set
#define ST_STATUS_SET 1
//Identify system timer status set
#define ST_STATUS_RESET 0

//Identify system timer free
#define ST_FREE 1
//Identify system timer not free
#define ST_BUSY 0

//Identify system timer without owner
#define ST_NO_OWNER 0
//Identify system timer without id
#define ST_NO_ID 0

//Identify system timer free
#define ST_ENABLED 1
//Identify system timer not free
#define ST_DISABLED 0

//Identify system timer running status
#define ST_RUNNING 1
//Identify system timer stopping status
#define ST_STOPPED 0

//Identify system timer reset compare value
#define ST_CMP_RESET 0


//System Timer (ST) Macros
#define ST_IS_VALID_TIMER_COMPARE(ST_CMP) ST_CMP > 0 && ST_CMP < (2 << 32)
#define ST_IS_TIMER(ST_NR) ST_NR == SYSTEM_TIMER_0 || ST_NR == SYSTEM_TIMER_1 || ST_NR == SYSTEM_TIMER_2 || ST_NR == SYSTEM_TIMER_3
#define ST_IS_TIMER_EXPIRED(ST_CNT, ST_CMP) ST_CNT >= ST_CMP
#define ST_SELECT_TIMER_STATUS(ST_STATUS, ST_NR) ST_STATUS & (1 << ST_NR)
#define ST_IS_TIMER_MATCH(ST_STATUS, ST_NR) ((ST_STATUS & (1 << ST_NR)) >> ST_NR) == ST_MATCH

#define SYSTEM_TIMER_CS     ((volatile unsigned long int*)(MMIO_BASE+0x3000)) //System Timer Control/Status 32 bit
#define SYSTEM_TIMER_LO     ((volatile unsigned long int*)(MMIO_BASE+0x3004)) //System Timer Counter Lower 32 bit
#define SYSTEM_TIMER_HI     ((volatile unsigned long int*)(MMIO_BASE+0x3008)) //System Timer Counter Higher 32 bit
#define SYSTEM_TIMER_C0     ((volatile unsigned long int*)(MMIO_BASE+0x300C)) //System Timer Compare 0 32 bit
#define SYSTEM_TIMER_C1     ((volatile unsigned long int*)(MMIO_BASE+0x3010)) //System Timer Compare 1 32 bit
#define SYSTEM_TIMER_C2     ((volatile unsigned long int*)(MMIO_BASE+0x3014)) //System Timer Compare 2 32 bit
#define SYSTEM_TIMER_C3     ((volatile unsigned long int*)(MMIO_BASE+0x3018)) //System Timer Compare 3 32 bit


sys_timer_t sys_timers[N_SYSTEM_TIMERS];


//BrainOS System Timer internal routines
void sys_timer_boot(){
    for (register uint8_t i = 0; i < N_SYSTEM_TIMERS; i++)
    {
        sys_timers[i].st_en = ST_DISABLED;
        sys_timers[i].st_running = ST_STOPPED;
        sys_timers[i].st_free = ST_FREE;
        sys_timers[i].st_owner = ST_NO_OWNER;
        sys_timers[i].st_id = ST_NO_ID;
        sys_timers[i].st_cmp = SYSTEM_TIMER_C0 + (i * 4);
        sys_timers[i].st_cnt_hi = SYSTEM_TIMER_HI;
        sys_timers[i].st_cnt_lo = SYSTEM_TIMER_LO;
        sys_timers[i].st_stat = SYSTEM_TIMER_CS;
        sys_timers[i].st_irq_handler = NULL;
        sys_timers[i].st_fiq_handler = NULL;
        sys_timers[i].st_err_handler = NULL;
        sys_timers[i].st_sync_handler = NULL;
        sys_timer_stat_clear(i);
        sys_timer_cmp_reset(i);
    }
}

void sys_timer_shutdown(){
    
    for (register uint8_t i = 0; i < N_SYSTEM_TIMERS; i++)
    {
        sys_timers[i].st_en = ST_DISABLED;
        sys_timers[i].st_running = ST_STOPPED;
        sys_timers[i].st_free = ST_FREE;
        sys_timers[i].st_owner = ST_NO_OWNER;
        sys_timers[i].st_id = ST_NO_ID;
        sys_timers[i].st_cmp = NULL;
        sys_timers[i].st_cnt_hi = NULL;
        sys_timers[i].st_cnt_lo = NULL;
        sys_timers[i].st_stat = NULL;
        sys_timers[i].st_irq_handler = NULL;
        sys_timers[i].st_fiq_handler = NULL;
        sys_timers[i].st_err_handler = NULL;
        sys_timers[i].st_sync_handler = NULL;
        sys_timer_stat_clear(i);
        sys_timer_cmp_reset(i);
        sys_timers[i].st_running = ST_STOPPED;
    }

}

//BrainOS System Timer control level API
void sys_timer_acquire(register uint32_t sys_timer, register uint8_t sys_timer_owner, void* sys_timer_irq_handler, void* sys_timer_fiq_handler, void* sys_timer_err_handler, void* sys_timer_sync_handler){
    if(ST_IS_TIMER(sys_timer)){
        if(sys_timers[sys_timer].st_owner == ST_NO_OWNER){
            sys_timers[sys_timer].st_en = ST_ENABLED;
            sys_timers[sys_timer].st_free = ST_BUSY;
            sys_timers[sys_timer].st_owner = sys_timer_owner;
            sys_timers[sys_timer].st_id = sys_timer;
            sys_timers[sys_timer].st_irq_handler = sys_timer_irq_handler;
            sys_timers[sys_timer].st_fiq_handler = sys_timer_fiq_handler;
            sys_timers[sys_timer].st_err_handler = sys_timer_err_handler;
            sys_timers[sys_timer].st_sync_handler = sys_timer_sync_handler;
            sys_timer_stat_clear(sys_timer);
        }
    }
}

void sys_timer_start(register uint32_t sys_timer, register uint8_t sys_timer_owner, uint32_t sys_timer_cmp_val){
    if(ST_IS_TIMER(sys_timer)){
        if(sys_timers[sys_timer].st_owner == sys_timer_owner && sys_timers[sys_timer].st_en){
            sys_timer_stat_clear(sys_timer);
            if(ST_IS_VALID_TIMER_COMPARE(sys_timer_cmp_val)){
                sys_timer_cmp_set(sys_timer, sys_timer_cmp_val);
            }
            sys_timers[sys_timer].st_running = ST_RUNNING;
        }
    }
}

void sys_timer_stop(register uint32_t sys_timer, register uint8_t sys_timer_owner){
    if(ST_IS_TIMER(sys_timer)){
        if(sys_timers[sys_timer].st_owner == sys_timer_owner && sys_timers[sys_timer].st_en){
            sys_timer_cmp_reset(sys_timer);
            sys_timer_stat_clear(sys_timer);
            sys_timers[sys_timer].st_running = ST_STOPPED;
        }
    }
}

void sys_timer_restart(register uint32_t sys_timer, register uint8_t sys_timer_owner){
    if(ST_IS_TIMER(sys_timer)){
        if(sys_timers[sys_timer].st_owner == sys_timer_owner && sys_timers[sys_timer].st_en){
            sys_timer_stat_clear(sys_timer);
        }
    }
}

void sys_timer_release(register uint32_t sys_timer, register uint8_t sys_timer_owner){
    if(ST_IS_TIMER(sys_timer)){
        if(sys_timers[sys_timer].st_owner == sys_timer_owner){
            sys_timer_stat_clear(sys_timer);
            sys_timer_cmp_reset(sys_timer);
            sys_timers[sys_timer].st_en = ST_DISABLED;
            sys_timers[sys_timer].st_free = ST_FREE;
            sys_timers[sys_timer].st_owner = ST_NO_OWNER;
            sys_timers[sys_timer].st_id = ST_NO_ID;
            sys_timers[sys_timer].st_irq_handler = NULL;
            sys_timers[sys_timer].st_fiq_handler = NULL;
            sys_timers[sys_timer].st_err_handler = NULL;
            sys_timers[sys_timer].st_sync_handler = NULL;
        }
    }
}

void sys_timer_irq_handler_set(register uint32_t sys_timer, register uint8_t sys_timer_owner, void* sys_timer_irq_handler){
     if(ST_IS_TIMER(sys_timer)){
        if(sys_timers[sys_timer].st_owner == sys_timer_owner && sys_timers[sys_timer].st_en){
            sys_timers[sys_timer].st_irq_handler = sys_timer_irq_handler;
        }
    }
}

void sys_timer_fiq_handler_set(register uint32_t sys_timer, register uint8_t sys_timer_owner, void* sys_timer_fiq_handler){
     if(ST_IS_TIMER(sys_timer)){
        if(sys_timers[sys_timer].st_owner == sys_timer_owner && sys_timers[sys_timer].st_en){
            sys_timers[sys_timer].st_fiq_handler = sys_timer_fiq_handler;
        }
    }
}

void sys_timer_sync_handler_set(register uint32_t sys_timer, register uint8_t sys_timer_owner, void* sys_timer_sync_handler){
     if(ST_IS_TIMER(sys_timer)){
        if(sys_timers[sys_timer].st_owner == sys_timer_owner && sys_timers[sys_timer].st_en){
            sys_timers[sys_timer].st_sync_handler = sys_timer_sync_handler;
        }
    }
}

void sys_timer_err_handler_set(register uint32_t sys_timer, register uint8_t sys_timer_owner, void* sys_timer_err_handler){
     if(ST_IS_TIMER(sys_timer)){
        if(sys_timers[sys_timer].st_owner == sys_timer_owner && sys_timers[sys_timer].st_en){
            sys_timers[sys_timer].st_err_handler = sys_timer_err_handler;
        }
    }
}

void sys_timer_enable(register uint8_t sys_timer, register uint8_t sys_timer_owner){
    if(ST_IS_TIMER(sys_timer)){
        if(sys_timers[sys_timer].st_owner == (sys_timer_owner)){
            sys_timers[sys_timer].st_en = ST_ENABLED;
        }
    }
}

void sys_timer_disable(register uint8_t sys_timer, register uint8_t sys_timer_owner){
    if(ST_IS_TIMER(sys_timer)){
        if(sys_timers[sys_timer].st_owner == (sys_timer_owner)){
            sys_timers[sys_timer].st_en = ST_DISABLED;
        }
    }
}




//BrainOS System Timer system level API
void sys_timer_stat_set(register uint8_t sys_timer){
    uint32_t sys_timer_status = 0;
    if (ST_IS_TIMER(sys_timer)){
        sys_timer_status = *SYSTEM_TIMER_CS;
        sys_timer_status |= (ST_STATUS_SET << sys_timer);
        *SYSTEM_TIMER_CS = sys_timer_status;
    }
    return;
}

void sys_timer_stat_reset(register uint8_t sys_timer){
    register uint32_t sys_timer_status = 0;
    if (ST_IS_TIMER(sys_timer)){
        sys_timer_status = *SYSTEM_TIMER_CS;
        sys_timer_status |= (ST_STATUS_RESET << sys_timer);
        *SYSTEM_TIMER_CS = sys_timer_status;
    }
    return;
}

void sys_timer_stat_clear(register uint8_t sys_timer){
    register uint32_t sys_timer_status = 0;
    if(ST_IS_TIMER(sys_timer)){
        sys_timer_status = *SYSTEM_TIMER_CS;
        if(ST_IS_TIMER_MATCH(sys_timer_status, sys_timer)){
            sys_timer_status |= (ST_STATUS_CLEAR << sys_timer);
            *SYSTEM_TIMER_CS = sys_timer_status;
        }
    }
    return;
}

uint8_t sys_timer_stat_get(register uint8_t sys_timer){
    register uint32_t sys_timer_status = 0;
    if (ST_IS_TIMER(sys_timer)){
        sys_timer_status = *SYSTEM_TIMER_CS;
        sys_timer_status = ST_SELECT_TIMER_STATUS(sys_timer_status, sys_timer);
        sys_timer_status >>= sys_timer;
    }
    return ((uint8_t)sys_timer_status);
}

uint64_t sys_timer_cnt_get(){
    register uint32_t sys_timer_cnt_lo = 0;
    register uint32_t sys_timer_cnt_hi = 0;
    sys_timer_cnt_hi = *SYSTEM_TIMER_HI;
    sys_timer_cnt_lo = *SYSTEM_TIMER_LO; 
    while(sys_timer_cnt_hi != *SYSTEM_TIMER_HI){
        sys_timer_cnt_hi = *SYSTEM_TIMER_HI;
        sys_timer_cnt_lo = *SYSTEM_TIMER_LO;   
    }
    return((uint64_t) (((unsigned long int) sys_timer_cnt_hi << 32) | sys_timer_cnt_lo));
}

uint32_t sys_timer_cnt_lo_get(){
    register uint32_t sys_timer_cnt_lo = 0;
    sys_timer_cnt_lo = *SYSTEM_TIMER_LO;
    return((uint32_t)sys_timer_cnt_lo);
}

uint32_t sys_timer_cnt_hi_get(){
    register uint32_t sys_timer_cnt_hi = 0;
    sys_timer_cnt_hi = *SYSTEM_TIMER_LO;
    return((uint32_t)sys_timer_cnt_hi);
}

uint32_t sys_timer_cmp_get(register uint8_t sys_timer){
    register uint32_t* sys_timer_cmp = SYSTEM_TIMER_C0;
    if(ST_IS_TIMER(sys_timer)){
        sys_timer_cmp += sys_timer * 4;
    }
    return((uint32_t)*sys_timer_cmp);
}

void sys_timer_cmp_set(register uint8_t sys_timer, uint32_t sys_timer_cmp_val){
    register uint32_t* sys_timer_cmp = SYSTEM_TIMER_C0;
    if(ST_IS_TIMER(sys_timer)){
        sys_timer_cmp += sys_timer * 4;
        *sys_timer_cmp = sys_timer_cmp_val;
    }
    return;
}

void sys_timer_cmp_reset(register uint8_t sys_timer){
    register uint32_t* sys_timer_cmp = SYSTEM_TIMER_C0;
    if(ST_IS_TIMER(sys_timer)){
        sys_timer_cmp += sys_timer * 4;
        *sys_timer_cmp = ST_CMP_RESET;
    }
    return;
}

int sys_timer_is_expired(register uint8_t sys_timer){
    register uint32_t sys_timer_cmp = sys_timer_cmp_get(sys_timer);
    register uint32_t sys_timer_cnt_hi = sys_timer_cnt_hi_get();
    if(ST_IS_TIMER(sys_timer)){
        return(ST_IS_TIMER_EXPIRED(sys_timer_cnt_hi, sys_timer_cmp));
    }
    return 0;
}

int sys_timer_is_match(register uint8_t sys_timer){
    register uint32_t sys_timer_status = 0;
    if(ST_IS_TIMER(sys_timer)){
        sys_timer_status = *SYSTEM_TIMER_CS;
        return(ST_IS_TIMER_MATCH(sys_timer_status, sys_timer));
    }
    return 0;
}


/*

int main(int argc, char* argv[]){
    register uint8_t sys_timer_0 = SYSTEM_TIMER_0;
    register uint8_t sys_timer_1 = SYSTEM_TIMER_1;
    register uint8_t sys_timer_2 = SYSTEM_TIMER_2;
    register uint8_t sys_timer_3 = SYSTEM_TIMER_3;
    register uint32_t i;
    register uint8_t st_owner = 0;
    uint32_t st_comparer[N_SYSTEM_TIMERS];

    sys_timer_boot();

    for (i = 0; i < N_SYSTEM_TIMERS; i++)
    {
        sys_timer_acquire(i, st_owner, NULL, NULL, NULL, NULL);
        st_comparer[i] = 1024 * (i + 1);
    }

    for (i = 0; i < N_SYSTEM_TIMERS; i++)
    {
        sys_timer_start(i, st_owner, st_comparer[i]);
    }

    for (i = 0; i < N_SYSTEM_TIMERS; i++)
    {
        sys_timer_stop(i, st_owner);
    }

    for (i = 0; i < N_SYSTEM_TIMERS; i++)
    {
        sys_timer_restart(i, st_owner);
    }

    for (i = 0; i < N_SYSTEM_TIMERS; i++)
    {
        sys_timer_stop(i, st_owner);
    }

    for (i = 0; i < N_SYSTEM_TIMERS; i++)
    {
        sys_timer_release(i, st_owner);
    }

    sys_timer_shutdown();

    
}

*/

