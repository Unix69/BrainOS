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
.file "hv.S"

.section ".text.hv"

.data
.align  2
BOOTDIV:			.asciz "--------------------------------------------------------------------\n"
BOOTMODEHEADER:		.asciz "Hyper Bootmode "
BOOTHEADER:			.asciz "Hyper Boot\n"
EVTEL2HEADER:		.asciz "Vector Table EL2 Base Address "
CURELHEADER:		.asciz "Current EL "
EXCHEADER:			.asciz "Excpetion\n"
CPUNHEADER:			.asciz "CPU number "
UNITIDHEADER:		.asciz "Unit/Multiproc "
TIDEL2HEADER:		.asciz "Software thread Register EL2 "
SPSREL2HEADER:		.asciz "Saved Program Status Register EL2 "
ELREL2HEADER:		.asciz "Excpetion Link Register EL2 "
CTFEL0HEADER:		.asciz "Counter Time Frequency Register EL0 "
CTCVEL0HEADER:		.asciz "Counter Count Value Register EL0 "
CHPTEL2HEADER:		.asciz "Hypervisor Physical Timer Control register "
CHPTTVEL2HEADER:	.asciz "Hypervisor Physical Timer Time Value register "
CHPTCVEL2HEADER:	.asciz "Hypervisor Physical Timer Compare Value register "
SPEL1HEADER:		.asciz "Stack Pointer EL1 "
SPEL2HEADER:		.asciz "Stack Pointer EL2 "
SPEL3HEADER:		.asciz "Stack Pointer EL3 "
SPSELHEADER:		.asciz "Stack Pointer Selector "
SCTLREL2HEADER:		.asciz "System Control Register EL2 "
HCREL2HEADER:		.asciz "Hypervisor Control Regsiter "
CNTHCTLEL2HEADER:	.asciz "Counter Hypervisor Control "
CNTVOFFEL2HEADER:	.asciz "Counter Virtual Offset "
DAIFHEADER:			.asciz "DAIF flag "
NL:					.asciz "\n"
CR:					.asciz "\r"
TB:					.asciz "\t"

.equ TIMER_IRQ_CTRL, 0x40000040

.text

