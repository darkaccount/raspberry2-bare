#define PBASE 0x3F000000

extern void PUT32 ( unsigned int, unsigned int );
extern void PUT16 ( unsigned int, unsigned int );
extern void PUT8 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern void dummy ( unsigned int );

#define GPFSEL1         (PBASE+0x00200004)
#define GPSET0          (PBASE+0x0020001C)
#define GPCLR0          (PBASE+0x00200028)
#define GPPUD           (PBASE+0x00200094)
#define GPPUDCLK0       (PBASE+0x00200098)

#define AUX_ENABLES     (PBASE+0x00215004)
#define AUX_MU_IO_REG   (PBASE+0x00215040)
#define AUX_MU_IER_REG  (PBASE+0x00215044)
#define AUX_MU_IIR_REG  (PBASE+0x00215048)
#define AUX_MU_LCR_REG  (PBASE+0x0021504C)
#define AUX_MU_MCR_REG  (PBASE+0x00215050)
#define AUX_MU_LSR_REG  (PBASE+0x00215054)
#define AUX_MU_MSR_REG  (PBASE+0x00215058)
#define AUX_MU_SCRATCH  (PBASE+0x0021505C)
#define AUX_MU_CNTL_REG (PBASE+0x00215060)
#define AUX_MU_STAT_REG (PBASE+0x00215064)
#define AUX_MU_BAUD_REG (PBASE+0x00215068)

void uart_init (void)
{
    unsigned ra;

    PUT32(AUX_ENABLES,1);       //get access to mini UART register
    PUT32(AUX_MU_IER_REG,0);    //disable interrupt
    PUT32(AUX_MU_CNTL_REG,0);   //disable functions
    PUT32(AUX_MU_LCR_REG,3);    //set 8-bit mode
    PUT32(AUX_MU_MCR_REG,0);    //set RTS, not use
    PUT32(AUX_MU_IER_REG,0);    
    PUT32(AUX_MU_IIR_REG,0xC6); //FIFO enables(always), clear FIFO
    PUT32(AUX_MU_BAUD_REG,270); //set BAUD
    ra=GET32(GPFSEL1);
    ra&=~(7<<12); //gpio14
    ra|=2<<12;    //alt5
    ra&=~(7<<15); //gpio15
    ra|=2<<15;    //alt5
    PUT32(GPFSEL1,ra);
    PUT32(GPPUD,0);
    for (ra=0;ra<150;ra++) dummy(ra);
    PUT32(GPPUDCLK0,(1<<14)||(1<<15));
    for (ra=0;ra<150;ra++) dummy(ra);
    PUT32(GPPUDCLK0,0);
    PUT32(AUX_MU_CNTL_REG,3);   //enable R and T
}

void uart_send ( unsigned int c )
{
    while(1)
    {
        if(GET32(AUX_MU_LSR_REG)&0x20) break;   //send when no Receiver Overrun 
    }
    PUT32(AUX_MU_IO_REG,c);
}

/* send a byte once*/
void hexstrings ( unsigned int d )
{
    unsigned int rb;
    unsigned int rc;

    rb=32;
    while(1)
    {
        rb-=4;
        rc=(d>>rb)&0xF;
        if(rc>9) rc+=0x37; else rc+=0x30;
        uart_send(rc);
        if(rb==0) break;
    }
    uart_send(0x20);
}

/* send string and add /r/n to end*/
void hexstring ( unsigned int d )
{
    hexstrings(d);
    uart_send(0x0D);
    uart_send(0x0A);
}

void leds_off ( void )
{
#define GPFSEL3 0x3F20000C
#define GPFSEL4 0x3F200010
#define GPSET1  0x3F200020
#define GPCLR1  0x3F20002C
    unsigned int ra;

    ra=GET32(GPFSEL4);
    ra&=~(7<<21);
    ra|=1<<21;
    PUT32(GPFSEL4,ra);

    ra=GET32(GPFSEL3);
    ra&=~(7<<15);
    ra|=1<<15;
    PUT32(GPFSEL3,ra);

    PUT32(GPCLR1,1<<(47-32));
    PUT32(GPCLR1,1<<(35-32));
}
