#ifndef LCD_1602_H
#define LCD_1602_H

#include <reg52.h>
#include <time_ctl.h>


sfr LCD_bus = 0x80;
sbit LCD_RS = P3^3;             
sbit LCD_RW = P3^4;
sbit LCD_EN = P3^5;

extern void lcd_init(void);
BOOL lcd_is_busy(void);
void lcd_write_cmd(uchar cmd);
void lcd_write_data(uchar dat);
extern void lcd_clr_screen(void);
void lcd_pos_set(uchar pos);
BOOL lcd_set_disp_position(int line,uchar row);
extern void lcd_disp_string(char *disp_string);
/******************************************************************************
*****************************LCD display Real time*********************************
*******************************************************************************/
void convert_time_to_string(time_val_type *time,uchar *disp_dat1,uchar *disp_dat2);
extern void lcd_disp_real_time(void);
void convert_alarm_state_to_string(time_val_type *time,uchar *disp_dat1,uchar *disp_dat2);
extern void lcd_disp_alarm_state(void);
/******************************************************************************
*****************************LCD display Stop Watch*******************************
*******************************************************************************/
extern void lcd_disp_stop_watch_init(void);
void convert_stop_watch_time_to_string(time_val_type *time,uchar *disp_dat);
extern void lcd_disp_stop_watch_time(void);
/******************************************************************************
*****************************LCD display Alarm Set*********************************
*******************************************************************************/
extern void lcd_disp_alarm_set_init(void);
void convert_alarm_set_to_string(time_val_type *time,uchar *disp_dat);
extern void lcd_disp_alarm_set(void);
/******************************************************************************
*****************************LCD display Date Set**********************************
*******************************************************************************/
extern void lcd_disp_date_set_init(void);
void convert_date_set_to_string(time_val_type *time,uchar *disp_dat);
extern void lcd_disp_date_set(void);
/******************************************************************************
*****************************LCD display Time Set**********************************
*******************************************************************************/
extern void lcd_disp_time_set_init(void);
void convert_time_set_to_string(time_val_type *time,uchar *disp_dat);
extern void lcd_disp_time_set(void);
#endif
