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

struct timer {
    uint32_t t_id;
    uint8_t t_owner;
    uint8_t t_free;
    uint8_t t_en;
    uint8_t t_running;
    volatile unsigned long int* t_load;
    volatile unsigned long int* t_val;
    volatile unsigned long int* t_reload;
    volatile unsigned long int* t_ctrl;
    volatile unsigned long int* t_mask_irq;
    volatile unsigned long int* t_raw_irq;
    volatile unsigned long int* t_clear_irq;
    volatile unsigned long int* t_prediv;
    volatile unsigned long int* t_free_run_cnt;
    void *t_irq_handler;
    void *t_fiq_handler;
    void *t_err_handler;
    void *t_sync_handler;
};

#include "timer.h"



//Defines
#define N_TIMERS 1


//Identify timer status match
#define T_MATCH 1
//Identify timer status no match
#define T_NOMATCH 0



//Identify timer free
#define T_FREE 1
//Identify timer not free
#define T_BUSY 0

//Identify timer without owner
#define T_NO_OWNER 0
//Identify timer without id
#define T_NO_ID 0

//Identify timer free
#define T_ENABLED 1
//Identify timer not free
#define T_DISABLED 0

//Identify timer running status
#define T_RUNNING 1
//Identify timer stopping status
#define T_STOPPED 0

//Identify timer no prescale word
#define T_NOPRESCALE 0
//Identify timer prescale low word
#define T_PRESCALE_LOW 1
//Identify timer prescale high word 
#define T_PRESCALE_HIGH 2


//Identify timer interrupt disable word
#define T_INTERRUPT_DIS 0
//Identify timer interrupt enable word
#define T_INTERRUPT_EN 1


//Identify timer free enable word
#define T_FREE_RUN_EN 1
//Identify timer free run enable word
#define T_FREE_RUN_DIS 0



//Identify timer load clear
#define T_LOAD_CLEAR 1
//Identify timer load reset
#define T_LOAD_RESET 0

//Identify timer value clear
#define T_CTRL_CLEAR 1
//Identify timer value set
#define T_CTRL_RESET 0

//Identify timer reset compare value
#define T_PREDIV_CLEAR 0
//Identify timer reset compare value
#define T_PREDIV_RESET 1

//Identify timer reset compare value
#define T_FREE_RUN_CLEAR 0
//Identify timer reset compare value
#define T_FREE_RUN_RESET 1

//Identify timer clear irq
#define T_IRQ_CLEAR 0
//Identify timer reset irq
#define T_IRQ_RESET 1



//Identify timer width in 23 bits
#define T_BITS_HIGH 1
//Identify timer width in 16 bits
#define T_BITS_LOW 0



//timer (T) Macros
#define T_IS_VALID_TIMER_LOAD(T_LOAD) T_LOAD >= 0 && T_LOAD < (2 << 32)
#define T_IS_VALID_TIMER_CTRL(T_CTRL) T_CTRL >= 0 && T_CTRL < (2 << 32)
#define T_IS_VALID_TIMER_FREE_RUN (T_FREERUN) T_FREERUN >= 0 && T_FREERUN < (2 << 32)
#define T_IS_VALID_TIMER_IRQ_CLEAR_(T_IRQ_CLEAR_) T_IRQ_CLEAR_ >= 0 && T_IRQ_CLEAR_ < (2 << 32)
#define T_GET_TIMER_BITS_MODE(T_BITS) T_BITS == 23 ? T_BITS_HIGH : T_BITS_LOW

#define T_IS_TIMER(T_NR) T_NR == TIMER_0 || T_NR == TIMER_1 || T_NR == TIMER_2 || T_NR == TIMER_3
#define T_IS_TIMER_EXPIRED(T_CNT, T_CMP) T_CNT >= T_CMP
//#define T_SELECT_TIMER_STATUS(T_STATUS, T_NR) T_STATUS & (1 << T_NR)
#define T_IS_TIMER_MATCH(T_STATUS, T_NR) ((T_STATUS & (1 << T_NR)) >> T_NR) == T_MATCH

