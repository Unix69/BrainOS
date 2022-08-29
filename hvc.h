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
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPHVCE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */

#include "uart.h"

typedef enum hvc_test_status {} HVC_Test_Status;
typedef enum hvc_test_mode {} HVC_Test_Mode;
typedef enum hvc_boot_mode {} HVC_Boot_Mode;
typedef enum hvc_cpu_test_mode {} HVC_CPU_Test_Mode;
typedef enum hvc_system_test_mode {} HVC_System_Test_Mode;
typedef enum hvc_exception_test_mode {} HVC_Exception_Test_Mode; 
typedef enum hvc_interrupt_test_mode {} HVC_Interrupt_Test_Mode;
typedef enum hvc_stack_test_mode{} HVC_Stack_Test_Mode;
typedef enum hvc_power_test_mode {} HVC_Power_Test_Mode;
typedef enum hvc_monitor_test_mode {} HVC_Monitor_Test_Mode;
typedef enum hvc_mmu_test_mode {} HVC_MMU_Test_Mode;

typedef struct hvc_id {
    char* hvc_id = "";
    char* name = "";
    float version = 0;
    uint8_t hvc_mode = 0;
    uint8_t bit_parallelism = 0;
    uint8_t gic_enabled = 0;
    uint8_t fpu_enabled = 9;
    uint8_t simd_enabled = 0;
} HVC_Id;

typedef struct hvc_cpu_info {
    uint8_t hvc_cpuid = 0;
} HVC_CPU_Info;

typedef struct hvc_exception_info {
    void* evt_base_address = (void *) 0x00000000;
    uint64_t evt_limit_offset = 0;
    uint8_t std_exception_level = 0;
    uint8_t nstd_exception_level = 0;
} HVC_Exception_Info;

typedef struct hvc_mmu_info {
    uint8_t mmu_enabled = 0;
    uint8_t pag_enabled = 0;
    uint8_t seg_enabled = 0;
} HVC_MMU_Info;

typedef struct hvc_crypto_info {
    uint8_t crypto_enabled = 0;
} HVC_Crypto_Info;

typedef struct hvc_os_info {
    void* os_entry_address = (void*) 0x00000000;
} HVC_OS_Info;

typedef struct hvc_interrupt_info {
    void* ivt_base_address = (void*) 0x00000000;
    uint8_t int_enabled = 0;
    uint8_t int_mode = 0;
} HVC_Interrupt_Info;

typedef struct hvc_stack_info {
    uint64_t stack_base_address = 0x00000000;
    uint64_t stack_limit_offset = 0;
    uint64_t stack_pointer_selector;
} HVC_Stack_Info;

typedef struct hvc_permission_info {
} HVC_Permission_Info;

typedef struct hvc_boot_info {
    void* boot_address = (void*) 0x00000000;
    uint8_t boot_mode = 0;
} HVC_Boot_Info;

typedef struct hvc_power_info {
    void* boot_address = (void*) 0x00000000;
    uint8_t boot_mode = 0;
} HVC_Power_Info;

typedef struct hvc_monitor_info {
    void* boot_address = (void*) 0x00000000;
    uint8_t boot_mode = 0;
} HVC_Monitor_Info;

typedef struct hvc_system_info {
    void* system_boot_address = (void*) 0x00000000;
    uint8_t system_boot_mode = 0;
} HVC_System_Info;

typedef struct hvc_info {
    HVC_Boot_Info *boot_info = (HVC_Boot_Info *) 0;
    HVC_Exception_Info *exception_info = (HVC_Exception_Info *) 0;
    HVC_Crypto_Info *crypto_info = (HVC_Crypto_Info *) 0;
    HVC_OS_Info *os_info = (HVC_OS_Info *) 0;
    HVC_MMU_Info *mmu_info = (HVC_MMU_Info *) 0;
    HVC_Stack_Info *mem_info = (HVC_Stack_Info *) 0;
    HVC_Interrupt_Info *interrupt_info = (HVC_Interrupt_Info *) 0;
    HVC_Exception_Info *exception_info = (HVC_Exception_Info *) 0;
    HVC_Permission_Info *exception_info = (HVC_Permission_Info *) 0;
    HVC_System_Info *exception_info = (HVC_System_Info *) 0;   
} HVC_Info;

typedef struct hvc_cpu {
    uint8_t hvc_cpuid = 0;
} HVC_CPU;

typedef struct hvc_exception {
    uint64_t evt_base_address = 0x00000000;
    uint64_t evt_limit_offset = 0;
    uint8_t std_exception_level = 0;
    uint8_t nstd_exception_level = 0;
} HVC_Exception;

typedef struct hvc_mmu {
    uint8_t mmu_enabled = 0;
    uint8_t pag_enabled = 0;
    uint8_t seg_enabled = 0;
} HVC_MMU;

typedef struct hvc_crypto {
    uint8_t crypto_enabled = 0;
} HVC_Crypto;

typedef struct hvc_os {
    void* os_entry_address = (void*) 0x00000000;
} HVC_OS;

typedef struct hvc_interrupt {
    void* ivt_base_address = (void*) 0x00000000;
    uint8_t int_enabled = 0;
    uint8_t int_mode = 0;
} HVC_Interrupt;

