#include "periph.h"

extern void PUT32 ( unsigned int, unsigned int );
extern void PUT16 ( unsigned int, unsigned int );
extern void PUT8 ( unsigned int, unsigned int );

extern unsigned int GET32 ( unsigned int );
extern void dummy ( unsigned int );


int notmain ( void )
{
    leds_off();
    uart_init();
    hexstring(0x12345678);

    return(0);
}
