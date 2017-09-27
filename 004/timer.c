
extern unsigned int GET32 ( unsigned int );
extern void PUT32 ( unsigned int, unsigned int );

#define ARM_TIMER_LOD 0x3F00B400
#define ARM_TIMER_VAL 0x3F00B404
#define ARM_TIMER_CTL 0x3F00B408
#define ARM_TIMER_CLI 0x3F00B40C
#define ARM_TIMER_RIS 0x3F00B410
#define ARM_TIMER_MIS 0x3F00B414
#define ARM_TIMER_RLD 0x3F00B418
#define ARM_TIMER_DIV 0x3F00B41C
#define ARM_TIMER_CNT 0x3F00B420

/* init and set timer */
void set_arm_timer (unsigned int reload)
{
	PUT32(ARM_TIMER_CTL, 0x003E0000);	//init timer
	PUT32(ARM_TIMER_LOD, reload);		//set load
	PUT32(ARM_TIMER_RLD, reload);		//set reload
	PUT32(ARM_TIMER_DIV, 0x000000F9);	//set div 
    PUT32(ARM_TIMER_CLI,0);				//cleat irq ack
    PUT32(ARM_TIMER_CTL, 0x003E0082);	//timer enabled, 16bit counter
}

/* Timer IRQ clear register,ARM_TIMER_RIS is read only,
   Timer Raw IRQ register,ARM_TIMER_CLI is write only */
/* return arm itmer raw irq*/
unsigned int get_arm_timer_ris (void)
{
	return GET32(ARM_TIMER_RIS);
}

/* write to timer irq clear register to clear raw irq bit */
void clear_arm_timer_ri (void)
{
	PUT32(ARM_TIMER_CLI, 0);
}
