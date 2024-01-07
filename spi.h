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
#define SPI_RUNNING 1
#define SPI_FREE 1
#define SPI_BUSY 0
#define SPI_NO_OWNER 0 
#define SPI_NO_ID 0


#define SPI_IS_VALID_SPI_CS(SPI_CS) SPI_CS >= 0 && SPI_CS <=2
#define SPI_IS_VALID_SPI_CPHA(SPI_CPHA) SPI_CPHA >= 0 && SPI_CPHA <=1
#define SPI_IS_VALID_SPI_CPOL(SPI_CPOL) SPI_CPOL >= 0 && SPI_CPOL <=1
#define SPI_IS_VALID_SPI_CLEAR(SPI_CLEAR) SPI_CLEAR >= 0 && SPI_CLEAR <= 3
#define SPI_IS_VALID_SPI_CSPOL(SPI_CSPOL) SPI_CSPOL >= 0 && SPI_CSPOL <=1
#define SPI_IS_VALID_SPI_TA(SPI_TA) SPI_TA >= 0 && SPI_TA <=1
#define SPI_IS_VALID_SPI_DMA_ENABLE(SPI_DMA_ENABLE) SPI_DMA_ENABLE >= 0 && SPI_DMA_ENABLE <=1
#define SPI_IS_VALID_SPI_REN(SPI_REN) SPI_REN >= 0 && SPI_REN <=1
#define SPI_IS_VALID_SPI_LEN(SPI_LEN) SPI_LEN >= 0 && SPI_LEN <=1
#define SPI_IS_VALID_SPI_CSPOL0(SPI_CSPOL0) SPI_CSPOL0 >= 0 && SPI_CSPOL0 <=1
#define SPI_IS_VALID_SPI_CSPOL1(SPI_CSPOL1) SPI_CSPOL1 >= 0 && SPI_CSPOL1 <=1
#define SPI_IS_VALID_SPI_CSPOL2(SPI_CSPOL2) SPI_CSPOL2 >= 0 && SPI_CSPOL2 <=1
#define SPI_IS_VALID_SPI_LONG_ENABLE(SPI_LONG_ENABLE) SPI_LONG_ENABLE >= 0 && SPI_LONG_ENABLE <=1
#define SPI_IS_VALID_SPI_IRQ_ENABLE(SPI_IRQ_ENABLE) SPI_IRQ_ENABLE >= 0 && SPI_IRQ_ENABLE <=1
#define SPI_IS_VALID_SPI_CLK(SPI_CLK) SPI_CLK >= 0 && SPI_CLK < (2 << 16)
#define SPI_IS_VALID_SPI_DLEN(SPI_DLEN) SPI_DLEN >= 0 && SPI_DLEN < (2 << 16)
#define SPI_IS_VALID_SPI_LTOH(SPI_LTOH) SPI_LTOH >= 0 && SPI_LTOH <=15
#define SPI_IS_VALID_SPI_DC(SPI_DC) SPI_DC >= 0 && SPI_DC < (2 << 32)

typedef struct spi spi_t;

//BrainOS spi internal routines
/// @brief boot spis
void spi_boot();

/// @brief shutdown spis
void spi_shutdown();

void spi_init(register uint8_t spi_cs, register uint8_t spi_cpha, register uint8_t spi_cpol, register uint8_t spi_clear, register uint8_t spi_cspol,register uint8_t spi_ta, register uint8_t spi_dma_en, register uint8_t spi_irq_tx_d,register uint8_t spi_irq_rx_r,register uint8_t spi_adcs, register uint8_t spi_read_en, register uint8_t spi_lossi_en, register uint8_t spi_cspol0, register uint8_t spi_cspol1, register uint8_t spi_cspol2, register uint8_t spi_dma_lossi_enable, register uint8_t spi_lossi_long_en);


int spi_open( int spi_owner);
int spi_close( int spi_owner);
int spi_read( int spi_owner,  uint32_t *data);
int spi_write(int spi_owner,  uint32_t data);
int spi_lossi_cmd_write( int spi_owner,  uint32_t cmd);
int spi_lossi_cmd_read( int spi_owner,  uint32_t *cmd);
int spi_lossi_param_write( int spi_owner, uint32_t param);
int spi_lossi_param_read( int spi_owner,  uint32_t *param);







//Brain OS high level API
//BrainOS spi system level API

/// @brief 
/// @return 
uint8_t spi_cs_cs_get();

/// @brief 
/// @param spi_cs_cs 
void spi_cs_cs_set(uint8_t spi_cs_cs);

/// @brief 
/// @return 
uint8_t spi_cs_cpol_get();

/// @brief 
/// @param spi_cpol 
void spi_cs_cpol_set(uint8_t spi_cpol);

/// @brief 
/// @return 
uint8_t spi_cs_cspol_get();

/// @brief 
/// @param spi_cspol 
void spi_cs_cspol_set(uint8_t spi_cspol);

