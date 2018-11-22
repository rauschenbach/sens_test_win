#ifndef _GLOBDEFS_H
#define _GLOBDEFS_H

#include <signal.h>
#include <stdint.h>
#include <time.h>

/* Сколько пакетов в пачке  */
#define 	NUM_ADS131_PACK	100

#define		SEC_IN_HOUR		3600

/* Включения для 504 блекфина, не 506!!!!  */
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


/* Длинное время */
#ifndef	time64
#define time64	int64_t
#endif


#ifndef _WIN32			/* Embedded platform */

/* long double не поддержываеца  */
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
/* long double не поддержываеца  */
#ifndef f32
#define f32 float
#endif

#endif


#ifndef IDEF
#define IDEF static inline
#endif

/* На этот адрес будет отвечать наше устройство */
#define 	BROADCAST_ADDR	0xff


#define 	ADC_CHAN			4	/* число каналов  */
#define		MAGIC				0x4b495245

/* Делители */
#define		TIMER_NS_DIVIDER	(1000000000UL)
#define		TIMER_US_DIVIDER	(1000000)
#define		TIMER_MS_DIVIDER	(1000)

/* Причины сброса прибора - перечислены здесь. Из запишем в eeprom  */
#define		CAUSE_POWER_OFF		0x12345678	/* Выключение питания */
#define		CAUSE_EXT_RESET		0xabcdef90	/* Внешний ресет */
#define		CAUSE_BROWN_OUT		0xaa55aa55	/* Снижение питания */
#define		CAUSE_WDT_RESET		0x07070707	/* WDT reset (во время регистрации) */
#define		CAUSE_NO_LINK		0xE7E7E7E7	/* Нет связи - самовыключение прибора */
#define		CAUSE_UNKNOWN_RESET	0xFFFFFFFF	/* Неизвестная причина-выдернули питание */


#define         MAX_GAS_NAME_LEN	32

/*******************************************************************************
 * Состояние устройств для описания State machine
 *******************************************************************************/
typedef enum {
    DEV_POWER_ON_STATE = 0,
    DEV_CHOOSE_MODE_STATE = 1,
    DEV_INIT_STATE,
    DEV_TUNE_WITHOUT_GPS_STATE,	/*Запуск таймера БЕЗ GPS */
    DEV_GET_3DFIX_STATE,
    DEV_TUNE_Q19_STATE,		/* Первичная настройка кварца 19 МГц */
    DEV_SLEEP_AND_DIVE_STATE,	/* Погружение и сон перед началом работы */
    DEV_WAKEUP_STATE,
    DEV_REG_STATE,
    DEV_FINISH_REG_STATE,
    DEV_BURN_WIRE_STATE,
    DEV_SLEEP_AND_POPUP_STATE,
    DEV_WAIT_GPS_STATE,
    DEV_HALT_STATE,
    DEV_COMMAND_MODE_STATE,
    DEV_POWER_OFF_STATE,
    DEV_ERROR_STATE = -1	/* Авария */
} DEV_STATE_ENUM;


/* Главный статус */
#define		STATUS_NO_TIME		1
#define		STATUS_NO_CONST		2
#define		STATUS_VCP_CONNECTED	4
#define		STATUS_CMD_ERROR	8
#define		STATUS_DEV_DEFECT	16
#define		STATUS_MEM_OVERFLOW	32
#define		STATUS_DEV_TEST		64
#define		STATUS_DEV_RUN		128



/**
 * Состояние сенсоров
 */
typedef enum {
   SENS_SLEEP,
   SENS_RUN,
   SENS_STOPED,
   SENS_ERR = 0xFF,	 		
} SENS_STATE_TYPE;

/**
 * Состояние сенсоров. 8 штук
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
 * Статус и ошибки устройств на отправление 
 * Короткий статус (main_status) посылается при ошибке (только 1 байта + 1 len + 2 CRC16)
 * сместим на позицию для совместимости
 */