typedef struct hvc_stack {
    uint64_t stack_base_address = 0x00000000;
    uint64_t stack_limit_offset = 0;
    uint64_t stack_pointer_selector;
} HVC_Stack;

typedef struct hvc_permission {
} HVC_Permission;

typedef struct hvc_boot {
    void* boot_address = (void*) 0x00000000;
    uint8_t boot_mode = 0;
} HVC_Boot;

typedef struct hvc_power {

} HVC_Power;

typedef struct hvc_monitor {

} HVC_Monitor;

typedef struct hvc_system {
    void* system_boot_address = (void*) 0x00000000;
    uint8_t system_boot_mode = 0;
} HVC_System;

typedef struct hvc_state {

} HVC_State;

typedef struct hvc {
    HVC_Id* id;
    HVC_Info* info;
    HVC_State* state;
    HVC_Boot *boot = (HVC_Boot *) 0;
    HVC_Exception *exception = (HVC_Exception *) 0;
    HVC_Crypto *crypto = (HVC_Crypto *) 0;
    HVC_OS *os = (HVC_OS *) 0;
    HVC_MMU *mmu = (HVC_MMU *) 0;
    HVC_Stack *stack = (HVC_Stack *) 0;
    HVC_Interrupt *interrupt = (HVC_Interrupt *) 0;
    HVC_Exception *exception = (HVC_Exception *) 0;
    HVC_Permission *exception = (HVC_Permission *) 0;
    HVC_System *exception = (HVC_System *) 0;  
    HVC_Monitor *monitor = (HVC_Monitor *) 0;
    HVC_CPU *cpu = (HVC_CPU *) 0;   
} HVC;




void hvc_boot(HVC_Boot_Mode hvc_boot_mode);
void hvc_test(HVC_Test_Mode hvc_test_mode);
void hvc_init(HVC *hvc);


void hvc_cpu_boot(uint8_t hvc_cpu_mode);
void hvc_sys_boot(uint8_t hvc_system_mode);
void hvc_exc_boot(uint8_t hvc_exception_mode);
void hvc_int_boot(uint8_t hvc_interrupt_mode);
void hvc_stack_boot(uint8_t hvc_stack_mode);
void hvc_power_boot(uint8_t hvc_power_mode);
void hvc_monitor_boot(uint8_t hvc_monitor_mode);
void hvc_mmu_boot(uint8_t hvc_mmu_mode);


void hvc_boot_info_init(HVC_Boot_Info *hvc_boot_info);
void hvc_cpu_info_init(HVC_CPU_Info *hvc_cpu_info);
void hvc_sys_info_init(HVC_System_Info *hvc_system_info);
void hvc_exc_info_init(HVC_Exception_Info *hvc_exception_info);
void hvc_int_info_init(HVC_Interrupt_Info *hvc_interrupt_info);
void hvc_stack_info_init(HVC_Stack_Info *hvc_mem_info);
void hvc_power_info_init(HVC_Power_Info *hvc_power_info);
void hvc_monitor_info_init(HVC_Monitor_Info *hvc_monitor_info);
void hvc_mmu_info_init(HVC_MMU_Info *hvc_mmu_info);
void hvc_permission_info_init(HVC_Permission_Info *hvc_permission_info);
void hvc_crypto_info_init(HVC_Crypto_Info *hvc_crypto_info);
void hvc_os_info_init(HVC_OS_Info *hvc_os_info);


void hvc_id_init(HVC_Id* hvc_id);
void hvc_info_init(HVC_Info* hvc_info);
void hvc_state_init(HVC_State* hvc_state);
void hvc_boot_init(HVC_Boot *hvc_boot);
void hvc_cpu_init(HVC_CPU *hvc_cpu);
void hvc_sys_init(HVC_System *hvc_system);
void hvc_exc_init(HVC_Exception *hvc_exception);
void hvc_int_init(HVC_Interrupt *hvc_interrupt);
void hvc_stack_init(HVC_Stack *hvc_mem);
void hvc_power_init(HVC_Power *hvc_power);
void hvc_monitor_init(HVC_Monitor *hvc_monitor);
void hvc_mmu_init(HVC_MMU *hvc_mmu);
void hvc_permission_init(HVC_Permission *hvc_permission);
void hvc_crypto_init(HVC_Crypto *hvc_crypto);
void hvc_os_init(HVC_OS *hvc_os);

void hvc_cpu_test(HVC_CPU_Test_Mode hvc_cpu_test_mode);
void hvc_sys_test(HVC_System_Test_Mode hvc_system_test_mode);
void hvc_exc_test(HVC_Exception_Test_Mode hvc_exception_test_mode);
void hvc_int_test(HVC_Interrupt_Test_Mode hvc_interrupt_test_mode);
void hvc_stack_test(HVC_Stack_Test_Mode hvc_stack_test_mode);
void hvc_power_test(HVC_Power_Test_Mode hvc_power_test_mode);
void hvc_monitor_test(HVC_Monitor_Test_Mode hvc_monitor_test_mode);
void hvc_mmu_test(HVC_MMU_Test_Mode hvc_mmu_test_mode);
