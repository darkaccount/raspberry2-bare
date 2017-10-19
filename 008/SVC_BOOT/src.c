#include "periph.h"

extern void PUT32 ( unsigned int, unsigned int );
extern void PUT16 ( unsigned int, unsigned int );
extern void PUT8 ( unsigned int, unsigned int );

extern unsigned int GET32 ( unsigned int );
extern void dummy ( unsigned int );


int notmain ( void )
{
    unsigned int ra;
    leds_on();
    uart_init();
    hexstring(0x12a45678);
    
    timer_init();
    while(1)
    {
    blink_off();
    delay(1000000);
    blink_on();
    ra = dhtinit();
    hexstring(ra);
    }
    return(0);
}
