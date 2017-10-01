
#include "periph.h"
#include "timer.h"
#include "interrupts.h"

/*It's about 1s*/
#define TIMER_BIT 1000000

/* If irq, asm code in vector.s jump there, toggle led */
void c_irq_handler ( void )
{
	toggle_led();
	clear_arm_timer_cli();
}

/* Main function - we'll never return from here */
int notmain(void)
{	
    unsigned int ra;
	/* Disable timer irq
	   Init led 47 to output
	   init and set timer count with arg */
	irq_basic_disable();
	init_led();	
	init_arm_timer_count(TIMER_BIT);
	
	//test mask irq ,blink/1s
	for(ra=0;ra<1;ra++)
	{
		while(1) if(get_arm_timer_mis()) break;
		toggle_led();
		clear_arm_timer_cli(); 
	}

	//test irq_base timer bit ,blink/2s
	irq_basic_enable();
	set_arm_timer_count(TIMER_BIT*2);
	for(ra=0;ra<2;ra++)
	{
		while(1) if(get_irq_timer()) break;		
		toggle_led();
		clear_arm_timer_cli();
	}

	//test irq ,blink/4s
	init_arm_timer_count(TIMER_BIT*4);
	enable_irq();
	while(1) continue;
}
