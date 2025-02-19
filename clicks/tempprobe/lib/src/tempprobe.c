/****************************************************************************
** Copyright (C) 2020 MikroElektronika d.o.o.
** Contact: https://www.mikroe.com/contact
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
** copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions:
** The above copyright notice and this permission notice shall be
** included in all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
** OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
** IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
** DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT
** OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
**  USE OR OTHER DEALINGS IN THE SOFTWARE.
****************************************************************************/

/*!
 * @file tempprobe.c
 * @brief Temp Probe Click Driver.
 */

#include "tempprobe.h"

/**
 * @brief Dummy data.
 * @details Definition of dummy data.
 */
#define DUMMY  0x00

// -------------------------------------------- PRIVATE FUNCTION DECLARATIONS

/**
 * @brief Temp Probe Click reset delay.
 * @details Reset delay of 100 milliseconds.
 */
void dev_reset_delay ( void );

// --------------------------------------------------------- PUBLIC FUNCTIONS

void tempprobe_cfg_setup ( tempprobe_cfg_t *cfg ) {
    cfg->sck  = HAL_PIN_NC;
    cfg->miso = HAL_PIN_NC;
    cfg->mosi = HAL_PIN_NC;
    cfg->cs   = HAL_PIN_NC;
    cfg->rst = HAL_PIN_NC;
    cfg->int_pin = HAL_PIN_NC;

    cfg->spi_speed   = 100000;
    cfg->spi_mode    = SPI_MASTER_MODE_0;
    cfg->cs_polarity = SPI_MASTER_CHIP_SELECT_POLARITY_ACTIVE_LOW;
}

err_t tempprobe_init ( tempprobe_t *ctx, tempprobe_cfg_t *cfg ) {
    spi_master_config_t spi_cfg;

    spi_master_configure_default( &spi_cfg );

    spi_cfg.sck  = cfg->sck;
    spi_cfg.miso = cfg->miso;
    spi_cfg.mosi = cfg->mosi;

    ctx->chip_select = cfg->cs;

    if ( SPI_MASTER_ERROR == spi_master_open( &ctx->spi, &spi_cfg ) ) {
        return SPI_MASTER_ERROR;
    }

    if ( SPI_MASTER_ERROR == spi_master_set_default_write_data( &ctx->spi, DUMMY ) ) {
        return SPI_MASTER_ERROR;
    }

    if ( SPI_MASTER_ERROR == spi_master_set_mode( &ctx->spi, cfg->spi_mode ) ) {
        return SPI_MASTER_ERROR;
    }

    if ( SPI_MASTER_ERROR == spi_master_set_speed( &ctx->spi, cfg->spi_speed ) ) {
        return SPI_MASTER_ERROR;
    }

    spi_master_set_chip_select_polarity( cfg->cs_polarity );
    spi_master_deselect_device( ctx->chip_select );

    digital_out_init( &ctx->rst, cfg->rst );

    digital_in_init( &ctx->int_pin, cfg->int_pin );

    return SPI_MASTER_SUCCESS;
}

err_t tempprobe_default_cfg ( tempprobe_t *ctx ) {
    err_t error_flag;
    ctx->drv_temperature_format = TEMPPROBE_CELSIUS;

    error_flag = tempprobe_write_word( ctx, TEMPPROBE_REG_PN_JUNCTION_ASSIGN_DATA,
                                            TEMPPROBE_PN_JUNCTION_DEF_CFG );
    error_flag |= tempprobe_write_word( ctx, TEMPPROBE_REG_THERMO_K_ASSIGN_DATA, 
                                             TEMPPROBE_TYPE_K_THERMOCOUPLE |
                                             TEMPPROBE_CH9_COLD_JUNCTION |
                                             TEMPPROBE_SINGLE_ENDED_1MILIA_CURR );
    error_flag |= tempprobe_write_word( ctx, TEMPPROBE_REG_MEAS_MULTIPLE_CHANN_MASK, 
                                             TEMPPROBE_CH9_MASK |
                                             TEMPPROBE_CH6_MASK |
                                             TEMPPROBE_CH1_MASK );
    error_flag |= tempprobe_write_byte( ctx, TEMPPROBE_REG_GLOBAL_CONFIG, 
                                             TEMPPROBE_GLOBAL_CONFIG_DEF_CFG );
    return error_flag;
}

