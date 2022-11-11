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
.section ".text.sm"
.data
.align  2
BOOTDIV:			.asciz "--------------------------------------------------------------------\n"
BOOTMODEHEADER:		.asciz "Secure Monitor Bootmode "
BOOTHEADER:			.asciz "Secure Monitor Boot\n"
EVTEL3HEADER:		.asciz "Vector Table EL3 Base Address "
SCTLREL3HEADER:		.asciz "System Control Register EL3 "
SPSREL3HEADER:		.asciz "Saved Program Status Register EL3 "
CURELHEADER:		.asciz "Current EL "
CPUNHEADER:			.asciz "CPU number "
NL:					.asciz "\n"
CR:					.asciz "\r"
TB:					.asciz "\t"


.text
.global sm_boot

sm_boot:

    

	LDR X0, [SP], #8
	STP X29, X30, [SP, #-16]!

	MOV X20, X0
	
	//BOOT DIV
	MOV X0, #0
	LDR X0, =BOOTDIV
	BL uart_puts

	//BOOT HEADER
	MOV X0, #0
	LDR X0, =BOOTHEADER
	BL uart_puts
	//print saved BOOTMODE
	MOV X0, #0
	LDR X0, =BOOTMODEHEADER
	BL uart_puts
	MOV X0, #0
	MOV X0, x20
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

	//print newline
	MOV X0, #0 
	LDR X0, =NL
	BL uart_puts	
	
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


	//set up VECTOR BASE ADDRESS REGISTER EL2 to =vector_table_el2
    LDR x2, =vector_table_el3
    MSR VBAR_EL3, x2

    //print VECTOR BASE ADDRESS REGISTER EL2
    MOV X0, #0 
	LDR X0, =EVTEL3HEADER
	BL uart_puts
	MOV X0, #0 
	MRS X0, VBAR_EL3
	BL uart_hex_uint64
	
	//print newline
	MOV X0, #0
	LDR X0, =NL
	BL uart_puts

    //set STATUS REGISTER EL3
    MOV x2, #0x780
    MSR SCR_EL3, x2
    	
    //print STATUS REGISTER EL3
    MOV X0, #0 
	LDR X0, =SCTLREL3HEADER
	BL uart_puts
	MOV X0, #0 
	MRS X0, SCR_EL3
	BL uart_hex_uint64
    	
	
	//print newline
    MOV X0, #0 
	LDR X0, =NL
	BL uart_puts
	
	//set up SAVED PROGRAM STATUS REGISTER EL3
    MOV X0, #0x3c9
    MSR SPSR_EL3, X0
    	
    //print SAVED PROGRAM STATUS REGISTER EL3
    MOV X0, #0
	LDR X0, =SPSREL3HEADER
	BL uart_puts
	MOV X0, #0 
	MRS X0, SPSR_EL3
	BL uart_hex_uint64
    	


	LDP X29, X30, [SP], #16

	RET




//Exception Vector Table EL2
.balign 0x800
vector_table_el3:
curr_el3_sp0_sync:
// The exception handler for the synchronous
// exception from the current EL using SP0.
	//print nExcp = 0
	MOV X0, #0
	MRS x1, ESR_EL3
    MRS x2, ELR_EL3
    MRS x3, SPSR_EL3
    MRS x4, FAR_EL3
	MRS x5, CURRENTEL
	AND x5, x5, #12
	LSR x5, x5, #2
    BL exception_handler
	ERET
.balign 0x80
curr_el3_sp0_irq:
// The exception handler for the IRQ exception
// from the current EL using SP0.
	//print nExcp = 1
	MOV X0, #1
	MRS x1, ESR_EL3
    MRS x2, ELR_EL3
    MRS x3, SPSR_EL3
    MRS x4, FAR_EL3
	MRS x5, CURRENTEL
	AND x5, x5, #12
	LSR x5, x5, #2
    BL exception_handler
	ERET
.balign 0x80
curr_EL3_sp0_fiq:
// The exception handler for the FIQ exception
// from the current EL using SP0.
	//print nExcp = 2
	MOV X0, #2 
	MRS x1, ESR_EL3
    MRS x2, ELR_EL3
    MRS x3, SPSR_EL3
    MRS x4, FAR_EL3
	MRS x5, CURRENTEL
	AND x5, x5, #12
	LSR x5, x5, #2
    BL exception_handler
	ERET
.balign 0x80
curr_EL3_sp0_serror:
// The exception handler for the system error
// exception from the current EL using SP0.
	//print nExcp = 3
	MOV X0, #3 
	MRS x1, ESR_EL3
    MRS x2, ELR_EL3
    MRS x3, SPSR_EL3
    MRS x4, FAR_EL3
	MRS x5, CURRENTEL
	AND x5, x5, #12
	LSR x5, x5, #2
    BL exception_handler
	ERET
.balign 0x80
curr_EL3_spx_sync:
// The exception handler for the synchronous
// exception from the current EL using the
// current SP.
	//print nExcp = 4
	MOV X0, #4
	MRS x1, ESR_EL3
    MRS x2, ELR_EL3
    MRS x3, SPSR_EL3
    MRS x4, FAR_EL3
	MRS x5, CURRENTEL
	AND x5, x5, #12
	LSR x5, x5, #2
    BL exception_handler
	ERET
.balign 0x80
curr_EL3_spx_irq:
// The exception handler for IRQ exception
// from the current EL using the current SP.
	
	//print nExcp = 5
	MOV X0, #5
	MRS x1, ESR_EL3
    MRS x2, ELR_EL3
    MRS x3, SPSR_EL3
    MRS x4, FAR_EL3
	MRS x5, CURRENTEL
	AND x5, x5, #12
	LSR x5, x5, #2
    BL exception_handler
	
	ERET

.balign 0x80
curr_EL3_spx_fiq:
// The exception handler for the FIQ exception
// from the current EL using the current SP.
	//print nExcp = 6
	MOV X0, #6
	MRS x1, ESR_EL3
    MRS x2, ELR_EL3
    MRS x3, SPSR_EL3
    MRS x4, FAR_EL3
	MRS x5, CURRENTEL
	AND x5, x5, #12
	LSR x5, x5, #2
    BL exception_handler
	ERET
.balign 0x80
curr_EL3_spx_serror:
// The exception handler for the system error
// exception from the current EL using the
// current SP.
	//print nExcp = 7
	MOV X0, #7 
	MRS x1, ESR_EL3
    MRS x2, ELR_EL3
    MRS x3, SPSR_EL3
    MRS x4, FAR_EL3
	MRS x5, CURRENTEL
	AND x5, x5, #12
	LSR x5, x5, #2
    BL exception_handler
	ERET
.balign 0x80
lower_EL3_aarch64_sync:
// The exception handler for the synchronous
// exception from a lower EL (AArch64).
	MOV X0, #8
    MRS x1, ESR_EL3
    MRS x2, ELR_EL3
    MRS x3, SPSR_EL3
    MRS x4, FAR_EL3
	MRS x5, CURRENTEL
	AND x5, x5, #12
	LSR x5, x5, #2
    BL exception_handler
	ERET
.balign 0x80
lower_EL3_aarch64_irq:
// The exception handler for the IRQ exception
// from a lower EL (AArch64).
	//print nExcp = 9
	MOV X0, #9 
	MRS x1, ESR_EL3
    MRS x2, ELR_EL3
    MRS x3, SPSR_EL3
    MRS x4, FAR_EL3
	MRS x5, CURRENTEL
	AND x5, x5, #12
	LSR x5, x5, #2
    BL exception_handler
	ERET
.balign 0x80
lower_EL3_aarch64_fiq:
// The exception handler for the FIQ exception
// from a lower EL (AArch64).
	//print nExcp = 10
	MOV X0, #10
	MRS x1, ESR_EL3
    MRS x2, ELR_EL3
    MRS x3, SPSR_EL3
    MRS x4, FAR_EL3
	MRS x5, CURRENTEL
	AND x5, x5, #12
	LSR x5, x5, #2
    BL exception_handler
	ERET
.balign 0x80
lower_EL3_aarch64_serror: // The exception handler for the system error
// exception from a lower EL(AArch64).
	//print nExcp = 11
	MOV X0, #11
	MRS x1, ESR_EL3
    MRS x2, ELR_EL3
    MRS x3, SPSR_EL3
    MRS x4, FAR_EL3
	MRS x5, CURRENTEL
	AND x5, x5, #12
	LSR x5, x5, #2
    BL exception_handler
	ERET
.balign 0x80
lower_EL3_aarch32_sync:
// The exception handler for the synchronous
// exception from a lower EL(AArch32).
	//print nExcp = 12
	MOV X0, #12
	MRS x1, ESR_EL3
    MRS x2, ELR_EL3
    MRS x3, SPSR_EL3
    MRS x4, FAR_EL3
	MRS x5, CURRENTEL
	AND x5, x5, #12
	LSR x5, x5, #2
    BL exception_handler
	ERET
.balign 0x80
lower_EL3_aarch32_irq:
// The exception handler for the IRQ exception
// from a lower EL (AArch32).
	//print nExcp = 13
	MOV X0, #13
	MRS x1, ESR_EL3
    MRS x2, ELR_EL3
    MRS x3, SPSR_EL3
    MRS x4, FAR_EL3
	MRS x5, CURRENTEL
	AND x5, x5, #12
	LSR x5, x5, #2
    BL exception_handler
	ERET
.balign 0x80
lower_EL3_aarch32_fiq:
// The exception handler for the FIQ exception
// from a lower EL (AArch32).
	//print nExcp = 14
	MOV X0, #14
	MRS x1, ESR_EL3
    MRS x2, ELR_EL3
    MRS x3, SPSR_EL3
    MRS x4, FAR_EL3
	MRS x5, CURRENTEL
	AND x5, x5, #12
	LSR x5, x5, #2
    BL exception_handler
	ERET
.balign 0x80
lower_EL3_aarch32_serror: // The exception handler for the system error
// exception from a lower EL(AArch32).
	//print nExcp = 15
	MOV X0, #15
	MRS x1, ESR_EL3
    MRS x2, ELR_EL3
    MRS x3, SPSR_EL3
    MRS x4, FAR_EL3
	MRS x5, CURRENTEL
	AND x5, x5, #12
	LSR x5, x5, #2
    BL exception_handler
	ERET