.global hv_boot
hv_boot:


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

	//test MPIDR_EL1 register for multiproc/uniproc Bit
	MOV X0, #0
	LDR X0, =UNITIDHEADER			
	BL uart_puts
	MOV X0, #0
	MRS X0, MPIDR_EL1
	ANDS X0, X0, #0X0000000040000000
	LSR X0, X0, #30
	BL uart_hex_uint64

	//print newline
	MOV X0, #0 
	LDR X0, =NL
	BL uart_puts

	//print ThreadID
	MOV X0, #0 
	LDR X0, =TIDEL2HEADER
	BL uart_puts
	MOV X0, #0
	MRS X0, TPIDR_EL1
	AND X0, X0, #3
	BL uart_hex_uint64

	//print newline
	MOV X0, #0 
	LDR X0, =NL
	BL uart_puts
	
	//set STACK POINTER SELECTOR
	MOV X0, XZR
	ORR X0, X0, #1
	MSR SPSEL, X0


	//print STACK POINTER SELECTOR
	MOV X0, #0 
	LDR X0, =SPSELHEADER
	BL uart_puts
	MOV X0, #0 
	MRS X0, SPSEL
	BL uart_hex_uint64
	

	//print newline
	MOV X0, #0 
	LDR X0, =NL
	BL uart_puts


	//print STACK POINTER EL1
	MOV X0, #0 
	LDR X0, =SPEL1HEADER
	BL uart_puts
	MOV X0, #0 
	MOV X0, SP
	BL uart_hex_uint64

	//print newline
	MOV X0, #0 
	LDR X0, =NL
	BL uart_puts
	
	//Set HCR_EL2
	MSR HCR_EL2, XZR
	MOV X0, XZR	
	ORR X0, X0, #(1<<3) // The FMO bit.
	ORR X0, X0, #(1<<4) // The IMO bit.
	ORR X0, X0, #(1<<5) // The AMO bit.	
	ORR X0, X0, #(1<<8) // The VA bit.
	ORR X0, X0, #(1<<13) // The TWI bit.
	ORR X0, X0, #(1<<14) // The TWE bit.
	ORR X0, X0, #(1<<19) // The TSC bit.
	ORR X0, X0, #(1<<22) // The TSW bit.
	ORR X0, X0, #(1<<23) // The TPCP bit.
	ORR X0, X0, #(1<<24) // The TPU bit.
	ORR X0, X0, #(1<<25) // The TTLB bit.
	ORR X0, X0, #(1<<26) // The TVM bit.
	//ORR X0, X0, #(1<<27) // The TGE bit.
	ORR X0, X0, #(1<<30) // The TRVM bit.
	ORR X0, X0, #(1<<31) // The RW bit.
	ORR X0, X0, #(1<<34) // The E2H bit.
	ORR X0, X0, #(1<<35) // The TLOR bit.
	ORR X0, X0, #(1<<37) // The TEA bit.
	ORR X0, X0, #(1<<40) // The APK bit.
	MOV x1, X0
	ORR x1, x1, #(1<<42)
	AND X0, X0, x1 // The NV bit.
	MOV x1, X0
	ORR x1, x1, #(1<<43)
	AND X0, X0, x1 // The NV1 bit.
	MOV x1, X0
	ORR x1, x1, #(1<<45)
	AND X0, X0, x1 // The NV2 bit.
	ORR X0, X0, #(1<<51) // The AMVOFFEN bit.
	ORR X0, X0, #(1<<52) // The TOCU bit.
	ORR X0, X0, #(1<<54) // The TTLBIS bit.
	ORR X0, X0, #(1<<55) // The TTLBOS bit.
	ORR X0, X0, #(1<<56) // The ATA bit.
	MSR HCR_EL2, X0


	//print HYPERVISOR CONTROL REGISTER EL2
    MOV X0, #0 
	LDR X0, =HCREL2HEADER
	BL uart_puts
	MOV X0, #0 
	MRS X0, HCR_EL2
	BL uart_hex_uint64

	//print newline
	MOV X0, #0
	LDR X0, =NL
	BL uart_puts
	
	
	//set SYSTEM CONTROL REGISTER EL2
	MRS X0, SCTLR_EL2
	//ORR X0, X0, #1 // The M bit.
	ORR X0, X0, #(1<<1) // The A bit.
	MOV x1, X0
	ORR x1, x1, #(1<<2)
	AND X0, X0, x1 // The C bit. When HCR_EL2.{E2H, TGE} is {1, 1}, this bit has effect.
	ORR X0, X0, #(1<<3) // The SA bit.
	ORR X0, X0, #(1<<4) // The SA0 bit. When HCR_EL2.{E2H, TGE} is {1, 1}, this bit has effect.
	ORR X0, X0, #(1<<5) // The CP15BEN bit. When HCR_EL2.{E2H, TGE} is {1, 1}, this bit has effect.	
	MOV x1, X0
	ORR x1, x1, #(1<<6)
	AND X0, X0, x1 // The nAA bit.
	MOV x1, X0
	ORR x1, x1, #(1<<7) 
	AND X0, X0, x1 // The ITD bit. When HCR_EL2.{E2H, TGE} is {1, 1}, this bit has effect.
	MOV x1, X0
	ORR x1, x1, #(1<<8)
	AND X0, X0, x1 // The SED bit. When HCR_EL2.{E2H, TGE} is {1, 1}, this bit has effect.
	ORR X0, X0, #(1<<10) // The EnRCTX bit. When HCR_EL2.{E2H, TGE} is {1, 1}, this bit has effect.
	ORR X0, X0, #(1<<11) // The EOS bit.
	MOV x1, X0
	ORR x1, x1, #(1<<12)
	AND X0, X0, x1 // The I bit. When HCR_EL2.{E2H, TGE} is {1, 1}, this bit has effect.
	MOV x1, X0
	ORR x1, x1, #(1<<14)
	AND X0, X0, x1 // The DZE bit. When HCR_EL2.{E2H, TGE} is {1, 1}, this bit has effect.
	MOV x1, X0
	ORR x1, x1, #(1<<15)
	AND X0, X0, x1 // The UCT bit. When HCR_EL2.{E2H, TGE} is {1, 1}, this bit has effect
	MOV x1, X0
	ORR x1, x1, #(1<<16)
	AND X0, X0, x1 // The nTWI bit. When HCR_EL2.{E2H, TGE} is {1, 1}, this bit has effect.
	MOV x1, X0
	ORR x1, x1, #(1<<18)
	AND X0, X0, x1 // The nTWE bit. When HCR_EL2.{E2H, TGE} is {1, 1}, this bit has effect.
	ORR X0, X0, #(1<<19) // The WXN bit.
	ORR X0, X0, #(1<<20) // The TSCXT bit. When HCR_EL2.{E2H, TGE} is {1, 1}, this bit has effect.
	ORR X0, X0, #(1<<21) // The IESB bit
	MOV x1, X0
	ORR x1, x1, #(1<<22)
	AND X0, X0, x1 // The EIS bit.
	ORR X0, X0, #(1<<23) // The SPAN bit. When HCR_EL2.{E2H, TGE} is {1, 1}, this bit has effect.
	ORR X0, X0, #(1<<24) // The E0E bit. When HCR_EL2.{E2H, TGE} is {1, 1}, this bit has effect.
	//ORR X0, X0, #(1<<25) // The EE bit
	MOV x1, X0
	ORR x1, x1, #(1<<26)
	AND X0, X0, x1 // The BT0 bit. When HCR_EL2.{E2H, TGE} is {1, 1}, this bit has effect.
	ORR X0, X0, #(1<<28) // The nTLSMD bit. When HCR_EL2.{E2H, TGE} is {1, 1}, this bit has effect.
	ORR X0, X0, #(1<<29) // The LSMAOE, bit. When HCR_EL2.{E2H, TGE} is {1, 1}, this bit has effect.
	MOV x1, X0
	ORR x1, x1, #(1<<35)
	AND X0, X0, x1 // The BT0 bit. When HCR_EL2.{E2H, TGE} is {1, 1}, this bit has effect.
	MOV x1, X0
	ORR x1, x1, #(1<<36)
	AND X0, X0, x1 // The BT bit.
	ORR X0, X0, #(1<<37) // The ITFSB bit.
	ORR X0, X0, #(1<<38)
	MOV x1, X0
	ORR x1, x1, #(1<<39)
	AND X0, X0, x1 // The TCF0, bits [39:38]. When HCR_EL2.{E2H, TGE} is {1, 1}, this bit has effect.
	ORR X0, X0, #(1<<40)
	MOV x1, X0
	ORR x1, x1, #(1<<41)
	AND X0, X0, x1 // The TCF, bits [41:40].
	ORR X0, X0, #(1<<42) // The ATA0 bit. When HCR_EL2.{E2H, TGE} is {1, 1}, this bit has effect.
	ORR X0, X0, #(1<<43) // The ATA bit.
	ORR X0, X0, #(1<<45) // The TWEDEn bit. When HCR_EL2.{E2H, TGE} is {1, 1}, this bit has effect.
	MOV x1, X0
	ORR x1, x1, #(1<<46)
	ORR x1, x1, #(1<<47)
	ORR x1, x1, #(1<<48)
	ORR x1, x1, #(1<<49)
	AND X0, X0, x1 // The TWEDEL bits [49:46]. When HCR_EL2.{E2H, TGE} is {1, 1}, this bits have effect
	ORR X0, X0, #(1<<54) // The ENASR bit. When HCR_EL2.{E2H, TGE} is {1, 1}, this bit has effect.
	ORR X0, X0, #(1<<55) // The ENAS0 bit. When HCR_EL2.{E2H, TGE} is {1, 1}, this bit has effect.
	ORR X0, X0, #(1<<56) // The ENALS bit. When HCR_EL2.{E2H, TGE} is {1, 1}, this bit has effect.
	ORR X0, X0, #(1<<57) // The EPAN bit.
	MSR SCTLR_EL2, X0


	//print SYSTEM CONTROL REGISTER EL2
	MOV X0, #0
	LDR X0, =SCTLREL2HEADER
	BL uart_puts
	MOV X0, #0 
	MRS X0, SCTLR_EL2
	BL uart_hex_uint64
	

	//print newline
	MOV X0, #0 
	LDR X0, =NL
	BL uart_puts

	//set DAIF Flag to 0000
    MOV X0, #0b0000
	LSL X0, X0, #6  
    MSR DAIF, X0

	//print newline
	MOV X0, #0 
	LDR X0, =NL
	BL uart_puts

	//DAIF configuration setting
	//print DAIF Flag
    MOV X0, #0 
	LDR X0, =DAIFHEADER
	BL uart_puts
	MOV X0, #0 
	MRS X0, DAIF
	BL uart_hex_uint64

	//print newline
	MOV X0, #0 
	LDR X0, =NL
	BL uart_puts



	//set SAVED PROGRAM STATUS REGISTER to 00100
	MOV X0, #0b00100 
	MSR SPSR_EL2, X0 // M[4:0]=00100 EL1t must match HCR_EL2.RW.

	//print newline
	MOV X0, #0 
	LDR X0, =NL
	BL uart_puts

	//print SAVED PROGRAM STATUS REGISTER EL2
    MOV X0, #0 
	LDR X0, =SPSREL2HEADER
	BL uart_puts
	MOV X0, #0 
	MRS X0, SPSR_EL2
	BL uart_hex_uint64


	//print newline
	MOV X0, #0 
	LDR X0, =NL
	BL uart_puts


	//set up VECTOR BASE ADDRESS REGISTER EL2 to =vector_table_el2
    LDR x2, =vector_table_el2
    MSR VBAR_EL2, x2

    //print VECTOR BASE ADDRESS REGISTER EL2
    MOV X0, #0 
	LDR X0, =EVTEL2HEADER
	BL uart_puts
	MOV X0, #0 
	MRS X0, VBAR_EL2
	BL uart_hex_uint64
	
	//print newline
	MOV X0, #0 
	LDR X0, =NL
	BL uart_puts




