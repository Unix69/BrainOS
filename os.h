/*
 * Copyright (C) 2018 bzt (bztsrc@github)
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

#include "uart.h"

typedef struct os_gic {
} OS_GIC;

typedef struct os_crypto {
} OS_Crypto;


typedef struct os_cpu {
} OS_CPU;

typedef struct os_mmu {
} OS_MMU;

typedef struct os_system {
} OS_System;

typedef struct os_fpu{
} OS_FPU;

typedef struct os_simd {
} OS_SIMD;

typedef struct os_dsp {
} OS_DSP;

typedef struct os_timer {
} OS_DMA;

typedef struct os_ram {
} OS_Ram;

typedef struct os_device {
} OS_Device;

typedef struct os_id {
    char* os_id = "";
    char* name = "";
    float version = 0;
    uint8_t mode = 0;
    uint8_t bit_parallelism = 0;
} OS_Id;

typedef struct os_boot {
    uint64_t boot_address = 0x00000000;    
} OS_Boot;

typedef struct os_configuration {
} OS_Configuration;

typedef struct os_kernel{
    void* main_address = 0x00000000;
} OS_Kernel;

typedef struct os_memory {
    uint64_t heap_base_address = 0x00000000;
    uint64_t heap_limit_offset = 0;
    uint64_t stack_base_address = 0x00000000;
    uint64_t stack_limit_offset = 0;
} OS_Memory;


typedef struct os_timer{
    uint64_t counter = 0x00000000;
    uint64_t clock = 0x00000000;
    uint64_t counter_offset = 0x00000000;
} OS_Timer;

typedef struct os_power{
} OS_Power;

typedef struct os_cache{
} OS_Cache;

typedef struct os_clock{
} OS_Clock;

typedef struct os_storage {

} OS_Storage;

typedef struct os_exception {
    uint64_t evt_base_address = 0x00000000;
    uint64_t evt_limit_offset = 0;
    uint8_t std_exception_level = 0;
} OS_Exception;


typedef struct os_kernel_info {
    void* main_address = 0x00000000;
} OS_Kernel_Info;

typedef struct os_boot_info {
    uint64_t boot_address = 0x00000000;    
} OS_Boot_Info;

typedef struct os_mmu_info {
    bool mmu_enabled = false;
    bool paged = false;
    bool segmented = false;
} OS_MMU_Info;

typedef struct os_system_info {
    uint64_t heap_base_address = 0x00000000;
    uint64_t heap_limit_offset = 0;
    uint64_t stack_base_address = 0x00000000;
    uint64_t stack_limit_offset = 0;
} OS_System_Info;

typedef struct os_timer_info {

} OS_Timer_Info;

typedef struct os_power_info {
} OS_Power_Info;

typedef struct os_cache_info {
} OS_Cache_Info;

typedef struct os_clock_info {
} OS_Clock_Info;

typedef struct os_storage_info {
} OS_Storage_Info;

typedef struct os_exception_info {
    uint64_t evt_base_address = 0x00000000;
    uint64_t evt_limit_offset = 0;
    uint8_t std_exception_level = 0;
} OS_Exception_Info;

typedef struct os_cpu_info {
} OS_CPU_Info;

typedef struct os_fpu_info {
} OS_FPU_Info;

typedef struct os_simd_info {
} OS_SIMD_Info;

typedef struct os_dsp_info {
} OS_DSP_Info;

typedef struct os_dma_info {
} OS_DMA_Info;

typedef struct os_ram_info {
} OS_Ram_Info;

typedef struct os_device_info {
} OS_Device_Info;

typedef struct os_gic_info {
} OS_GIC_Info;

typedef struct os_crypto_info {
} OS_Crypto_Info;

typedef struct os_info {
    OS_GIC_Info** gic_info;
    OS_System_Info* system_info;
    OS_Cache_Info** caches_info;
    OS_MMU_Info* mmu_info;
    OS_Storage_Info** storages_info;
    OS_Timer_Info** timers_info;
    OS_Clock_Info** clocks_info;
    OS_Exception_Info** exceptions_info;
    OS_Boot_Info* boot_info;
    OS_Kernel_Info* kernel_info;
    OS_Ram_Info* ram_info;
    OS_DMA_Info* dma_info;
    OS_DSP_Info* dsp_info;
    OS_FPU_Info* fpu_info;
    OS_SIMD_Info* simd_info;
    OS_CPU_Info** cpus_info;
    OS_Device_Info** devices_info;
} OS_Info;

typedef struct os_state {
    OS_System* system;
    OS_GIC** gic;
    OS_Cache** caches;
    OS_MMU* mmu;
    OS_Storage** storages;
    OS_Timer** timers;
    OS_Clock** clocks;
    OS_Exception** exceptions;
    OS_Boot* boot;
    OS_Kernel* kernel;
    OS_Ram* ram;
    OS_DMA* dma;
    OS_DSP* dsp;
    OS_FPU* fpu;
    OS_SIMD* simd;
    OS_CPU** cpus;
    OS_Device** devices;
} OS_State;

typedef struct os {
    OS_Id* id;
    OS_Info* info;
    OS_State* state;
} OS;

void os_boot(uint32_t os_boot_mode);

