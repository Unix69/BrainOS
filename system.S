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
.file "system.S"


.section ".text.system"
.data
.align  2

BOOTDIV:			.asciz "--------------------------------------------------------------------\n"
BOOTMODEHEADER:		.asciz "System Bootmode "
BOOTHEADER:			.asciz "System Boot\n"
EVTEL1HEADER:		.asciz "Vector Table EL1 Base Address "
CURELHEADER:		.asciz "Current EL "
EXCHEADER:			.asciz "Excpetion\n"
CPUNHEADER:			.asciz "CPU number "
EL0IEHEADER:		.asciz "EL0 instruction enabled\n"
EL1IEHEADER:		.asciz "EL1 instruction enabled\n"
EL2IEHEADER:		.asciz "EL2 instruction enabled\n"
EL3IEHEADER:		.asciz "EL3 instruction enabled\n"
EL0INIHEADER:		.asciz "El0 instruction not implemented\n"
EL1INIHEADER:		.asciz "EL1 instruction not implemented\n"
EL2INIHEADER:		.asciz "EL2 instruction not implemented\n"
EL3INIHEADER:		.asciz "EL3 instruction not implemented\n"
FPNIHEADER:			.asciz "Floating Point not implemented\n"
FPEHEADER:			.asciz "Floating Point enabled\n"
SIMDNIHEADER:		.asciz "SIMD not implemented\n"
SIMDEHEADER:		.asciz "SIMD enabled\n"
GICNIHEADER:		.asciz "GIC CPU Interface not implemented\n"
GICEHEADER:			.asciz "GIC CPU Interface enabled\n"
MTENIHEADER:		.asciz "MTE not implemented\n"
MTEEHEADER:			.asciz "MTE enabled\n"
SBSSNIHEADER:		.asciz "SSBS not implemented\n"
SBSSEHEADER:		.asciz "SSBS enabled\n"
MPAMNIHEADER:		.asciz "MPAM not implemented\n"
MPAMEHEADER:		.asciz "MPAM enabled\n"
RASNIHEADER:		.asciz "RAS not implemented\n"
RASEHEADER:			.asciz "RAS enabled\n"
MPAMFRACNIHEADER:	.asciz "Fractional MPAM not implemented\n"
MPAMFRACEHEADER:	.asciz "Fractional MPAM enabled\n"
RASFRACNIHEADER:	.asciz "Fractional RAS not implemented\n"
RASFRACEHEADER:		.asciz "Fractional RAS enabled\n"
CSV2FRACNIHEADER:	.asciz "Fractional CSV2 not implemented\n"
CSV2FRACEHEADER:	.asciz "Fractional CSV2 enabled\n"
CSV2NIHEADER:		.asciz "CSV2 not implemented\n"
CSV2EHEADER:		.asciz "CSV2 enabled\n"
CSV3NIHEADER:		.asciz "CSV3 not implemented\n"
CSV3EHEADER:		.asciz "CSV3 enabled\n"
SVENIHEADER:		.asciz "SVE not implemented\n"
SVEEHEADER:			.asciz "SVE enabled\n"
SEL2NIHEADER:		.asciz "SEL2 not implemented\n"
SEL2EHEADER:		.asciz "SEL2 enabled\n"
BTNIHEADER:			.asciz "BT not implemented\n"
BTEHEADER:			.asciz "BT enabled\n"
DITNIHEADER:		.asciz "DIT not implemented\n"
DITEHEADER:			.asciz "DIT enabled\n"
AMUNIHEADER:		.asciz "Active Monitor Unit not implemented\n"
AMUEHEADER:			.asciz "Active Monitor Unit enabled\n"
F64MMNIHEADER:		.asciz "F64MM not implemented\n"
F64MMEHEADER:		.asciz "F64MM enabled\n"
F32MMNIHEADER:		.asciz "F32MM not implemented\n"
F32MMEHEADER:		.asciz "F32MM enabled\n"
BF16NIHEADER:		.asciz "BF16 not implemented\n"
BF16EHEADER:		.asciz "BF16 enabled\n"
I8MMNIHEADER:		.asciz "I8MM not implemented\n"
I8MMEHEADER:		.asciz "I8MM enabled\n"
SVEVERNIHEADER:		.asciz "SVE instructions are not implemented\n"
SVEVEREHEADER:		.asciz "SVE instructions enabled\n"
UNITIDHEADER:		.asciz "Unit/Multiproc "
MTEHEADER:			.asciz "MT enabled\n"
MTNIHEADER:			.asciz "MT not implemented\n"
AFF3LHEADER:		.asciz "Affinity level 3  "
AFF2LHEADER:		.asciz "Affinity level 2  "
AFF1LHEADER:		.asciz "Affinity level 1  "
AFF0LHEADER:		.asciz "Affinity level 0  "
TIDEL1HEADER:		.asciz "Software thread Register EL1 "
SPEL1HEADER:		.asciz "Stack Pointer EL1 "
SPSELHEADER:		.asciz "Stack Pointer Selector "
SCTLREL1HEADER:		.asciz "System Control Register EL1 "
DAIFHEADER:			.asciz "DAIF flag "
NL:					.asciz "\n"
CR:					.asciz "\r"
TB:					.asciz "\t"


