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


#include "types.h"
#include "spi.h"

struct spi {
    
    uint8_t spi_running;
    uint32_t spi_id;
    uint8_t spi_owner;
    uint8_t spi_free;

    volatile unsigned long int* spi_cs;
    volatile unsigned long int* spi_dlen;
    volatile unsigned long int* spi_fifo;
    volatile unsigned long int* spi_ltoh;
    volatile unsigned long int* spi_dc;
    volatile unsigned long int* spi_clk;
    
    void *spi_irq_handler;
    void *spi_fiq_handler;
    void *spi_err_handler;
    void *spi_sync_handler;
};

#include "spi.h"

spi_t spi;


//BrainOS spi internal routines
void spi_boot(){
    spi.spi_running = SPI_STOPPED;
    spi.spi_free = SPI_FREE;
    spi.spi_owner = SPI_NO_OWNER;
    spi.spi_id = SPI_NO_ID;
    spi.spi_cs = SPI_CS;
    spi.spi_dlen = SPI_DLEN;
    spi.spi_fifo = SPI_FIFO;
    spi.spi_ltoh = SPI_LTOH;
    spi.spi_dc = SPI_DC;
    spi.spi_clk = SPI_CLK;
    spi.spi_irq_handler = NULL;
    spi.spi_fiq_handler = NULL;
    spi.spi_err_handler = NULL;
    spi.spi_sync_handler = NULL;
}

void spi_shutdown(){
    spi.spi_running = SPI_STOPPED;
    spi.spi_free = SPI_FREE;
    spi.spi_owner = SPI_NO_OWNER;
    spi.spi_id = SPI_NO_ID;
    spi.spi_cs = NULL;
    spi.spi_dlen = NULL;
    spi.spi_fifo = NULL;
    spi.spi_ltoh = NULL;
    spi.spi_dc = NULL;
    spi.spi_clk = NULL;
    spi.spi_irq_handler = NULL;
    spi.spi_fiq_handler = NULL;
    spi.spi_err_handler = NULL;
    spi.spi_sync_handler = NULL;
}

//BrainOS spi control level API
void spi_acquire(uint8_t spi_owner, void* spi_irq_handler, void* spi_fiq_handler, void* spi_err_handler, void* spi_sync_handler){
    if(spi.spi_owner == SPI_NO_OWNER){
        spi.spi_owner = spi_owner;
        spi.spi_free = SPI_BUSY;
        spi.spi_id = spi_owner;
        spi.spi_irq_handler = spi_irq_handler;
        spi.spi_fiq_handler = spi_fiq_handler;
        spi.spi_err_handler = spi_err_handler;
        spi.spi_sync_handler = spi_sync_handler;
    }
}

void spi_start(uint8_t spi_owner){
    if(spi.spi_owner == spi_owner){
        spi.spi_running = SPI_RUNNING;
    }
}

void spi_stop(uint8_t spi_owner){
    if(spi.spi_owner == spi_owner){
        if(spi.spi_running){
            spi.spi_running = SPI_STOPPED;
        }
    }
}


void spi_release(uint8_t spi_owner){
    if(spi.spi_owner == spi_owner){
        spi.spi_free = SPI_FREE;
        spi.spi_owner = SPI_NO_OWNER;
        spi.spi_id = SPI_NO_ID;
        spi.spi_irq_handler = NULL;
        spi.spi_fiq_handler = NULL;
        spi.spi_err_handler = NULL;
        spi.spi_sync_handler = NULL;
    }
}

void spi_irq_handler_set(uint8_t spi_owner, void* spi_irq_handler){
    if(spi.spi_owner == spi_owner){
        spi.spi_irq_handler = spi_irq_handler;
    }
}

void spi_fiq_handler_set(uint8_t spi_owner, void* spi_fiq_handler){
     
    if(spi.spi_owner == spi_owner){
        spi.spi_fiq_handler = spi_fiq_handler;
    }
    
}

void spi_sync_handler_set(uint8_t spi_owner, void* spi_sync_handler){
     
    if(spi.spi_owner == spi_owner){
        spi.spi_sync_handler = spi_sync_handler;
    }
    
}

void spi_err_handler_set(uint8_t spi_owner, void* spi_err_handler){
     
    if(spi.spi_owner == spi_owner){
        spi.spi_err_handler = spi_err_handler;
    }
}



