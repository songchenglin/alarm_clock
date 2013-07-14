#include <reg52.h>
#include <key_and_beep.h>
#include <time_ctl.h>

int beep_count;
key_fun_type key_funs;

extern void key_init(void)
{
	key_funs.key1_fun = beep_short;
	key_funs.key2_fun = beep_short;
	key_funs.key3_fun = beep_short;
	key_funs.key4_fun = beep_short;
}
extern void get_key_val(void)
{
	if(!KEY1) {
		delaytime(5);
		if(!KEY1) {
			key_funs.key1_fun();
			while(!KEY1)
				;
			return ;
		}
	}
	if(!KEY2) {
		delaytime(5);
		if(!KEY2) {
			key_funs.key2_fun();
			while(!KEY2)
				;
			return ;
		}
	}
	if(!KEY3) {
		delaytime(5);
		if(!KEY3) {
			key_funs.key3_fun();
			while(!KEY3)
				;
			return ;
		}
	}
	if(!KEY4) {
		delaytime(5);
		if(!KEY4) {
			key_funs.key4_fun();
			while(!KEY4)
				;
			return ;
		}
	}
}
extern void beep_init(void)
{
	EA = 1;
	ET2 = 1;
	T2CON &= 0x00;	//T2CON = 0x00
	TH2 = 0xfe;
	TL2 = 0x0c;
	RCAP2H = 0xfe;
	RCAP2L = 0x0c;
}
extern void beep_short(void)
{
	beep_count = 800;
	TH2 = 0xfe;
	TL2 = 0x0c;
	ET2 = 1;
	TR2 = 1;
}