.text
.global system_boot




system_boot:

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
	LDR X0, =TIDEL1HEADER
	BL uart_puts
	MOV X0, #0
	MRS X0, TPIDR_EL1
	AND X0, X0, #3
	BL uart_hex_uint64


	//print newline
	MOV X0, #0 
	LDR X0, =NL
	BL uart_puts

	


	//print STACK POINTER SELECTOR
	MOV X0, #0 
	LDR X0, =SPSELHEADER
	BL uart_puts
	MOV X0, #0 
	MRS X0, SPSEL
	BL uart_hex_uint64

	//print newline
	MOV X0, XZR 
	LDR X0, =NL
	BL uart_puts



	
	//set up SYSTEM CONTROL REGISTER EL1
	MRS X0, SCTLR_EL1
	//ORR X0, X0, #1 // The M bit. When HCR_EL2.{E2H, TGE} is {1, 1}, this bit has no effect.
	ORR X0, X0, #(1<<1) // The A bit.  When HCR_EL2.{E2H, TGE} is {1, 1}, this bit has no effect on EL0.
	MOV x1, X0
	ORR x1, x1, #(1<<2)
	AND X0, X0, x1 // The C bit. When HCR_EL2.{E2H, TGE} != {1, 1}, this bit has no effect.
	ORR X0, X0, #(1<<3) // The SA bit.  When HCR_EL2.{E2H, TGE} is {1, 1}, this bit has no effect.
	ORR X0, X0, #(1<<4) // The SA0 bit.  When HCR_EL2.{E2H, TGE} is {1, 1}, this bit has no effect on EL0.
	ORR X0, X0, #(1<<5) // The CP15BEN bit.	 When HCR_EL2.{E2H, TGE} is {1, 1}, this bit has no effect on EL0.
	MOV x1, X0
	ORR x1, x1, #(1<<6)
	AND X0, X0, x1 // The nAA bit.When HCR_EL2.{E2H, TGE} is {1, 1}, this bit has no effect on EL0.
	MOV x1, X0
	ORR x1, x1, #(1<<7)
	AND X0, X0, x1 // The ITD bit. When HCR_EL2.{E2H, TGE} is {1, 1}, this bit has no effect on EL0.
	MOV x1, X0
	ORR x1, x1, #(1<<8)
	AND X0, X0, x1 // The SED bit.When HCR_EL2.{E2H, TGE} is {1, 1}, this bit has no effect on EL0.
	ORR X0, X0, #(1<<10) // The EnRCTX bit. When HCR_EL2.{E2H, TGE} is {1, 1}, this bit has no effect on EL0.
	ORR X0, X0, #(1<<11) // The EOS bit. When HCR_EL2.{E2H, TGE} is {1, 1}, this bit has no effect on EL0.
	MOV x1, X0
	ORR x1, x1, #(1<<12)
	AND X0, X0, x1 // The I bit. When HCR_EL2.{E2H, TGE} is {1, 1}, this bit has no effect.
	MOV x1, X0
	ORR x1, x1, #(1<<14)
	AND X0, X0, x1 // The DZE bit. When HCR_EL2.{E2H, TGE} is {1, 1}, this bit has no effect on EL0.
	MOV x1, X0
	ORR x1, x1, #(1<<15)
	AND X0, X0, x1 // The UCT bit. When HCR_EL2.{E2H, TGE} is {1, 1}, this bit has no effect on EL0.
	MOV x1, X0
	ORR x1, x1, #(1<<16)
	AND X0, X0, x1 // The nTWI bit. When HCR_EL2.{E2H, TGE} is {1, 1}, this bit has no effect on EL0.
	MOV x1, X0
	ORR x1, x1, #(1<<18)
	AND X0, X0, x1 // The nTWE bit. When HCR_EL2.{E2H, TGE} is {1, 1}, this bit has no effect on EL0.
	ORR X0, X0, #(1<<19) // The WXN bit. When HCR_EL2.{E2H, TGE} is {1, 1}, this bit has no effect.
	ORR X0, X0, #(1<<20) // The TSCXT bit. When HCR_EL2.{E2H, TGE} is {1, 1}, this bit has no effect on EL0.
	ORR X0, X0, #(1<<21) // The IESB bit.
	MOV x1, X0
	ORR x1, x1, #(1<<22) 
	AND X0, X0, x1 // The EIS bit. When HCR_EL2.{E2H, TGE} is {1, 1}, this bit has no effect on EL0.
	ORR X0, X0, #(1<<23) // The SPAN bit. When HCR_EL2.{E2H, TGE} is {1, 1}, this bit has no effect on EL0.
	ORR X0, X0, #(1<<24) // The E0E bit. When HCR_EL2.{E2H, TGE} is {1, 1}, this bit has no effect on EL0.
	//ORR X0, X0, #(1<<25) // The EE bit.
	MOV x1, X0
	ORR x1, x1, #(1<<26)
	AND X0, X0, x1 // The BT0 bit. When HCR_EL2.{E2H, TGE} is {1, 1}, this bit has no effect on EL0.
	ORR X0, X0, #(1<<28) // The nTLSMD bit. When HCR_EL2.{E2H, TGE} is {1, 1}, this bit has no effect on EL0.
	ORR X0, X0, #(1<<29) // The LSMAOE, bit. When HCR_EL2.{E2H, TGE} is {1, 1}, this bit has no effect on EL0.
	MOV x1, X0
	ORR x1, x1, #(1<<35)
	AND X0, X0, x1 // The BT0 bit. When HCR_EL2.{E2H, TGE} is {1, 1}, this bit has no effect on EL0.
	MOV x1, X0
	ORR x1, x1, #(1<<36)
	AND X0, X0, x1 // The BT bit.
	ORR X0, X0, #(1<<37) // The ITFSB bit.
	ORR X0, X0, #(1<<38)
	MOV x1, X0
	ORR x1, x1, #(1<<39)
	AND X0, X0, x1 // The TCF0, bits [39:38]. When HCR_EL2.{E2H, TGE} is {1, 1}, this bit has no effect on EL0.
	ORR X0, X0, #(1<<40)
	MOV x1, X0
	ORR x1, x1, #(1<<41)
	AND X0, X0, x1 // The TCF, bits [41:40].
	ORR X0, X0, #(1<<42) // The ATA0 bit. When SCR_EL3.ATA=1, HCR_EL2.ATA=1, and HCR_EL2.{E2H, TGE} != {1, 1}, controls EL0 access to Allocation Tags.
	ORR X0, X0, #(1<<43) // The ATA bit.
	ORR X0, X0, #(1<<45) // The TWEDEn bit.
	MOV x1, X0
	ORR x1, x1, #(1<<46)
	ORR x1, x1, #(1<<47)
	ORR x1, x1, #(1<<48)
	ORR x1, x1, #(1<<49)
	AND X0, X0, x1 // The TWEDEL bits [49:46]
	ORR X0, X0, #(1<<54) // The ENASR bit. When HCR_EL2.{E2H, TGE} != {1, 1}, traps execution of an ST64BV instruction at EL0 to EL1.
	ORR X0, X0, #(1<<55) // The ENAS0 bit. When HCR_EL2.{E2H, TGE} != {1, 1}, traps execution of an ST64BV0 instruction at EL0 to EL1.
	ORR X0, X0, #(1<<56) // The ENALS bit. When HCR_EL2.{E2H, TGE} != {1, 1}, traps execution of an LD64B or ST64B instruction at EL0 to EL1.
	ORR X0, X0, #(1<<57) // The EPAN bit.
	MSR SCTLR_EL1, X0

	
	//print SYSTEM CONTROL REGISTER EL1
	MOV X0, #0
	LDR X0, =SCTLREL1HEADER
	BL uart_puts

	MOV X0, #0 
	MRS X0, SCTLR_EL1
	BL uart_hex_uint64
	
	//print newline
	MOV X0, #0
	LDR X0, =NL
	BL uart_puts

	
	//set DAIF Flag to 1111
    MOV X0, #0b1111
	LSL X0, X0, #6  
    MSR DAIF, X0

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
	
    	
    //print VECTOR BASE ADDRESS REGISTER EL1
    MOV X0, #0 
	LDR X0, =EVTEL1HEADER
	BL uart_puts
	MOV X0, #0 
	MRS X0, VBAR_EL1
	BL uart_hex_uint64


    //print newline
	MOV X0, #0 
	LDR X0, =NL
	BL uart_puts


