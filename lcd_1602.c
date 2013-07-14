#include <lcd_1602.h>
#include <time_ctl.h>

/***********************
*LCD init
***********************/
extern void lcd_init(void)
{    
	lcd_write_cmd(0x01);	//clear scr           
	lcd_write_cmd(0x38);	//function set:16*2disp,5*8point,8bit data
	lcd_delaytime(5);
	lcd_write_cmd(0x38);         
	lcd_delaytime(5);
	lcd_write_cmd(0x38);
	lcd_delaytime(5);

	lcd_write_cmd(0x0c) ;	//disp control:disp ON,cursor OFF
	lcd_delaytime(5);
	lcd_write_cmd(0x06);	//entry mode set:Addr++,move OFF
	lcd_delaytime(5);
	lcd_write_cmd(0x01);	//clear scr
	lcd_delaytime(5);
}

/*****************
*Test LCD busy status
*
*	1:busy 
*	0:ok
******************/ 
BOOL lcd_is_busy(void)
{                          
	BOOL result;
	LCD_RS = 0;
	LCD_RW = 1;
	LCD_EN = 1;
	delay5us();
	result = (LCD_bus & 0x80) ? 1 : 0;
	LCD_EN = 0 ;
	return(result) ; 
}

/***********************
*Write CMD to LCD
*
*RS=0 RW=0 E=1_0
*D0-D7=CMD
************************/
void lcd_write_cmd(uchar cmd)
{                          
	while(lcd_is_busy())
		;
	LCD_RS = 0;
	LCD_RW = 0;
	LCD_EN = 0;
	delay5us();
	LCD_bus = cmd ;
	delay5us();
	LCD_EN = 1 ;
	delay5us();
	LCD_EN = 0;  
}

/**********************
*Write DATA to LCD
*
*RS=1 RW=0 E=1_0
*D0-D7=DATA
***********************/
void lcd_write_data(uchar dat)
{                          
	while(lcd_is_busy())
		;
	LCD_RS = 1;
	LCD_RW = 0;
	LCD_EN = 0;
	LCD_bus = dat ;
	delay5us();
	LCD_EN = 1;
	delay5us();
	LCD_EN = 0 ; 
}

extern void lcd_clr_screen(void)
{
	lcd_write_cmd(0x01);
}
/***********************
*Set display position
************************/
void lcd_pos_set(uchar pos)
{                          
	lcd_write_cmd(pos | 0x80);	//addr = pos+0x80 line = 2:addr+=0x40
}
BOOL lcd_set_disp_position(int line,uchar row)
{
	if(line == 1)
		lcd_pos_set(row);
	else if(line == 2)
		lcd_pos_set(row+0x40);
	else
		return 0;
	return 1;
}

/**************************
*Disp string
*MUST set disp position before use
**************************/
void lcd_disp_string(uchar *disp_string)
{
	uchar *disp_data = disp_string;
	while(*disp_data != '\0') {
		lcd_write_data(*disp_data);
		disp_data++;
	}
}

/******************************************************************************
*****************************LCD display Real time*********************************
*******************************************************************************/
void convert_time_to_string(time_val_type *time,uchar *disp_dat1,uchar *disp_dat2)
/*
** xxxx-xx-xx 10bit
** xx:xx:xx 8bit
*/
{
	*(disp_dat1+0) = time->year/1000+'0';
	*(disp_dat1+1) = time->year/100%10+'0';
	*(disp_dat1+2) = time->year/10%10+'0';
	*(disp_dat1+3) = time->year%10+'0';
	*(disp_dat1+4) = '-';
	*(disp_dat1+5) = time->month/10+'0';
	*(disp_dat1+6) = time->month%10+'0';
	*(disp_dat1+7) = '-';
	*(disp_dat1+8) = time->day/10+'0';
	*(disp_dat1+9) = time->day%10+'0';

	*(disp_dat2+0) = time->hour/10+'0';
	*(disp_dat2+1) = time->hour%10+'0';
	*(disp_dat2+2) = ':';
	*(disp_dat2+3) = time->min/10+'0';
	*(disp_dat2+4) = time->min%10+'0';
	*(disp_dat2+5) = ':';
	*(disp_dat2+6) = time->sec/10+'0';
	*(disp_dat2+7) = time->sec%10+'0';
}
extern void lcd_disp_real_time(void)
{
	uchar disp_dat1[11] = 0;
	uchar disp_dat2[9] = 0;
	convert_time_to_string(&real_time,disp_dat1,disp_dat2);
	lcd_set_disp_position(1,0);
	lcd_disp_string(disp_dat1);
	lcd_set_disp_position(2,0);
	lcd_disp_string(disp_dat2);
}


