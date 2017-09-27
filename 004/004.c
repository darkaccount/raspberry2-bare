
#include "periph.h"
#include "timer.h"

/*It's about 4s*/
#define TIMER_BIT 4000000

/** Main function - we'll never return from here */
int main(void)
{	
	
//init_led from periph
	init_led();
	set_arm_timer(TIMER_BIT);
    /* Never exit as there is no OS to exit to! */
	while(1)
	{
		/*loop till timer raw irq ack*/
    	while(1)	if (get_arm_timer_ris()) break;
    	
    	/*clear timer irq ack,by write to timer irq clear */
		clear_arm_timer_ri();
        toggle_led();
	}
}