aff3_level:
	//test MPIDR_EL1 register affinity 2 level Bit
	MOV X0, #0
	LDR X0, =AFF2LHEADER			
	BL uart_puts
	MOV X0, #0
	MRS X0, MPIDR_EL1
	ANDS X0, X0, #0X000000FF00000000
	LSR X0, X0, #32
	BL uart_hex_uint64

	//print newline
	MOV X0, #0 
	LDR X0, =NL
	BL uart_puts



mt_test:
	//test MPIDR_EL1 register sve instructions enable Bit
	MOV X0, #0
	MRS X0, MPIDR_EL1
	ANDS X0, X0, #0X0000000001000000
	CBNZ X0, mt_enabled
mt_not_implemented:	
	LDR X0, =MTNIHEADER			
	BL uart_puts
	BL aff2_level
mt_enabled:					
	LDR X0, =MTEHEADER			
	BL uart_puts

	//print newline
	MOV X0, #0
	LDR X0, =NL
	BL uart_puts

aff2_level:
	//test MPIDR_EL1 register affinity 2 level Bit
	MOV X0, #0
	LDR X0, =AFF2LHEADER			
	BL uart_puts
	MOV X0, #0
	MRS X0, MPIDR_EL1
	ANDS X0, X0, #0X0000000000FF0000
	LSR X0, X0, #16
	BL uart_hex_uint64

	//print newline
	MOV X0, #0 
	LDR X0, =NL
	BL uart_puts

