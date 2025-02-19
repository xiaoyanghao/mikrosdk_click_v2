/*!
 * \file 
 * \brief CanFd2 Click example
 * 
 * # Description
 * This example reads and processes data from CAN FD 2 clicks.
 *
 * The demo application is composed of two sections :
 * 
 * ## Application Init 
 * Initializes the driver and configures the click board for the selected mode.
 * 
 * ## Application Task  
 * Depending on the selected mode, it reads all the received data or sends the desired message
 * every 2 seconds.
 * 
 * ## Additional Function
 * - canfd2_process ( ) - The general process of collecting the received data.
 * 
 * \author MikroE Team
 *
 */
// ------------------------------------------------------------------- INCLUDES

#include "board.h"
#include "log.h"
#include "canfd2.h"
#include "string.h"

#define PROCESS_RX_BUFFER_SIZE 500

#define TEXT_TO_SEND "MikroE\r\n"

// ------------------------------------------------------------------ VARIABLES

#define DEMO_APP_RECEIVER
// #define DEMO_APP_TRANSMITTER

static canfd2_t canfd2;
static log_t logger;

// ------------------------------------------------------- ADDITIONAL FUNCTIONS

static void canfd2_process ( void )
{
    int32_t rsp_size;
    
    char uart_rx_buffer[ PROCESS_RX_BUFFER_SIZE ] = { 0 };
    uint8_t check_buf_cnt;
    
    rsp_size = canfd2_generic_read( &canfd2, uart_rx_buffer, PROCESS_RX_BUFFER_SIZE );

    if ( rsp_size > 0 )
    {  
        log_printf( &logger, "Received data: " );
        
        for ( check_buf_cnt = 0; check_buf_cnt < rsp_size; check_buf_cnt++ )
        {
            log_printf( &logger, "%c", uart_rx_buffer[ check_buf_cnt ] );
        }
    }
    Delay_ms( 100 );
}

// ------------------------------------------------------ APPLICATION FUNCTIONS

void application_init ( void )
{
    log_cfg_t log_cfg;
    canfd2_cfg_t cfg;

    /** 
     * Logger initialization.
     * Default baud rate: 115200
     * Default log level: LOG_LEVEL_DEBUG
     * @note If USB_UART_RX and USB_UART_TX 
     * are defined as HAL_PIN_NC, you will 
     * need to define them manually for log to work. 
     * See @b LOG_MAP_USB_UART macro definition for detailed explanation.
     */
    LOG_MAP_USB_UART( log_cfg );
    log_init( &logger, &log_cfg );
    log_info( &logger, "---- Application Init ----" );

    //  Click initialization.

    canfd2_cfg_setup( &cfg );
    CANFD2_MAP_MIKROBUS( cfg, MIKROBUS_1 );
    canfd2_init( &canfd2, &cfg );

    Delay_ms( 100 );
    
#ifdef DEMO_APP_RECEIVER

    canfd2_set_mode( &canfd2, CANFD2_OP_MODE_RECEIVE_ONLY );
    if ( CANFD2_OP_MODE_RECEIVE_ONLY == canfd2_get_mode ( &canfd2 ) )
    {
        log_info( &logger, "--- RECEIVER MODE ---" );
    }
    else
    {
        log_info( &logger, "--- ERROR ---" );
        log_printf( &logger, "Please restart your system.\r\n" );
        for ( ; ; );
    }

#endif
#ifdef DEMO_APP_TRANSMITTER

    canfd2_set_mode( &canfd2, CANFD2_OP_MODE_NORMAL );
    if ( CANFD2_OP_MODE_NORMAL == canfd2_get_mode ( &canfd2 ) )
    {
        log_info( &logger, "--- TRANSMITTER MODE ---" );
    }
    else
    {
        log_info( &logger, "--- ERROR ---" );
        log_printf( &logger, "Please restart your system.\r\n" );
        for ( ; ; );
    }

#endif
    Delay_ms( 100 );
}

void application_task ( void )
{
#ifdef DEMO_APP_RECEIVER
    canfd2_process( );
#endif
#ifdef DEMO_APP_TRANSMITTER
    canfd2_generic_write( &canfd2, TEXT_TO_SEND, 8 );
    log_info( &logger, "--- The message is sent ---" );
    Delay_ms( 3000 );
#endif
}

void main ( void )
{
    application_init( );

    for ( ; ; )
    {
        application_task( );
    }
}

// ------------------------------------------------------------------------ END
