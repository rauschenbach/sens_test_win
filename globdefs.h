#ifndef _GLOBDEFS_H
#define _GLOBDEFS_H

#include <signal.h>
#include <stdint.h>
#include <time.h>

/* ������� ������� � �����  */
#define 	NUM_ADS131_PACK	100

#define		SEC_IN_HOUR		3600

/* ��������� ��� 504 ��������, �� 506!!!!  */
#ifndef _WIN32			/* Embedded platform */
#include <float.h>



#else				/* windows   */
#include <windows.h>
#include <tchar.h>
#endif

#ifndef u8
#define u8 unsigned char
#endif

#ifndef s8
#define s8 char
#endif

#ifndef c8
#define c8 char
#endif

#ifndef u16
#define u16 unsigned short
#endif


#ifndef s16
#define s16 short
#endif

#ifndef i32
#define i32  int
#endif


#ifndef u32
#define u32 unsigned long
#endif


#ifndef s32
#define s32 long
#endif


#ifndef u64
#define u64 uint64_t
#endif


#ifndef s64
#define s64 int64_t
#endif


/* ������� ����� */
#ifndef	time64
#define time64	int64_t
#endif


#ifndef _WIN32			/* Embedded platform */

/* long double �� �������������  */
#ifndef f32
#define f32 float
#endif

#ifndef bool
#define bool u8
#endif


#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

#else
/* long double �� �������������  */
#ifndef f32
#define f32 float
#endif

#endif


#ifndef IDEF
#define IDEF static inline
#endif

/* �� ���� ����� ����� �������� ���� ���������� */
#define 	BROADCAST_ADDR	0xff


#define 	ADC_CHAN			4	/* ����� �������  */
#define		MAGIC				0x4b495245

/* �������� */
#define		TIMER_NS_DIVIDER	(1000000000UL)
#define		TIMER_US_DIVIDER	(1000000)
#define		TIMER_MS_DIVIDER	(1000)

/* ������� ������ ������� - ����������� �����. �� ������� � eeprom  */
#define		CAUSE_POWER_OFF		0x12345678	/* ���������� ������� */
#define		CAUSE_EXT_RESET		0xabcdef90	/* ������� ����� */
#define		CAUSE_BROWN_OUT		0xaa55aa55	/* �������� ������� */
#define		CAUSE_WDT_RESET		0x07070707	/* WDT reset (�� ����� �����������) */
#define		CAUSE_NO_LINK		0xE7E7E7E7	/* ��� ����� - �������������� ������� */
#define		CAUSE_UNKNOWN_RESET	0xFFFFFFFF	/* ����������� �������-��������� ������� */


#define         MAX_GAS_NAME_LEN	32

/*******************************************************************************
 * ��������� ��������� ��� �������� State machine
 *******************************************************************************/
typedef enum {
    DEV_POWER_ON_STATE = 0,
    DEV_CHOOSE_MODE_STATE = 1,
    DEV_INIT_STATE,
    DEV_TUNE_WITHOUT_GPS_STATE,	/*������ ������� ��� GPS */
    DEV_GET_3DFIX_STATE,
    DEV_TUNE_Q19_STATE,		/* ��������� ��������� ������ 19 ��� */
    DEV_SLEEP_AND_DIVE_STATE,	/* ���������� � ��� ����� ������� ������ */
    DEV_WAKEUP_STATE,
    DEV_REG_STATE,
    DEV_FINISH_REG_STATE,
    DEV_BURN_WIRE_STATE,
    DEV_SLEEP_AND_POPUP_STATE,
    DEV_WAIT_GPS_STATE,
    DEV_HALT_STATE,
    DEV_COMMAND_MODE_STATE,
    DEV_POWER_OFF_STATE,
    DEV_ERROR_STATE = -1	/* ������ */
} DEV_STATE_ENUM;


