//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include "globdefs.h"
#include "frmMain.h"
#pragma hdrstop
#include "Utils.h"
//---------------------------------------------------------------------------
//#pragma package(smart_init)

#define 	MAX_MENU_STRING         32

extern TformMain *formMain;


// Завершить процесс
bool bTerminate = false;

static char TimeStringWithZero[32];

/*
  Name  : CRC-16
  Poly  : 0x8005    x^16 + x^15 + x^2 + 1
  Init  : 0xFFFF
  Revert: true
  XorOut: 0x0000
  Check : 0x4B37 ("123456789")
  MaxLen: 4095 байт (32767 бит) - обнаружение
  одинарных, двойных, тройных и всех нечетных ошибок
*/
static const u16 Crc16Table[256] = {
    0x0000, 0x8005, 0x800F, 0x000A, 0x801B, 0x001E, 0x0014, 0x8011,
    0x8033, 0x0036, 0x003C, 0x8039, 0x0028, 0x802D, 0x8027, 0x0022,
    0x8063, 0x0066, 0x006C, 0x8069, 0x0078, 0x807D, 0x8077, 0x0072,
    0x0050, 0x8055, 0x805F, 0x005A, 0x804B, 0x004E, 0x0044, 0x8041,
    0x80C3, 0x00C6, 0x00CC, 0x80C9, 0x00D8, 0x80DD, 0x80D7, 0x00D2,
    0x00F0, 0x80F5, 0x80FF, 0x00FA, 0x80EB, 0x00EE, 0x00E4, 0x80E1,
    0x00A0, 0x80A5, 0x80AF, 0x00AA, 0x80BB, 0x00BE, 0x00B4, 0x80B1,
    0x8093, 0x0096, 0x009C, 0x8099, 0x0088, 0x808D, 0x8087, 0x0082,
    0x8183, 0x0186, 0x018C, 0x8189, 0x0198, 0x819D, 0x8197, 0x0192,
    0x01B0, 0x81B5, 0x81BF, 0x01BA, 0x81AB, 0x01AE, 0x01A4, 0x81A1,
    0x01E0, 0x81E5, 0x81EF, 0x01EA, 0x81FB, 0x01FE, 0x01F4, 0x81F1,
    0x81D3, 0x01D6, 0x01DC, 0x81D9, 0x01C8, 0x81CD, 0x81C7, 0x01C2,
    0x0140, 0x8145, 0x814F, 0x014A, 0x815B, 0x015E, 0x0154, 0x8151,
    0x8173, 0x0176, 0x017C, 0x8179, 0x0168, 0x816D, 0x8167, 0x0162,
    0x8123, 0x0126, 0x012C, 0x8129, 0x0138, 0x813D, 0x8137, 0x0132,
    0x0110, 0x8115, 0x811F, 0x011A, 0x810B, 0x010E, 0x0104, 0x8101,
    0x8303, 0x0306, 0x030C, 0x8309, 0x0318, 0x831D, 0x8317, 0x0312,
    0x0330, 0x8335, 0x833F, 0x033A, 0x832B, 0x032E, 0x0324, 0x8321,
    0x0360, 0x8365, 0x836F, 0x036A, 0x837B, 0x037E, 0x0374, 0x8371,
    0x8353, 0x0356, 0x035C, 0x8359, 0x0348, 0x834D, 0x8347, 0x0342,
    0x03C0, 0x83C5, 0x83CF, 0x03CA, 0x83DB, 0x03DE, 0x03D4, 0x83D1,
    0x83F3, 0x03F6, 0x03FC, 0x83F9, 0x03E8, 0x83ED, 0x83E7, 0x03E2,
    0x83A3, 0x03A6, 0x03AC, 0x83A9, 0x03B8, 0x83BD, 0x83B7, 0x03B2,
    0x0390, 0x8395, 0x839F, 0x039A, 0x838B, 0x038E, 0x0384, 0x8381,
    0x0280, 0x8285, 0x828F, 0x028A, 0x829B, 0x029E, 0x0294, 0x8291,
    0x82B3, 0x02B6, 0x02BC, 0x82B9, 0x02A8, 0x82AD, 0x82A7, 0x02A2,
    0x82E3, 0x02E6, 0x02EC, 0x82E9, 0x02F8, 0x82FD, 0x82F7, 0x02F2,
    0x02D0, 0x82D5, 0x82DF, 0x02DA, 0x82CB, 0x02CE, 0x02C4, 0x82C1,
    0x8243, 0x0246, 0x024C, 0x8249, 0x0258, 0x825D, 0x8257, 0x0252,
    0x0270, 0x8275, 0x827F, 0x027A, 0x826B, 0x026E, 0x0264, 0x8261,
    0x0220, 0x8225, 0x822F, 0x022A, 0x823B, 0x023E, 0x0234, 0x8231,
    0x8213, 0x0216, 0x021C, 0x8219, 0x0208, 0x820D, 0x8207, 0x0202
};