#define TIMER_LOAD          ((volatile unsigned long int*)(MMIO_BASE+0xb400)) //load register sets the time for the timer to count down
#define TIMER_VALUE         ((volatile unsigned long int*)(MMIO_BASE+0xb404)) //holds the current timer value and is counted down when the counter is running
#define TIMER_CONTROL       ((volatile unsigned long int*)(MMIO_BASE+0xb408)) //the standard SP804 timer control register consist of 8 bits but in the BCM implementation there are more control bits for the extra features.
#define TIMER_CLEAR_IRQ     ((volatile unsigned long int*)(MMIO_BASE+0xb40C)) //the timer IRQ clear register is write only
#define TIMER_RAW_IRQ       ((volatile unsigned long int*)(MMIO_BASE+0xb410)) //raw IRQ register is a read-only register. It shows the status of the interrupt pending bit
#define TIMER_MASK_IRQ      ((volatile unsigned long int*)(MMIO_BASE+0xb414)) //masked IRQ register is a read-only register. It shows the status of the interrupt signal. It is simply a logical AND of the interrupt pending bit and the interrupt enable bit
#define TIMER_RELOAD        ((volatile unsigned long int*)(MMIO_BASE+0xb418)) //is a copy of the timer load register. The difference is that a write to this register does not trigger an immediate reload of the timer value register.
#define TIMER_PREDIV        ((volatile unsigned long int*)(MMIO_BASE+0xb41C)) //The pre-divider register is 10 bits wide and can be written or read from. This register has been added as the SP804 expects a 1MHz clock which we do not have.
#define TIMER_FREE_COUNT    ((volatile unsigned long int*)(MMIO_BASE+0xb420)) //free running counter is not present in the SP804. The free running counter is a 32 bits wide read only register. The register is enabled by setting bit 9 of the Timer control register

timer_t timer;


//BrainOS timer internal routines
void timer_boot(){
    if(timer != NULL){
        timer->t_en = T_DISABLED;
        timer->t_running = T_STOPPED;
        timer->t_free = T_FREE;
        timer->t_owner = T_NO_OWNER;
        timer->t_id = T_NO_ID;
        timer->t_load = TIMER_LOAD;
        timer->t_val = TIMER_VALUE;
        timer->t_ctrl = TIMER_CONTROL;
        timer->t_clear_irq = TIMER_CLEAR_IRQ;
        timer->t_mask_irq = TIMER_MASK_IRQ;
        timer->t_raw_irq = TIMER_RAW_IRQ;
        timer->t_reload = TIMER_RELOAD;
        timer->t_prediv = TIMER_PREDIV;
        timer->t_free_run_cnt = TIMER_FREE_COUNT;
        timer->t_irq_handler = NULL;
        timer->t_fiq_handler = NULL;
        timer->t_err_handler = NULL;
        timer->t_sync_handler = NULL;
    }
}

void timer_shutdown(){
    if(timer != NULL){
        timer->t_en = T_DISABLED;
        timer->t_running = T_STOPPED;
        timer->t_free = T_FREE;
        timer->t_owner = T_NO_OWNER;
        timer->t_id = T_NO_ID;
        timer->t_load = NULL;
        timer->t_val = NULL;
        timer->t_ctrl = NULL;
        timer->t_clear_irq = NULL;
        timer->t_mask_irq = NULL;
        timer->t_raw_irq = NULL;
        timer->t_reload = NULL;
        timer->t_prediv = NULL;
        timer->t_free_run_cnt = NULL;
        timer->t_irq_handler = NULL;
        timer->t_fiq_handler = NULL;
        timer->t_err_handler = NULL;
        timer->t_sync_handler = NULL;
    }
}

//BrainOS timer control level API
void timer_acquire(register uint8_t timer_owner, void* timer_irq_handler, void* timer_fiq_handler, void* timer_err_handler, void* timer_sync_handler){
    register uint8_t timer_irq_enabled = 0;
    if(timer != NULL){
        if(timer->t_owner == T_NO_OWNER){
            timer->t_owner = timer_owner;
            timer_irq_enabled = timer_ctrl_irq_enable_get();
            timer_ctrl_irq_disable();
            timer_irq_clear();
            timer_load_reset();
            timer->t_free = T_BUSY;
            timer->t_id = timer;
            timer->t_irq_handler = timer_irq_handler;
            timer->t_fiq_handler = timer_fiq_handler;
            timer->t_err_handler = timer_err_handler;
            timer->t_sync_handler = timer_sync_handler;
            timer_ctrl_irq_set(timer_irq_enabled);
            timer_irq_clear_set()
        }
    }
}