void spi_init(register uint8_t spi_cs, register uint8_t spi_cpha, register uint8_t spi_cpol, register uint8_t spi_clear, register uint8_t spi_cspol,register uint8_t spi_ta, register uint8_t spi_dma_en, register uint8_t spi_irq_tx_d,register uint8_t spi_irq_rx_r,register uint8_t spi_adcs, register uint8_t spi_read_en, register uint8_t spi_lossi_en, register uint8_t spi_cspol0, register uint8_t spi_cspol1, register uint8_t spi_cspol2, register uint8_t spi_dma_lossi_enable, register uint8_t spi_lossi_long_en){
    spi_cs_cs_set(spi_cs);
    spi_cs_cpha_set(spi_cpha);
    spi_cs_cpol_set(spi_cpol);
    spi_cs_clear(spi_clear);
    spi_cs_cspol_set(spi_cspol);
    spi_cs_ta_set(spi_ta);
    spi_cs_dma_enable_set(spi_dma_en);
    spi_cs_irq_on_tx_done_set(spi_irq_tx_d);
    spi_cs_irq_on_rxr_set(spi_irq_rx_r);
    spi_cs_adcs_set(spi_adcs);
    spi_cs_ren_set(spi_read_en);
    spi_cs_len_set(spi_lossi_en);
    spi_cs_cspol0_set(spi_cspol0);
    spi_cs_cspol1_set(spi_cspol1);
    spi_cs_cspol2_set(spi_cspol2);
    spi_cs_dma_len_set(spi_dma_lossi_enable);
    spi_cs_len_long_set(spi_lossi_long_en);
}




int spi_open(int spi_owner){
    if(spi.spi_owner == SPI_NO_OWNER){
        spi.spi_owner = spi_owner;
    }
}
int spi_close(int spi_owner);
int spi_read(int spi_owner,uint32_t *data);
int spi_write(int spi_owner,uint32_t data){
    if(spi.spi_owner == spi_owner){
        spi_fifo_set(data);
    }
    
}



int spi_lossi_cmd_write(int spi_owner,uint32_t cmd);
int spi_lossi_cmd_read(int spi_owner,uint32_t *cmd);
int spi_lossi_param_write(int spi_owner,uint32_t param);
int spi_lossi_param_read(int spi_owner,uint32_t *param);


/*
int main(int argc, char* argv[]){
    register uint32_t i;
    register uint8_t n_spis;
    register uint8_t spi_owner = 0;
    register uint32_t spi_load = 1024;
}
*/

