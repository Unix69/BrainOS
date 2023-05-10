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

    uint8_t spi_lossi_long_dma_enable;
    uint8_t spi_dma_lossi_enable;
    uint8_t spi_lossi_enable;
    uint8_t spi_dma_enable;
    uint8_t spi_enable;
    uint8_t spi_irq_on_comp_enable;
    uint8_t spi_irq_on_rx_enable;
    uint8_t spi_read_enable;
    uint8_t spi_adcs_enable;

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
    if(spi != NULL){
        spi->spi_en = SPI_DISABLED;
        spi->spi_running = SPI_STOPPED;
        spi->spi_free = SPI_FREE;
        spi->spi_owner = SPI_NO_OWNER;
        spi->spi_id = SPI_NO_ID;
        spi->spi_load = SPI_LOAD;
        spi->spi_val = SPI_VALUE;
        spi->spi_ctrl = SPI_CONTROL;
        spi->spi_clear_irq = SPI_CLEAR_IRQ;
        spi->spi_mask_irq = SPI_MASK_IRQ;
        spi->spi_raw_irq = SPI_RAW_IRQ;
        spi->spi_reload = SPI_RELOAD;
        spi->spi_prediv = SPI_PREDIV;
        spi->spi_free_run_cnt = SPI_FREE_COUNT;
        spi->spi_irq_handler = NULL;
        spi->spi_fiq_handler = NULL;
        spi->spi_err_handler = NULL;
        spi->spi_sync_handler = NULL;
    }
}

void spi_shutdown(){
    if(spi != NULL){
        spi->spi_en = SPI_DISABLED;
        spi->spi_running = SPI_STOPPED;
        spi->spi_free = SPI_FREE;
        spi->spi_owner = SPI_NO_OWNER;
        spi->spi_id = SPI_NO_ID;
        spi->spi_load = NULL;
        spi->spi_val = NULL;
        spi->spi_ctrl = NULL;
        spi->spi_clear_irq = NULL;
        spi->spi_mask_irq = NULL;
        spi->spi_raw_irq = NULL;
        spi->spi_reload = NULL;
        spi->spi_prediv = NULL;
        spi->spi_free_run_cnt = NULL;
        spi->spi_irq_handler = NULL;
        spi->spi_fiq_handler = NULL;
        spi->spi_err_handler = NULL;
        spi->spi_sync_handler = NULL;
    }
}

//BrainOS spi control level API
void spi_acquire(uint8_t spi_owner, void* spi_irq_handler, void* spi_fiq_handler, void* spi_err_handler, void* spi_sync_handler){
    register uint8_t spi_irq_enabled = 0;
    if(spi != NULL){
        if(spi->spi_owner == SPI_NO_OWNER){
            spi->spi_owner = spi_owner;
            spi->spi_free = SPI_BUSY;
            spi->spi_id = spi;
            spi->spi_irq_handler = spi_irq_handler;
            spi->spi_fiq_handler = spi_fiq_handler;
            spi->spi_err_handler = spi_err_handler;
            spi->spi_sync_handler = spi_sync_handler;
        }
    }
}

void spi_start(uint8_t spi_owner,uint32_t spi_load_val,uint8_t spi_irq_en,uint8_t spi_freerun_en,uint32_t spi_prescale_val,uint8_t spi_prescale_bits,uint8_t spi_prediv_val){
    if(spi != NULL){
        if(spi->spi_owner == spi_owner){
            spi->spi_running = SPI_RUNNING;
        }

    }
}

void spi_stop(uint8_t spi_owner){
    if(spi != NULL){
        if(spi->spi_owner == spi_owner){
            if(spi->spi_en && spi->spi_running){
                spi->spi_running = SPI_STOPPED;
            }
        }
    }
}

void spi_restart(uint8_t spi_owner){
    register uint32_t spi_val = 0;
    if(spi != NULL){
        if(spi->spi_owner == spi_owner){
            if(spi->spi_en && !spi->spi_running){
                spi->spi_running = SPI_RUNNING;
            }
        }
    }
}

void spi_release(uint8_t spi_owner){
    if(spi != NULL){
        if(spi->spi_owner == spi_owner){
            spi->spi_en = SPI_DISABLED;
            spi->spi_free = SPI_FREE;
            spi->spi_owner = SPI_NO_OWNER;
            spi->spi_id = SPI_NO_ID;
            spi->spi_irq_handler = NULL;
            spi->spi_fiq_handler = NULL;
            spi->spi_err_handler = NULL;
            spi->spi_sync_handler = NULL;
        }
    }
}

void spi_irq_handler_set(uint8_t spi_owner, void* spi_irq_handler){
     if(spi != NULL){
        if(spi->spi_owner == spi_owner && spi->spi_en){
            spi->spi_irq_handler = spi_irq_handler;
        }
    }
}