void timer_start(register uint8_t timer_owner, register uint32_t timer_load_val, register uint8_t timer_irq_en, register uint8_t timer_freerun_en, register uint32_t timer_prescale_val, register uint8_t timer_prescale_bits, register uint8_t timer_prediv_val){
    if(timer != NULL){
        if(timer->t_owner == timer_owner){
            timer_load_set(timer_load_val);
            timer_ctrl_irq_enable_set(timer_irq_en);
            timer_ctrl_freerun_enable_set(timer_freerun_en);
            timer_free_run_set(timer_freerun_val);
            timer_ctrl_prescale_bits_set(timer_prescale_bits);
            timer_ctrl_prescale_val_set(timer_prescale_val);
            timer_ctrl_bits_set(timer_bits);
            timer_prediv_set(timer_prediv_val);
            timer->t_running = T_RUNNING;
            timer_ctrl_enable();
        }

    }
}

void timer_stop(register uint8_t timer_owner){
    if(timer != NULL){
        if(timer->t_owner == timer_owner){
            if(timer->t_en && timer->t_running){
                timer_ctrl_disable();
                timer->t_running = T_STOPPED;
                timer->t_en = T_DISABLED;
            }
        }
    }
}

void timer_restart(register uint8_t timer_owner){
    register uint32_t timer_val = 0;
    if(timer != NULL){
        if(timer->t_owner == timer_owner){
            if(!timer->t_en || !timer->t_running){
                timer_val = timer_val_get();
                timer_reload_set(timer_val);
                timer->t_running = T_RUNNING;
                timer->t_en = T_ENABLED;
                timer_ctrl_enable();
            }
        }
    }
}

void timer_release(register uint8_t timer_owner){
    if(timer != NULL){
        if(timer->t_owner == timer_owner){
            timer->t_en = T_DISABLED;
            timer->t_free = T_FREE;
            timer->t_owner = T_NO_OWNER;
            timer->t_id = T_NO_ID;
            timer->t_irq_handler = NULL;
            timer->t_fiq_handler = NULL;
            timer->t_err_handler = NULL;
            timer->t_sync_handler = NULL;

            timer_irq_clear();
            timer_ctrl_irq_disable();
            timer_ctrl_disable();
            timer_ctrl_freerun_disable();

        }
    }
}

void timer_irq_handler_set(register uint8_t timer_owner, void* timer_irq_handler){
     if(timer != NULL){
        if(timer->t_owner == timer_owner && timer->t_en){
            timer->t_irq_handler = timer_irq_handler;
        }
    }
}

void timer_fiq_handler_set(register uint8_t timer_owner, void* timer_fiq_handler){
     if(timer != NULL){
        if(timer->t_owner == timer_owner && timer->t_en){
            timer->t_fiq_handler = timer_fiq_handler;
        }
    }
}

void timer_sync_handler_set(register uint8_t timer_owner, void* timer_sync_handler){
     if(timer != NULL){
        if(timer->t_owner == timer_owner && timer->t_en){
            timer->t_sync_handler = timer_sync_handler;
        }
    }
}

void timer_err_handler_set(register uint8_t timer_owner, void* timer_err_handler){
     if(timer != NULL){
        if(timer->t_owner == timer_owner && timer->t_en){
            timer->t_err_handler = timer_err_handler;
        }
    }
}





void timer_ctrl_enable_halt_on_debug(){
    register uint8_t timer_ctrl = 0;
    timer_ctrl = timer_ctrl_get();
    timer_ctrl |= (T_HALT_ON_DEBUG_EN << 7);
    timer_ctrl_set(timer_ctrl);
    return;
}

void timer_ctrl_disable_halt_on_debug(){
    register uint8_t timer_ctrl = 0;
    timer_ctrl = timer_ctrl_get();
    timer_ctrl |= (T_HALT_ON_DEBUG_DIS << 7);
    timer_ctrl_set(timer_ctrl);
    return;
}

void timer_ctrl_enable(){
    register uint8_t timer_ctrl = 0;
    timer_ctrl = timer_ctrl_get();
    timer_ctrl |= (T_ENABLED << 6);
    timer_ctrl_set(timer_ctrl);
    return;
}

void timer_ctrl_disable(){
    register uint8_t timer_ctrl = 0;
    timer_ctrl = timer_ctrl_get();
    timer_ctrl |= (T_DISABLED << 6);
    timer_ctrl_set(timer_ctrl);
    return;
}

void timer_ctrl_freerun_enable(){
    register uint8_t timer_ctrl = 0;
    timer_ctrl = timer_ctrl_get();
    timer_ctrl |= (T_FREERUN_ENABLED << 9);
    timer_ctrl_set(timer_ctrl);
    return;
}

void timer_ctrl_freerun_disable(){
    register uint8_t timer_ctrl = 0;
    timer_ctrl = timer_ctrl_get();
    timer_ctrl |= (T_FREERUN_DISABLED << 9);
    timer_ctrl_set(timer_ctrl);
    return;
}

