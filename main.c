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
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES O
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */

#include "uart.h"
#include "delays.h"

#define BOOTMODE 64





void exception_handler(unsigned long type, unsigned long esr, unsigned long elr, unsigned long spsr, unsigned long far, unsigned long currel);
void print_uint64(uint64_t reg);
void print_uint32(uint32_t reg);
void delay_test();


void exception_handler(unsigned long type, unsigned long esr, unsigned long elr, unsigned long spsr, unsigned long far, unsigned long currel){
	switch(type) {
        	case 0: 
        		uart_puts("Synchronous from same EL using SP0 ");
        		break;
       		case 1: 
       			uart_puts("IRQ from same EL using SP0 ");
       			break;
        	case 2:
        		uart_puts("FIQ from same EL using SP0 ");
        		break;
        	case 3:
        		uart_puts("SError from same EL using SP0 ");
        		break;
        	case 4:
        		uart_puts("Synchronous from same EL using current SP ");
        		break;
       		case 5:
       			uart_puts("IRQ from same EL using current SP ");
       			break;
        	case 6:
        		uart_puts("FIQ from same EL using current SP ");
        		break;
        	case 7:
        		uart_puts("SError from same EL using current SP ");
        		break;
        	case 8:
        		uart_puts("Synchronous from lower EL using SP0 ");
        		break;
       		case 9:
       			uart_puts("IRQ from lower EL using SP0 ");
       			break;
        	case 10:
        		uart_puts("FIQ from lower EL AArch64 ");
        		break;
        	case 11:
        		uart_puts("SError from lower EL AArch64 ");
        		break;
        	case 12:
        		uart_puts("Synchronous from lower EL AArch32 ");
        		break;
       		case 13:
       			uart_puts("IRQ from lower EL AArch32 ");
       			break;
        	case 14:
        		uart_puts("FIQ from lower EL AArch32 ");
        		break;
        	case 15:
        		uart_puts("SError from lower EL AArch32 ");
        		break;
    	}

	uart_puts("Current Excpetion Level ");
	uart_hex_uint64(currel);	    	
	uart_puts("	Excpetion Type ");
	uart_hex_uint64(type);
	uart_puts(" Excpetion Syndrome Register ");
	uart_hex_uint64(esr);
	uart_puts(" Excpetion Link Register ");
	uart_hex_uint64(elr);
	uart_puts(" Saved Program Status Register ");
	uart_hex_uint64(spsr);
	uart_puts(" Fault Register ");
	uart_hex_uint64(far);
	uart_puts("\n");
	
	return;
}

void print_uint32(uint32_t reg){ 	
	wait_msec_st(1000000);
	uart_puts("reg=");
	uart_hex_uint32((unsigned int)reg);
	uart_puts("\n");
	return;
}

void print_uint64(uint64_t reg){ 	
	wait_msec_st(1000000);
	uart_puts("reg=");
	uart_hex_uint64((unsigned long long)reg);
	uart_puts("\n");
	return;
}


void Happy_Birthday_Mattia_By_PiiRace(){
	int i = 0;

	wait_msec_st(3000000);
	uart_puts("Starting PiRace1.0\n");
	wait_msec_st(3000000);
	uart_puts("Happy Birdthday Mandino\n");
	wait_msec_st(2000000);
	uart_puts("By PiRace1.0 to Puntozer.0\n");
	wait_msec_st(3000000);
	uart_puts("Good Party guys :)\n");

	for(;i < 200;i++)
		uart_puts("Mandino\n");


	wait_msec_st(2500000);
	uart_puts("Ni viremu n´Sicilia Mandino!\n");

	return;



}


void  delay_test(){
    uart_puts("Waiting 1000000 CPU cycles (ARM CPU): ");
    wait_cycles(1000000);
    uart_puts("OK\n");

    uart_puts("Waiting 1000000 microsec (ARM CPU): ");
    wait_msec(1000000);
    uart_puts("OK\n");

    uart_puts("Waiting 1000000 microsec (BCM System Timer): ");
    if(get_system_timer()==0) {
        uart_puts("Not available\n");
    } else {
        wait_msec_st(1000000);
        uart_puts("OK\n");
    }
}

char OS_NAME [] = "BrainOS";
char OS_LEVEL [] = "Root";
char DIRECTORY [] = "/Home/Desktop";

void main()
{
                                 
	uart_puts("\n\n\n--------------------------------------------------\n");
	uart_puts("                Bootstap Brain OS               -\n");
	uart_puts("--------------------------------------------------\n");
	//boot(BOOTMODE);
	
	while(1){
		char uart_input;
		uart_puts("@");
		uart_puts(OS_LEVEL);
		uart_puts("-");		
		uart_puts(OS_NAME);
		uart_puts(":~");
		uart_puts(DIRECTORY);
		uart_puts("$ ");
		
		while((uart_input = uart_getc()) != '\n'){
			uart_send(((unsigned int)uart_input) - 32);
		}

		
		
		uart_puts("\n");


	}
}