void spi_fiq_handler_set(uint8_t spi_owner, void* spi_fiq_handler){
     if(spi != NULL){
        if(spi->spi_owner == spi_owner && spi->spi_en){
            spi->spi_fiq_handler = spi_fiq_handler;
        }
    }
}

void spi_sync_handler_set(uint8_t spi_owner, void* spi_sync_handler){
     if(spi != NULL){
        if(spi->spi_owner == spi_owner && spi->spi_en){
            spi->spi_sync_handler = spi_sync_handler;
        }
    }
}

void spi_err_handler_set(uint8_t spi_owner, void* spi_err_handler){
     if(spi != NULL){
        if(spi->spi_owner == spi_owner && spi->spi_en){
            spi->spi_err_handler = spi_err_handler;
        }
    }
}



int spi_init();
int spi_open(int spi_owner);
int spi_close(int spi_owner);
int spi_read(int spi_owner,uint32_t *data);
int spi_write(int spi_owner,uint32_t data);
int spi_lossi_cmd_write(int spi_owner,uint32_t cmd);
int spi_lossi_cmd_read(int spi_owner,uint32_t *cmd);
int spi_lossi_param_write(int spi_owner,uint32_t param);
int spi_lossi_param_read(int spi_owner,uint32_t *param);



int main(int argc, char* argv[]){
    register uint32_t i;
    register uint8_t n_spis;
    register uint8_t spi_owner = 0;
    register uint32_t spi_load = 1024;


    // Timer initialization function: MUST BE CALLED BEFORE EVERY OPERATION
    spi_boot();
    
    spi_acquire(spi_owner, NULL, NULL, NULL);
    spi_start(spi_owner, spi_load, SPI_INTERRUPSPI_EN, 0, SPI_NOPRESCALE, SPI_BITS_HIGH, SPI_FREE_RUN_EN);
    // perform starting operation
    spi_acquire(SPI, spi_owner, NULL, NULL, NULL, NULL);
    spi_comparer = 1024 * 2
    spi_stop(SPI, spi_owner);
    spi_ctrl_irq_disable();
    //raise condition: add proper protected task 
    spi_ctrl_irq_enable();
    spi_restart(SPI, spi_owner, spi_comparer);
    spi_stop(SPI, spi_owner);
    spi_ctrl_irq_disable();
    //raise condition: add proper protected task 
    spi_ctrl_irq_enable();
    spi_release(SPI, spi_owner);
    spi_shutdown();

    
}


//BrainOS spi system level API




uint8_t spi_cs_cs_get(){
    register uint32_t spi_cs_cs = 0;
    spi_cs_cs = ((*SPI_CS) >> 4);
    return((uint32_t)spi_cs_cs);
}
void spi_cs_cs_set(uint8_t spi_cs_cs){
    register uint32_t spi_cs = 0;
    if (SPI_IS_VALID_SPI_CS(spi_cs_cs)){
        spi_cs = spi_cs_get();
        spi_cs |= (spi_cs_cs << 4);
        *SPI_CS = spi_cs;
    }
}
uint8_t spi_cs_cpol_get(){
    register uint32_t spi_long_enable = 0;
    spi_long_enable = ((*SPI_CS) >> 4);
    return((uint32_t)spi_long_enable);
}
void spi_cs_cpol_set(uint8_t spi_cpol){
    register uint32_t spi_cs = 0;
    if (SPI_IS_VALID_SPI_CPOL(spi_cpol)){
        spi_cs = spi_cs_get();
        spi_cs |= (spi_cpol << 4);
        *SPI_CS = spi_cs;
    }
}
uint8_t spi_cs_cspol_get(){
    register uint32_t spi_long_enable = 0;
    spi_long_enable = ((*SPI_CS) >> 4);
    return((uint32_t)spi_long_enable);
}
void spi_cs_cspol_set(uint8_t spi_cspol){
    register uint32_t spi_cs = 0;
    if (SPI_IS_VALID_SPI_CSPOL(spi_cspol)){
        spi_cs = spi_cs_get();
        spi_cs |= (spi_cspol << 4);
        *SPI_CS = spi_cs;
    }
}
void spi_cs_cspol0_set(uint8_t spi_cspol0){
     register uint32_t spi_cs = 0;
    if (SPI_IS_VALID_SPI_CSPOL(spi_cspol0)){
        spi_cs = spi_cs_get();
        spi_cs |= (spi_cspol0 << 4);
        *SPI_CS = spi_cs;
    }
}
uint8_t spi_cs_cspol0_get(){
    register uint32_t spi_long_enable = 0;
    spi_long_enable = ((*SPI_CS) >> 4);
    return((uint32_t)spi_long_enable);
}
void spi_cs_cspol1_set(uint8_t spi_cs_cspol1){
    register uint32_t spi_cs = 0;
    if (SPI_IS_VALID_SPI_CSPOL1(spi_cspol1)){
        spi_cs = spi_cs_get();
        spi_cs |= (spi_cspol1 << 4);
        *SPI_CS = spi_cs;
    }
}
uint8_t spi_cs_cspol1_get(){
    register uint32_t spi_cspol1 = 0;
    spi_cspol1 = ((*SPI_CS) >> 4);
    return((uint32_t)spi_cspol1);
}
void spi_cs_cspol2_set(uint8_t spi_cspol2){
    register uint32_t spi_cs = 0;
    if (SPI_IS_VALID_SPI_CSPOL2(spi_cspol2)){
        spi_cs = spi_cs_get();
        spi_cs |= (spi_cspol2 << 4);
        *SPI_CS = spi_cs;
    }
}
uint8_t spi_cs_cspol2_get(){
    register uint32_t spi_cspol2 = 0;
    spi_cspol2 = ((*SPI_CS) >> 4);
    return((uint32_t)spi_cspol2);
}
uint8_t spi_cs_cpha_get(){
    register uint32_t spi_long_enable = 0;
    spi_long_enable = ((*SPI_CS) >> 4);
    return((uint32_t)spi_long_enable);

}
void spi_cs_cpha_set(uint8_t spi_cpha){
    register uint32_t spi_cs = 0;
    if (SPI_IS_VALID_SPI_CS(spi_cpha)){
        spi_cs = spi_cs_get();
        spi_cs |= (spi_cpha << 4);
        *SPI_CS = spi_cs;
    }
}