/* ������� ������ */
#define		STATUS_NO_TIME		1
#define		STATUS_NO_CONST		2
#define		STATUS_VCP_CONNECTED	4
#define		STATUS_CMD_ERROR	8
#define		STATUS_DEV_DEFECT	16
#define		STATUS_MEM_OVERFLOW	32
#define		STATUS_DEV_TEST		64
#define		STATUS_DEV_RUN		128



/**
 * ��������� ��������
 */
typedef enum {
   SENS_SLEEP,
   SENS_RUN,
   SENS_STOPED,
   SENS_ERR = 0xFF,	 		
} SENS_STATE_TYPE;

/**
 * ��������� ��������. 8 ����
 */
typedef struct {
   unsigned sens0 : 4;
   unsigned sens1 : 4;
   unsigned sens2 : 4;
   unsigned sens3 : 4;
   unsigned sens4 : 4;
   unsigned sens5 : 4;
   unsigned sens6 : 4;
   unsigned sens7 : 4;
} SENS_ERROR_t;


/** 
 * ������ � ������ ��������� �� ����������� 
 * �������� ������ (main_status) ���������� ��� ������ (������ 1 ����� + 1 len + 2 CRC16)
 * ������� �� ������� ��� �������������
 */
#pragma pack(4)
typedef struct {
    uint8_t main_status;	/* �������: ��� �������, ��� ��������, ��� SD, ������ � �������, ����������, ������������ ������, ������������, ������� */
    uint8_t self_test0;		/* ���������������� � ������0: 0 - ����, 1 - ������ �����������, 2 - ������ ��������, 3 - EEPROM, 4 - ����� SD, 5 - flash... */
    uint8_t self_test1;		/* ������ ����������: 0 - ����� */
#define devices self_test1
    uint8_t reset_cause;	/* ������� ����������� ������ */


    SENS_ERROR_t sensor_error;	/* ERROR ������-������� 8 ��������. ����� �������� */

    uint32_t eeprom;		/* ������ EEPROM. ������������ ��� - ������ �/� ��������� */

    int32_t  gs_time;		/* ����� �������. ������� ������� UNIX */


    int16_t temperature;	/* ����������� ������ ������� * 10 */
    int16_t humidity;		/* ������������� ��������� ������� * 10 % */

    uint32_t pressure;		/* �������� ��� */

    int16_t  batt_curr;		/* ��� �����������. �� */
    int16_t  batt_pow;		/* ���������� ������� �������. �� */
    uint32_t freq;		/* ������� ����� */

    uint32_t rsvd[8];
} DEV_STATUS_PACK;

/**
 * ��������� ����� �������� ������� ����� � ������ ������ 
 */
typedef struct {
    u32 cmd_error;
    u32 read_timeout;
    u32 write_timeout;
    u32 any_error;
} SD_CARD_ERROR_STRUCT;


/**
 * ��� � �����. ��� ������������� �� ������ ������� (����)
 * ����� ������ ���� � ������ ����������( �.�. ������� u8 ver;) 
 */
typedef struct {
    u8 addr;			/* ����� ����� */
    u8 ver;			/* ������ ��: 1, 2, 3, 4 ��� */
    u8 rev;			/* ������� �� 0, 1, 2 ��� */
    u8 res0;			/* ������ */
    u32 time;			/* ����� ���������� */
} DEV_ADDR_PACK;



/**
 * �������� ������� �� ����
 */
typedef struct {
    u32 rx_pack;		/* �������� ������ */
    u32 rx_cmd_err;		/* ������ � ������� */
    u32 rx_stat_err;		/* ������ ���������, ����� (��� ������) � �� */
    u32 rx_crc_err;		/* ������ ����������� ����� */
    u32 tx_pack;		/* ���������� ������ */
    u32 tx_err;
} UART_COUNTS_PACK;



#define EEPROM_ADDR_SHIFT 	sizeof(channel_params_settings)