aff1_level:
	//test MPIDR_EL1 register affinity 1 level Bit
	MOV X0, #0
	LDR X0, =AFF1LHEADER			
	BL uart_puts
	MOV X0, #0
	MRS X0, MPIDR_EL1
	ANDS X0, X0, #0X000000000000FF00
	LSR X0, X0, #8
	BL uart_hex_uint64

	//print newline
	MOV X0, #0 
	LDR X0, =NL
	BL uart_puts


aff0_level:
	//test MPIDR_EL1 register affinity 0 level Bit
	MOV X0, #0
	LDR X0, =AFF0LHEADER			
	BL uart_puts
	MOV X0, #0
	MRS X0, MPIDR_EL1
	ANDS X0, X0, #0X00000000000000FF
	BL uart_hex_uint64

	//print newline
	MOV X0, #0 
	LDR X0, =NL
	BL uart_puts

/*
sve_test:
	//test ID_AA64ZFR0_EL1 register sve instructions enable Bit
	MOV X0, #0
	MRS X0, ID_AA64ZFR0_EL1
	ANDS X0, X0, #0X000000000000000F
	CBNZ X0, sve_enabled
sve_not_implemented:	
	LDR X0, =SVENIHEADER			
	BL uart_puts
	BL bf16_test
sve_enabled:					//SVE instructions are implemented.
	LDR X0, =SVEEHEADER			
	BL uart_puts


bf16_test:
	//test ID_AA64ZFR0_EL1 register bf16 Bit
	MOV X0, #0
	MRS X0, ID_AA64ZFR0_EL1
	ANDS X0, X0, #0X0000000000F00000
	CBNZ X0, bf16_enabled
bf16_not_implemented:	
	LDR X0, =BF16NIHEADER			
	BL uart_puts
	BL i8mm_test
bf16_enabled:					//BFCVT, BFCVTNT, BFDOT, BFMLALB, BFMLALT, and BFMMLA instructions are implemented.
	LDR X0, =BF16EHEADER			
	BL uart_puts



i8mm_test:
	//test ID_AA64ZFR0_EL1 register i8mm Bit
	MOV X0, #0
	MRS X0, ID_AA64ZFR0_EL1
	ANDS X0, X0, #0X0000F00000000000
	CBNZ X0, i8mm_enabled
i8mm_not_implemented:	
	LDR X0, =I8MMNIHEADER			
	BL uart_puts
	BL f32mm_test
i8mm_enabled:					//SMMLA, SUDOT, UMMLA, USMMLA, and USDOT instructions are implemented.
	LDR X0, =I8MMEHEADER			
	BL uart_puts


f32mm_test:
	//test ID_AA64ZFR0_EL1 register f32mm Bit
	MOV X0, #0
	MRS X0, ID_AA64ZFR0_EL1
	ANDS X0, X0, #0X00F0000000000000
	CBNZ X0, f32mm_enabled
f32mm_not_implemented:	
	LDR X0, =F32MMNIHEADER			
	BL uart_puts
	BL f64mm_test
f32mm_enabled:					//FP32 variant of the FMMLA instruction is implemented
	LDR X0, =F32MMEHEADER			
	BL uart_puts

f64mm_test:
	//test ID_AA64ZFR0_EL1 register f64mm Bit
	MOV X0, #0
	MRS X0, ID_AA64ZFR0_EL1
	ANDS X0, X0, #0X0F00000000000000
	CBNZ X0, f64mm_enabled
f64mm_not_implemented:	
	LDR X0, =F64MMNIHEADER			
	BL uart_puts
	BL gic_test
f64mm_enabled:					//FP64 variant of the FMMLA instruction, and LD1RO* instructions are implemented. The 128-bit element variations of TRN12,UZP12 amd ZIP12 are also implemented.
	LDR X0, =F64MMEHEADER			
	BL uart_puts
*/