uint8_t timer_ctrl_freerun_enable_get(){
    register uint8_t timer_freerun = 0;
    timer_freerun = (uint8_t) ((timer_ctrl_get() >> 9) & 1);
    return(timer_freerun);
}

void timer_ctrl_freerun_enable_set(register uint8_t timer_freerun){
    register uint8_t timer_ctrl = 0;
    if (T_IS_VALID_TIMER_FREERUN_EN(timer_freerun)){
        timer_ctrl = timer_ctrl_get();
        timer_ctrl |= (timer_freerun << 9);
        timer_ctrl_set(timer_ctrl);
    }
    return;
}

void timer_ctrl_bits_set(register uint8_t timer_bits){
    register uint8_t timer_ctrl = 0;
    if (T_IS_VALID_TIMER_BITS(timer_bits)){
        timer_ctrl = timer_ctrl_get();
        timer_ctrl |= (timer_bits << 1);
        timer_ctrl_set(timer_ctrl);
    }
    return;
}

uint8_t timer_ctrl_bits_get(){
    register uint8_t timer_bits = 0;
    timer_bits = (uint8_t) ((timer_ctrl_get()>>1) & 1);
    return(timer_bits);
}

void timer_ctrl_prescale_bits_set(register uint8_t timer_prescale){
    register uint8_t timer_ctrl = 0;
    if (T_IS_VALID_TIMER_PRESCALE_BITS(timer_prescale)){
        timer_ctrl = timer_ctrl_get();
        timer_ctrl |= (timer_prescale << 2);
        timer_ctrl_set(timer_ctrl);
    }
    return;
}

uint8_t timer_ctrl_prescale_bits_get(){
    register uint8_t timer_prescale_bits = 0;
    timer_prescale_bits = (uint8_t) ((timer_ctrl_get()>>2) & 3);
    return(timer_prescale_bits);
}

void timer_ctrl_irq_enable(){
    register uint8_t timer_ctrl = 0;
    timer_ctrl = timer_ctrl_get();
    timer_ctrl |= (T_INTERRUPT_EN << 4);
    timer_ctrl_set(timer_ctrl);
    return;
}

void timer_ctrl_irq_disable(){
    register uint8_t timer_ctrl = 0;
    timer_ctrl = timer_ctrl_get();
    timer_ctrl |= (T_INTERRUPT_DIS << 4);
    timer_ctrl_set(timer_ctrl);
    return;
}

uint8_t timer_ctrl_irq_enable_get(){
    register uint8_t timer_irq_enable = 0;
    timer_irq_enable = (uint8_t) ((timer_ctrl_get()>>4) & 1);
    return(timer_irq_enable);
}

void timer_ctrl_irq_enable_set(register uint8_t timer_irq_enable){
    register uint8_t timer_ctrl = 0;
    if (T_IS_VALID_TIMER_IRQ_EN(timer_irq_enable)){
        timer_ctrl = timer_ctrl_get();
        timer_ctrl |= (timer_irq_enable << 4);
        timer_ctrl_set(timer_ctrl);
    }
    return;
}



void timer_ctrl_prescale_val_set(register uint8_t timer_prescale_val){
    register uint8_t timer_ctrl = 0;
    if (T_IS_VALID_TIMER_PRESCALE_VAL(timer_prescale_val)){
        timer_ctrl = timer_ctrl_get();
        timer_ctrl |= (timer_bits << 15);
        timer_ctrl_set(timer_ctrl);
    }
    return;
}

uint8_t timer_ctrl_prescale_val_get(){
    register uint8_t timer_prescale_val = 0;
    timer_prescale_val = (uint8_t) ((timer_ctrl_get()>>15) & 255);
    return(timer_prescale_val);
}

uint8_t timer_irq_status_get(){
    register uint8_t timer_irq_status = 0;
    timer_irq_status = (uint8_t) (timer_irq_raw_get() & 1);
    return (timer_irq_status);
}

uint8_t timer_irq_masked_get(){
    register uint8_t timer_irq_masked = 0;
    timer_irq_masked = (uint8_t) (timer_irq_mask_get() & 1);
    return (timer_irq_masked);
}




void timer_load_set(register uint32_t timer_load){
    if (T_IS_VALID_TIMER_LOAD(timer_load)){
        *TIMER_LOAD = timer_load;
    }
    return;
}

void timer_load_reset(){
    *TIMER_LOAD = T_LOAD_RESET
}

void timer_load_clear(){
    *TIMER_LOAD = T_LOAD_CLEAR
}

