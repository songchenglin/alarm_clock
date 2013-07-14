#ifndef TIME_CTL_H
#define TIME_CTL_H

#define MAX_YEAR 2030
typedef bit BOOL;
typedef unsigned char uchar;
typedef unsigned int uint;

enum time_set_state {
	TIME_SET_YEAR,
	TIME_SET_MONTH,
	TIME_SET_DAY,
	TIME_SET_HOUR,
	TIME_SET_MIN,
	TIME_SET_SEC
};
extern int time_set_state;
typedef struct time_val {
	int year;
	uchar month;
	uchar day;
	uchar hour;
	uchar min;
	uchar sec;
	int msec;
}time_val_type;
extern time_val_type real_time;
extern time_val_type real_time_tmp;
extern time_val_type alarm_time;
extern time_val_type alarm_time_tmp;
extern time_val_type stop_watch_time;
extern int time_set_state;
extern BOOL alarm_ctl;

extern int month_days[13];

extern void delay5us(void);
extern void lcd_delaytime(int ms);
extern void delaytime(int x);

void leap_year_process(void);
extern void real_time_process(void);
extern void stop_watch_time_process(void);

#endif
