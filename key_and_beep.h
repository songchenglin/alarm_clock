#ifndef KEY_AND_BEEP_H
#define KEY_AND_BEEP_H
#include <reg52.h>

sbit BEEP = P2^3;
sbit KEY1 = P3^2;
sbit KEY2 = P2^5;
sbit KEY3 = P2^6;
sbit KEY4 = P2^7;

typedef void (*key_cb_type)(void);
typedef struct key_fun {
	key_cb_type key1_fun;
	key_cb_type key2_fun;
	key_cb_type key3_fun;
	key_cb_type key4_fun;	
}key_fun_type;
extern key_fun_type key_funs;
extern void key_init(void);
extern void get_key_val(void);
extern void beep_init(void);
extern void beep_short(void);
#endif