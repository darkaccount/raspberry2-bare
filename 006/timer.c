
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

//init and set timer
void init_arm_timer_count (unsigned int load)
{
	PUT32(ARM_TIMER_CTL,0x003E0000);
	PUT32(ARM_TIMER_LOD,load);
	PUT32(ARM_TIMER_RLD,load);
	PUT32(ARM_TIMER_DIV,0x000000F9);
	PUT32(ARM_TIMER_CLI,0);
	PUT32(ARM_TIMER_CTL,0x003E00A2);
}

//set timer load and reload
void set_arm_timer_count (unsigned int load)
{
	PUT32(ARM_TIMER_LOD,load);
	PUT32(ARM_TIMER_RLD,load);
	PUT32(ARM_TIMER_CLI,0);
}

//return timer masked irq
unsigned int get_arm_timer_mis (void)
{
	return GET32(ARM_TIMER_MIS);
}

//clear timer interrput pending
void clear_arm_timer_cli (void)
{
	PUT32(ARM_TIMER_CLI, 0);
}
