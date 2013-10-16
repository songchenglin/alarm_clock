#ifndef MAIN_H
#define MAIN_H


typedef void (*lcd_disp_cb_type)(void);
enum running_state {
	NORMAL_STATE,
	STOP_WATCH_STATE,
	ALARM_SET_STATE,
	DATE_SET_STATE,
	TIME_SET_STATE,
};

void init_all(void);
void timer_init(void);

/*********Normal_state funs**************************/
void key_normal_state_init(void);
void key_change_to_stop_watch(void);
void key_do_nothing(void);
void key_change_to_disp_alarm_state(void);
void key_change_to_disp_real_time(void);
void key_change_alarm_state(void);
/*************************************************/

/*********Stop_watch_state funs**********************/
void key_stop_watch_state_init(void);
void key_change_to_alarm_set_state(void);
void key_change_to_normal_state(void);
void key_stop_watch_switch(void);
void key_stop_watch_reset(void);
/*************************************************/

/***********Alarm_set_state funs*********************/
void key_alarm_set_init(void);
void key_alarm_set_cancle(void);
void key_alarm_set_confirm(void);
void key_alarm_set_change(void);
void key_alarm_set_add(void);
/*************************************************/

/************Date_set_state funs*********************/
void key_date_set_init(void);
void key_date_set_cancle(void);
void key_date_set_confirm(void);
void key_date_set_change(void);
void key_date_set_add(void);
/*************************************************/
/************Time_set_state funs*********************/
void key_time_set_init(void);
void key_time_set_cancle(void);
void key_time_set_confirm(void);
void key_time_set_change(void);
void key_time_set_add(void);
/*************************************************/

#endif
