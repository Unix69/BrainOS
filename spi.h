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

#ifndef SPI_H
#define SPI_H

#include "mmu.h"
#include "types.h"


#define SPI_CS              ((volatile unsigned int*)(MMIO_BASE+0x204000))
#define SPI_FIFO            ((volatile unsigned int*)(MMIO_BASE+0x204004))
#define SPI_CLK             ((volatile unsigned int*)(MMIO_BASE+0x204008))
#define SPI_DLEN            ((volatile unsigned int*)(MMIO_BASE+0x20400C))
#define SPI_LTOH            ((volatile unsigned int*)(MMIO_BASE+0x204010))
#define SPI_DC              ((volatile unsigned int*)(MMIO_BASE+0x204014))

#define SPI_IRQ_ON_RXFIFO_ENABLE 1
#define SPI_IRQ_ON_COMPLETE_ENABLE 1
#define SPI_IRQ_ON_RXFIFO_DISABLE 0
#define SPI_IRQ_ON_COMPLETE_DISABLE 0
#define SPI_LOSSI_LONG_DATA_ENABLE 1
#define SPI_LOSSI_LONG_DATA_DISABLE 0
#define SPI_DMA_ENABLE 1
#define SPI_DMA_DISABLE 0
#define SPI_CSPOL2_HIGH 1
#define SPI_CSPOL2_LOW 0
#define SPI_CSPOL1_HIGH 1
#define SPI_CSPOL1_LOW 0
#define SPI_RX_FIFO_NOT_FULL 0
#define SPI_RX_FIFO_FULL 1
#define SPI_TX_NOT_ACCEPT_DATA 0
#define SPI_TX_ACCEPT_DATA 1
#define SPI_RX_NEEDS_READING 1
#define SPI_RX_DO_NOT_NEEDS_READING 0
#define SPI_RX_FIFO_EMPTY 0
#define SPI_RX_FIFO_NOT_EMPTY 1
#define SPI_TRASNFER_IN_PROGRESS 0
#define SPI_TRASNFER_DONE 1
#define SPI_TA_POLICY 1
#define SPI_TA_HIGH 0
#define SPI_SPI_ENABLE 0
#define SPI_LOSSI_ENABLE 1
#define SPI_READ_ENABLE 1
#define SPI_WRITE_ENABLE 0
#define SPI_AD_CS_ENABLE 1 
#define SPI_AD_CS_DISABLE 0
#define SPI_CSPOL_HIGH 1
#define SPI_CSPOL_LOW 0
#define SPI_CLEAR_RX 0x01b
#define SPI_CLEAR_TX 0x10b
#define SPI_CPOL_HIGH 1
#define SPI_CPOL_LOW 0
#define SPI_CPHA_MIDDLE 0
#define SPI_CPHA_BEGIN 1
#define SPI_CS_0 0
#define SPI_CS_1 1
#define SPI_CS_2 2
#define SPI_CSPOL_HIGH 1
#define SPI_CSPOL_LOW 0
#define SPI_CLK_DIV [0, 15]
#define SPI_DMA_DLEN [0, 15]
#define SPI_OUTPUT_HOLD [0, 3]
#define SPI_ENABLED 1
#define SPI_DISABLED 0
#define SPI_STOPPED 0
#define SPI_FREE 1
#define SPI_NO_OWNER 0 
#define SPI_NO_ID 0

typedef struct spi * spi_t;

//BrainOS spi internal routines
/// @brief boot spis
void spi_boot();

/// @brief shutdown spis
void spi_shutdown();

int spi_init();
int spi_open( int spi_owner);
int spi_close( int spi_owner);
int spi_read( int spi_owner,  uint32_t *data);
int spi_write(int spi_owner,  uint32_t data);
int spi_lossi_cmd_write( int spi_owner,  uint32_t cmd);
int spi_lossi_cmd_read( int spi_owner,  uint32_t *cmd);
int spi_lossi_param_write( int spi_owner, uint32_t param);
int spi_lossi_param_read( int spi_owner,  uint32_t *param);

//BrainOS spi control level API

