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



.arch armv8-a
.file   "start.S"


.section ".text.start"

.data
.align  2
BOOTPHRASE:			.asciz "BrainOS Kernel\n"
PWONHEADER:			.asciz "Power on\n"
CURELHEADER:		.asciz "Current EL "
CPUNHEADER:			.asciz "CPU number "
ELREL3HEADER:		.asciz "Excpetion Link Register EL3 "
ELREL2HEADER:		.asciz "Excpetion Link Register EL2 "
NL:					.asciz "\n"
CR:					.asciz "\r"
TB:					.asciz "\t"
.equ DEFAULT_SYSTEM, 0xFFFFFFFFFFFFFFFF
.equ DEFAULT_HYPERVISOR, 0xFFFFFFFFFFFFFFFF
.equ DEFAULT_SECURE_MONITOR, 0xFFFFFFFFFFFFFFFF

.text
.global _start

_start:

	// read cpu id, stop slave cores
	MRS X0, MPIDR_EL1
	AND X0, X0, #3
	CBZ X0, boot_cpu0
    // cpu id > 0, stop

	//initialize uart module	
	BL uart_init

	//print nCPU > 0 
	MOV X0, #0 
	LDR X0, =CPUNHEADER
	BL uart_puts
	MOV X0, #0
	MRS X0, MPIDR_EL1
	BL uart_hex_uint64
	
wait_for_exc:
	WFE
	//print nCPU 
	MOV X0, #0 
	LDR X0, =CPUNHEADER
	BL uart_puts
	MOV X0, #0
	MRS X0, MPIDR_EL1
	BL uart_hex_uint64
	B wait_for_exc
	

boot_cpu0:

	// cpu id == 0
	// set stack
	LDR X0, =_start
	MOV SP, X0

	//initialize uart module	
	BL uart_init

	//print nCPU 
	MOV X0, #0 
	LDR X0, =CPUNHEADER
	BL uart_puts
	MOV X0, #0
	MRS X0, MPIDR_EL1
	AND X0, X0, #3
	BL uart_hex_uint64
	
	//print newline
	MOV X0, #0 
	LDR X0, =NL
	BL uart_puts
	
	//print CURRENTEL
	MOV X0, #0 
	LDR X0, =CURELHEADER
	BL uart_puts
	MOV X0, #0
	MRS X0, CURRENTEL
	AND X0, X0, #12
	LSR X0, X0, #2
	BL uart_hex_uint64
	
	//print NEWLINE
	MOV X0, #0 
	LDR X0, =NL
	BL uart_puts

	//clear BSS
    LDR x1, =__bss_start
    LDR w2, =__bss_size

clear_bss:
	CBZ w2, run_el3
	STR XZR, [x1], #8
    SUB w2, w2, #1
    CBNZ w2, clear_bss


	
run_el3:

	//if CURRENTEL == 2 -> Branch to run_el2
	//else -> branch to run_el3
    MOV X0, #0
	MRS X0, CURRENTEL
    CMP X0, #12
    BNE run_el2
	
	
	//print newline
    MOV X0, #0 
	LDR X0, =NL
	BL uart_puts

	//SELECT SECURE MONITOR BOOTCODE
	LDR X0, =DEFAULT_SECURE_MONITOR
	STR X0, [SP, #8]!
	BL sm_boot

	
	//set EXCEPTION LINK REGISTER EL3
    ADR x2, run_el2
    MSR ELR_EL3, x2
    	
    //print EXCEPTION LINK REGISTER EL3
    MOV X0, #0 
	LDR X0, =ELREL3HEADER
	BL uart_puts
	MOV X0, #0 
	MRS X0, ELR_EL3
	BL uart_hex_uint64
    	
    //print newline
    MOV X0, #0 
	LDR X0, =NL
	BL uart_puts
	
    ERET
    	
    	
    	
run_el2:
	
	//if CURRENTEL == 1 -> branch to run_el1
    MOV X0, #0
	MRS X0, CURRENTEL
	CMP X0, #4
	BEQ run_el1


	//select HYPERVISOR MODE and call HV_BOOT
	LDR X0, =DEFAULT_HYPERVISOR
	STR X0, [SP, #8]!
	BL hv_boot

	//set EXCEPTION LINK REGISTER EL2 to run_el1
	MOV X0, #0
    ADR X0, run_el1
   	MSR ELR_EL2, X0
    	
    //print EXCEPTION LINK REGISTER EL2
    MOV X0, #0
	LDR X0, =ELREL2HEADER
	BL uart_puts
	MOV X0, #0 
	MRS X0, ELR_EL2
	BL uart_hex_uint64

	//print newline
    MOV X0, #0 
	LDR X0, =NL
	BL uart_puts

	ERET


run_el1:


	//SELECT SYSTEM BOOTCODE
	LDR X0, =DEFAULT_SYSTEM
	STR X0, [SP, #8]!
	BL system_boot

    MOV X0, #0 
	LDR X0, =BOOTPHRASE
	BL uart_puts

	//set STACK POINTER
	LDR X0, =_start
	MOV SP, X0

	BL main
    
	Bl wait_for_exc
