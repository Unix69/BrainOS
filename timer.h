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

#include "gpio.h"
#include "mmu.h"


//System timer Macros
#define IS_TIMER(ST_NR) ST_NR == SYSTEM_TIMER_0 || ST_NR == SYSTEM_TIMER_1 || ST_NR == SYSTEM_TIMER_2 || ST_NR == SYSTEM_TIMER_3
#define MATCH(ST_STATUS) ST_STATUS == ST_STATUS
#define IS_EXPIRED(ST_CNT, ST_CMP) ST_CNT >= ST_CMP



//system timer 0 
#define SYSTEM_TIMER_0 0001
//system timer 1
#define SYSTEM_TIMER_1 0010
//system timer 2
#define SYSTEM_TIMER_2 0100
//system timer 3
#define SYSTEM_TIMER_3 1000

//Identify system timer status match
#define ST_MATCH 1
//Identify system timer status no match
#define ST_NOMATCH 0
//Identify system timer status clear
#define ST_CLEAR 1

#define SYSTEM_TIMER_CS     ((volatile unsigned int*)(MMIO_BASE+0x3000)) //System Timer Control/Status 32 bit
#define SYSTEM_TIMER_LO     ((volatile unsigned int*)(MMIO_BASE+0x3004)) //System Timer Counter Lower 32 bit
#define SYSTEM_TIMER_HI     ((volatile unsigned int*)(MMIO_BASE+0x3008)) //System Timer Counter Higher 32 bit
#define SYSTEM_TIMER_C0     ((volatile unsigned int*)(MMIO_BASE+0x300C)) //System Timer Compare 0 32 bit
#define SYSTEM_TIMER_C1     ((volatile unsigned int*)(MMIO_BASE+0x3010)) //System Timer Compare 1 32 bit
#define SYSTEM_TIMER_C2     ((volatile unsigned int*)(MMIO_BASE+0x3014)) //System Timer Compare 2 32 bit
#define SYSTEM_TIMER_C3     ((volatile unsigned int*)(MMIO_BASE+0x3018)) //System Timer Compare 3 32 bit


void sys_timer_boot();
void sys_timer_init();

uint32_t sys_timer_enable(uint8_t sys_timer);
uint32_t sys_timer_disable(uint8_t sys_timer);


//set and get system timer control/status
void sys_timer_stat_set(uint8_t sys_timer, uint32_t sys_timer_status);
void sys_timer_stat_clear(uint8_t sys_timer);
uint32_t sys_timer_stat_get(uint8_t sys_timer);

//get system timer counter value
uint64_t sys_timer_cnt_get(uint8_t sys_timer);
uint32_t sys_timer_cnt_lo_get(uint8_t sys_timer);
uint32_t sys_timer_cnt_hi_get(uint8_t sys_timer);

//set and get system timer compare value
uint32_t sys_timer_cmp_get(uint8_t sys_timer);
void sys_timer_cmp_set(uint8_t sys_timer, uint32_t sys_timer_cmp_val);

#endif