/// @brief 
/// @param spi_owner 
/// @param spi_irq_handler 
/// @param spi_fiq_handler 
/// @param spi_err_handler 
/// @param spi_sync_handler 
void spi_acquire( uint8_t spi_owner, void* spi_irq_handler, void* spi_fiq_handler, void* spi_err_handler, void* spi_sync_handler);

/// @brief 
/// @param spi_owner 
/// @param spi_load_val 
/// @param spi_en_int 
/// @param spi_prescale_val 
/// @param spi_prescale_bits 
/// @param spi_bits 
/// @param spi_freerun 
void spi_start( uint8_t spi_owner);

/// @brief 
/// @param spi_owner  
void spi_stop( uint8_t spi_owner);

/// @brief restart a spi
/// @param spi 
/// @param spi_owner 
void spi_restart( uint8_t spi_owner);


/// @brief release a spi
/// @param spi 
/// @param spi_owner 
void spi_release(  uint8_t spi_owner);

/// @brief set spi irq handler
/// @param spi 
/// @param spi_owner 
/// @param spi_irq_handler 
void spi_irq_handler_set(  uint8_t spi_owner, void* spi_irq_handler);

/// @brief set spi fiq handler
/// @param spi 
/// @param spi_owner 
/// @param spi_fiq_handler 
void spi_fiq_handler_set( uint8_t spi_owner, void* spi_fiq_handler);

/// @brief set spi sync handler
/// @param spi 
/// @param spi_owner 
/// @param spi_sync_handler 
void spi_sync_handler_set(  uint8_t spi_owner, void* spi_sync_handler);

/// @brief set spi err handler
/// @param spi 
/// @param spi_owner 
/// @param spi_err_handler 
void spi_err_handler_set( uint8_t spi_owner, void* spi_err_handler);


/// @brief get spi freerun
/// @return spi free running
uint32_t spi_cs_get();

/// @brief set spi pre-division scaler
/// @param spi_pre_div 
void spi_cs_set( uint32_t spi_pre_div);

/// @brief reset spi pre-division scaler
void spi_cs_reset();

/// @brief clear spi pre-division scaler
void spi_cs_clear();


/// @brief 
/// @param spi_bits 
void spi_cspol1_set(register uint8_t spi_cspol1);

/// @brief 
/// @return 
uint8_t spi_cspol2_get();

/// @brief 
/// @param spi_prescale 
void spi_cspol2_set(register uint8_t spi_cspol2);

/// @brief 
/// @return 
uint8_t spi_cspol2_get();


/// @brief 
/// @return 
uint8_t spi_cs_irq_enable_get();

/// @brief 
/// @return 
uint8_t spi_cs_irq_enable_set(uint8_t spi_irq_enable);

/// @brief 
/// @return 
uint8_t spi_cs_irq_status_get();

/// @brief 
/// @return 
uint8_t spi_cs_irq_status_set(uint8_t spi_irq_status);


/// @brief 
/// @return 
uint8_t spi_cs_irq_on_rx_enable_get();

/// @brief 
/// @return 
uint8_t spi_cs_irq_on_rx_enable_set(uint8_t spi_irq_on_rx_enable);

/// @brief 
void spi_cs_irq_on_rx_enable_clear();

/// @brief get spi raw irq
/// @return spi raw irq
uint32_t spi_cs_irq_on_comp_enable(uint8_t spi_cs_irq_on_comp_enable);

/// @brief 
void spi_cs_irq_on_comp_enable_clear();

uint8_t spi_cs_irq_on_comp_enable_get();

void spi_ltoh_set(register uint32_t spi_ltoh);

/// @brief reset spi reload
void spi_ltoh_reset();

/// @brief clear spi reload
void spi_ltoh_clear();

/// @brief get spi reload
/// @return spi reload
uint32_t spi_ltoh_get();

uint32_t spi_cs_get();

void spi_cs_set(register uint32_t spi_cs);


/// @brief reset spi control
void spi_cs_reset();

/// @brief clear spi control
void spi_cs_clear();


/// @brief set spi load
/// @param spi_fifo 
void spi_fifo_set(register uint32_t spi_fifo);

/// @brief reset spi fifo
void spi_fifo_reset();

/// @brief clear spi fifo
void spi_fifo_clear();

/// @brief get spi fifo
/// @return fifo value of spi
uint32_t spi_fifo_get();



