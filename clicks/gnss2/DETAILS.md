
---
# GNSS2 click

GNSS2 click carries Quectel’s L76 module and an SMA antenna connector.

<p align="center">
  <img src="https://download.mikroe.com/images/click_for_ide/gnss2_click.png" height=300px>
</p>

[click Product page](https://www.mikroe.com/gnss-2-click)

---


#### Click library 

- **Author**        : MikroE Team
- **Date**          : apr 2020.
- **Type**          : UART GPS/GNSS type


# Software Support

We provide a library for the Gnss2 Click 
as well as a demo application (example), developed using MikroElektronika 
[compilers](https://shop.mikroe.com/compilers). 
The demo can run on all the main MikroElektronika [development boards](https://shop.mikroe.com/development-boards).

Package can be downloaded/installed directly form compilers IDE(recommended way), or downloaded from our LibStock, or found on mikroE github account. 

## Library Description

> This library contains API for Gnss2 Click driver.

#### Standard key functions :

- Config Object Initialization function.
> void gnss2_cfg_setup ( gnss2_cfg_t *cfg ); 
 
- Initialization function.
> GNSS2_RETVAL gnss2_init ( gnss2_t *ctx, gnss2_cfg_t *cfg );

- Click Default Configuration function.
> void gnss2_default_cfg ( gnss2_t *ctx );


#### Example key functions :

- Generic parser function.
> gnss2_error_t gnss2_generic_parser ( char *rsp,  uint8_t command, uint8_t element, char *parser_buf );
 
- Generic read function.
> int32_t gnss2_generic_read ( gnss2_t *ctx, char *data_buf, uint16_t max_len );

- Wake-up module.
> void gnss2_module_wakeup ( gnss2_t *ctx );

## Examples Description

> This example reads and processes data from GNSS2 clicks.

**The demo application is composed of two sections :**

### Application Init 

> Initializes driver and wake-up module.

```c

void application_init ( void )
{
    log_cfg_t log_cfg;
    gnss2_cfg_t cfg;

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

    gnss2_cfg_setup( &cfg );
    GNSS2_MAP_MIKROBUS( cfg, MIKROBUS_1 );
    gnss2_init( &gnss2, &cfg );

    gnss2_module_wakeup( &gnss2 );
    Delay_ms( 5000 );
}
  
```

### Application Task

> Reads the received data and parses it.

```c

void application_task ( void )
{
    gnss2_process(  );
    parser_application( current_parser_buf );
} 

```

The full application code, and ready to use projects can be  installed directly form compilers IDE(recommneded) or found on LibStock page or mikroE GitHub accaunt.

**Other mikroE Libraries used in the example:** 

- MikroSDK.Board
- MikroSDK.Log
- Click.Gnss2

**Additional notes and informations**

Depending on the development board you are using, you may need 
[USB UART click](https://shop.mikroe.com/usb-uart-click), 
[USB UART 2 Click](https://shop.mikroe.com/usb-uart-2-click) or 
[RS232 Click](https://shop.mikroe.com/rs232-click) to connect to your PC, for 
development systems with no UART to USB interface available on the board. The 
terminal available in all Mikroelektronika 
[compilers](https://shop.mikroe.com/compilers), or any other terminal application 
of your choice, can be used to read the message.



---