//BrainOS spi system level API
uint8_t spi_cs_cs_get(){
    register uint32_t spi_cs_cs = 0;
    spi_cs_cs = ((*SPI_CS) >> 1);
    return((uint32_t)spi_cs_cs);
}
void spi_cs_cs_set(uint8_t spi_cs_cs){
    register uint32_t spi_cs = 0;
    if (SPI_IS_VALID_SPI_CS(spi_cs_cs)){
        spi_cs = spi_cs_get();
        spi_cs |= (spi_cs_cs << 1);
        *SPI_CS = spi_cs;
    }
}
uint8_t spi_cs_cpol_get(){
    register uint32_t spi_clk_polarity = 0;
    spi_clk_polarity = ((*SPI_CS) >> 3);
    return((uint32_t)spi_clk_polarity);
}
void spi_cs_cpol_set(uint8_t spi_cpol){
    register uint32_t spi_cs = 0;
    if (SPI_IS_VALID_SPI_CPOL(spi_cpol)){
        spi_cs = spi_cs_get();
        spi_cs |= (spi_cpol << 3);
        *SPI_CS = spi_cs;
    }
}
uint8_t spi_cs_cspol_get(){
    register uint32_t spi_cs_polarity = 0;
    spi_cs_polarity = ((*SPI_CS) >> 6);
    return((uint32_t)spi_cs_polarity);
}
void spi_cs_cspol_set(uint8_t spi_cspol){
    register uint32_t spi_cs = 0;
    if (SPI_IS_VALID_SPI_CSPOL(spi_cspol)){
        spi_cs = spi_cs_get();
        spi_cs |= (spi_cspol << 6);
        *SPI_CS = spi_cs;
    }
}
void spi_cs_cspol0_set(uint8_t spi_cspol0){
     register uint32_t spi_cs = 0;
    if (SPI_IS_VALID_SPI_CSPOL0(spi_cspol0)){
        spi_cs = spi_cs_get();
        spi_cs |= (spi_cspol0 << 21);
        *SPI_CS = spi_cs;
    }
}
uint8_t spi_cs_cspol0_get(){
    register uint32_t spi_cs_cspol0 = 0;
    spi_cs_cspol0 = ((*SPI_CS) >> 21);
    return((uint32_t)spi_cs_cspol0);
}
void spi_cs_cspol1_set(uint8_t spi_cs_cspol1){
    register uint32_t spi_cs = 0;
    if (SPI_IS_VALID_SPI_CSPOL1(spi_cs_cspol1)){
        spi_cs = spi_cs_get();
        spi_cs |= (spi_cs_cspol1 << 22);
        *SPI_CS = spi_cs;
    }
}
uint8_t spi_cs_cspol1_get(){
    register uint32_t spi_cs_cspol1 = 0;
    spi_cs_cspol1 = ((*SPI_CS) >> 22);
    return((uint32_t)spi_cs_cspol1);
}
void spi_cs_cspol2_set(uint8_t spi_cs_cspol2){
    register uint32_t spi_cs = 0;
    if (SPI_IS_VALID_SPI_CSPOL2(spi_cs_cspol2)){
        spi_cs = spi_cs_get();
        spi_cs |= (spi_cs_cspol2 << 23);
        *SPI_CS = spi_cs;
    }
}
uint8_t spi_cs_cspol2_get(){
    register uint32_t spi_cs_cspol2 = 0;
    spi_cs_cspol2 = ((*SPI_CS) >> 23);
    return((uint32_t)spi_cs_cspol2);
}
uint8_t spi_cs_cpha_get(){
    register uint32_t spi_cpha = 0;
    spi_cpha = ((*SPI_CS) >> 2);
    return((uint32_t)spi_cpha);

}
void spi_cs_cpha_set(uint8_t spi_cpha){
    register uint32_t spi_cs = 0;
    if (SPI_IS_VALID_SPI_CS(spi_cpha)){
        spi_cs = spi_cs_get();
        spi_cs |= (spi_cpha << 2);
        *SPI_CS = spi_cs;
    }
}

void spi_cs_irq_on_rxr_set(uint8_t spi_irq_on_rxr){
    register uint32_t spi_cs = 0;
    if (SPI_IS_VALID_SPI_IRQ_ENABLE(spi_irq_on_rxr)){
        spi_cs = spi_cs_get();
        spi_cs |= (spi_irq_on_rxr << 10);
        *SPI_CS = spi_cs;
    }
}

uint8_t spi_cs_irq_on_rxr_get(){
    register uint32_t spi_irq_on_rxr = 0;
    spi_irq_on_rxr = ((*SPI_CS) >> 10);
    return((uint32_t)spi_irq_on_rxr);
}


void spi_cs_irq_on_tx_done_set(uint8_t spi_irq_on_tx_done){
    register uint32_t spi_cs = 0;
    if (SPI_IS_VALID_SPI_IRQ_ENABLE(spi_irq_on_tx_done)){
        spi_cs = spi_cs_get();
        spi_cs |= (spi_irq_on_tx_done << 9);
        *SPI_CS = spi_cs;
    }
}

uint8_t spi_cs_irq_on_tx_done_get(){
    register uint32_t spi_irq_on_tx_done = 0;
    spi_irq_on_tx_done = ((*SPI_CS) >> 9);
    return((uint32_t)spi_irq_on_tx_done);
}

uint8_t spi_cs_long_enable_get(){
    register uint32_t spi_len_long = 0;
    spi_len_long = ((*SPI_CS) >> 25);
    return((uint32_t)spi_len_long);
}
void spi_cs_len_long_set(uint8_t spi_len_long){
     register uint32_t spi_cs = 0;
    if (SPI_IS_VALID_SPI_CS(spi_len_long)){
        spi_cs = spi_cs_get();
        spi_cs |= (spi_len_long << 25);
        *SPI_CS = spi_cs;
    }
}