/// @brief set spi load
/// @param spi_dc 
void spi_dc_set(register uint32_t spi_dc);

/// @brief reset spi dc
void spi_dc_reset();

/// @brief clear spi dc
void spi_dc_clear();

/// @brief get spi dc
/// @return dc value of spi
uint32_t spi_dc_get();



/// @brief set spi load
/// @param spi_dlen 
void spi_dlen_set(register uint32_t spi_dlen);

/// @brief reset spi dlen
void spi_dlen_reset();

/// @brief clear spi dlen
void spi_dlen_clear();

/// @brief get spi dlen
/// @return dlen value of spi
uint32_t spi_dlen_get();


/// @brief set spi load
/// @param spi_clk 
void spi_clk_set(register uint32_t spi_clk);


/// @brief reset spi clk
void spi_clk_reset();

/// @brief clear spi clk
void spi_clk_clear();

/// @brief get spi clk
/// @return clk value of spi
uint32_t spi_clk_get()



/// @brief get spi clk
/// @return clk value of spi
uint8_t spi_long_enable_get();

/// @brief get spi clk
/// @return clk value of spi
void spi_long_enable_set(uint8_t spi_long_enable);



/// @brief get spi clk
/// @return clk value of spi
void spi_dma_inlossi_enable_set(uint8_t spi_long_enable);

/// @brief get spi clk
/// @return clk value of spi
uint8_t spi_dma_inlossi_enable_get();


/// @brief get spi clk
/// @return clk value of spi
void spi_cspol0_set(uint8_t spi_cspol0);

/// @brief get spi clk
/// @return clk value of spi
uint8_t spi_cspol0_get();


/// @brief get spi clk
/// @return clk value of spi
uint8_t spi_rx_full_fifo_get();


/// @brief get spi clk
/// @return clk value of spi
uint8_t spi_rx_empty_fifo_get();


/// @brief get spi clk
/// @return clk value of spi
uint8_t spi_tx_accept_fifo();


/// @brief get spi clk
/// @return clk value of spi
uint8_t spi_rx_contains_fifo();


/// @brief get spi clk
/// @return clk value of spi
uint8_t spi_tdone_get();



/// @brief get spi clk
/// @return clk value of spi
void spi_len_lossi_set(uint8_t spi_len_lossi);

/// @brief get spi clk
/// @return clk value of spi
uint8_t spi_len_lossi_get();


/// @brief get spi clk
/// @return clk value of spi
void spi_ren_set(uint8_t spi_ren);

/// @brief get spi clk
/// @return clk value of spi
uint8_t spi_ren_get();



/// @brief get spi clk
/// @return clk value of spi
void spi_adcs_set(uint8_t spi_adcs);

/// @brief get spi clk
/// @return clk value of spi
uint8_t spi_adcs_get();





/// @brief get spi clk
/// @return clk value of spi
uint8_t spi_dma_enable_get();

/// @brief get spi clk
/// @return clk value of spi
void spi_dma_enable_set(uint8_t spi_dma_enable);



/// @brief get spi clk
/// @return clk value of spi
uint8_t spi_dma_ta_get();

/// @brief get spi clk
/// @return clk value of spi
void spi_dma_ta_set(uint8_t spi_ta);




/// @brief get spi clk
/// @return clk value of spi
uint8_t spi_cspol_get();

/// @brief get spi clk
/// @return clk value of spi
void spi_cspol_set(uint8_t spi_cspol);




/// @brief get spi clk
/// @return clk value of spi
uint8_t spi_cpol_get();

/// @brief get spi clk
/// @return clk value of spi
void spi_cpol_set(uint8_t spi_cpol);





/// @brief get spi clk
/// @return clk value of spi
uint8_t spi_clear_get();

/// @brief get spi clk
/// @return clk value of spi
void spi_clear_set(uint8_t spi_clear);





/// @brief get spi clk
/// @return clk value of spi
uint8_t spi_cpha_get();

/// @brief get spi clk
/// @return clk value of spi
void spi_cpha_set(uint8_t spi_cpha);



/// @brief get spi clk
/// @return clk value of spi
uint8_t spi_cs_get();

/// @brief get spi clk
/// @return clk value of spi
void spi_cs_set(uint8_t spi_cs);














#endif