//---------------------------------------------------------------------------
#include <stdio.h>
#include <vcl.h>
#pragma hdrstop
#include "proto.h"
#include "Utils.h"
#include "PortThread.h"
#include "frmEeprom.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall PortThread::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------
#define BAUD_RATE       115200
#define TIMER_HSEC_DIVIDER	        (3)
#define TIMER_SEC_DIVIDER		(10)
#define TIMER_MIN_DIVIDER		(10*60)


__fastcall PortThread::PortThread(bool CreateSuspended)
:TThread(CreateSuspended)
{
}

//---------------------------------------------------------------------------
// Вставляет только одну станцию
void __fastcall PortThread::Execute()
{
    int i, j = 0, z = 0;
    int timer;
    char str[512];
    c8 *stick[] = {
	"--",
	"\\",
	"|",
	"/"
    };

    i = sizeof(EEPROM_VALUE_PACK);

    do {
	timer = formMain->GetTimerTick();

	// Ищем здесь свободный порт
	while (!PortIsOpen() && !formMain->EndThread() && !j) {

	    for (i = 1; i < 255; i++) {
		memset(&formMain->dev_addr, 0, sizeof(formMain->dev_addr));
		if (PortInit(i, BAUD_RATE) && WhoAreU(&formMain->dev_addr)) {
		    formMain->EnableStation(i, j);
		    j++;
		    break;
		} else {
		    PortClose();
		}
		Sleep(5);
	    }
	    formMain->sbInfo->Panels->Items[0]->Text = "Try... " + IntToStr(z++);
	}

	// Ждем нажатия
	formMain->sbRunModule->Enabled = true;	// Разблокируем кнопку подклбючения
	while (!formMain->sbRunModule->Down) {
	    if (formMain->EndThread()) {
		goto out;
	    }
	}


#if 10
	// Раз запрашивает EEPROM чтобы прочитать
	if (formMain->bGetAllEeprom) {

	    for (i = 0; i < 8; i++) {

		if (ConstGet(&formMain->pack[i], i)) {
		    formMain->pbLoad->Position = i;
                    if (strncpy(str, (formMain->pack[i].payload.sens.formula), 31)) {
			formMain->memoLog->Lines->Add(IntToStr(i) + ": " + str);
		    }

		}

		Sleep(10);
	    }
		formMain->pnStuff1->Caption = koi8_to_win(formMain->pack[0].payload.sens.meas_gas);
		formMain->pnStuff2->Caption = formMain->pack[1].payload.sens.meas_gas;
		formMain->pnStuff3->Caption = formMain->pack[2].payload.sens.meas_gas;
		formMain->pnStuff4->Caption = formMain->pack[3].payload.sens.meas_gas;
		formMain->pnStuff5->Caption = koi8_to_win(formMain->pack[4].payload.sens.meas_gas);
		formMain->pnStuff6->Caption = formMain->pack[5].payload.sens.meas_gas;
		formMain->pnStuff7->Caption = formMain->pack[6].payload.sens.meas_gas;
		formMain->pnStuff8->Caption = formMain->pack[7].payload.sens.meas_gas;


	    frmEepromParam->ShowEepromParam(0);
	    formMain->pbLoad->Position = 0;
	    formMain->bGetAllEeprom = false;
	}
#endif

	// Получаем статус раз в секунду или чаще
	if (timer % (TIMER_SEC_DIVIDER + 1) == 0) {
	    if (StatusGet(&formMain->status)) {
		SendMessage(formMain->Handle, CM_UPDATE_STATUS, 0, 0);
	    }
	}

	// счетчики обмена
	if (timer % TIMER_HSEC_DIVIDER == 0) {
	    PcCountsGet(&formMain->cnt0);
	    if (CountsGet(&formMain->cnt1)) {
		formMain->sbInfo->Panels->Items[2]->Text = "        " + String(stick[j++ % 4]);
	    }
	    SendMessage(formMain->Handle, CM_SHOW_COUNTS, 0, 0);
	}

	// Данные измерений
	if ((formMain->status.main_status & 128) && (timer % TIMER_SEC_DIVIDER == 0)) {

	    SENSOR_DATA_PACK pack;
	    if (DataGet(&pack)) {
		memcpy(&formMain->aquis, &pack, sizeof(SENSOR_DATA_PACK));
		SendMessage(formMain->Handle, CM_READ_DATA, 0, 0);
		Sleep(1);
	    }

	}


    } while (!formMain->EndThread());
  out:
    PortClose();		//можно переместить в функцию потока
}

//---------------------------------------------------------------------------
