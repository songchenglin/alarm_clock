#include <reg52.h>
#include <main.h>
#include <time_ctl.h>
#include <lcd_1602.h>
#include <key_and_beep.h>

lcd_disp_cb_type lcd_display = lcd_disp_real_time;
int run_state = NORMAL_STATE;

int main(void)
{
	init_all();
	while(1) {
		switch(run_state) {
		case NORMAL_STATE:
			key_normal_state_init();
			lcd_disp_real_time();
			while(run_state == NORMAL_STATE)
				get_key_val();
			break;
		case STOP_WATCH_STATE:
			key_stop_watch_state_init();
			lcd_disp_stop_watch_init();
			while(run_state == STOP_WATCH_STATE)
				get_key_val();
			break;
		case ALARM_SET_STATE:
			key_alarm_set_init();
			lcd_disp_alarm_set_init();
			while(run_state == ALARM_SET_STATE)
				get_key_val();
			break;
		case DATE_SET_STATE:
			key_date_set_init();
			lcd_disp_date_set_init();
			while(run_state == DATE_SET_STATE)
				get_key_val();
			break;
		case TIME_SET_STATE:
			key_time_set_init();
			lcd_disp_time_set_init();
			while(run_state == TIME_SET_STATE)
				get_key_val();
			break;
		default:
			run_state = NORMAL_STATE;
			break;
		}
	}
}

void init_all(void)
{
	lcd_init();
	beep_init();
	timer_init();
	key_init();
}
void timer_init(void)
{
	EA = 1;	
	ET0 = 1;
	TMOD = 0x11;	//mode 1:16bit
	TH0	= 0x3c;		//circle = 50ms
	TL0 = 0xb0;
	ET1 = 1;
	TH1 = 0xd8;		//circle = 10ms
	TL1 = 0xf0;
	TR0 = 1;
}
/**************************************************
*
*Normal_state key funs
*
**************************************************/
void key_normal_state_init(void)
{
	lcd_clr_screen();
	key_funs.key1_fun = key_change_to_stop_watch;
	key_funs.key2_fun = key_do_nothing;
	key_funs.key3_fun = key_change_to_disp_alarm_state;
	key_funs.key4_fun = key_change_alarm_state;
	lcd_display = lcd_disp_real_time;
}
void key_change_to_stop_watch(void)
{
	run_state = STOP_WATCH_STATE;
}
void key_do_nothing(void)
{
	return ;
}
void key_change_to_disp_alarm_state(void)
{
	lcd_clr_screen();
	lcd_display = lcd_disp_alarm_state;
	key_funs.key3_fun = key_change_to_disp_real_time;
	lcd_disp_alarm_state();
}
void key_change_to_disp_real_time(void)
{
	lcd_clr_screen();
	lcd_display = lcd_disp_real_time;
	key_funs.key3_fun = key_change_to_disp_alarm_state;
	lcd_disp_real_time();
}
void key_change_alarm_state(void)
{
	if(lcd_display == lcd_disp_alarm_state) {
		alarm_ctl = ~alarm_ctl;
		lcd_disp_alarm_state();
	}
}
/**************************************************/
/**************************************************
*
*Stop_watch_state key funs
*
***************************************************/
void key_stop_watch_state_init(void)
{
	key_funs.key1_fun = key_change_to_alarm_set_state;
	key_funs.key2_fun = key_change_to_normal_state;
	key_funs.key3_fun = key_stop_watch_reset;
	key_funs.key4_fun = key_stop_watch_switch;
	lcd_display = lcd_disp_stop_watch_time;
}
void key_change_to_alarm_set_state(void)
{
	TR1 = 0;
	TH1 = 0xd8;
	TL1 = 0xf0;
	run_state = ALARM_SET_STATE;
}
void key_change_to_normal_state(void)
{
	TR1 = 0;
	TH1 = 0xd8;
	TL1 = 0xf0;
	run_state = NORMAL_STATE;
}

void key_stop_watch_switch(void)
{
	TH1 = 0xd8;
	TL1 = 0xf0;
	TR1 = ~TR1;
	lcd_disp_stop_watch_time();
}
void key_stop_watch_reset(void)
{
	TR1 = 0;
	TH1 = 0xd8;
	TL1 = 0xf0;
	stop_watch_time.msec = 0;
	stop_watch_time.sec = 0;
	stop_watch_time.min = 0;
	lcd_disp_stop_watch_time();
}
/*************************************************/

/**************************************************
*
*Alarm_set_state key funs
*
***************************************************/
void key_alarm_set_init(void)
{
	key_funs.key1_fun = key_alarm_set_cancle;
	key_funs.key2_fun = key_alarm_set_confirm;
	key_funs.key3_fun = key_alarm_set_change;
	key_funs.key4_fun = key_alarm_set_add;
	time_set_state = TIME_SET_MIN;
	alarm_time_tmp = alarm_time;
	lcd_display = key_do_nothing;
}
void key_alarm_set_cancle(void)
{
	run_state = DATE_SET_STATE;
}
void key_alarm_set_confirm(void)
{
	alarm_time = alarm_time_tmp;
	run_state = NORMAL_STATE;
}
void key_alarm_set_change(void)
{
	if(time_set_state == TIME_SET_MIN)
		time_set_state = TIME_SET_HOUR;
	else
		time_set_state = TIME_SET_MIN;
	lcd_disp_alarm_set();
}
void key_alarm_set_add(void)
{
	switch(time_set_state) {
	case TIME_SET_MIN:
		alarm_time_tmp.min++;
		if(alarm_time_tmp.min == 60)
			alarm_time_tmp.min = 0;
		break;
	case TIME_SET_HOUR:
		alarm_time_tmp.hour++;
		if(alarm_time_tmp.hour == 24)
			alarm_time_tmp.hour = 0;
		break;
	default:
		time_set_state = TIME_SET_MIN;
		break;
	}
	lcd_disp_alarm_set();
}
/*************************************************/


