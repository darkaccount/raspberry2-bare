
#include "periph.h"

/** Main function - we'll never return from here */
int main(void)
{
	unsigned int ra;	
	
//init_led from periph
	init_led();
    /* Never exit as there is no OS to exit to! */
    while(1)
    {
        for (ra=0; ra<500000; ra++) ;
        toggle_led();
    }
}