uint8_t spi_cs_irq_enable_get(){
    register uint32_t spi_cspol2 = 0;
    spi_cspol2 = ((*SPI_CS) >> 4);
    return((uint32_t)spi_cspol2);
}
uint8_t spi_cs_irq_enable_set(uint8_t spi_irq_enable){
    register uint32_t spi_cs = 0;
    if (SPI_IS_VALID_SPI_IRQ_ENABLE(spi_irq_enable)){
        spi_cs = spi_cs_get();
        spi_cs |= (spi_irq_enable << 4);
        *SPI_CS = spi_cs;
    }
}
uint8_t spi_cs_irq_status_get(){
    register uint32_t spi_cspol2 = 0;
    spi_cspol2 = ((*SPI_CS) >> 4);
    return((uint32_t)spi_cspol2);
}
uint8_t spi_cs_irq_status_set(uint8_t spi_irq_status){
    register uint32_t spi_cs = 0;
    if (SPI_IS_VALID_SPI_IRQ_ENABLE(spi_irq_status)){
        spi_cs = spi_cs_get();
        spi_cs |= (spi_irq_status << 4);
        *SPI_CS = spi_cs;
    }
}
uint8_t spi_cs_irq_on_rx_enable_get(){
    register uint32_t spi_cspol2 = 0;
    spi_cspol2 = ((*SPI_CS) >> 4);
    return((uint32_t)spi_cspol2);
}
uint8_t spi_cs_irq_on_rx_enable_set(uint8_t spi_irq_on_rx_enable){
    register uint32_t spi_cs = 0;
    if (SPI_IS_VALID_SPI_IRQ_ENABLE(spi_irq_on_rx_enable)){
        spi_cs = spi_cs_get();
        spi_cs |= (spi_irq_on_rx_enable << 4);
        *SPI_CS = spi_cs;
    }
}
void spi_cs_irq_on_rx_enable_clear(){
    register uint32_t spi_cs = 0;
    spi_cs = spi_cs_get();
    spi_cs &= (0 << 4);
    *SPI_CS = spi_cs;
}
uint32_t spi_cs_irq_on_comp_enable(uint8_t spi_cs_irq_on_comp_enable){
    register uint32_t spi_cs = 0;
    spi_cs = spi_cs_get();
    spi_cs |= (spi_cs_irq_on_comp_enable << 5);
    *SPI_CS = spi_cs;
}
void spi_cs_irq_on_comp_enable_clear(){
    register uint32_t spi_cs = 0;
    spi_cs = spi_cs_get();
    spi_cs &= (0 << 5);
    *SPI_CS = spi_cs;
}
uint8_t spi_cs_irq_on_comp_enable_get(){
    register uint32_t spi_cspol2 = 0;
    spi_cspol2 = ((*SPI_CS) >> 4);
    return((uint32_t)spi_cspol2);
}
uint8_t spi_cs_long_enable_get(){
    register uint32_t spi_long_enable = 0;
    spi_long_enable = ((*SPI_CS) >> 4);
    return((uint32_t)spi_long_enable);
}
void spi_cs_long_enable_set(uint8_t spi_long_enable){
     register uint32_t spi_cs = 0;
    if (SPI_IS_VALID_SPI_CS(spi_long_enable)){
        spi_cs = spi_cs_get();
        spi_cs |= (spi_long_enable << 4);
        *SPI_CS = spi_cs;
    }
}
void spi_cs_dma_inlossi_enable_set(uint8_t spi_lossi_enable){
     register uint32_t spi_cs = 0;
    if (SPI_IS_VALID_SPI_LONG_ENABLE(spi_lossi_enable)){
        spi_cs = spi_cs_get();
        spi_cs |= (spi_lossi_enable << 4);
        *SPI_CS = spi_cs;
    }
}
uint8_t spi_cs_dma_lossi_enable_get(){
    register uint32_t spi_lossi_enable = 0;
    spi_lossi_enable = ((*SPI_CS) >> 4);
    return((uint32_t)spi_lossi_enable);
}
uint8_t spi_cs_rx_full_fifo_get(){
    register uint32_t spi_long_enable = 0;
    spi_long_enable = ((*SPI_CS) >> 4);
    return((uint32_t)spi_long_enable);
}
uint8_t spi_cs_rx_empty_fifo_get(){
    register uint32_t spi_long_enable = 0;
    spi_long_enable = ((*SPI_CS) >> 4);
    return((uint32_t)spi_long_enable);
}
uint8_t spi_cs_tx_accept_fifo(){}
uint8_t spi_cs_rx_contains_fifo(){}




