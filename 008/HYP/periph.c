#define PBASE 0x3F000000

extern void PUT32(unsigned int, unsigned int);
extern void PUT16(unsigned int, unsigned int);
extern void PUT8(unsigned int, unsigned int);
extern unsigned int GET32(unsigned int);
extern void dummy(unsigned int);

#define ARM_TIMER_LOD (PBASE + 0x0000B400)
#define ARM_TIMER_VAL (PBASE + 0x0000B404)
#define ARM_TIMER_CTL (PBASE + 0x0000B408)
#define ARM_TIMER_CLI (PBASE + 0x0000B40C)
#define ARM_TIMER_RIS (PBASE + 0x0000B410)
#define ARM_TIMER_MIS (PBASE + 0x0000B414)
#define ARM_TIMER_RLD (PBASE + 0x0000B418)
#define ARM_TIMER_DIV (PBASE + 0x0000B41C)
#define ARM_TIMER_CNT (PBASE + 0x0000B420)

#define GPFSEL3 (PBASE + 0x0020000C)
#define GPFSEL4 (PBASE + 0x00200010)
#define GPSET1 (PBASE + 0x00200020)
#define GPCLR1 (PBASE + 0x0020002C)
#define GPFSEL1 (PBASE + 0x00200004)
#define GPSET0 (PBASE + 0x0020001C)
#define GPCLR0 (PBASE + 0x00200028)
#define GPLEV0 (PBASE + 0x00200034)
#define GPPUD (PBASE + 0x00200094)
#define GPPUDCLK0 (PBASE + 0x00200098)

#define AUX_ENABLES (PBASE + 0x00215004)
#define AUX_MU_IO_REG (PBASE + 0x00215040)
#define AUX_MU_IER_REG (PBASE + 0x00215044)
#define AUX_MU_IIR_REG (PBASE + 0x00215048)
#define AUX_MU_LCR_REG (PBASE + 0x0021504C)
#define AUX_MU_MCR_REG (PBASE + 0x00215050)
#define AUX_MU_LSR_REG (PBASE + 0x00215054)
#define AUX_MU_MSR_REG (PBASE + 0x00215058)
#define AUX_MU_SCRATCH (PBASE + 0x0021505C)
#define AUX_MU_CNTL_REG (PBASE + 0x00215060)
#define AUX_MU_STAT_REG (PBASE + 0x00215064)
#define AUX_MU_BAUD_REG (PBASE + 0x00215068)

/*
1发送请求 主机拉低18ms以上,然后可以切换至输入模式,拉高20-40us
2发送响应 dht拉低80us,拉高80us
3发送数据 对于每bit,50us低电平开始,26-28us表示此bit为0,70us为1
 共40bit,高位先出,8+8湿度,8+8温度,8校验和
*/

unsigned int get_dht_bit (void)
{
    unsigned int dhtbit;
    dhtbit = GET32(GPLEV0);
    dhtbit &= 1<<18;
    return dhtbit;
}

void timer_init(void)
{
    PUT32(ARM_TIMER_CTL, 0x003E0000); //init timer
    PUT32(ARM_TIMER_DIV, 0x000000F9); //set div
    PUT32(ARM_TIMER_CLI, 0);          //cleat irq ack
}

unsigned int get_arm_timer_ris (void)
{
	return GET32(ARM_TIMER_RIS);
}

void reset_arm_timer (void)
{
    PUT32(ARM_TIMER_CLI, 0);
    PUT32(ARM_TIMER_CTL, 0x003E0000);
}

void set_arm_timer(unsigned int reload) 
{
    PUT32(ARM_TIMER_LOD, reload);     //set load
    PUT32(ARM_TIMER_RLD, reload);     //set reload
    PUT32(ARM_TIMER_CTL, 0x003E0082); //timer enabled, 16bit counter
}

void delay(unsigned int time)
{
    set_arm_timer(time);
    while(1) if(get_arm_timer_ris()) break;
    reset_arm_timer();
}

unsigned int com8(void)
{
    unsigned int i,bytedata,ra;
    bytedata=0;
    for (i=0;i<8;i++)
    {
        do  {
        ra = get_dht_bit();
        }
        while(ra<1);
        bytedata<<=1;
        delay(40);
        ra = get_dht_bit();
        if (ra>0)
        bytedata|=1;
        while(ra>0)
        {
           ra = get_dht_bit();
        } 
    }
    return bytedata;
}