gic_test:
	//test ID_AA64PFR0_EL1 register GIC Bit
	MOV X0, #0
	MRS X0, ID_AA64PFR0_EL1
	ANDS X0, X0, #0X0000000001000000
	CBNZ X0, gic_enabled
gic_not_implemented:	
	LDR X0, =GICNIHEADER			
	BL uart_puts
	BL mpam_test
gic_enabled:
	LDR X0, =GICEHEADER			
	BL uart_puts	

mpam_test:
	//test ID_AA64PFR0_EL1 register MPAM Bits
	MOV X0, #0
	MRS X0, ID_AA64PFR0_EL1
	ANDS X0, X0, #0X00000F0000000000
	CBNZ X0, mpam_enabled
mpam_not_implemented:	
	LDR X0, =MPAMNIHEADER			
	BL uart_puts
	BL ras_test
mpam_enabled:
	LDR X0, =MPAMEHEADER			
	BL uart_puts	


ras_test:
	//test ID_AA64PFR0_EL1 register RAS Bits
	MOV X0, #0
	MRS X0, ID_AA64PFR0_EL1
	ANDS X0, X0, #0X00000000F0000000
	CBNZ X0, ras_enabled
ras_not_implemented:	
	LDR X0, =RASNIHEADER			
	BL uart_puts
	BL amu_test
ras_enabled:
	LDR X0, =RASEHEADER			
	BL uart_puts	



amu_test:
	//test ID_AA64PFR0_EL1 register AMU Bits
	MOV X0, #0
	MRS X0, ID_AA64PFR0_EL1
	ANDS X0, X0, #0X0000F00000000000
	CBNZ X0, amu_enabled
amu_not_implemented:	
	LDR X0, =AMUNIHEADER			
	BL uart_puts
	BL sve_test
amu_enabled:
	LDR X0, =AMUEHEADER			
	BL uart_puts


sve_test:
	//test ID_AA64PFR0_EL1 register SVE Bits
	MOV X0, #0
	MRS X0, ID_AA64PFR0_EL1
	ANDS X0, X0, #0X0000000F00000000
	CBNZ X0, sve_enabled
sve_not_implemented:	
	LDR X0, =SVENIHEADER			
	BL uart_puts
	BL dit_test
sve_enabled:
	LDR X0, =SVEEHEADER			
	BL uart_puts





dit_test:
	//test ID_AA64PFR0_EL1 register DIT Bits
	MOV X0, #0
	MRS X0, ID_AA64PFR0_EL1
	ANDS X0, X0, #0X000F000000000000
	CBNZ X0, dit_enabled
dit_not_implemented:	
	LDR X0, =DITNIHEADER			
	BL uart_puts
	BL sel2_test
dit_enabled:
	LDR X0, =DITEHEADER		
	BL uart_puts


sel2_test:
	//test ID_AA64PFR0_EL1 register SEL2 Bits
	MOV X0, #0
	MRS X0, ID_AA64PFR0_EL1
	ANDS X0, X0, #0X000000F000000000
	CBNZ X0, sel2_enabled
sel2_not_implemented:	
	LDR X0, =SEL2NIHEADER			
	BL uart_puts
	BL csv2_test
sel2_enabled:
	LDR X0, =SEL2EHEADER		
	BL uart_puts



csv2_test:
	//test ID_AA64PFR0_EL1 register CSV2 Bits
	MOV X0, #0
	MRS X0, ID_AA64PFR0_EL1
	ANDS X0, X0, #0X0F00000000000000
	CBNZ X0, csv2_enabled
csv2_not_implemented:	
	LDR X0, =CSV2NIHEADER			
	BL uart_puts
	BL csv3_test
csv2_enabled:
	LDR X0, =CSV2EHEADER			
	BL uart_puts


csv3_test:
	//test ID_AA64PFR0_EL1 register CSV3 Bits
	MOV X0, #0
	MRS X0, ID_AA64PFR0_EL1
	ANDS X0, X0, #0XF000000000000000
	CBNZ X0, csv3_enabled
csv3_not_implemented:	
	LDR X0, =CSV3NIHEADER			
	BL uart_puts
	BL fp_test
csv3_enabled:
	LDR X0, =CSV3EHEADER			
	BL uart_puts

