/*
 * MikroSDK - MikroE Software Development Kit
 * Copyright© 2020 MikroElektronika d.o.o.
 * 
 * Permission is hereby granted, free of charge, to any person 
 * obtaining a copy of this software and associated documentation 
 * files (the "Software"), to deal in the Software without restriction, 
 * including without limitation the rights to use, copy, modify, merge, 
 * publish, distribute, sublicense, and/or sell copies of the Software, 
 * and to permit persons to whom the Software is furnished to do so, 
 * subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be 
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE 
 * OR OTHER DEALINGS IN THE SOFTWARE. 
 */

/*!
 * \file
 *
 */

#include "spectral3.h"
#include "string.h"
#include "stdlib.h"
#include "generic_pointer.h"

// ------------------------------------------------ PUBLIC FUNCTION DEFINITIONS

void spectral3_cfg_setup ( spectral3_cfg_t *cfg )
{
    // Communication gpio pins 

    cfg->rx_pin = HAL_PIN_NC;
    cfg->tx_pin = HAL_PIN_NC;
    
    // Additional gpio pins

    cfg->rst = HAL_PIN_NC;
    cfg->int_pin = HAL_PIN_NC;

    cfg->baud_rate      = 115200;
    cfg->data_bit       = UART_DATA_BITS_DEFAULT;
    cfg->parity_bit     = UART_PARITY_DEFAULT;
    cfg->stop_bit       = UART_STOP_BITS_DEFAULT;
    cfg->uart_blocking  = false;
}

SPECTRAL3_RETVAL spectral3_init ( spectral3_t *ctx, spectral3_cfg_t *cfg )
{
    uart_config_t uart_cfg;
    
    uart_configure_default( &uart_cfg );

    // Ring buffer mapping
    ctx->uart.tx_ring_buffer = ctx->uart_tx_buffer;
    ctx->uart.rx_ring_buffer = ctx->uart_rx_buffer;

    // UART module config
    uart_cfg.rx_pin = cfg->rx_pin;  // UART RX pin. 
    uart_cfg.tx_pin = cfg->tx_pin;  // UART TX pin. 
    uart_cfg.tx_ring_size = sizeof( ctx->uart_tx_buffer );  
    uart_cfg.rx_ring_size = sizeof( ctx->uart_rx_buffer );

    uart_open( &ctx->uart, &uart_cfg );
    uart_set_baud( &ctx->uart, cfg->baud_rate );
    uart_set_parity( &ctx->uart, cfg->parity_bit );
    uart_set_stop_bits( &ctx->uart, cfg->stop_bit );  
    uart_set_data_bits( &ctx->uart, cfg->data_bit );

    uart_set_blocking( &ctx->uart, cfg->uart_blocking );

    // Output pins 

    digital_out_init( &ctx->rst, cfg->rst );

    // Input pins

    digital_in_init( &ctx->int_pin, cfg->int_pin );

    return SPECTRAL3_OK;
}

void spectral3_module_reset ( spectral3_t *ctx )
{
    digital_out_high( &ctx->rst );
    Delay_100ms( );
    digital_out_low( &ctx->rst );
    Delay_100ms( );
    digital_out_high( &ctx->rst );
    Delay_1sec( );
    Delay_1sec( );
}

void spectral3_set_rst_pin ( spectral3_t *ctx, uint8_t state )
{
    digital_out_write( &ctx->rst, state );
}

void spectral3_generic_write ( spectral3_t *ctx, char *data_buf, uint16_t len )
{
    uart_write( &ctx->uart, data_buf, len );
}

int32_t spectral3_generic_read ( spectral3_t *ctx, char *data_buf, uint16_t max_len )
{
    return uart_read( &ctx->uart, data_buf, max_len );
}

void spectral3_send_command ( spectral3_t *ctx, char *command )
{
    char tmp_buf[ 100 ];
    uint8_t len;
    
    memset( tmp_buf, 0, 100 );
    len = strlen( command );
    
    strncpy( tmp_buf, command, len );
    strcat( tmp_buf, "\r\n" );

    spectral3_generic_write( ctx, tmp_buf, strlen( tmp_buf ) );
}

void spectral3_get_data ( char *rsp, uint16_t *c_data )
{
    char * __generic tmp_start;
    char * __generic tmp_end;
    char buff[ 10 ] = { 0 };

    tmp_start = rsp;
    tmp_end = strstr( tmp_start + 1, "," );
    strncpy( buff, tmp_start, tmp_end - ( tmp_start ) );
    c_data[ 0 ] = atof( buff );
    memset( buff, 0, 10 );
    
    tmp_start = strstr( rsp, "," );
    tmp_end = strstr( tmp_start + 1, "," );
    strncpy( buff, tmp_start + 2, tmp_end - ( tmp_start + 2 ) );
    c_data[ 1 ] = atof( buff );
    memset( buff, 0, 10 );
    
    tmp_start = ( tmp_end );
    tmp_end = strstr( tmp_start + 1, "," );
    strncpy( buff, tmp_start + 2, tmp_end - ( tmp_start + 2 ) );
    c_data[ 2 ] = atof( buff );
    memset( buff, 0, 10 );
    
    tmp_start = ( tmp_end ); 
    tmp_end = strstr( tmp_start + 1, "," );
    strncpy( buff, tmp_start + 2, tmp_end - ( tmp_start + 2 ) );
    c_data[ 3 ] = atof( buff );
    memset( buff, 0, 10 );
    
    tmp_start = ( tmp_end );
    tmp_end = strstr( tmp_start + 1, "," );
    strncpy( buff, tmp_start + 2, tmp_end - ( tmp_start + 2 ) );
    c_data[ 4 ] = atof( buff );
    memset( buff, 0, 10 );

    tmp_start = ( tmp_end );
    tmp_end = strstr( tmp_start + 1, "O" );
    strncpy( buff, tmp_start + 2, tmp_end - ( tmp_start + 2 ) );
    c_data[ 5 ] = atof( buff );
}

// ------------------------------------------------------------------------- END

