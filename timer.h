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

#ifndef TIMER_H
#define TIMER_H

#include "mmu.h"
#include "types.h"

//timer 0
#define TIMER 0


typedef struct timer * timer_t;


//BrainOS timer internal routines
/// @brief boot timers
void timer_boot();

/// @brief shutdown timers
void timer_shutdown();

//BrainOS timer control level API

/// @brief 
/// @param timer_owner 
/// @param timer_irq_handler 
/// @param timer_fiq_handler 
/// @param timer_err_handler 
/// @param timer_sync_handler 
void timer_acquire(register uint8_t timer_owner, void* timer_irq_handler, void* timer_fiq_handler, void* timer_err_handler, void* timer_sync_handler);

/// @brief 
/// @param timer_owner 
/// @param timer_load_val 
/// @param timer_en_int 
/// @param timer_prescale_val 
/// @param timer_prescale_bits 
/// @param timer_bits 
/// @param timer_freerun 
void timer_start(register uint8_t timer_owner, register uint32_t timer_load_val, register uint8_t timer_en_int, register uint32_t timer_prescale_val, register uint8_t timer_prescale_bits, register uint8_t timer_bits, register uint8_t timer_freerun);

/// @brief 
/// @param timer_owner  
void timer_stop(register uint8_t timer_owner);

/// @brief restart a timer
/// @param timer 
/// @param timer_owner 
void timer_restart(register uint32_t timer, register uint8_t timer_owner);


/// @brief release a timer
/// @param timer 
/// @param timer_owner 
void timer_release(register uint32_t timer, register  uint8_t timer_owner);

/// @brief set timer irq handler
/// @param timer 
/// @param timer_owner 
/// @param timer_irq_handler 
void timer_irq_handler_set(register uint32_t timer,register  uint8_t timer_owner, void* timer_irq_handler);

/// @brief set timer fiq handler
/// @param timer 
/// @param timer_owner 
/// @param timer_fiq_handler 
void timer_fiq_handler_set(register uint32_t timer,register  uint8_t timer_owner, void* timer_fiq_handler);

/// @brief set timer sync handler
/// @param timer 
/// @param timer_owner 
/// @param timer_sync_handler 
void timer_sync_handler_set(register uint32_t timer, register  uint8_t timer_owner, void* timer_sync_handler);

/// @brief set timer err handler
/// @param timer 
/// @param timer_owner 
/// @param timer_err_handler 
void timer_err_handler_set(register uint32_t timer, register uint8_t timer_owner, void* timer_err_handler);

/// @brief 
void timer_ctrl_enable_halt_on_debug();

/// @brief 
void timer_ctrl_disable_halt_on_debug();

/// @brief 
void timer_ctrl_enable();

/// @brief 
void timer_ctrl_disable();


/// @brief 
void timer_ctrl_freerun_enable();

/// @brief 
void timer_ctrl_freerun_disable();

/// @brief 
/// @return 
uint8_t timer_ctrl_freerun_enable_get();


/// @brief 
/// @param timer_freerun 
void timer_ctrl_freerun_enable_set(register uint8_t timer_freerun);

/// @brief 
/// @param timer_bits 
void timer_ctrl_bits_set(register uint8_t timer_bits);

/// @brief 
/// @return 
uint8_t timer_ctrl_bits_get();

/// @brief 
/// @param timer_prescale 
void timer_ctrl_prescale_bits_set(register uint8_t timer_prescale);

/// @brief 
/// @return 
uint8_t timer_ctrl_prescale_bits_get();

/// @brief 
void timer_ctrl_irq_enable();

/// @brief 
void timer_ctrl_irq_disable();


/// @brief 
/// @return 
uint8_t timer_ctrl_irq_enable_get();

/// @brief 
/// @param timer_prescale_val 
void timer_ctrl_prescale_val_set(register uint8_t timer_prescale_val);


/// @brief 
/// @return 
uint8_t timer_ctrl_prescale_val_get();

/// @brief 
/// @return 
uint8_t timer_irq_status_get();


/// @brief 
/// @return 
uint8_t timer_irq_masked_get();


/// @brief 
/// @return 
uint8_t timer_prediv_val_get();


/// @brief 
/// @param timer_prediv_val 
void timer_prediv_val_set(register uint32_t timer_prediv_val);


/// @brief 
void timer_irq_clear();

/// @brief 
/// @return 
uint32_t timer_freerun_get();





//BrainOS timer system level API

/// @brief set timer load
/// @param timer_load 
void timer_load_set(register uint32_t timer_load);

/// @brief reset timer load
void timer_load_reset();

/// @brief clear timer load
void timer_load_clear();

/// @brief get timer load
/// @return load value of timer
uint32_t timer_load_get();

/// @brief get timer value
/// @return value of timer
uint64_t timer_val_get();

/// @brief set timer control
/// @param timer_ctrl 
void timer_ctrl_set(register uint32_t timer_ctrl);

/// @brief reset timer control
void timer_ctrl_reset();

/// @brief clear timer control
void timer_ctrl_clear();

/// @brief get timer control
/// @return control of timer
uint32_t timer_ctrl_get();

/// @brief set timer irq clear/ack
/// @param timer_irq_clear
void timer_irq_clear_set(register uint32_t timer_irq_clear);

/// @brief reset timer irq clear/ack
void timer_irq_clear_ack_reset();

/// @brief clear timer irq clear/ack
void timer_irq_clear_ack_clear();

/// @brief get timer raw irq
/// @return timer raw irq
uint32_t timer_irq_raw_get();

/// @brief get timer irq mask
/// @return timer irq mask
uint32_t timer_irq_mask_get();

/// @brief set timer reload
/// @param timer_reload 
void timer_reload_set(register uint32_t timer_reload);

/// @brief reset timer reload
void timer_reload_reset();

/// @brief clear timer reload
void timer_reload_clear();

/// @brief get timer reload
/// @return timer reload
uint32_t timer_reload_get();


/// @brief get timer freerun
/// @return timer free running
uint32_t timer_freerun_get();

/// @brief set timer pre-division scaler
/// @param timer_pre_div 
void timer_prediv_set(register uint32_t timer_pre_div);

/// @brief reset timer pre-division scaler
void timer_prediv_reset();

/// @brief clear timer pre-division scaler
void timer_prediv_clear();

/// @brief get timer pre-division scaler
/// @return control of timer
uint32_t timer_prediv_get();

#endif