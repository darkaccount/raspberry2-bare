
extern unsigned int GET32 ( unsigned int );

#define SYSTIMERCS 0x3F003000
#define SYSTIMERCL 0x3F003004

/*return 32bits value in systimer counter lower*/
unsigned int get_sys_timer_cl (void)
{
	return GET32(SYSTIMERCL);
}