timer_boot:



	//print system counter frequency 	
	MOV X0, #0 
	LDR X0, =CTFEL0HEADER
	BL uart_puts
	MOV X0, #0
	MRS X0, CNTFRQ_EL0
	BL uart_hex_uint64

	//print newline
	MOV X0, #0 
	LDR X0, =NL
	BL uart_puts

	//print CNTPCT_EL0 register
	MOV X0, #0 
	LDR X0, =CTCVEL0HEADER
	BL uart_puts
	MOV X0, #0
	MRS X0, CNTPCT_EL0
	BL uart_hex_uint64

	//print newline
	MOV X0, #0 
	LDR X0, =NL
	BL uart_puts

	//print CNTHP_TVAL_EL2 register	
	MOV X0, #0 
	LDR X0, =CHPTTVEL2HEADER
	BL uart_puts
	MOV X0, #0
	MRS X0, CNTHP_TVAL_EL2
	BL uart_hex_uint64

	//print newline
	MOV X0, #0 
	LDR X0, =NL
	BL uart_puts

	//print CNTHP_TVAL_EL2 register	
	MOV X0, #0 
	LDR X0, =CHPTCVEL2HEADER
	BL uart_puts
	MOV X0, #0
	MRS X0, CNTHP_CVAL_EL2
	BL uart_hex_uint64

	//print newline
	MOV X0, #0 
	LDR X0, =NL
	BL uart_puts

	//print system CNTHP_CTL_EL2 register	
	MOV X0, #0
	LDR X0, =CHPTEL2HEADER
	BL uart_puts
	MOV X0, #0
	MRS X0, CNTHP_CTL_EL2
	BL uart_hex_uint64

	//print newline
	MOV X0, #0 
	LDR X0, =NL
	BL uart_puts

	//Set CNTP_TVAL_EL0 (CNTP_CVAL_EL0 = CNTPCT_EL0 + CNTP_TVAL_EL0)  
	MOV X0, #1000
	LSL X0, X0, #63
	MSR CNTP_TVAL_EL0, X0

	//Set CNTHP_CTL_EL2.ENABLE bit 
	MOV X0, #1
	MSR CNTP_CTL_EL0, X0

	//print system CNTHP_CTL_EL2 register	
	MOV X0, #0
	LDR X0, =CHPTEL2HEADER
	BL uart_puts
	MOV X0, #0
	MRS X0, CNTP_CTL_EL0
	BL uart_hex_uint64


	//print newline
	MOV X0, #0 
	LDR X0, =NL
	BL uart_puts

	//print CNTPCT_EL0 register
	MOV X0, #0 
	LDR X0, =CTCVEL0HEADER
	BL uart_puts
	MOV X0, #0
	MRS X0, CNTPCT_EL0
	BL uart_hex_uint64

	//print newline
	MOV X0, #0 
	LDR X0, =NL
	BL uart_puts

	//print CNTHP_TVAL_EL2 register	
	MOV X0, #0 
	LDR X0, =CHPTTVEL2HEADER
	BL uart_puts
	MOV X0, #0
	MRS X0, CNTHP_TVAL_EL2
	BL uart_hex_uint64

	//print newline
	MOV X0, #0 
	LDR X0, =NL
	BL uart_puts

	//print CNTHP_TVAL_EL2 register	
	MOV X0, #0 
	LDR X0, =CHPTCVEL2HEADER
	BL uart_puts
	MOV X0, #0
	MRS X0, CNTHP_CVAL_EL2
	BL uart_hex_uint64


	//print newline
	MOV X0, #0
	LDR X0, =NL
	BL uart_puts

	MOV X0, #4
	LDR X1, =TIMER_IRQ_CTRL
  	STR X0, [X1] // enable timer interrupt


	LDP X29, X30, [SP], #16

	RET




