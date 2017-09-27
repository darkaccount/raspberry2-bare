
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
void init_arm_timer_free (void)
{
	PUT32(ARM_TIMER_CTL, 0x00F90000);	//init timer,div 265
	PUT32(ARM_TIMER_CTL, 0x00F90200);	//free running enabled
}

/* return arm itmer free running counter */
unsigned int get_arm_timer_frt (void)
{
	return GET32(ARM_TIMER_CNT);
}