/* Таблица windows */
static const u8 cp1251_tab[] = "абвгдеёжзийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";

/* Таблица koi8r */
static const u8 koi8r_tab[] =
    // а б в г д е ё ж з и
    "\x8A\x8B\xA0\x90\x8D\x8E\x86\x9f\xA3\x92\x93\x94\x95\x96\x97\x98\x99\x9B\x9C\x9D\x9E\x8F\x91\x8C\xA7\xA4\xA6\xA8\xA2\xA1\xA5\x89\x9A"
    "\xAA\xAB\xC0\xB0\xAD\xAE\x87\xBF\xC3\xB2\xB3\xB4\xB5\xB6\xB7\xB8\xB9\xBB\xBC\xBD\xBE\xAF\xB1\xAC\xC7\xC4\xC6\xC8\xC2\xC1\xC5\xA9\xBA";

/**
 * Функция преобразования CP1251 в KOI8
 * для использования KOI8 шрифта
 */
c8 *win_to_koi8(c8 * str)
{
    int len, i, j;
    len = strlen(str);

    for (i = 0; i < len; i++) {
	for (j = 0; j < 66; j++) {
	    if (str[i] == cp1251_tab[j]) {
		str[i] = koi8r_tab[j];
		break;
	    }

	}
    }
    return &str[0];
}



/**
 * Функция преобразования KOI8 в CP1251
 * для вывода на консоль
 */
c8 *koi8_to_win(const c8 * str)
{
    int len, i, j;
    len = strlen(str) % MAX_MENU_STRING;

    static char win_str[MAX_MENU_STRING];

    memcpy(win_str, str, len);

    for (i = 0; i < len; i++) {
	for (j = 0; j < 66; j++) {
            u16 sym = (u8)win_str[i];

	    if (sym == koi8r_tab[j]) {
		win_str[i] = cp1251_tab[j];
		break;
	    }

	}
    }
    win_str[i] = 0;
    return &win_str[0];
}



/**
 * Проверка контрольной суммы
 * mes - pointer to NA-01 reply
 */
u8 test_crc16(u8 * mes)
{
    int k, len, ind;
    u16 CRC;			/* returns 0 = OK; !0 = wrong */

    for (len = mes[0] + 3, k = 0, CRC = 0; k < len; ++k) {
	ind = (CRC >> 8) & 255;
	CRC = ((CRC << 8) + mes[k]) ^ Crc16Table[ind];
    }
    return CRC & 0xffffu;
}

//---------------------------------------------------------------------------
/* Добавляет контрольную суммы к сообщению cmd - правильное!!!*/
void add_crc16(u8 * buf)
{
    int i, len, ind;
    unsigned crc = 0;

    // length - 3 байт + 5
    len = buf[2] + 5;
    buf[len - 2] = buf[len - 1] = 0;

    for (i = 0; i < len; ++i) {
	ind = (crc >> 8) & 255;
	crc = ((crc << 8) + buf[i]) ^ Crc16Table[ind];
    }

    buf[len - 1] = (u8) (crc & 0xff);
    buf[len - 2] = (u8) ((crc >> 8) & 0xff);
}

/***********************************************************************************
 *  Расчёт CRC-16 плохой как для модема :(
 ************************************************************************************/
u16 check_crc16(u8 * buf, u16 len)
{
    unsigned short crc = 0xFFFF;
    unsigned char i;

    while (len--) {
	crc ^= *buf;
	buf++;

	for (i = 0; i < 8; i++)
	    crc = crc & 0x8000 ? (crc << 1) ^ 0x1021 : crc << 1;
    }

    return crc;
}

