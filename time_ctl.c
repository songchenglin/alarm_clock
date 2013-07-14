#include <time_ctl.h>
#include <intrins.h>

time_val_type real_time = {2013,1,1,0,0,0,0};
time_val_type real_time_tmp = {0};
time_val_type alarm_time = {0};
time_val_type alarm_time_tmp = {0};
time_val_type stop_watch_time = {0};
int time_set_state = 0;
BOOL alarm_ctl = 0;
int month_days[13] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
extern void delay5us(void)
{
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
}
extern void lcd_delaytime(int ms)
{
	int a,b;
	for(a = 0;a < ms;a++)
		for(b = 0;b < 200;b++)
			delay5us();
}
extern void delaytime(int x)
{
	int a,b;
	for(a = 0;a < 500;a++)
		for(b = 0;b < x;b++)
			;
}
void leap_year_process(void)
{
	if(real_time.year%400 || (!real_time.year%400 && real_time.year%4))
		month_days[2] = 29;
	else
		month_days[2] = 28;
}
extern void real_time_process(void)
{
	real_time.sec++;
	if(real_time.sec == 60) {
		real_time.sec = 0;
		real_time.min++;
		if(real_time.min == 60) {
			real_time.min = 0;
			real_time.hour++;
			if(real_time.hour == 24) {
				real_time.hour = 0;
				real_time.day++;
				if(real_time.day > month_days[real_time.month]) {
					real_time.day = 1;
					real_time.month++;
					if(real_time.month > 12) {
						real_time.month = 1;
						real_time.year++;
						leap_year_process();
					}
				}
			}
		}
	}	
}
extern void stop_watch_time_process(void)
{
	stop_watch_time.msec++;
	if(stop_watch_time.msec == 100) {
		stop_watch_time.msec = 0;
		stop_watch_time.sec++;
		if(stop_watch_time.sec == 60) {
			stop_watch_time.sec = 0;
			stop_watch_time.min++;
			if(stop_watch_time.min == 100) {
				stop_watch_time.msec = 0;
				stop_watch_time.sec = 0;
				stop_watch_time.min = 0;
			}
		}
	}
}