#pragma pack(4)
typedef struct {
    uint8_t main_status;	/* главный: нет времени, нет констант, нет SD, ошибка в команде, неисправен, переполнение памяти, тестирование, запущен */
    uint8_t self_test0;		/* Самотестирование и ошибки0: 0 - часы, 1 - датчик температуры, 2 - датчик давления, 3 - EEPROM, 4 - карта SD, 5 - flash... */
    uint8_t self_test1;		/* прочие устройства: 0 - насос */
#define devices self_test1
    uint8_t reset_cause;	/* Причина предыдущего сброса */


    SENS_ERROR_t sensor_error;	/* ERROR ошибки-статусы 8 сенсоров. Потом распишем */

    uint32_t eeprom;		/* Статус EEPROM. Выставленный бит - ошибка к/л параметра */

    int32_t  gs_time;		/* Время прибора. Секунды времени UNIX */


    int16_t temperature;	/* Температура внутри прибора * 10 */
    int16_t humidity;		/* Относительная влажность прибора * 10 % */

    uint32_t pressure;		/* Давление кПа */

    int16_t  batt_curr;		/* Ток потребления. мА */
    int16_t  batt_pow;		/* Напряжение батареи питания. мВ */
    uint32_t freq;		/* частота проца */

    uint32_t rsvd[8];
} DEV_STATUS_PACK;

/**
 * Описывает когда случился таймаут карты и прочие ошибки 
 */
typedef struct {
    u32 cmd_error;
    u32 read_timeout;
    u32 write_timeout;
    u32 any_error;
} SD_CARD_ERROR_STRUCT;


/**
 * Имя и адрес. Для совместимости со старой версией (пока)
 * самый первый байт в пакете игнорируем( т.е. считаем u8 ver;) 
 */
typedef struct {
    u8 addr;			/* Адрес платы */
    u8 ver;			/* Версия ПО: 1, 2, 3, 4 итд */
    u8 rev;			/* Ревизия ПО 0, 1, 2 итд */
    u8 res0;			/* Резерв */
    u32 time;			/* Время компиляции */
} DEV_ADDR_PACK;



/**
 * Счетчики обменов по УАРТ
 */
typedef struct {
    u32 rx_pack;		/* Принятые пакеты */
    u32 rx_cmd_err;		/* Ошибка в команде */
    u32 rx_stat_err;		/* Ошибки набегания, кадра (без стопов) и пр */
    u32 rx_crc_err;		/* Ошибки контрольной суммы */
    u32 tx_pack;		/* переданные пакеты */
    u32 tx_err;
} UART_COUNTS_PACK;



#define EEPROM_ADDR_SHIFT 	sizeof(channel_params_settings)

/* адреса для параметров каждого канала */
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
 * Тип сенсора
*/
typedef enum {
    NO_SENSOR,			/* Нет сенсора */
    I2C_SENSOR,			/* На i2c электрохимия */
    MDB_SENSOR,			/* Modbus сенсор */
} sensor_type_en;

/* представление чисел: целые, с одной точкой и с 2-мя точками */
typedef enum {
    INT_MODE,
    TEN_MODE,
    HUN_MODE,
} digit_mode_en;

/* Единицы измерения: mg/m^3, ppm и %%  */
typedef enum {
    MV_MODE,	/* Милливольты - не пересчитываем */
    MMGM_MODE, // мг.м куб
    PPM_MODE,
    PERCENT_MODE,
} units_mode_en;


/**
 * Сенсоры. Часть параметров берется из EEPROM
 * Параметры измерительного канала 
 */
typedef struct {
    sensor_type_en sens_type;		/* Включен.выключен или sensor type */
    units_mode_en type_units;
    digit_mode_en num_digits;
    u8  num_of_gas;			/* Номер газа в таблице */
    u8  reg_set[20];    		/* Установки - регистры или что то еще */

    char formula[MAX_GAS_NAME_LEN];	/* Хим. формула */
#define meas_gas	formula
    FLT_2_INT data;		/* Текущее измерение. Или расчитанное */
    float mult_coef;		/* Коэффициент умножения для сырых данных или gain */
#define		gain  	mult_coef
    float zero_shift;		/* Смещение нуля  */
#define zero zero_shift
    float max_range;		/* МАХ диапазон измерений */
    float thresh_min;		/* Порог чувствительности min */
    float thresh_max;		/* Порог чувствительности макс */
    float filter;
} channel_params_settings;


/**
 * Что будем хранить в псевдо EEPROM - 1 + 1 + 8 + 1 + 1 + 4 = 16 int (64 байта)
 */
typedef struct {
	/* Payload с этого места  */
	union {
		channel_params_settings sens;
		u8 data[sizeof(channel_params_settings)];
	} payload;
} EEPROM_VALUE_PACK;


/**
 *  Регистры модуля LMP91000
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
 * Пакет измерений со всех каналов для передачи. 168 байт 
 */
typedef struct {
    int32_t time;		/* Время измерения первого пакета */
    int32_t freq;		/* Частота опроса, если запуск по таймеру или пауза между измерениями */

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