err_t tempprobe_generic_write ( tempprobe_t *ctx, uint8_t reg, uint8_t *data_in, uint8_t len ) {
    uint8_t tx_buf[ 257 ] = { 0 };

    tx_buf[ 0 ] = reg;
    for ( uint8_t cnt = 1; cnt <= len; cnt++ ) {
        tx_buf[ cnt ] = data_in[ cnt - 1 ];
    }

    spi_master_select_device( ctx->chip_select );
    err_t error_flag = spi_master_write( &ctx->spi, tx_buf, len + 1 );
    spi_master_deselect_device( ctx->chip_select );

    return error_flag;
}

err_t tempprobe_generic_read ( tempprobe_t *ctx, uint8_t reg, uint8_t *data_out, uint8_t len ) {
    spi_master_select_device( ctx->chip_select );
    err_t error_flag = spi_master_write_then_read( &ctx->spi, &reg, 1, data_out, len );
    spi_master_deselect_device( ctx->chip_select );

    return error_flag;
}

err_t tempprobe_write_byte ( tempprobe_t *ctx, uint16_t reg_addr, uint8_t data_in ) {
    uint8_t tx_buff[ 4 ];

    if ( reg_addr > 0x03FF ) {
        return TEMPPROBE_REG_ADDR_ERROR;
    }

    tx_buff[ 0 ] = TEMPPROBE_SPI_WRITE_INSTR;
    tx_buff[ 1 ] = reg_addr >> 8;
    tx_buff[ 2 ] = reg_addr;
    tx_buff[ 3 ] = data_in;

    spi_master_select_device( ctx->chip_select );
    err_t error_flag = spi_master_write( &ctx->spi, tx_buff, 4 );
    spi_master_deselect_device( ctx->chip_select );

    if ( reg_addr == TEMPPROBE_REG_GLOBAL_CONFIG ) {
        if ( data_in & TEMPPROBE_FAHRENHEIT ) {
            ctx->drv_temperature_format = TEMPPROBE_FAHRENHEIT;
        } else {
            ctx->drv_temperature_format = TEMPPROBE_CELSIUS;
        }
    }

    return error_flag;
}

err_t tempprobe_write_word ( tempprobe_t *ctx, uint16_t reg_addr, uint32_t data_in ) {
    uint8_t tx_buff[ 7 ];

    if ( reg_addr > 0x03FF ) {
        return TEMPPROBE_REG_ADDR_ERROR;
    }

    tx_buff[ 0 ] = TEMPPROBE_SPI_WRITE_INSTR;
    tx_buff[ 1 ] = reg_addr >> 8;
    tx_buff[ 2 ] = reg_addr;
    tx_buff[ 3 ] = data_in >> 24;
    tx_buff[ 4 ] = data_in >> 16;
    tx_buff[ 5 ] = data_in >> 8;
    tx_buff[ 6 ] = data_in;

    spi_master_select_device( ctx->chip_select );
    err_t error_flag = spi_master_write( &ctx->spi, tx_buff, 7 );
    spi_master_deselect_device( ctx->chip_select );

    return error_flag;
}

err_t tempprobe_read_bytes( tempprobe_t *ctx, uint16_t reg_addr, uint8_t *data_out, uint8_t n_bytes ) {
    uint8_t rx_buff[ 256 ] = { 0 };
    uint8_t tx_buff[ 256 ] = { 0 };
    uint8_t cnt;

    if ( reg_addr > 0x03FF ) {
        return TEMPPROBE_REG_ADDR_ERROR;
    }
    if ( n_bytes > 253 ) {
        return TEMPPROBE_NUM_BYTES_ERROR;
    }

    tx_buff[ 0 ] = TEMPPROBE_SPI_READ_INSTR;
    tx_buff[ 1 ] = reg_addr >> 8;
    tx_buff[ 2 ] = reg_addr;

    spi_master_select_device( ctx->chip_select );
    err_t error_flag = spi_master_write_then_read( &ctx->spi, tx_buff, 3, rx_buff, n_bytes );
    spi_master_deselect_device( ctx->chip_select );

    for ( cnt = 0; cnt < n_bytes; cnt++ ) {
        data_out[ cnt ] = rx_buff[ cnt ];
    }

    return error_flag;
}