/* ������ ��� ���������� ������� ������ */
#define SENS0_ADDR  0x0000
#define SENS1_ADDR  (SENS0_ADDR + EEPROM_ADDR_SHIFT)
#define SENS2_ADDR  (SENS1_ADDR + EEPROM_ADDR_SHIFT)
#define SENS3_ADDR  (SENS2_ADDR + EEPROM_ADDR_SHIFT)
#define SENS4_ADDR  (SENS3_ADDR + EEPROM_ADDR_SHIFT)
#define SENS5_ADDR  (SENS4_ADDR + EEPROM_ADDR_SHIFT)
#define SENS6_ADDR  (SENS5_ADDR + EEPROM_ADDR_SHIFT)
#define SENS7_ADDR  (SENS6_ADDR + EEPROM_ADDR_SHIFT)


#define SENS_ADDR(x)	(x * EEPROM_ADDR_SHIFT)

#define MAX_ADDR (SENS7_ADDR + EEPROM_ADDR_SHIFT)


typedef union {
  f32 f_val;
  u32 u_val;
} FLT_2_INT;


/*
 * ��� �������
*/
typedef enum {
    NO_SENSOR,			/* ��� ������� */
    I2C_SENSOR,			/* �� i2c ������������ */
    MDB_SENSOR,			/* Modbus ������ */
} sensor_type_en;

/* ������������� �����: �����, � ����� ������ � � 2-�� ������� */
typedef enum {
    INT_MODE,
    TEN_MODE,
    HUN_MODE,
} digit_mode_en;

/* ������� ���������: mg/m^3, ppm � %%  */
typedef enum {
    MV_MODE,	/* ����������� - �� ������������� */
    MMGM_MODE, // ��.� ���
    PPM_MODE,
    PERCENT_MODE,
} units_mode_en;


/**
 * �������. ����� ���������� ������� �� EEPROM
 * ��������� �������������� ������ 
 */
typedef struct {
    sensor_type_en sens_type;		/* �������.�������� ��� sensor type */
    units_mode_en type_units;
    digit_mode_en num_digits;
    u8  num_of_gas;			/* ����� ���� � ������� */
    u8  reg_set[20];    		/* ��������� - �������� ��� ��� �� ��� */

    char formula[MAX_GAS_NAME_LEN];	/* ���. ������� */
#define meas_gas	formula
    FLT_2_INT data;		/* ������� ���������. ��� ����������� */
    float mult_coef;		/* ����������� ��������� ��� ����� ������ ��� gain */
#define		gain  	mult_coef
    float zero_shift;		/* �������� ����  */
#define zero zero_shift
    float max_range;		/* ��� �������� ��������� */
    float thresh_min;		/* ����� ���������������� min */
    float thresh_max;		/* ����� ���������������� ���� */
    float filter;
} channel_params_settings;


/**
 * ��� ����� ������� � ������ EEPROM - 1 + 1 + 8 + 1 + 1 + 4 = 16 int (64 �����)
 */
typedef struct {
	/* Payload � ����� �����  */
	union {
		channel_params_settings sens;
		u8 data[sizeof(channel_params_settings)];
	} payload;
} EEPROM_VALUE_PACK;


/**
 *  �������� ������ LMP91000
 */
typedef enum {
    STATUS_REG,
    LOCK_REG,
    TIACN_REG = 0X10,
    REFCN_REG,
    MODECN_REG,
} LMP91k_REGS_ENUM;


#define NUM_AQUIS_IN_PACK	5

/**
 * ����� ��������� �� ���� ������� ��� ��������. 168 ���� 
 */
typedef struct {
    int32_t time;		/* ����� ��������� ������� ������ */
    int32_t freq;		/* ������� ������, ���� ������ �� ������� ��� ����� ����� ����������� */

    struct {
	FLT_2_INT chan0;
	FLT_2_INT chan1;
	FLT_2_INT chan2;
	FLT_2_INT chan3;

	FLT_2_INT chan4;
	FLT_2_INT chan5;
	FLT_2_INT chan6;
	FLT_2_INT chan7;
    } channels[NUM_AQUIS_IN_PACK];

} SENSOR_DATA_PACK;



#endif				/* globdefs.h */