/***************************
*convert alarm to string for LCD disp
****************************/
void convert_alarm_state_to_string(time_val_type *time,uchar *disp_dat1,uchar *disp_dat2)
/*
** alarm:xx:xx  11bit
** state:OFF 9bit
*/
{
	*(disp_dat1+0) = 'A';
	*(disp_dat1+1) = 'l';
	*(disp_dat1+2) = 'a';
	*(disp_dat1+3) = 'r';
	*(disp_dat1+4) = 'm';
	*(disp_dat1+5) = ':';
	*(disp_dat1+6) = time->hour/10+'0';
	*(disp_dat1+7) = time->hour%10+'0';
	*(disp_dat1+8) = ':';
	*(disp_dat1+9) = time->min/10+'0';
	*(disp_dat1+10) = time->min%10+'0';

	*(disp_dat2+0) = 's';
	*(disp_dat2+1) = 't';
	*(disp_dat2+2) = 'a';
	*(disp_dat2+3) = 't';
	*(disp_dat2+4) = 'e';
	*(disp_dat2+5) = ':';
	*(disp_dat2+6) = 'O';
	if(alarm_ctl) {
		*(disp_dat2+7) = 'N';
		*(disp_dat2+8) = ' ';
	}else {
		*(disp_dat2+7) = 'F';
		*(disp_dat2+8) = 'F';
	}
}
extern void lcd_disp_alarm_state(void)
{
	uchar disp_dat1[12] = 0;
	uchar disp_dat2[10] = 0;
	convert_alarm_state_to_string(&alarm_time,disp_dat1,disp_dat2);
	lcd_set_disp_position(1,0);
	lcd_disp_string(disp_dat1);
	lcd_set_disp_position(2,0);
	lcd_disp_string(disp_dat2);
}
/******************************************************************************
*****************************LCD display Stop Watch*******************************
*******************************************************************************/
extern void lcd_disp_stop_watch_init(void)
{
	lcd_clr_screen();
	lcd_set_disp_position(1,0);
	lcd_disp_string("Stop Watch");
	lcd_disp_stop_watch_time();
}
void convert_stop_watch_time_to_string(time_val_type *time,uchar *disp_dat)
/*
**xx:xx:xx 8bit
*/
{
	*(disp_dat+0) = time->min/10+'0';
	*(disp_dat+1) = time->min%10+'0';
	*(disp_dat+2) = ':';
	*(disp_dat+3) = time->sec/10+'0';
	*(disp_dat+4) = time->sec%10+'0';
	*(disp_dat+5) = ':';
	*(disp_dat+6) = time->msec/10+'0';
	*(disp_dat+7) = time->msec%10+'0';
}
extern void lcd_disp_stop_watch_time(void)
{
	uchar disp_dat[9] = 0;
	convert_stop_watch_time_to_string(&stop_watch_time,disp_dat);
	lcd_set_disp_position(2,0);
	lcd_disp_string(disp_dat);
}
/******************************************************************************
*****************************LCD display Alarm Set*********************************
*******************************************************************************/
extern void lcd_disp_alarm_set_init(void)
{
	lcd_clr_screen();
	lcd_set_disp_position(1,0);
	lcd_disp_string("Alarm Set:");
	lcd_disp_alarm_set();
}
void convert_alarm_set_to_string(time_val_type *time,uchar *disp_dat)
/*
**xx:xx 5bit
*/
{
	int i = 0;
	for(i = 0;i < 5;i++)
		*(disp_dat+i) = '*';
	*(disp_dat+2) = ':';	
	if(time_set_state == TIME_SET_MIN) {
		*(disp_dat+3) = time->min/10+'0';
		*(disp_dat+4) = time->min%10+'0';
	}else if(time_set_state == TIME_SET_HOUR) {
		*(disp_dat+0) = time->hour/10+'0';
		*(disp_dat+1) = time->hour%10+'0';
	}
}
extern void lcd_disp_alarm_set(void)
{
	uchar disp_dat[6] = 0;
	convert_alarm_set_to_string(&alarm_time_tmp,disp_dat);
	lcd_set_disp_position(2,0);
	lcd_disp_string(disp_dat);
}
/******************************************************************************
*****************************LCD display Date Set**********************************
*******************************************************************************/
extern void lcd_disp_date_set_init(void)
{
	lcd_clr_screen();
	lcd_set_disp_position(1,0);
	lcd_disp_string("Date Set:");
	lcd_disp_date_set();
}
void convert_date_set_to_string(time_val_type *time,uchar *disp_dat)
/*
**xxxx-xx-xx 10bit
*/
{
	int i = 0;
	for(i = 0;i < 10;i++)
		*(disp_dat+i) = '*';
	*(disp_dat+4) = '-';
	*(disp_dat+7) = '-';
	if(time_set_state == TIME_SET_YEAR) {
		*(disp_dat+0) = time->year/1000+'0';
		*(disp_dat+1) = time->year/100%10+'0';
		*(disp_dat+2) = time->year/10%10+'0';
		*(disp_dat+3) = time->year%10+'0';
	}else if(time_set_state == TIME_SET_MONTH) {
		*(disp_dat+5) = time->month/10+'0';
		*(disp_dat+6) = time->month%10+'0';
	}else if(time_set_state == TIME_SET_DAY) {
		*(disp_dat+8) = time->day/10+'0';
		*(disp_dat+9) = time->day%10+'0';
	}
}
extern void lcd_disp_date_set(void)
{
	uchar disp_dat[10] = 0;
	convert_date_set_to_string(&real_time_tmp,disp_dat);
	lcd_set_disp_position(2,0);
	lcd_disp_string(disp_dat);
}
/******************************************************************************
*****************************LCD display Time Set**********************************
*******************************************************************************/
extern void lcd_disp_time_set_init(void)
{
	lcd_clr_screen();
	lcd_set_disp_position(1,0);
	lcd_disp_string("Time Set:");
	lcd_disp_time_set();
}
void convert_time_set_to_string(time_val_type *time,uchar *disp_dat)
/*
**xx:xx:xx 8bit
*/
{
	int i = 0;
	for(i = 0;i < 8;i++)
		*(disp_dat+i) = '*';
	*(disp_dat+2) = ':';
	*(disp_dat+5) = ':';
	if(time_set_state == TIME_SET_HOUR) {
		*(disp_dat+0) = time->hour/10+'0';
		*(disp_dat+1) = time->hour%10+'0';
	}else if(time_set_state == TIME_SET_MIN) {
		*(disp_dat+3) = time->min/10+'0';
		*(disp_dat+4) = time->min%10+'0';
	}else if(time_set_state == TIME_SET_SEC) {
		*(disp_dat+6) = time->sec/10+'0';
		*(disp_dat+7) = time->sec%10+'0';
	}

}
extern void lcd_disp_time_set(void)
{
	uchar disp_dat[9] = 0;
	convert_time_set_to_string(&real_time_tmp,disp_dat);
	lcd_set_disp_position(2,0);
	lcd_disp_string(disp_dat);
}
