
#include "periph.h"
#include "systimer.h"

/*It's about 4s*/
#define TIMER_BIT 0x00400000

/** Main function - we'll never return from here */
int main(void)
{
	unsigned int ra;	
	
//init_led from periph
	init_led();
    /* Never exit as there is no OS to exit to! */
	while(1)
	{
		/*loop to judge break*/
    	while(1)
    	{
        	ra=get_sys_timer_cl();
			if ((ra&=TIMER_BIT) == TIMER_BIT) break;
    	}
		
        toggle_led();
	}
}
