extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );

#define IRQ_BASIC 0x3F00B200
#define IRQ_PEND1 0x3F00B204
#define IRQ_PEND2 0x3F00B208
#define ENABLE_IRQ1 0x3F00B210
#define IRQ_ENABLE_BASIC 0x3F00B218
#define IRQ_DISABLE_BASIC 0x3F00B224

// disable irq basic timer bit
void irq_basic_disable(void)
{
    PUT32(IRQ_DISABLE_BASIC,1);
}

// enable irq basic timer bit
void irq_basic_enable(void)
{
    PUT32(IRQ_ENABLE_BASIC,1);
}

// return irq basic pending status
unsigned int get_irq_timer (void)
{
	return GET32(IRQ_BASIC)&1;
}