uint8_t spi_cs_tdone_get(){
    register uint32_t spi_long_enable = 0;
    spi_long_enable = ((*SPI_CS) >> 4);
    return((uint32_t)spi_long_enable);
}
void spi_cs_len_lossi_set(uint8_t spi_len_lossi){
     register uint32_t spi_cs = 0;
    if (SPI_IS_VALID_SPI_LEN(spi_len_lossi)){
        spi_cs = spi_cs_get();
        spi_cs |= (spi_len_lossi << 4);
        *SPI_CS = spi_cs;
    }
}
uint8_t spi_cs_len_lossi_get(){
    register uint32_t spi_long_enable = 0;
    spi_long_enable = ((*SPI_CS) >> 4);
    return((uint32_t)spi_long_enable);
}
void spi_cs_ren_set(uint8_t spi_ren){
     register uint32_t spi_cs = 0;
    if (SPI_IS_VALID_SPI_REN(spi_ren)){
        spi_cs = spi_cs_get();
        spi_cs |= (spi_ren << 4);
        *SPI_CS = spi_cs;
    }
}
uint8_t spi_cs_ren_get(){
    register uint32_t spi_long_enable = 0;
    spi_long_enable = ((*SPI_CS) >> 4);
    return((uint32_t)spi_long_enable);
}
void spi_cs_adcs_set(uint8_t spi_adcs){
    register uint32_t spi_cs = 0;
    if (SPI_IS_VALID_SPI_CS(spi_adcs)){
        spi_cs = spi_cs_get();
        spi_cs |= (spi_adcs << 4);
        *SPI_CS = spi_cs;
    }
}
uint8_t spi_cs_adcs_get(){
    register uint32_t spi_long_enable = 0;
    spi_long_enable = ((*SPI_CS) >> 4);
    return((uint32_t)spi_long_enable);
}



uint8_t spi_cs_dma_enable_get(){
    register uint32_t spi_long_enable = 0;
    spi_long_enable = ((*SPI_CS) >> 4);
    return((uint32_t)spi_long_enable);
}
void spi_cs_dma_enable_set(uint8_t spi_dma_enable){
    register uint32_t spi_cs = 0;
    if (SPI_IS_VALID_SPI_DMA_ENABLE(spi_dma_enable)){
        spi_cs = spi_cs_get();
        spi_cs |= (spi_dma_enable << 4);
        *SPI_CS = spi_cs;
    }
}
uint8_t spi_cs_dma_ta_get(){
    register uint32_t spi_long_enable = 0;
    spi_long_enable = ((*SPI_CS) >> 4);
    return((uint32_t)spi_long_enable);
}
void spi_cs_dma_ta_set(uint8_t spi_ta){
    register uint32_t spi_cs = 0;
    if (SPI_IS_VALID_SPI_TA(spi_ta)){
        spi_cs = spi_cs_get();
        spi_cs |= (spi_ta << 4);
        *SPI_CS = spi_cs;
    }
}
uint8_t spi_cs_clear_get(){
    register uint32_t spi_long_enable = 0;
    spi_long_enable = ((*SPI_CS) >> 4);
    return((uint32_t)spi_long_enable);
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
    if (SPI_IS_VALID_SPI_DC(spi_dlen)){
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
    if (SPI_IS_VALID_SPI_DC(spi_clk)){
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




 