fp_test:
	//test ID_AA64PFR0_EL1 register FP Bit
	MOV X0, #0
	MRS X0, ID_AA64PFR0_EL1			
	ANDS X0, X0, 0X0000000000F00000
	CBNZ X0, fp_enabled
fp_not_implemented:	
	LDR X0, =FPNIHEADER			
	BL uart_puts
	BL simd_test
fp_enabled:	
	LDR X0, =FPEHEADER			
	BL uart_puts	
	
	
	
simd_test:
	//test ID_AA64PFR0_EL1 register SIMD Bit
	MOV X0, #0
	MRS X0, ID_AA64PFR0_EL1
	ANDS X0, X0, 0X00000000000F0000
	CBNZ X0, simd_enabled
simd_not_implemented:	
	LDR X0, =SIMDNIHEADER			
	BL uart_puts
	BL el3_instruction_enable_test 
simd_enabled:	
	LDR X0, =SIMDEHEADER			
	BL uart_puts	
	
	



el3_instruction_enable_test:
	//test ID_AA64PFR0_EL1 register Istruction excecutable in EL3
	MOV X0, #0
	MRS X0, ID_AA64PFR0_EL1
	ANDS X0, X0, #0X000000000002000
	CBNZ X0, el3_instruction_enabled
el3_instruction_not_implemented:	
	LDR X0, =EL3INIHEADER			
	BL uart_puts
	BL el2_instruction_enable_test
el3_instruction_enabled:	
	LDR X0, =EL3IEHEADER			
	BL uart_puts	
	
	

el2_instruction_enable_test:
	//test ID_AA64PFR0_EL1 register Istruction excecutable in EL2
	MOV X0, #0
	MRS X0, ID_AA64PFR0_EL1	
	ANDS X0, X0, #0X000000000000200
	CBNZ X0, el2_instruction_enabled
el2_instruction_not_implemented:	
	LDR X0, =EL2INIHEADER			
	BL uart_puts
	BL el1_instruction_enable_test
el2_instruction_enabled:	
	LDR X0, =EL2IEHEADER			
	BL uart_puts	
	
	
	
el1_instruction_enable_test:
	//test ID_AA64PFR0_EL1 register Istruction excecutable in EL1
	MOV X0, #0
	MRS X0, ID_AA64PFR0_EL1	
	ANDS X0, X0, #0X0000000000000020
	CBNZ X0, el1_instruction_enabled
el1_instruction_not_implemented:	
	LDR X0, =EL1INIHEADER			
	BL uart_puts
	BL el0_instruction_enable_test
el1_instruction_enabled:	
	LDR X0, =EL1IEHEADER			
	BL uart_puts	

	
	
el0_instruction_enable_test:
	//test ID_AA64PFR0_EL1 register Istruction excecutable in EL0
	MOV X0, #0
	MRS X0, ID_AA64PFR0_EL1
	ANDS X0, X0, #0X000000000000002
	CBNZ X0, el0_instruction_enabled
el0_instruction_not_implemented:	
	LDR X0, =EL0INIHEADER			
	BL uart_puts
	BL csv2_frac_test
el0_instruction_enabled:	
	LDR X0, =EL0IEHEADER			
	BL uart_puts




csv2_frac_test:
	//test ID_AA64PFR1_EL1 register CSV2 fractional field Bits
	MOV X0, #0
	MRS X0, ID_AA64PFR1_EL1
	ANDS X0, X0, #0X0000000F00000000
	CBNZ X0, csv2_frac_enabled
csv2_frac_not_implemented:	
	LDR X0, =CSV2FRACNIHEADER			
	BL uart_puts
	BL mpam_frac_test
csv2_frac_enabled:
	LDR X0, =CSV2FRACEHEADER			
	BL uart_puts

mpam_frac_test:
	//test ID_AA64PFR1_EL1 register MPAM fractional field Bits
	MOV X0, #0
	MRS X0, ID_AA64PFR1_EL1
	ANDS X0, X0, #0X0000000F00000000
	CBNZ X0, mpam_frac_enabled
mpam_frac_not_implemented:	
	LDR X0, =MPAMFRACNIHEADER			
	BL uart_puts
	BL ras_frac_test
mpam_frac_enabled:
	LDR X0, =MPAMFRACEHEADER			
	BL uart_puts

ras_frac_test:
	//test ID_AA64PFR1_EL1 register RAS fractional field Bits
	MOV X0, #0
	MRS X0, ID_AA64PFR1_EL1
	ANDS X0, X0, #0X0000000F00000000
	CBNZ X0, ras_frac_enabled
ras_frac_not_implemented:	
	LDR X0, =RASFRACNIHEADER			
	BL uart_puts
	BL mte_test
