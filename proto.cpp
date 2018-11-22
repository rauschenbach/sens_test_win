//---------------------------------------------------------------------------
#include <stdio.h>
#include "frmMain.h"
#include "globdefs.h"
#include "ComPort.h"
#pragma hdrstop
#include "proto.h"
#include "Utils.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#define SHORT_STATUS_BYTES	  4

static CComPort *CommPort = NULL;
static CRITICAL_SECTION cs;
static u8 rx_buf[4096];		/* Приемный буфер */
static UART_COUNTS_PACK pc_cnt;

static int portnum = 0;
static int portcmd = 0;


bool PortIsOpen(void)
{
    return (CommPort == NULL) ? false : true;
}


// Открываем и настраиваем порт
bool PortInit(int num, u32 baud)
{
    bool res;
    AnsiString temp;
    struct CONNECTION_PARAMETERS params;

    do {
	temp = "COM" + IntToStr(num);

	//Открываем порт и записываем настройки
	portnum = num;
	params.COMPort = num;
	params.PortSpeed = baud;
	params.MaxFullRetrains = 5;
	params.MaxTimeout = 50;

	// закроем порт если он есть
	if (CommPort != NULL) {
	    delete CommPort;
	    CommPort = NULL;
	    DeleteCriticalSection(&cs);
	}


	CommPort = new CComPort(params);
	Sleep(1);
	res = CommPort->Init();
	if (res == false) {
	    if (13 == portnum) {
		formMain->memoLog->Lines->Add("Error open port");
	    }
	    break;
	}
	res = true;
	memset(&pc_cnt, 0, sizeof(UART_COUNTS_PACK));
	InitializeCriticalSection(&cs);	// Критическая секция
    } while (0);
    return res;
}

// Выдать счетчики обмена PC
bool PcCountsGet(void *p)
{
    bool res = false;

    if (p != NULL) {
	memcpy((UART_COUNTS_PACK *) p, &pc_cnt, sizeof(pc_cnt));
	res = true;
    }
    return res;
}



// Закрыть порт
bool PortClose(void)
{

    // закроем порт
    if (CommPort != NULL) {
	delete CommPort;
	CommPort = NULL;
	DeleteCriticalSection(&cs);
    }
    return true;
}

// Запись в порт
static int PortWrite(u8 * buf, int len)
{
    int num;

    if (CommPort != NULL) {
	num = CommPort->Write(buf, len);
	pc_cnt.tx_pack++;	//передано
    } else {
	return -1;
    }
    return num;
}


// Чтение из порта
static int PortRead(char *buf, int len, int timeout)
{
    int num;

    try {

	// Меньше всего статуса но не короткий статус
	num = CommPort->Read(buf, len, timeout+5);
    } catch(...) {
	formMain->memoLog->Lines->Add("Error read");
    }

    if (num < len && num != SHORT_STATUS_BYTES) {
	pc_cnt.rx_cmd_err++;	//принято
	CommPort->Reset();
    }

    pc_cnt.rx_pack++;
    return num;
}


// Чтение из порта
static int PortReadToTimeout(u8 * buf, int t)
{
    int num;

    num = CommPort->ReadUntilTimeout(buf, t);
    if (num < 0) {		// Все принятые пакеты
	pc_cnt.rx_cmd_err++;	//принято
	CommPort->Reset();
	return -1;		// Нет связи
    }

    CommPort->Reset();
    pc_cnt.rx_pack++;		//принято
    return num;
}


// Выдать счетчики обмена PC
int PcCountsGet(UART_COUNTS_PACK * cnt)
{
    int res = -1;
    if (cnt != NULL) {
	memcpy(cnt, &pc_cnt, sizeof(pc_cnt));
	res = 0;
    }
    return res;
}

/**
 * Послать любую команду. Оформить data по протоколу
 */
bool SendSomeCommand(u8 cmd, void *data, int data_len)
{
    u8 buf[1255];
    int num;
    bool res = false;

    do {
	if (data_len > 255 || data_len < 0)
	    break;		// неправильная длина посылки

	buf[0] = 0xff;		// адрес
	buf[1] = 0;
	buf[2] = data_len + 1;	// на единицу больше самих данных
	buf[3] = cmd;		// Команда

	portcmd = cmd;        

	// В команде есть данные
	if (data != NULL && data_len > 0) {
	    memcpy(buf + 4, (u8 *) data, data_len);
	}
	add_crc16(buf);		// Контрольная сумма

	num = PortWrite(buf, buf[2] + 5);
	if (num != buf[2] + 5) {
	    break;		// Порт не открыт
	}
	res = true;		// Все ОК
    } while (0);

    return res;
}

/**
 * Получить ответ из порта.
 * к длине прибавляем 2 байта CRC16 и 1 самый первый байт длины
 */