err_t tempprobe_read_word ( tempprobe_t *ctx, uint16_t reg_addr, uint32_t *data_out ) {
    uint8_t rx_buff[ 7 ] = { 0 };
    uint8_t tx_buff[ 7 ] = { 0 };
    uint32_t temp_var;

    if ( reg_addr > 0x03FF ) {
        return TEMPPROBE_REG_ADDR_ERROR;
    }

    tx_buff[ 0 ] = TEMPPROBE_SPI_READ_INSTR;
    tx_buff[ 1 ] = reg_addr >> 8;
    tx_buff[ 2 ] = reg_addr;

    spi_master_select_device( ctx->chip_select );
    err_t error_flag = spi_master_write_then_read( &ctx->spi, tx_buff, 3, rx_buff, 4 );
    spi_master_deselect_device( ctx->chip_select );

    temp_var = rx_buff[ 0 ];
    temp_var <<= 8;
    temp_var |= rx_buff[ 1 ];
    temp_var <<= 8;
    temp_var |= rx_buff[ 2 ];
    temp_var <<= 8;
    temp_var |= rx_buff[ 3 ];
    *data_out = temp_var;

    return error_flag;
}

uint8_t tempprobe_check_busy ( tempprobe_t *ctx ) {
    uint8_t check_byte;

    tempprobe_read_bytes( ctx, TEMPPROBE_REG_COMM_STATUS, &check_byte, 1 );
    check_byte >>= 6;

    return check_byte;
}

void tempprobe_reset( tempprobe_t *ctx ) {
    digital_out_low( &ctx->rst );
    dev_reset_delay( );
    digital_out_high( &ctx->rst );
    dev_reset_delay( );

    while ( TEMPPROBE_NO_BUSY_STATE != tempprobe_check_busy( ctx ) );
}

uint8_t tempprobe_write_eeprom_data ( tempprobe_t *ctx ) {
    uint8_t status_byte;

    tempprobe_write_word( ctx, TEMPPROBE_REG_EEPROM_KEY, TEMPPROBE_EEPROM_KEY );
    tempprobe_write_byte( ctx, TEMPPROBE_REG_COMM_STATUS, TEMPPROBE_WRITE_EEPROM );

    while ( TEMPPROBE_NO_BUSY_STATE != tempprobe_check_busy( ctx ) );

    tempprobe_read_bytes( ctx, TEMPPROBE_REG_EEPROM_STATUS, &status_byte, 1 );
    return status_byte;
}

uint8_t tempprobe_read_eeprom_data ( tempprobe_t *ctx ) {
    uint8_t status_byte;

    tempprobe_write_word( ctx, TEMPPROBE_REG_EEPROM_KEY, TEMPPROBE_EEPROM_KEY );
    tempprobe_write_byte( ctx, TEMPPROBE_REG_COMM_STATUS, TEMPPROBE_READ_EEPROM );

    while ( TEMPPROBE_NO_BUSY_STATE != tempprobe_check_busy( ctx ) );

    tempprobe_read_bytes( ctx, TEMPPROBE_REG_EEPROM_READ_RES_CODE, &status_byte, 1 );

    if ( status_byte == TEMPPROBE_EEPROM_READ_PASS ) {
        return status_byte;
    } else {
        tempprobe_read_bytes( ctx, TEMPPROBE_REG_EEPROM_STATUS, &status_byte, 1 );
        return status_byte;
    }
}

uint8_t tempprobe_read_temp ( tempprobe_t *ctx, uint16_t sel_channel, float *data_out ) {
    uint16_t channel_addr;
    uint32_t temp_data;
    uint8_t temp_status;
    float temp_float;

    channel_addr = sel_channel;
    tempprobe_read_word( ctx, channel_addr, &temp_data );

    temp_status = temp_data >> 24;
    temp_data &= 0x00FFFFFF;

    if ( temp_status != 1 ) {
        return temp_status;
    }

    temp_float = 1.0 / 1024.0;

    if ( temp_data & 0x00800000 ) {
        temp_data = ~temp_data;
        temp_data &= 0x00FFFFFF;
        temp_data += 1;
        temp_float = -temp_data;
    }

    temp_float *= temp_data;
    *data_out = temp_float ;

    return temp_status;
}

// --------------------------------------------- PRIVATE FUNCTION DEFINITIONS

void dev_reset_delay ( void ) {
    Delay_100ms( );
    Delay_100ms( );
}

// ------------------------------------------------------------------------- END
