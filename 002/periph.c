
//----------define gpio------------

/*import extern asm function in vestors.s
  PUT32 : put arg2 to arg1 in mem
  GET32 : get value of arg in mem
  dummy : just wasting time and do nothing*/
 
extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern void dummy ( unsigned int );

//ACK LED link to GPIO47, DRI in GPFSEL4, SET in SET1, CLR in CLR1
#define GPFSEL4 0x3F200010
#define GPSET1  0x3F200020
#define GPCLR1  0x3F20002C

unsigned int ledstate;

void toggle_led ( void )
{
	ledstate++;
	if(ledstate&1)
	{
		PUT32(GPSET1, 1<<(47-32));
	}
	else
	{
		PUT32(GPCLR1, 1<<(47-32));
	}
}

//init led, set DRI
void init_led ( void )
{
	//temp val ra
	unsigned int ra;
	
	/*get present GPFSEL4
	  make 47 output, 001 means output
	  set GPFSEL4
	*/
	ra=GET32(GPFSEL4);
	ra&=~(7<<21);
	ra|=1<<21;
	PUT32(GPFSEL4, ra);
	
	ledstate=0;
	toggle_led();
}

