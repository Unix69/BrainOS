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
#include "timer.h"

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


timer_t timer;


void timer_init(register uint8_t timer_en, register uint8_t timer_bits, register uint32_t timer_load_val, register uint8_t timer_irq_en, register uint8_t timer_freerun_en, register uint32_t timer_prescale_val, register uint8_t timer_prescale_bits, register uint8_t timer_prediv){
    timer_ctrl_bits_set(timer_bits); //23 bits counter
    timer_ctrl_prescale_bits_set(timer_prescale_bits); //prescale is clock / 1
    if(timer_irq_en){
        timer_ctrl_irq_enable();
    } else {
        timer_ctrl_irq_disable();
    }
    if(timer_en){
        timer_ctrl_enable();
    } else {
        timer_ctrl_disable();
    }
    timer_ctrl_disable_halt_on_debug();
    timer_ctrl_freerun_enable_set(timer_freerun_en);
    timer_ctrl_prescale_val_set(timer_prescale_val);
    timer_load_set(timer_load_val);
    timer_prediv_set((uint16_t)timer_prediv);
}


//BrainOS timer internal routines
void timer_boot(){
    timer.t_en = T_DISABLED;
    timer.t_running = T_STOPPED;
    timer.t_free = T_FREE;
    timer.t_owner = T_NO_OWNER;
    timer.t_id = T_NO_ID;
    timer.t_load = TIMER_LOAD;
    timer.t_val = TIMER_VALUE;
    timer.t_ctrl = TIMER_CONTROL;
    timer.t_clear_irq = TIMER_CLEAR_IRQ;
    timer.t_mask_irq = TIMER_MASK_IRQ;
    timer.t_raw_irq = TIMER_RAW_IRQ;
    timer.t_reload = TIMER_RELOAD;
    timer.t_prediv = TIMER_PREDIV;
    timer.t_free_run_cnt = TIMER_FREE_COUNT;
    timer.t_irq_handler = NULL;
    timer.t_fiq_handler = NULL;
    timer.t_err_handler = NULL;
    timer.t_sync_handler = NULL;
}

void timer_shutdown(){
    timer.t_en = T_DISABLED;
    timer.t_running = T_STOPPED;
    timer.t_free = T_FREE;
    timer.t_owner = T_NO_OWNER;
    timer.t_id = T_NO_ID;
    timer.t_load = NULL;
    timer.t_val = NULL;
    timer.t_ctrl = NULL;
    timer.t_clear_irq = NULL;
    timer.t_mask_irq = NULL;
    timer.t_raw_irq = NULL;
    timer.t_reload = NULL;
    timer.t_prediv = NULL;
    timer.t_free_run_cnt = NULL;
    timer.t_irq_handler = NULL;
    timer.t_fiq_handler = NULL;
    timer.t_err_handler = NULL;
    timer.t_sync_handler = NULL;
}

//BrainOS timer control level API
void timer_acquire(register uint8_t timer_owner, void* timer_irq_handler, void* timer_fiq_handler, void* timer_err_handler, void* timer_sync_handler){
    if(timer.t_owner == T_NO_OWNER){
        timer.t_owner = timer_owner;
        timer.t_free = T_BUSY;
        timer.t_id = timer_owner;
        timer.t_irq_handler = timer_irq_handler;
        timer.t_fiq_handler = timer_fiq_handler;
        timer.t_err_handler = timer_err_handler;
        timer.t_sync_handler = timer_sync_handler;
    }
}

void timer_start(register uint8_t timer_owner){
    if(timer.t_owner == timer_owner){
        if(!timer.t_en || !timer.t_running){
            timer_ctrl_enable();
            timer.t_running = T_RUNNING;
            timer.t_en = T_ENABLED;
        }
    }
}

void timer_stop(register uint8_t timer_owner){
    if(timer.t_owner == timer_owner){
        if(timer.t_en && timer.t_running){
            timer_ctrl_disable();
            timer.t_running = T_STOPPED;
            timer.t_en = T_DISABLED;
        }
    }
}

void timer_release(register uint8_t timer_owner){
    if(timer.t_owner == timer_owner){
        timer.t_en = T_DISABLED;
        timer.t_free = T_FREE;
        timer.t_owner = T_NO_OWNER;
        timer.t_id = T_NO_ID;
        timer.t_irq_handler = NULL;
        timer.t_fiq_handler = NULL;
        timer.t_err_handler = NULL;
        timer.t_sync_handler = NULL;

        timer_irq_clear();
        timer_ctrl_irq_disable();
        timer_ctrl_disable();
        timer_ctrl_freerun_disable();

    }
}

void timer_irq_handler_set(register uint8_t timer_owner, void* timer_irq_handler){
    if(timer.t_owner == timer_owner && timer.t_en){
        timer.t_irq_handler = timer_irq_handler;
    }
}

void timer_fiq_handler_set(register uint8_t timer_owner, void* timer_fiq_handler){
    if(timer.t_owner == timer_owner && timer.t_en){
        timer.t_fiq_handler = timer_fiq_handler;
    }
}