/**************************************************
*
*Date_set_state key funs
*
***************************************************/
void key_date_set_init(void)
{
	key_funs.key1_fun = key_date_set_cancle;
	key_funs.key2_fun = key_date_set_confirm;
	key_funs.key3_fun = key_date_set_change;
	key_funs.key4_fun = key_date_set_add;
	time_set_state = TIME_SET_DAY;
	real_time_tmp = real_time;
	lcd_display = key_do_nothing;
}
void key_date_set_cancle(void)
{
	run_state = TIME_SET_STATE;
}
void key_date_set_confirm(void)
{
	real_time.year= real_time_tmp.year;
	real_time.month = real_time_tmp.month;
	real_time.day = real_time_tmp.day;
	run_state = NORMAL_STATE;
}
void key_date_set_change(void)
{
	if(time_set_state == TIME_SET_DAY)
		time_set_state = TIME_SET_MONTH;
	else if(time_set_state == TIME_SET_MONTH)
		time_set_state = TIME_SET_YEAR;
	else
		time_set_state = TIME_SET_DAY;
	lcd_disp_date_set();
}
void key_date_set_add(void)
{
	switch(time_set_state) {
	case TIME_SET_DAY:
		real_time_tmp.day++;
		if(real_time_tmp.day > month_days[real_time_tmp.month])
			real_time_tmp.day = 1;
		break;
	case TIME_SET_MONTH:
		real_time_tmp.month++;
		if(real_time_tmp.month > 12)
			real_time_tmp.month = 1;
		if(real_time_tmp.day > month_days[real_time_tmp.month])
			real_time_tmp.day = month_days[real_time_tmp.month];
		break;
	case TIME_SET_YEAR:
		real_time_tmp.year++;
		if(real_time_tmp.year > MAX_YEAR)
			real_time_tmp.year = 2013;
		break;
	default:
		time_set_state = TIME_SET_DAY;
		break;
	}
	lcd_disp_date_set();
}
/*************************************************/

/**************************************************
*
*Time_set_state key funs
*
**************************************************/
void key_time_set_init(void)
{
	key_funs.key1_fun = key_time_set_cancle;
	key_funs.key2_fun = key_time_set_confirm;
	key_funs.key3_fun = key_time_set_change;
	key_funs.key4_fun = key_time_set_add;
	time_set_state = TIME_SET_SEC;
	real_time_tmp = real_time;
	lcd_display = lcd_disp_time_set;
}
void key_time_set_cancle(void)
{
	run_state = NORMAL_STATE;
}
void key_time_set_confirm(void)
{
	real_time.hour= real_time_tmp.hour;
	real_time.min= real_time_tmp.min;
	real_time.sec= real_time_tmp.sec;
	run_state = NORMAL_STATE;
}
void key_time_set_change(void)
{
	if(time_set_state == TIME_SET_SEC)
		time_set_state = TIME_SET_MIN;
	else if(time_set_state == TIME_SET_MIN)
		time_set_state = TIME_SET_HOUR;
	else
		time_set_state = TIME_SET_SEC;
	lcd_disp_time_set();
}
void key_time_set_add(void)
{
	switch(time_set_state) {
	case TIME_SET_SEC:
		real_time_tmp.sec = 0;
		break;
	case TIME_SET_MIN:
		real_time_tmp.min++;
		if(real_time_tmp.min == 60)
			real_time_tmp.min = 0;
		break;
	case TIME_SET_HOUR:
		real_time_tmp.hour++;
		if(real_time_tmp.hour == 24)
			real_time_tmp.hour = 0;
		break;
	default:
		time_set_state = TIME_SET_SEC;
		break;
	}
	lcd_disp_time_set();
}
/*************************************************/

/*************************************************
******************Interrupt Enty*********************
**************************************************/
void timer0_handler(void) interrupt 1	//for real time
{
	static uchar timer0_count = 0;
	TH0	= 0x3c;		//circle = 50ms
	TL0 = 0xb0;
	timer0_count++;
	if(timer0_count == 20) {
		timer0_count = 0;
		real_time_process();
		if(run_state == TIME_SET_STATE) {
			real_time_tmp.sec++;
			if(real_time_tmp.sec == 60)
				real_time_tmp.sec = 0;
		}
		if(real_time.hour == alarm_time.hour
			&&real_time.min == alarm_time.min
			&& alarm_ctl)
			beep_short();
		lcd_display();
	}
}
void timer1_handler(void) interrupt 3	//for stopwatch
{
	TH1 = 0xd8;		//circle = 10ms
	TL1 = 0xf0;
	stop_watch_time_process();
	lcd_display();
}

extern int beep_count;
void timer2_handler(void) interrupt 5	//for beep
{
	//auto reload 16 bit timer
	static uint timer2_count = 0;
	TF2 = 0;		//reset flag bit by hand
	timer2_count++;
	BEEP = ~BEEP;
	if(timer2_count > beep_count) {
		timer2_count = 0;
		TR2 = 0;
		ET2 = 0;
		BEEP = 1;
	}
}
