\mainpage Main Page
 
---
# OBDII click

OBDII click offers a unique opportunity to tap into the car diagnostic systems. 

<p align="center">
  <img src="https://download.mikroe.com/images/click_for_ide/obdii_click.png" height=300px>
</p>

[click Product page](https://www.mikroe.com/obdii-click)

---


#### Click library 

- **Author**        : MikroE Team
- **Date**          : Jun 2020.
- **Type**          : UART GPS/GNSS type


# Software Support

We provide a library for the Obdii Click 
as well as a demo application (example), developed using MikroElektronika 
[compilers](https://shop.mikroe.com/compilers). 
The demo can run on all the main MikroElektronika [development boards](https://shop.mikroe.com/development-boards).

Package can be downloaded/installed directly form compilers IDE(recommended way), or downloaded from our LibStock, or found on mikroE github account. 

## Library Description

> This library contains API for Obdii Click driver.

#### Standard key functions :

- Config Object Initialization function.
> void obdii_cfg_setup ( obdii_cfg_t *cfg ); 
 
- Initialization function.
> OBDII_RETVAL obdii_init ( obdii_t *ctx, obdii_cfg_t *cfg );

#### Example key functions :

- Reset module.
> void obdii_module_reset ( obdii_t *ctx );
 
- Generic read function.
> uint16_t obdii_generic_read ( obdii_t *ctx, char *data_buf, uint16_t max_len );

- Function for send command.
> void obdii_send_command ( obdii_t *ctx, char *command );

## Examples Description

> This example reads and processes data from OBDII clicks. 

**The demo application is composed of two sections :**

### Application Init 

> Initializes driver and reset module.

```c

void application_init ( void )
{
    log_cfg_t log_cfg;
    obdii_cfg_t cfg;

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

    obdii_cfg_setup( &cfg );
    OBDII_MAP_MIKROBUS( cfg, MIKROBUS_1 );
    obdii_init( &obdii, &cfg );
    obdII_send_command( &obdii, OBDII_AUTOMODE );
    obdii_process( );

    log_printf( &logger, "OBD II initialized \r\n" );
}
  
```

### Application Task

> Reads the received data.

```c

void application_task ( void )
{
    uint8_t tmp[ 25 ];

    obdII_send_command( &obdii, RPM_CMD );
    obdii_process( );
        

    memcpy( &tmp[0], &current_parser_buf[11], 4 );
    tmp[5] = 0;
    rpm = xtoi( &tmp[0] );
    rpm = rpm / 4;

    log_printf( &logger, "Current RPM : %d\r\n", rpm );

    Delay_ms( 1000 );

    obdii_process( );
}

```


The full application code, and ready to use projects can be  installed directly form compilers IDE(recommneded) or found on LibStock page or mikroE GitHub accaunt.

**Other mikroE Libraries used in the example:** 

- MikroSDK.Board
- MikroSDK.Log
- Click.Obdii

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