void spi_cs_dma_len_set(uint8_t spi_dma_len){
     register uint32_t spi_cs = 0;
    if (SPI_IS_VALID_SPI_LONG_ENABLE(spi_dma_len)){
        spi_cs = spi_cs_get();
        spi_cs |= (spi_dma_len << 24);
        *SPI_CS = spi_cs;
    }
}


uint8_t spi_cs_dma_len_get(){
    register uint32_t spi_dma_len = 0;
    spi_dma_len = ((*SPI_CS) >> 24);
    return((uint32_t)spi_dma_len);
}

uint8_t spi_cs_rx_reading_get(){
    register uint32_t spi_rx_reading = 0;
    spi_rx_reading = ((*SPI_CS) >> 19);
    return((uint32_t)spi_rx_reading);
}

uint8_t spi_cs_rx_full_get(){
    register uint32_t spi_rx_full = 0;
    spi_rx_full = ((*SPI_CS) >> 20);
    return((uint32_t)spi_rx_full);
}

uint8_t spi_cs_tx_full_fifo_get(){
    register uint32_t spi_tx_full = 0;
    spi_tx_full = ((*SPI_CS) >> 18);
    return((uint32_t)spi_tx_full);
}

uint8_t spi_cs_rx_empty_fifo_get(){
    register uint32_t spi_long_enable = 0;
    spi_long_enable = ((*SPI_CS) >> 17);
    return((uint32_t)spi_long_enable);
}

uint8_t spi_cs_tdone_get(){
    register uint32_t spi_tdone = 0;
    spi_tdone = ((*SPI_CS) >> 4);
    return((uint32_t)spi_tdone);
}


void spi_cs_len_set(uint8_t spi_len){
     register uint32_t spi_cs = 0;
    if (SPI_IS_VALID_SPI_LEN(spi_len)){
        spi_cs = spi_cs_get();
        spi_cs |= (spi_len << 13);
        *SPI_CS = spi_cs;
    }
}

uint8_t spi_cs_len_get(){
    register uint32_t spi_long_enable = 0;
    spi_long_enable = ((*SPI_CS) >> 13);
    return((uint32_t)spi_long_enable);
}


void spi_cs_ren_set(uint8_t spi_ren){
     register uint32_t spi_cs = 0;
    if (SPI_IS_VALID_SPI_REN(spi_ren)){
        spi_cs = spi_cs_get();
        spi_cs |= (spi_ren << 12);
        *SPI_CS = spi_cs;
    }
}

uint8_t spi_cs_ren_get(){
    register uint32_t spi_ren = 0;
    spi_ren = ((*SPI_CS) >> 12);
    return((uint32_t)spi_ren);
}
void spi_cs_adcs_set(uint8_t spi_adcs){
    register uint32_t spi_cs = 0;
    if (SPI_IS_VALID_SPI_CS(spi_adcs)){
        spi_cs = spi_cs_get();
        spi_cs |= (spi_adcs << 11);
        *SPI_CS = spi_cs;
    }
}
uint8_t spi_cs_adcs_get(){
    register uint32_t spi_adcs = 0;
    spi_adcs = ((*SPI_CS) >> 11);
    return((uint32_t)spi_adcs);
}



uint8_t spi_cs_dma_enable_get(){
    register uint32_t spi_dma_enable = 0;
    spi_dma_enable = ((*SPI_CS) >> 8);
    return((uint32_t)spi_dma_enable);
}
void spi_cs_dma_enable_set(uint8_t spi_dma_enable){
    register uint32_t spi_cs = 0;
    if (SPI_IS_VALID_SPI_DMA_ENABLE(spi_dma_enable)){
        spi_cs = spi_cs_get();
        spi_cs |= (spi_dma_enable << 8);
        *SPI_CS = spi_cs;
    }
}
uint8_t spi_cs_ta_get(){
    register uint32_t spi_ta = 0;
    spi_ta = ((*SPI_CS) >> 7);
    return((uint32_t)spi_ta);
}
void spi_cs_ta_set(uint8_t spi_ta){
    register uint32_t spi_cs = 0;
    if (SPI_IS_VALID_SPI_TA(spi_ta)){
        spi_cs = spi_cs_get();
        spi_cs |= (spi_ta << 7);
        *SPI_CS = spi_cs;
    }
}
uint8_t spi_cs_clear_get(){
    register uint32_t spi_clear = 0;
    spi_clear = ((*SPI_CS) >> 4);
    return((uint32_t)spi_clear);
}
void spi_cs_clear_set(uint8_t spi_clear){
    register uint32_t spi_cs = 0;
    if (SPI_IS_VALID_SPI_CLEAR(spi_clear)){
        spi_cs = spi_cs_get();
        spi_cs |= (spi_clear << 4);
        *SPI_CS = spi_cs;
    }
}