/// @brief 
/// @param spi_cspol0 
void spi_cs_cspol0_set(uint8_t spi_cspol0);

/// @brief 
/// @return 
uint8_t spi_cs_cspol0_get();

/// @brief 
/// @param spi_cs_cspol1 
void spi_cs_cspol1_set(uint8_t spi_cs_cspol1);

/// @brief 
/// @return 
uint8_t spi_cs_cspol1_get();

/// @brief 
/// @param spi_cs_cspol2 
void spi_cs_cspol2_set(uint8_t spi_cs_cspol2);

/// @brief 
/// @return 
uint8_t spi_cs_cspol2_get();

/// @brief 
/// @return 
uint8_t spi_cs_cpha_get();

/// @brief 
/// @param spi_cpha 
void spi_cs_cpha_set(uint8_t spi_cpha);

/// @brief 
/// @param spi_irq_on_rxr 
/// @return 
void spi_cs_irq_on_rxr_set(uint8_t spi_irq_on_rxr);

/// @brief 
/// @return 
uint8_t spi_cs_irq_on_rxr_get();

/// @brief 
/// @param spi_irq_on_tx_done 
/// @return 
void spi_cs_irq_on_tx_done_set(uint8_t spi_irq_on_tx_done);

/// @brief 
/// @return 
uint8_t spi_cs_irq_on_tx_done_get();

/// @brief 
/// @return 
uint8_t spi_cs_long_enable_get();

/// @brief 
/// @param spi_len_long 
void spi_cs_len_long_set(uint8_t spi_len_long);

/// @brief 
/// @param spi_dma_len 
void spi_cs_dma_len_set(uint8_t spi_dma_len);

/// @brief 
/// @return 
uint8_t spi_cs_dma_len_get();

/// @brief 
/// @return 
uint8_t spi_cs_rx_reading_get();

/// @brief 
/// @return 
uint8_t spi_cs_rx_full_get();

/// @brief 
/// @return 
uint8_t spi_cs_tx_full_fifo_get();

/// @brief 
/// @return 
uint8_t spi_cs_rx_empty_fifo_get();

/// @brief 
/// @return 
uint8_t spi_cs_tdone_get();

/// @brief 
/// @param spi_len 
void spi_cs_len_set(uint8_t spi_len);

/// @brief 
/// @return 
uint8_t spi_cs_len_get();

/// @brief 
/// @param spi_ren 
void spi_cs_ren_set(uint8_t spi_ren);

/// @brief 
/// @return 
uint8_t spi_cs_ren_get();

/// @brief 
/// @param spi_adcs 
void spi_cs_adcs_set(uint8_t spi_adcs);

/// @brief 
/// @return 
uint8_t spi_cs_adcs_get();

/// @brief 
/// @return 
uint8_t spi_cs_dma_enable_get();

/// @brief 
/// @param spi_dma_enable 
void spi_cs_dma_enable_set(uint8_t spi_dma_enable);

/// @brief 
/// @return 
uint8_t spi_cs_ta_get();

/// @brief 
/// @param spi_ta 
void spi_cs_ta_set(uint8_t spi_ta);

/// @brief 
/// @return 
uint8_t spi_cs_clear_get();

/// @brief 
/// @param spi_clear 
void spi_cs_clear_set(uint8_t spi_clear);


//BrainOS spi control level API
/// @brief 
/// @param spi_ltoh 
void spi_ltoh_set(uint32_t spi_ltoh);

/// @brief 
void spi_ltoh_reset();

/// @brief 
void spi_ltoh_clear();

/// @brief 
/// @return 
uint32_t spi_ltoh_get();

/// @brief 
/// @return 
uint32_t spi_cs_get();


/// @brief 
/// @param spi_cs 
void spi_cs_set(uint32_t spi_cs);


/// @brief 
void spi_cs_reset();

/// @brief 
void spi_cs_clear();


/// @brief 
/// @param spi_fifo 
void spi_fifo_set(uint32_t spi_fifo);

/// @brief 
void spi_fifo_reset();

/// @brief 
void spi_fifo_clear();

/// @brief 
/// @return 
uint32_t spi_fifo_get();

/// @brief 
/// @param spi_dc 
void spi_dc_set(uint32_t spi_dc);

/// @brief 
void spi_dc_reset();

/// @brief 
void spi_dc_clear();

/// @brief 
/// @return 
uint32_t spi_dc_get();

/// @brief 
/// @param spi_dlen 
void spi_dlen_set(uint32_t spi_dlen);

/// @brief 
void spi_dlen_reset();

/// @brief 
void spi_dlen_clear();

/// @brief 
/// @return 
uint32_t spi_dlen_get();

/// @brief 
/// @param spi_clk 
void spi_clk_set(uint32_t spi_clk);

/// @brief 
void spi_clk_reset();

/// @brief 
void spi_clk_clear();

/// @brief 
/// @return 
uint32_t spi_clk_get();














#endif