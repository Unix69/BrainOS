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

#ifndef SYSTEM_TIMER_H
#define SYSTEM_TIMER_H

#include "mmu.h"
#include "types.h"

#define N_STIMERS 4

//system timer 0
#define SYSTEM_TIMER_0 0
//system timer 1
#define SYSTEM_TIMER_1 1
//system timer 2
#define SYSTEM_TIMER_2 2
//system timer 3
#define SYSTEM_TIMER_3 3


typedef struct sys_timer * sys_timer_t;



//BrainOS System Timer internal routines
/// @brief boot system timers
void sys_timer_boot();

/// @brief shutdown system timers
void sys_timer_shutdown();

//BrainOS System Timer control level API

/// @brief acquire a system timer
/// @param sys_timer 
/// @param sys_timer_owner 
/// @param sys_timer_irq_handler 
/// @param sys_timer_fiq_handler 
/// @param sys_timer_err_handler 
/// @param sys_timer_sync_handler 
void sys_timer_acquire(register uint32_t sys_timer, register uint8_t sys_timer_owner, void* sys_timer_irq_handler, void* sys_timer_fiq_handler, void* sys_timer_err_handler, void* sys_timer_sync_handler);


/// @brief start a system timer
/// @param sys_timer 
/// @param sys_timer_owner 
/// @param sys_timer_cmp_val 
void sys_timer_start(register uint32_t sys_timer, register uint8_t sys_timer_owner, uint32_t sys_timer_cmp_val);

/// @brief stop a system timer
/// @param sys_timer 
/// @param sys_timer_owner 
void sys_timer_stop(register uint32_t sys_timer, register uint8_t sys_timer_owner);

/// @brief restart a system timer
/// @param sys_timer 
/// @param sys_timer_owner 
void sys_timer_restart(register uint32_t sys_timer, register uint8_t sys_timer_owner);


/// @brief release a system timer
/// @param sys_timer 
/// @param sys_timer_owner 
void sys_timer_release(register uint32_t sys_timer, register  uint8_t sys_timer_owner);

/// @brief set system timer irq handler
/// @param sys_timer 
/// @param sys_timer_owner 
/// @param sys_timer_irq_handler 
void sys_timer_irq_handler_set(register uint32_t sys_timer,register  uint8_t sys_timer_owner, void* sys_timer_irq_handler);

/// @brief set system timer fiq handler
/// @param sys_timer 
/// @param sys_timer_owner 
/// @param sys_timer_fiq_handler 
void sys_timer_fiq_handler_set(register uint32_t sys_timer,register  uint8_t sys_timer_owner, void* sys_timer_fiq_handler);

/// @brief set system timer sync handler
/// @param sys_timer 
/// @param sys_timer_owner 
/// @param sys_timer_sync_handler 
void sys_timer_sync_handler_set(register uint32_t sys_timer, register  uint8_t sys_timer_owner, void* sys_timer_sync_handler);

/// @brief set system timer err handler
/// @param sys_timer 
/// @param sys_timer_owner 
/// @param sys_timer_err_handler 
void sys_timer_err_handler_set(register uint32_t sys_timer, register uint8_t sys_timer_owner, void* sys_timer_err_handler);

/// @brief enable system timer
/// @param sys_timer 
/// @param sys_timer_owner 
void sys_timer_enable(register uint8_t sys_timer, register uint8_t sys_timer_owner);

/// @brief disable system timer
/// @param sys_timer 
/// @param sys_timer_owner 
void sys_timer_disable(register uint8_t sys_timer, register uint8_t sys_timer_owner);



//BrainOS System Timer system level API

/// @brief set system timer control/status
/// @param sys_timer the timer to select
void sys_timer_stat_set(register uint8_t sys_timer);

/// @brief reset system timer control/status
/// @param sys_timer the timer to select
void sys_timer_stat_reset(register uint8_t sys_timer);

/// @brief clear system timer control/status
/// @param sys_timer the timer to select
void sys_timer_stat_clear(register uint8_t sys_timer);


/// @brief get system timer control/status
/// @param sys_timer the timer to select
/// @return sys_timer status value 
uint8_t sys_timer_stat_get(register uint8_t sys_timer);

/// @brief get system timer counter value
/// @return sys_timer counter value
uint64_t sys_timer_cnt_get();


/// @brief get system timer counter low value
/// @return sys_timer low counter value
uint32_t sys_timer_cnt_lo_get();

/// @brief get system timer counter high value
/// @return sys_timer high counter value
uint32_t sys_timer_cnt_hi_get();

/// @brief get system timer compare value
/// @param sys_timer the timer to select
/// @return sys_timer compare value
uint32_t sys_timer_cmp_get(register uint8_t sys_timer);


/// @brief set system timer compare value
/// @param sys_timer the timer to select
/// @param sys_timer_cmp_val the timer compare value to set
void sys_timer_cmp_set(register uint8_t sys_timer,register  uint32_t sys_timer_cmp_val);

/// @brief reset system timer compare value
/// @param sys_timer 
void sys_timer_cmp_reset(register uint8_t sys_timer);

/// @brief check if system timer is expired
/// @param sys_timer the timer to select
/// @return 1 if sys_timer is expired, 0 otherwhise
int sys_timer_is_expired(register uint8_t sys_timer);

/// @brief check if system timer is matched
/// @param sys_timer the timer to select
/// @return 1 if sys_timer is matched, 0 otherwhise
int sys_timer_is_match(register uint8_t sys_timer);

#endif