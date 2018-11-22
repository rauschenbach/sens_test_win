#ifndef UtilsH
#define UtilsH

#include <windows.h>
#include "globdefs.h"


u8 test_crc16(u8 *);
void add_crc16(u8 *);
void sec_to_str(long, char *);
int time_get(void);
String TimeString(void);

int st_to_sec(SYSTEMTIME *);

void sec_to_st(int, SYSTEMTIME *);
s64 get_msec_ticks(void);
long get_sec_ticks(void);
void str_to_cap(char *, int);

void sec_to_simple_str(long, char *);
void ver_to_str(DEV_ADDR_PACK *, char *);
u16 check_crc16(u8 *, u16);


void clr_xchg_err(void);
int bufs_to_st(char *, char *, SYSTEMTIME *);
c8 *koi8_to_win(const c8 * str);
c8 *win_to_koi8(c8 * str);

#endif