ras_frac_enabled:
	LDR X0, =RASFRACEHEADER			
	BL uart_puts


mte_test:
	//test ID_AA64PFR1_EL1 register RAS fractional field Bits
	MOV X0, #0
	MRS X0, ID_AA64PFR1_EL1
	ANDS X0, X0, #0X0000000F00000000
	CBNZ X0, mte_enabled
mte_not_implemented:	
	LDR X0, =MTENIHEADER			
	BL uart_puts
	BL sbss_test
mte_enabled:
	LDR X0, =MTEEHEADER			
	BL uart_puts


sbss_test:
	//test ID_AA64PFR1_EL1 register SBSS field Bits
	MOV X0, #0
	MRS X0, ID_AA64PFR1_EL1
	ANDS X0, X0, #0X0000000F00000000
	CBNZ X0, sbss_enabled
sbss_not_implemented:	
	LDR X0, =SBSSNIHEADER			
	BL uart_puts
	BL bt_test
sbss_enabled:
	LDR X0, =SBSSEHEADER			
	BL uart_puts



bt_test:
	//test ID_AA64PFR1_EL1 register BT fractional field Bits
	MOV X0, #0
	MRS X0, ID_AA64PFR1_EL1
	ANDS X0, X0, #0X0000000F00000000
	CBNZ X0, bt_enabled
bt_not_implemented:	
	LDR X0, =BTNIHEADER			
	BL uart_puts
	BL boot_exit
bt_enabled:
	LDR X0, =BTEHEADER			
	BL uart_puts

	

boot_exit:

	//print newline
	MOV X0, #0 
	LDR X0, =NL
	BL uart_puts


	LDP X29, X30, [SP], #16



	RET




//Exception Vector Table EL1
.balign 0x800
vector_table_el1:
curr_el1_sp0_sync:
// The exception handler for the synchronous
// exception from the current EL using SP0.
	//print nExcp = 0
	MOV X0, #0 
	MRS x1, ESR_EL1
    MRS x2, ELR_EL1
    MRS x3, SPSR_EL1
    MRS x4, FAR_EL1
	MRS x5, CURRENTEL
	AND x5, x5, #12
	LSR x5, x5, #2
	BL exception_handler
	ERET
.balign 0x80
curr_el1_sp0_irq:
// The exception handler for the IRQ exception
// from the current EL using SP0.
	//print nExcp = 1
	MOV X0, #1 
	MRS x1, ESR_EL1
    MRS x2, ELR_EL1
    MRS x3, SPSR_EL1
    MRS x4, FAR_EL1
	MRS x5, CURRENTEL
	AND x5, x5, #12
	LSR x5, x5, #2
	BL exception_handler
	ERET
.balign 0x80
curr_el1_sp0_fiq:
// The exception handler for the FIQ exception
// from the current EL using SP0.
	//print nExcp = 2
	MOV X0, #3 
	MRS x1, ESR_EL1
    MRS x2, ELR_EL1
    MRS x3, SPSR_EL1
    MRS x4, FAR_EL1
	MRS x5, CURRENTEL
	AND x5, x5, #12
	LSR x5, x5, #2
	BL exception_handler	
	ERET
.balign 0x80
curr_el1_sp0_serror:
// The exception handler for the system error
// exception from the current EL using SP0.
	//print nExcp = 3
	MOV X0, #3 
	MRS x1, ESR_EL1
    MRS x2, ELR_EL1
    MRS x3, SPSR_EL1
    MRS x4, FAR_EL1
	MRS x5, CURRENTEL
	AND x5, x5, #12
	LSR x5, x5, #2
	BL exception_handler
	ERET
.balign 0x80
curr_el1_spx_sync:
// The exception handler for the synchronous
// exception from the current EL using the
// current SP.
	//print nExcp = 4
	MOV X0, #4 
	MRS x1, ESR_EL1
    MRS x2, ELR_EL1
    MRS x3, SPSR_EL1
    MRS x4, FAR_EL1
	MRS x5, CURRENTEL
	AND x5, x5, #12
	LSR x5, x5, #2
	BL exception_handler	
	ERET
.balign 0x80
curr_el1_spx_irq:
// The exception handler for IRQ exception
// from the current EL using the current SP.
	//print nExcp = 5
	MOV X0, #5 
	MRS x1, ESR_EL1
    MRS x2, ELR_EL1
    MRS x3, SPSR_EL1
    MRS x4, FAR_EL1
	MRS x5, CURRENTEL
	AND x5, x5, #12
	LSR x5, x5, #2
	BL exception_handler
	ERET
