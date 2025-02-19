
---
# Fiber Opt click

Fiber Opt click is a compact and easy solution for adding fiber-optic communication to your design. It features IF-D91 fiber-optic photodiode, IF-E97 fiber-optic LED diode as well as two operational amplifiers.

<p align="center">
  <img src="https://download.mikroe.com/images/click_for_ide/fiberopt_click.png" height=300px>
</p>

[click Product page](https://www.mikroe.com/fiber-opt-33v-click)

---


#### Click library 

- **Author**        : MikroE Team
- **Date**          : Dec 2019.
- **Type**          : UART type


# Software Support

We provide a library for the FiberOpt Click 
as well as a demo application (example), developed using MikroElektronika 
[compilers](https://shop.mikroe.com/compilers). 
The demo can run on all the main MikroElektronika [development boards](https://shop.mikroe.com/development-boards).

Package can be downloaded/installed directly form compilers IDE(recommended way), or downloaded from our LibStock, or found on mikroE github account. 

## Library Description

> This library contains API for FiberOpt Click driver.

#### Standard key functions :

- Config Object Initialization function.
> void fiberopt_cfg_setup ( fiberopt_cfg_t *cfg ); 
 
- Initialization function.
> FIBEROPT_RETVAL fiberopt_init ( fiberopt_t *ctx, fiberopt_cfg_t *cfg );

#### Example key functions :

- Generic single write function.
> void fiberopt_generic_single_write ( fiberopt_t *ctx, fiberopt_data_t tx_data );
 
- Generic single read function.
> fiberopt_data_t fiberopt_generic_single_read ( fiberopt_t *ctx );

- Generic multi read function.
> void fiberopt_generic_multi_read ( fiberopt_t *ctx, fiberopt_data_t *data_buf,  uart_length_t len );

## Examples Description

> This application is an add-on for fiber-optical communication.

**The demo application is composed of two sections :**

### Application Init 

> Initalizes UART driver and makes an initial log.

```c

void application_init ( void )
{
    log_cfg_t log_cfg;
    fiberopt_cfg_t cfg;

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
    /log_info( &logger, "---- Application Init ----" );

    //  Click initialization.

    fiberopt_cfg_setup( &cfg );
    FIBEROPT_MAP_MIKROBUS( cfg, MIKROBUS_1 );
    fiberopt_init( &fiberopt, &cfg );

    log_printf( &logger, "Initialized \r\n" );
    Delay_ms( 100 );
}
  
```

### Application Task

> Example can either check if new data byte is received in rx buffer (ready for reading),
> if ready than reads one byte from rx buffer and displays on USART terminal, or transmit message every 2 seconds.

```c

void application_task ( void )
{
    fiberopt_data_t tmp;
    
    //  Task implementation.
    
#ifdef DEMO_APP_RECEIVER

       // RECEIVER - UART polling

       tmp =  fiberopt_generic_single_read( &fiberopt );
       log_write( &logger, &tmp, LOG_FORMAT_BYTE );
#endif
#ifdef DEMO_APP_TRANSMITER

       // TRANSMITER - TX each 2 sec
       
       fiberopt_generic_multi_write( &fiberopt, demo_message, 9 );
       Delay_ms( 2000 );
#endif
}

```

The full application code, and ready to use projects can be  installed directly form compilers IDE(recommneded) or found on LibStock page or mikroE GitHub accaunt.

**Other mikroE Libraries used in the example:** 

- MikroSDK.Board
- MikroSDK.Log
- Click.FiberOpt

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