//Exception Vector Table EL2
.balign 0x800
vector_table_el2:
curr_el2_sp0_sync:
// The exception handler for the synchronous
// exception from the current EL using SP0.
	//print nExcp = 0
	MOV X0, #0
	MRS x1, ESR_EL2
    MRS x2, ELR_EL2
    MRS x3, SPSR_EL2
    MRS x4, FAR_EL2
	MRS x5, CURRENTEL
	AND x5, x5, #12
	LSR x5, x5, #2
    BL exception_handler
	ERET
.balign 0x80
curr_el2_sp0_irq:
// The exception handler for the IRQ exception
// from the current EL using SP0.
	//print nExcp = 1
	MOV X0, #1
	MRS x1, ESR_EL2
    MRS x2, ELR_EL2
    MRS x3, SPSR_EL2
    MRS x4, FAR_EL2
	MRS x5, CURRENTEL
	AND x5, x5, #12
	LSR x5, x5, #2
    BL exception_handler
	ERET
.balign 0x80
curr_el2_sp0_fiq:
// The exception handler for the FIQ exception
// from the current EL using SP0.
	//print nExcp = 2
	MOV X0, #2 
	MRS x1, ESR_EL2
    MRS x2, ELR_EL2
    MRS x3, SPSR_EL2
    MRS x4, FAR_EL2
	MRS x5, CURRENTEL
	AND x5, x5, #12
	LSR x5, x5, #2
    BL exception_handler
	ERET