.balign 0x80
curr_el1_spx_fiq:
// The exception handler for the FIQ exception
// from the current EL using the current SP.
	//print nExcp = 6
	MOV X0, #6 
	MRS x1, ESR_EL1
    MRS x2, ELR_EL1
    MRS x3, SPSR_EL1
    MRS x4, FAR_EL1
	MRS x5, CURRENTEL
	AND x5, x5, #12
	LSR x5, x5, #2
	BL exception_handler
	ERET
.balign 0x80
curr_el1_spx_serror:
// The exception handler for the system error
// exception from the current EL using the
// current SP.
	//print nExcp = 7
	MOV X0, #7
	MRS x1, ESR_EL1
    MRS x2, ELR_EL1
    MRS x3, SPSR_EL1
    MRS x4, FAR_EL1
	MRS x5, CURRENTEL
	AND x5, x5, #12
	LSR x5, x5, #2
	BL exception_handler
	ERET
.balign 0x80
lower_el1_aarch64_sync:
// The exception handler for the synchronous
// exception from a lower EL (AArch64).
	//print nExcp = 8
	MOV X0, #8
    MRS x1, ESR_EL1
    MRS x2, ELR_EL1
    MRS x3, SPSR_EL1
    MRS x4, FAR_EL1
	MRS x5, CURRENTEL
	AND x5, x5, #12
	LSR x5, x5, #2
	BL exception_handler
	ERET
.balign 0x80
lower_el1_aarch64_irq:
// The exception handler for the IRQ exception
// from a lower EL (AArch64).
	//print nExcp = 9
	MOV X0, #9 
	MRS x1, ESR_EL1
    MRS x2, ELR_EL1
    MRS x3, SPSR_EL1
    MRS x4, FAR_EL1
	MRS x5, CURRENTEL
	AND x5, x5, #12
	LSR x5, x5, #2
	BL exception_handler
	ERET
.balign 0x80
lower_el1_aarch64_fiq:
// The exception handler for the FIQ exception
// from a lower EL (AArch64).
	//print nExcp = 10
	MOV X0, #10
	MRS x1, ESR_EL1
    MRS x2, ELR_EL1
    MRS x3, SPSR_EL1
    MRS x4, FAR_EL1
	MRS x5, CURRENTEL
	AND x5, x5, #12
	LSR x5, x5, #2
	BL exception_handler
	ERET
.balign 0x80
lower_el1_aarch64_serror: // The exception handler for the system error
// exception from a lower EL(AArch64).
	//print nExcp = 11
	MOV X0, #11
	MRS x1, ESR_EL1
    MRS x2, ELR_EL1
    MRS x3, SPSR_EL1
    MRS x4, FAR_EL1
	MRS x5, CURRENTEL
	AND x5, x5, #12
	LSR x5, x5, #2
	BL exception_handler
	ERET
.balign 0x80
lower_el1_aarch32_sync:
// The exception handler for the synchronous
// exception from a lower EL(AArch32).
	//print nExcp = 12
	MOV X0, #12
	MRS x1, ESR_EL1
    MRS x2, ELR_EL1
    MRS x3, SPSR_EL1
    MRS x4, FAR_EL1
	MRS x5, CURRENTEL
	AND x5, x5, #12
	LSR x5, x5, #2
	BL exception_handler
	ERET
.balign 0x80
lower_el1_aarch32_irq:
// The exception handler for the IRQ exception
// from a lower EL (AArch32).
	//print nExcp = 13
	MOV X0, #13
	MRS x1, ESR_EL1
    MRS x2, ELR_EL1
    MRS x3, SPSR_EL1
    MRS x4, FAR_EL1
	MRS x5, CURRENTEL
	AND x5, x5, #12
	LSR x5, x5, #2
	BL exception_handler
	ERET
.balign 0x80
lower_el1_aarch32_fiq:
// The exception handler for the FIQ exception
// from a lower EL (AArch32).
	//print nExcp = 14
	MOV X0, #14
	MRS x1, ESR_EL1
    MRS x2, ELR_EL1
    MRS x3, SPSR_EL1
    MRS x4, FAR_EL1
	MRS x5, CURRENTEL
	AND x5, x5, #12
	LSR x5, x5, #2
	BL exception_handler
	ERET
.balign 0x80
lower_el1_aarch32_serror: // The exception handler for the system error
// exception from a lower EL(AArch32).
	//print nExcp = 15
	MOV X0, #15
	MRS x1, ESR_EL1
    MRS x2, ELR_EL1
    MRS x3, SPSR_EL1
    MRS x4, FAR_EL1
	MRS x5, CURRENTEL
	AND x5, x5, #12
	LSR x5, x5, #2
	BL exception_handler
	ERET