void timer_sync_handler_set(register uint8_t timer_owner, void* timer_sync_handler){ 
    if(timer.t_owner == timer_owner && timer.t_en){
        timer.t_sync_handler = timer_sync_handler;
    }
    
}

void timer_err_handler_set(register uint8_t timer_owner, void* timer_err_handler){
    if(timer.t_owner == timer_owner && timer.t_en){
        timer.t_err_handler = timer_err_handler;
    }
}





void timer_ctrl_enable_halt_on_debug(){
    register uint8_t timer_ctrl = 0;
    timer_ctrl = timer_ctrl_get();
    timer_ctrl |= (T_HALT_ON_DEBUG_EN << 8);
    timer_ctrl_set(timer_ctrl);
    return;
}

void timer_ctrl_disable_halt_on_debug(){
    register uint8_t timer_ctrl = 0;
    timer_ctrl = timer_ctrl_get();
    timer_ctrl |= (T_HALT_ON_DEBUG_DIS << 8);
    timer_ctrl_set(timer_ctrl);
    return;
}

void timer_ctrl_enable(){
    register uint8_t timer_ctrl = 0;
    timer_ctrl = timer_ctrl_get();
    timer_ctrl |= (T_ENABLED << 7);
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
    timer_ctrl |= (T_FREE_RUN_EN << 9);
    timer_ctrl_set(timer_ctrl);
    return;
}

void timer_ctrl_freerun_disable(){
    register uint8_t timer_ctrl = 0;
    timer_ctrl = timer_ctrl_get();
    timer_ctrl |= (T_FREE_RUN_DIS << 9);
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
    timer_ctrl = timer_ctrl_get();
    timer_ctrl |= (timer_freerun << 9);
    timer_ctrl_set(timer_ctrl);
    return;
}

void timer_ctrl_bits_set(register uint8_t timer_bits){
    register uint8_t timer_ctrl = 0;
    timer_ctrl = timer_ctrl_get();
    timer_ctrl |= (timer_bits << 1);
    timer_ctrl_set(timer_ctrl);
    return;
}

uint8_t timer_ctrl_bits_get(){
    register uint8_t timer_bits = 0;
    timer_bits = (uint8_t) ((timer_ctrl_get()>>1) & 1);
    return(timer_bits);
}

void timer_ctrl_prescale_bits_set(register uint8_t timer_prescale){
    register uint8_t timer_ctrl = 0;
    timer_ctrl = timer_ctrl_get();
    timer_ctrl |= (timer_prescale << 2);
    timer_ctrl_set(timer_ctrl);
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
    timer_ctrl |= (T_INTERRUPT_EN << 5);
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
    timer_ctrl = timer_ctrl_get();
    timer_ctrl |= (timer_irq_enable << 4);
    timer_ctrl_set(timer_ctrl);
    return;
}



void timer_ctrl_prescale_val_set(register uint8_t timer_prescale_val){
    register uint8_t timer_ctrl = 0;
    timer_ctrl = timer_ctrl_get();
    timer_ctrl |= (timer_prescale_val << 16);
    timer_ctrl_set(timer_ctrl);
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
    *TIMER_LOAD = timer_load;
    return;
}

void timer_load_reset(){
    *TIMER_LOAD = T_LOAD_RESET;
}

void timer_load_clear(){
    *TIMER_LOAD = T_LOAD_CLEAR;
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
    *TIMER_CONTROL = timer_ctrl;
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

void timer_irq_clear_set(register uint32_t timer_irq_clear){
    *TIMER_CLEAR_IRQ = timer_irq_clear;
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
    *TIMER_RELOAD = timer_reload;
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

void timer_prediv_set(register uint16_t timer_pre_div){
    *TIMER_PREDIV = (timer_pre_div & 1023);
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

uint16_t timer_prediv_get(){
    register uint32_t timer_prediv = 0;
    timer_prediv = *TIMER_PREDIV;
    timer_prediv &= 1023;
    return((uint16_t)timer_prediv);
}


/*

int main(int argc, char* argv[]){
    register uint32_t i;
    register uint8_t n_timers;
    register uint8_t t_owner = 0;
    register uint32_t t_load = 1024;
    register uint8_t t_comparer = 0;

    // Timer initialization function: MUST BE CALLED BEFORE EVERY OPERATION
    timer_boot();
    
    timer_acquire(t_owner, NULL, NULL, NULL, NULL);
    timer_start(t_owner, t_load, T_INTERRUPT_EN, 0, T_NOPRESCALE, T_BITS_HIGH, T_FREE_RUN_EN);
    // perform starting operation
    t_comparer = 1024 * 2;
    timer_stop(t_owner);
    timer_ctrl_irq_disable();
    //raise condition: add proper protected task 
    timer_ctrl_irq_enable();
    timer_restart(t_owner);
    timer_stop(t_owner);
    timer_ctrl_irq_disable();
    
    //raise condition: add proper protected task 
    
    timer_ctrl_irq_enable();
    timer_release(t_owner);
    timer_shutdown();

    
}

*/


 