uint32_t timer_load_get(){
    register uint32_t timer_load = 0;
    timer_load = *TIMER_LOAD;
    return ((uint32_t)timer_load);
}

uint64_t timer_val_get(){
    register uint32_t timer_val = 0;
    timer_val = *TIMER_VALUE;
    return ((uint32_t)timer_val);
}

void timer_ctrl_set(register uint32_t timer_ctrl){
    if (T_IS_VALID_TIMER_CTRL(timer_ctrl)){
        *TIMER_CONTROL = timer_ctrl;
    }
    return;
}

void timer_ctrl_reset(){
    *TIMER_CONTROL = T_CTRL_RESET;
    return;
}

void timer_ctrl_clear(){
    *TIMER_CONTROL = T_CTRL_CLEAR;
    return;
}

uint32_t timer_ctrl_get(){
    register uint32_t timer_ctrl = 0;
    timer_ctrl = *TIMER_CONTROL;
    return ((uint32_t)timer_ctrl);
}

void timer_irq_clear_set(register uint32_t timer_irq_clear_){
    if (T_IS_VALID_TIMER_IRQ_CLEAR_(timer_irq_clear_)){
        *TIMER_CLEAR_IRQ = timer_irq_clear_;
    }
    return;
}

void timer_irq_clear_reset(){
    *TIMER_CLEAR_IRQ = T_IRQ_RESET;
    return;
}

void timer_irq_clear(){
    *TIMER_CLEAR_IRQ = T_IRQ_CLEAR;
    return;
}

uint32_t timer_irq_raw_get(){
    register uint32_t timer_raw_irq = 0;
    timer_raw_irq = *TIMER_RAW_IRQ;
    return((uint32_t)timer_raw_irq);
}

uint32_t timer_irq_mask_get(){
    register uint32_t timer_mask_irq = 0;
    timer_mask_irq = *TIMER_MASK_IRQ;
    return((uint32_t)timer_mask_irq);
}

void timer_reload_set(register uint32_t timer_reload){
    if (T_IS_VALID_TIMER_LOAD(timer_reload)){
        *TIMER_RELOAD = timer_reload;
    }
    return;
}

void timer_reload_reset(){
    *TIMER_RELOAD = T_LOAD_RESET;
    return;
}

void timer_reload_clear(){
    *TIMER_RELOAD = T_LOAD_CLEAR;
    return;
}

uint32_t timer_reload_get(){
    register uint32_t timer_prediv_irq = 0;
    timer_prediv_irq = *TIMER_PREDIV;
    return((uint32_t)timer_prediv_irq);
}

uint32_t timer_freerun_get(){
    register uint32_t timer_freerun = 0;
    timer_freerun = *TIMER_FREE_COUNT;
    return((uint32_t)timer_freerun);
}

void timer_prediv_set(register uint32_t timer_pre_div){
    if (T_IS_VALID_TIMER_PREDIV(timer_pre_div)){
        *TIMER_PREDIV = timer_pre_div;
    }
    return;
}

void timer_prediv_reset(){
    *TIMER_PREDIV = T_PREDIV_RESET;
    return;
}

void timer_prediv_clear(){
    *TIMER_PREDIV = T_PREDIV_CLEAR;
    return;
}

uint32_t timer_prediv_get(){
    register uint32_t timer_prediv_irq = 0;
    timer_prediv_irq = *TIMER_PREDIV;
    return((uint32_t)timer_prediv_irq);
}


int main(int argc, char* argv[]){
    register uint32_t i;
    register uint8_t n_timers;
    register uint8_t t_owner = 0;
    register uint32_t t_load = 1024;


    // Timer initialization function: MUST BE CALLED BEFORE EVERY OPERATION
    timer_boot();
    
    timer_acquire(t_owner, NULL, NULL, NULL);
    timer_start(t_owner, t_load, T_INTERRUPT_EN, 0, T_NOPRESCALE, T_BITS_HIGH, T_FREE_RUN_EN);
    // perform starting operation
    timer_acquire(TIMER, t_owner, NULL, NULL, NULL, NULL);
    t_comparer = 1024 * 2
    timer_stop(TIMER, t_owner);
    timer_ctrl_irq_disable();
    //raise condition: add proper protected task 
    timer_ctrl_irq_enable();
    timer_restart(TIMER, t_owner, t_comparer);
    timer_stop(TIMER, t_owner);
    timer_ctrl_irq_disable();
    //raise condition: add proper protected task 
    timer_ctrl_irq_enable();
    timer_release(TIMER, t_owner);
    timer_shutdown();

    
}




 