unsigned int dhtinit(void)
{
    unsigned int ra;
    ra = GET32(GPFSEL1); //set gpio18 out
    ra &= ~(7 << 24);
    ra |= (1 << 24);
    PUT32(GPFSEL1, ra);

    ra = GET32(GPCLR0); //gpio18 out 0 for 20ms
    ra |= 1 << 18;
    PUT32(GPCLR0, ra);
    delay(20000);

    ra = GET32(GPSET0); //gpio18 out 0 for 40us
    ra |= 1 << 18;
    PUT32(GPSET0, ra);
    delay(40);
    
    ra = GET32(GPFSEL1);    //set gpio18 in
    ra &= ~(7 << 24);
    PUT32(GPFSEL1, ra);

    ra = get_dht_bit();

    if (ra > 0)         //gpio18 should be 0 for 80us
        return 3;
    delay(81);

    ra = get_dht_bit();

    if (ra < 1)         //gpio18 should be 1
        return 5;
    
    while(ra>0)
    {
        ra = get_dht_bit();
    }
    unsigned int data,check;
    data=check=0x0;

    data=com8();
    data<<=8;
    data+=com8();
    data<<=8;
    data+=com8();
    data<<=8;
    data+=com8();
    check=com8();
                    //get data
    return data;
}

void uart_init(void)
{
    unsigned int ra;

    PUT32(AUX_ENABLES, 1);     //get access to mini UART register
    PUT32(AUX_MU_IER_REG, 0);  //disable interrupt
    PUT32(AUX_MU_CNTL_REG, 0); //disable functions
    PUT32(AUX_MU_LCR_REG, 3);  //set 8-bit mode
    PUT32(AUX_MU_MCR_REG, 0);  //set RTS, not use
    PUT32(AUX_MU_IER_REG, 0);
    PUT32(AUX_MU_IIR_REG, 0xC6); //FIFO enables(always), clear FIFO
    PUT32(AUX_MU_BAUD_REG, 270); //set BAUD
    ra = GET32(GPFSEL1);
    ra &= ~(7 << 12); //gpio14
    ra |= 2 << 12;    //alt5
    ra &= ~(7 << 15); //gpio15
    ra |= 2 << 15;    //alt5
    PUT32(GPFSEL1, ra);
    PUT32(GPPUD, 0);
    for (ra = 0; ra < 150; ra++)
        dummy(ra);
    PUT32(GPPUDCLK0, (1 << 14) || (1 << 15));
    for (ra = 0; ra < 150; ra++)
        dummy(ra);
    PUT32(GPPUDCLK0, 0);
    PUT32(AUX_MU_CNTL_REG, 3); //enable R and T
}

void uart_send(unsigned int c)
{
    while (1)
    {
        if (GET32(AUX_MU_LSR_REG) & 0x20)
            break; //send when no Receiver Overrun
    }
    PUT32(AUX_MU_IO_REG, c);
}

/* send a byte once*/
void hexstrings(unsigned int d)
{
    unsigned int rb;
    unsigned int rc;

    rb = 32;
    while (1)
    {
        rb -= 4;
        rc = (d >> rb) & 0xF;
        if (rc > 9)
            rc += 0x37;
        else
            rc += 0x30;
        uart_send(rc);
        if (rb == 0)
            break;
    }
    uart_send(0x20);
}

/* send string and add /r/n to end*/
void hexstring(unsigned int d)
{
    hexstrings(d);
    uart_send(0x0D);
    uart_send(0x0A);
}

void blink_off(void)
{
    PUT32(GPCLR1, 1 << (47 - 32));
    PUT32(GPCLR1, 1 << (35 - 32));
}

void blink_on(void)
{
    PUT32(GPSET1, 1 << (47 - 32));
    PUT32(GPSET1, 1 << (35 - 32));
}

void leds_on(void)
{

    unsigned int ra;

    ra = GET32(GPFSEL4);
    ra &= ~(7 << 21);
    ra |= 1 << 21;
    PUT32(GPFSEL4, ra);

    ra = GET32(GPFSEL3);
    ra &= ~(7 << 15);
    ra |= 1 << 15;
    PUT32(GPFSEL3, ra);
    PUT32(GPSET1, 1 << (47 - 32));
    PUT32(GPSET1, 1 << (35 - 32));
}