bool ReadAnyData(void *data, int size, int timeout = 0)
{
    int num, len;
    u8 byte;
    bool res = false;


    do {
	if (size == 0 || data == NULL) {
	    break;
	}

	len = size + 3;

	// Ждем ответа
	num = PortRead(rx_buf, len, timeout);
	if (num < len && num != SHORT_STATUS_BYTES && rx_buf[0] != size) {
	    if (portnum == 13 && portcmd == UART_CMD_COMMAND_PC) {
		formMain->memoLog->Lines->Add("Error num = " + IntToStr(num));
	    }
	    break;
	}

	byte = rx_buf[1];
	if (num == SHORT_STATUS_BYTES && byte & 8) {
	    formMain->memoLog->Lines->Add("READ ERROR");
	    break;
	}

	if (test_crc16(rx_buf)) {
	    if (portnum == 13 && portcmd == UART_CMD_COMMAND_PC) {
		formMain->memoLog->Lines->Add("Error crc");
	    }

	    break;
	} else {
	    memcpy(data, rx_buf + 1, size);
	    res = true;
	}
    } while (0);

    return res;
}


/**
 * Получить ответ из порта
 */
bool ReadAnyDataToTimeout(void *data, int time)
{
    int num, len;
    bool res = false;

    do {
	if (time == 0 || data == NULL) {
	    break;
	}
	// Ждем ответа
	num = PortReadToTimeout(rx_buf, time);
	if (num <= 0) {
	    break;
	}

	if (test_crc16(rx_buf)) {
	    break;
	} else {
	    memcpy(data, rx_buf + 1, rx_buf[0]);
	    res = true;
	}

    } while (0);

    return res;
}

////////////////////////////////////////////////////////////////////////////////
//                      Сами команды
////////////////////////////////////////////////////////////////////////////////
// Найти станцию с открытым портом
// Команда Who Are U ?
bool WhoAreU(void *p)
{
    bool res = false;
    DEV_ADDR_PACK *pack;

    EnterCriticalSection(&cs);
    do {
	if (!PortIsOpen()) {
	    break;
	}
	pack = (DEV_ADDR_PACK *) p;

	if (SendSomeCommand(UART_CMD_COMMAND_PC, NULL, 0) == false) {
	    break;
	}
	Sleep(50);

	// Читаем
	res = ReadAnyData((u8 *) pack, sizeof(DEV_ADDR_PACK), 50);
    } while (0);
    LeaveCriticalSection(&cs);
    return res;
}

//------------------------------------------------------------------------------
// Получить статус - new
bool StatusGet(void *p)
{
    bool res = false;

    EnterCriticalSection(&cs);

    do {
	if (!PortIsOpen()) {
	    break;
	}

	if (SendSomeCommand(UART_CMD_GET_DSP_STATUS, NULL, 0) == false) {
	    break;
	}
	Sleep(5);
	// Читаем
	res = ReadAnyData(p, sizeof(DEV_STATUS_PACK));
    } while (0);

    LeaveCriticalSection(&cs);
    return res;
}

//--------------------------------------------------------------------------------------------------
// Получить счетчики - new
bool CountsGet(void *p)
{
    bool res = false;
    UART_COUNTS_PACK *cnt;

    EnterCriticalSection(&cs);

    do {
	if (!PortIsOpen()) {
	    break;
	}

	cnt = (UART_COUNTS_PACK *) p;

	if (SendSomeCommand(UART_CMD_GET_COUNTS, NULL, 0) == false) {
	    break;
	}
	Sleep(5);

	// Читаем
	res = ReadAnyData((u8 *) cnt, sizeof(UART_COUNTS_PACK));
    } while (0);

    LeaveCriticalSection(&cs);
    return res;
}

//---------------------------------------------------------------------------
// Включить насос
bool PumpOn(void)
{
    bool res = false;
    u8 buf[256];

    EnterCriticalSection(&cs);
    do {
	if (!PortIsOpen()) {
	    break;
	}

	if (SendSomeCommand(UART_CMD_PUMP_ON, NULL, 0) == false) {
	    break;
	}
	Sleep(5);

	// Читаем короткий статус
	res = ReadAnyData(buf, SHORT_STATUS_BYTES);
    } while (0);
    LeaveCriticalSection(&cs);
    return res;
}

//---------------------------------------------------------------------------
// вЫключить насос
bool PumpOff(void)
{
    bool res = false;
    u8 buf[256];

    EnterCriticalSection(&cs);
    do {
	if (!PortIsOpen()) {
	    break;
	}

	if (SendSomeCommand(UART_CMD_PUMP_OFF, NULL, 0) == false) {
	    break;
	}
	Sleep(5);

	// Читаем короткий статус
	res = ReadAnyData(buf, SHORT_STATUS_BYTES);
    } while (0);
    LeaveCriticalSection(&cs);
    return res;
}

//---------------------------------------------------------------------------
// Синхронизация времени
bool TimeSync(void *t)
{
    bool res = false;
    u8 buf[256];

    EnterCriticalSection(&cs);
    do {
	if (!PortIsOpen()) {
	    break;
	}

	if (SendSomeCommand(UART_CMD_SYNC_RTC_CLOCK, t, sizeof(s32)) == false) {
	    break;
	}
	Sleep(5);

	// Читаем короткий статус
	res = ReadAnyData(buf, SHORT_STATUS_BYTES);
    } while (0);

    LeaveCriticalSection(&cs);
    return res;
}

