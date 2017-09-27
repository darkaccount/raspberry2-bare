
#include "periph.h"
#include "timer.h"

/*It's about 4s*/
#define TIMER_BIT 4000000

/** Main function - we'll never return from here */
int main(void)
{	
	unsigned int count_now;
	unsigned int count_limit;

//init_led from periph
	init_led();
	init_arm_timer_free();
	count_limit = get_arm_timer_frt();
    /* Never exit as there is no OS to exit to! */
	while(1)
	{
		/*loop till count >= limit+timebit */
    	while(1)	
		{
			count_now = get_arm_timer_frt();
			if ((count_now-count_limit) >= TIMER_BIT)
				break;
		}
    	
    	/* set new limit */
		count_limit+=TIMER_BIT;
        toggle_led();
	}
}