//---------------------------------------------------------------------------
void ver_to_str(DEV_ADDR_PACK * addr, char *str)
{
    SYSTEMTIME t;

    //ver: 12.06.13 11.24

    sec_to_st(addr->time, &t);
    sprintf(str, "%d.%03d.%02d%02d%02d%02d%02d", addr->ver, addr->rev, t.wDay,
	    t.wMonth, t.wYear - 100, t.wHour, t.wMinute);
}


//---------------------------------------------------------------------------
void sec_to_simple_str(long ls, char *str)
{
    int h, m, s;

    s = ls % 60;
    m = (ls / 60) % 60;
    h = ls / 3600;

    /* Записываем дату в формате: [08:57:22] */
    sprintf(str, "%#4d:%02d:%02d", h, m, s);
}

//---------------------------------------------------------------------------
String TimeString(void)
{
    SYSTEMTIME st;

    GetSystemTime(&st);

    // 09-07-2013 - 13:11:39.871
    sprintf(TimeStringWithZero, "%02d.%02d.%04d - %02d:%02d:%02d.%03d ",
	    st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond,
	    st.wMilliseconds);
    return String(TimeStringWithZero);
}

//---------------------------------------------------------------------------
// Секунды UNIX во время Windows
void sec_to_st(int ls, SYSTEMTIME * t)
{
    struct tm t0;

    if (ls != -1) {
        formMain->gmtime32_s(&t0, (long*)&ls);


	/* Записываем дату, что получилось */
	t->wSecond = t0.tm_sec;
	t->wMinute = t0.tm_min;
	t->wHour = t0.tm_hour;
	t->wDay = t0.tm_mday;
	t->wMonth = t0.tm_mon + 1;	/* В Systemtime месяцы с 1 по 12, в tm_time с 0 по 11 */
	t->wYear = t0.tm_year + 1900;	/* В SYSTEM годы считаются с 0 - го */
    }
}

//---------------------------------------------------------------------------
/* Строку в верхний регистр   */
void str_to_cap(char *str, int len)
{
    int i = len;

    while (i--)
	str[i] = (str[i] > 0x60) ? str[i] - 0x20 : str[i];
}


//-----------------------------------------------------------------------------------
int bufs_to_st(char *b1, char *b2, SYSTEMTIME * st)
{
    char buf[32];

    //день
    memcpy(buf, b1, 2);
    buf[2] = 0;
    st->wDay = atoi(buf);

    //месец
    memcpy(buf, b1 + 3, 2);
    buf[2] = 0;
    st->wMonth = atoi(buf);

    //Год
    memcpy(buf, b1 + 6, 2);
    buf[2] = 0;
    st->wYear = atoi(buf) + 2000;


    //чесы
    memcpy(buf, b2, 2);
    buf[2] = 0;
    st->wHour = atoi(buf);

    //месец
    memcpy(buf, b2 + 3, 2);
    buf[2] = 0;
    st->wMinute = atoi(buf);

    //Год
    memcpy(buf, b2 + 6, 2);
    buf[2] = 0;
    st->wSecond = atoi(buf);

    return st_to_sec(st);
}

//-----------------------------------------------------------------------------------
// Получить миллисекунды
s64 get_msec_ticks(void)
{
    SYSTEMTIME st;
    s64 msec;

    GetSystemTime(&st);

    // Получим время с миллисекундами
    msec = st_to_sec(&st) * (s64) 1000 + st.wMilliseconds;

    return msec;
}

//-----------------------------------------------------------------------------------
// Получить секунды
long get_sec_ticks(void)
{
    SYSTEMTIME st;
    long sec;

    GetSystemTime(&st);

    // Получим время с миллисекундами
    sec = st_to_sec(&st);
    return sec;
}

//---------------------------------------------------------------------------

// Секунды в строку   - заменили gmtime на безопасную!
void sec_to_str(long ls, char *str)
{
    struct tm t;

    if (ls != -1) {
        formMain->gmtime32_s(&t, &ls);
	sprintf(str, "%02d.%02d.%02d %02d:%02d:%02d", t.tm_mday, t.tm_mon + 1,
		t.tm_year-100, t.tm_hour, t.tm_min, t.tm_sec);
    }
}

//-----------------------------------------------------------------------------------