//---------------------------------------------------------------------------
// Старт
bool StartDevice(void)
{
    bool res = false;
    u8 buf[256];

    EnterCriticalSection(&cs);
    do {
	if (!PortIsOpen()) {
	    break;
	}

	if (SendSomeCommand(UART_CMD_DEV_START, NULL, 0) == false) {
	    break;
	}
	Sleep(5);

	// Читаем короткий статус
	res = ReadAnyData(buf, SHORT_STATUS_BYTES);
    } while (0);

    LeaveCriticalSection(&cs);
    return res;
}


//---------------------------------------------------------------------------
// Стоп
bool StopDevice(void)
{
    bool res = false;
    u8 buf[256];


    EnterCriticalSection(&cs);
    do {
	if (!PortIsOpen()) {
	    break;
	}

	if (SendSomeCommand(UART_CMD_DEV_STOP, NULL, 0) == false) {
	    break;
	}
	Sleep(5);
	// Читаем короткий статус
	res = ReadAnyData(buf, SHORT_STATUS_BYTES);
    } while (0);

    LeaveCriticalSection(&cs);
    return res;
}

//---------------------------------------------------------------------------
// Получить данные EEPROM (со всех 8-ми каналов)
bool ConstGet(void *p, int chan)
{
    bool res = false;
    u16 addr;
    EEPROM_VALUE_PACK *pack;

    EnterCriticalSection(&cs);
    do {
	if (!PortIsOpen()) {
	    break;
	}
	// с какого канала получаем?
        if(p == NULL) {
	    formMain->memoLog->Lines->Add("SEND ERROR!!!");
            break;
        }
	pack = (EEPROM_VALUE_PACK *) p;
        addr = chan * sizeof(channel_params_settings);

	// получить данные EEPROM для канала chan
	if (SendSomeCommand(UART_CMD_GET_ADC_CONST, &addr, 2) == false) {
	    break;
	}
        formMain->memoLog->Lines->Add("SEND OK");
	Sleep(100);

	// Читаем
	res = ReadAnyData(pack, sizeof(EEPROM_VALUE_PACK), 2500);

    } while (0);
    LeaveCriticalSection(&cs);
    return res;
}

//---------------------------------------------------------------------------
// команда Выдай данные измерений или короткий статус
bool DataGet(void *p)
{
    bool res = false;
    SENSOR_DATA_PACK *pack;


    EnterCriticalSection(&cs);
    do {
	if (!PortIsOpen() || p == NULL) {
	    break;
	}
	pack = (SENSOR_DATA_PACK *) p;

	if (SendSomeCommand(UART_CMD_GET_DATA, NULL, 0) == false) {
	    break;
	}

	Sleep(10);
	// Мы не знаем что придет - короткий статус или пакет
	// поэтому увидим, что старшие байты нули
	memset(pack, 0, sizeof(SENSOR_DATA_PACK));
	if (ReadAnyDataToTimeout(pack, 250)) {
	    if (pack->time != 0 && pack->freq != 0) {
		res = true;
//                pack->channels->chan0.f_val = 0.25 + ((rand() / 255) / 255.0);
//                pack->channels->chan1.f_val = 0.125 + ((rand() / 255) / 255.0);
		pack->channels->chan2.f_val = 0.512 + ((rand() / 255) / 255.0);
		pack->channels->chan3.f_val = 0.35 + ((rand() / 255) / 255.0);
		pack->channels->chan4.f_val = 0.55 + ((rand() / 255) / 255.0);
		pack->channels->chan5.f_val = 0.51 + ((rand() / 255) / 255.0);
		pack->channels->chan6.f_val = 0.55 + ((rand() / 255) / 255.0);
		pack->channels->chan7.f_val = 0.22 + ((rand() / 255) / 255.0);
	    }
	}
    } while (0);
    LeaveCriticalSection(&cs);
    return res;
}

//---------------------------------------------------------------------------
// Записать константы EEPROM
bool ConstSet(void *p)
{
    bool res = false;
    u16 chan;
    u8 buf[256];
    EEPROM_VALUE_PACK *pack;

    EnterCriticalSection(&cs);
    do {
	if (!PortIsOpen() || p == NULL) {
	    break;
	}

	if (p == NULL)
	    break;
	pack = (EEPROM_VALUE_PACK *) p;

	// записать данные EEPROM для канала chan и со 2-го байта саму посылку
        memcpy(&chan, &pack->payload.sens.reg_set[18], 2);
	memcpy(buf, &chan, 2);
	memcpy(buf + 2, pack, sizeof(EEPROM_VALUE_PACK));

	if (SendSomeCommand(UART_CMD_SET_ADC_CONST, buf, sizeof(EEPROM_VALUE_PACK) + 2) == false) {
	    break;
	}
	Sleep(1);

	// Читаем короткий статус
	res = ReadAnyDataToTimeout(buf, 250);
    } while (0);
    LeaveCriticalSection(&cs);
    return res;
}

//---------------------------------------------------------------------------