.balign 0x80
curr_el2_sp0_serror:
// The exception handler for the system error
// exception from the current EL using SP0.
	//print nExcp = 3
	MOV X0, #3 
	MRS x1, ESR_EL2
    MRS x2, ELR_EL2
    MRS x3, SPSR_EL2
    MRS x4, FAR_EL2
	MRS x5, CURRENTEL
	AND x5, x5, #12
	LSR x5, x5, #2
    BL exception_handler
	ERET
.balign 0x80
curr_el2_spx_sync:
// The exception handler for the synchronous
// exception from the current EL using the
// current SP.
	//print nExcp = 4
	MOV X0, #4
	MRS x1, ESR_EL2
    MRS x2, ELR_EL2
    MRS x3, SPSR_EL2
    MRS x4, FAR_EL2
	MRS x5, CURRENTEL
	AND x5, x5, #12
	LSR x5, x5, #2
    BL exception_handler
	ERET
.balign 0x80
curr_el2_spx_irq:
// The exception handler for IRQ exception
// from the current EL using the current SP.
	
	//print nExcp = 5
	MOV X0, #5
	MRS x1, ESR_EL2
    MRS x2, ELR_EL2
    MRS x3, SPSR_EL2
    MRS x4, FAR_EL2
	MRS x5, CURRENTEL
	AND x5, x5, #12
	LSR x5, x5, #2
    BL exception_handler
	
	ERET

.balign 0x80
curr_el2_spx_fiq:
// The exception handler for the FIQ exception
// from the current EL using the current SP.
	//print nExcp = 6
	MOV X0, #6
	MRS x1, ESR_EL2
    MRS x2, ELR_EL2
    MRS x3, SPSR_EL2
    MRS x4, FAR_EL2
	MRS x5, CURRENTEL
	AND x5, x5, #12
	LSR x5, x5, #2
    BL exception_handler
	ERET