void spi_ltoh_set(uint32_t spi_ltoh){
    if (SPI_IS_VALID_SPI_LTOH(spi_ltoh)){
        *SPI_LTOH = spi_ltoh;
    }
}
void spi_ltoh_reset(){
    register uint32_t spi_ltoh = 0;
    *SPI_LTOH = spi_ltoh;
}
void spi_ltoh_clear(){
    register uint32_t spi_ltoh = 0;
    *SPI_LTOH = spi_ltoh;
}
uint32_t spi_ltoh_get(){
    register uint32_t spi_ltoh = 0;
    spi_ltoh = *SPI_LTOH;
    return((uint32_t)spi_ltoh);
}

uint32_t spi_cs_get(){
    register uint32_t spi_cs = 0;
    spi_cs = *SPI_CS;
    return((uint32_t)spi_cs);
}
void spi_cs_set(uint32_t spi_cs){
    if (SPI_IS_VALID_SPI_CS(spi_cs)){
        *SPI_CS = spi_cs;
    }
}
void spi_cs_reset(){
    register uint32_t spi_cs = 0;
    *SPI_CS = spi_cs;
}
void spi_cs_clear(){
    register uint32_t spi_cs = 0;
    *SPI_CS = spi_cs;
}

void spi_fifo_set(uint32_t spi_fifo){
    if (SPI_IS_VALID_SPI_LTOH(spi_fifo)){
        *SPI_FIFO = spi_fifo;
    }
}
void spi_fifo_reset(){
    register uint32_t spi_fifo = 0;
    *SPI_FIFO = spi_fifo;
}
void spi_fifo_clear(){
    register uint32_t spi_fifo = 0;
    *SPI_FIFO = spi_fifo;
}
uint32_t spi_fifo_get(){
    register uint32_t spi_fifo = 0;
    spi_fifo = *SPI_FIFO;
    return((uint32_t)spi_fifo);
}

void spi_dc_set(uint32_t spi_dc){
    if (SPI_IS_VALID_SPI_DC(spi_dc)){
        *SPI_DC = spi_dc;
    }
}
void spi_dc_reset(){
    register uint32_t spi_dc = 0;
    *SPI_DC = spi_dc;
}
void spi_dc_clear(){
    register uint32_t spi_dc = 0;
    *SPI_DC = spi_dc;
}
uint32_t spi_dc_get(){
    register uint32_t spi_dc = 0;
    spi_dc = *SPI_DC;
    return((uint32_t)spi_dc);
}

void spi_dlen_set(uint32_t spi_dlen){
    if (SPI_IS_VALID_SPI_DLEN(spi_dlen)){
        *SPI_DLEN = spi_dlen;
    }
}
void spi_dlen_reset(){
    register uint32_t spi_dlen = 0;
    *SPI_DLEN = spi_dlen;
}
void spi_dlen_clear(){
    register uint32_t spi_dlen = 0;
    *SPI_DLEN = spi_dlen;
}
uint32_t spi_dlen_get(){
    register uint32_t spi_dlen = 0;
    spi_dlen = *SPI_DLEN;
    return((uint32_t)spi_dlen);
}

void spi_clk_set(uint32_t spi_clk){
    if (SPI_IS_VALID_SPI_CLK(spi_clk)){
        *SPI_CLK = spi_clk;
    }
}
void spi_clk_reset(){
    register uint32_t spi_clk = 0;
    *SPI_CLK = spi_clk;
}
void spi_clk_clear(){
    register uint32_t spi_clk = 0;
    *SPI_CLK = spi_clk;
}
uint32_t spi_clk_get(){
    register uint32_t spi_clk = 0;
    spi_clk = *SPI_CLK;
    return((uint32_t)spi_clk);
}




 