.balign 0x80
curr_el2_spx_serror:
// The exception handler for the system error
// exception from the current EL using the
// current SP.
	//print nExcp = 7
	MOV X0, #7 
	MRS x1, ESR_EL2
    MRS x2, ELR_EL2
    MRS x3, SPSR_EL2
    MRS x4, FAR_EL2
	MRS x5, CURRENTEL
	AND x5, x5, #12
	LSR x5, x5, #2
    BL exception_handler
	ERET
.balign 0x80
lower_el2_aarch64_sync:
// The exception handler for the synchronous
// exception from a lower EL (AArch64).
	MOV X0, #8
    MRS x1, ESR_EL2
    MRS x2, ELR_EL2
    MRS x3, SPSR_EL2
    MRS x4, FAR_EL2
	MRS x5, CURRENTEL
	AND x5, x5, #12
	LSR x5, x5, #2
    BL exception_handler
	ERET
.balign 0x80
lower_el2_aarch64_irq:
// The exception handler for the IRQ exception
// from a lower EL (AArch64).
	//print nExcp = 9
	MOV X0, #9 
	MRS x1, ESR_EL2
    MRS x2, ELR_EL2
    MRS x3, SPSR_EL2
    MRS x4, FAR_EL2
	MRS x5, CURRENTEL
	AND x5, x5, #12
	LSR x5, x5, #2
    BL exception_handler
	ERET
.balign 0x80
lower_el2_aarch64_fiq:
// The exception handler for the FIQ exception
// from a lower EL (AArch64).
	//print nExcp = 10
	MOV X0, #10
	MRS x1, ESR_EL2
    MRS x2, ELR_EL2
    MRS x3, SPSR_EL2
    MRS x4, FAR_EL2
	MRS x5, CURRENTEL
	AND x5, x5, #12
	LSR x5, x5, #2
    BL exception_handler
	ERET
.balign 0x80
lower_el2_aarch64_serror: // The exception handler for the system error
// exception from a lower EL(AArch64).
	//print nExcp = 11
	MOV X0, #11
	MRS x1, ESR_EL2
    MRS x2, ELR_EL2
    MRS x3, SPSR_EL2
    MRS x4, FAR_EL2
	MRS x5, CURRENTEL
	AND x5, x5, #12
	LSR x5, x5, #2
    BL exception_handler
	ERET
.balign 0x80
lower_el2_aarch32_sync:
// The exception handler for the synchronous
// exception from a lower EL(AArch32).
	//print nExcp = 12
	MOV X0, #12
	MRS x1, ESR_EL2
    MRS x2, ELR_EL2
    MRS x3, SPSR_EL2
    MRS x4, FAR_EL2
	MRS x5, CURRENTEL
	AND x5, x5, #12
	LSR x5, x5, #2
    BL exception_handler
	ERET
.balign 0x80
lower_el2_aarch32_irq:
// The exception handler for the IRQ exception
// from a lower EL (AArch32).
	//print nExcp = 13
	MOV X0, #13
	MRS x1, ESR_EL2
    MRS x2, ELR_EL2
    MRS x3, SPSR_EL2
    MRS x4, FAR_EL2
	MRS x5, CURRENTEL
	AND x5, x5, #12
	LSR x5, x5, #2
    BL exception_handler
	ERET
.balign 0x80
lower_el2_aarch32_fiq:
// The exception handler for the FIQ exception
// from a lower EL (AArch32).
	//print nExcp = 14
	MOV X0, #14
	MRS x1, ESR_EL2
    MRS x2, ELR_EL2
    MRS x3, SPSR_EL2
    MRS x4, FAR_EL2
	MRS x5, CURRENTEL
	AND x5, x5, #12
	LSR x5, x5, #2
    BL exception_handler
	ERET
.balign 0x80
lower_el2_aarch32_serror: // The exception handler for the system error
// exception from a lower EL(AArch32).
	//print nExcp = 15
	MOV X0, #15
	MRS x1, ESR_EL2
    MRS x2, ELR_EL2
    MRS x3, SPSR_EL2
    MRS x4, FAR_EL2
	MRS x5, CURRENTEL
	AND x5, x5, #12
	LSR x5, x5, #2
    BL exception_handler
	ERET


