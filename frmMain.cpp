//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <stdlib.h>
#include <stdio.h>
#include "frmMain.h"
#include "frmEEPROM.h"
#include "utils.h"
//---------------------------------------------------------------------------
#include "About.h"
#pragma package(smart_init)
#pragma link "LPComponent"
#pragma link "LPDrawLayers"
#pragma link "SLComponentCollection"
#pragma link "SLScope"
#pragma link "sensor"
#pragma link "comm"
#pragma resource "*.dfm"
TformMain *formMain;

//---------------------------------------------------------------------------
__fastcall TformMain::TformMain(TComponent * Owner)
:TForm(Owner), bEndThread(true), bGetAllEeprom(false), tick(0)
{
}

//---------------------------------------------------------------------------
// Выход из программы
void __fastcall TformMain::btnExitClick(TObject * Sender)
{
    Close();
}

//---------------------------------------------------------------------------
// Выход из программы
void __fastcall TformMain::menuExitClick(TObject * Sender)
{
    btnExitClick(Sender);
}

//---------------------------------------------------------------------------
// Активация окна
void __fastcall TformMain::FormActivate(TObject * Sender)
{
    this->Caption = "Gas Sensor";
    Application->Title = "Gas Sensor. Test & control!";

    LibHandle = LoadLibrary("msvcrt.dll");
    if (LibHandle == NULL) {
	Application->MessageBox("Error!", "Ошибка загрузки dll", MB_ICONERROR);
    }

    (FARPROC) gmtime32_s = GetProcAddress(LibHandle, "_gmtime32_s");

    // Запустить поток поиска прибора
    bEndThread = false;
    portThread = new PortThread(false);
    portThread->Priority = tpHigher;
    portThread->FreeOnTerminate = true;
    portThread->Resume();

    // Получить сразу все значения
    bGetAllEeprom = true;
}

//---------------------------------------------------------------------------
// Выгружаем библиотеку
// Завершаем поток
// Закрываем порт
void __fastcall TformMain::FormDestroy(TObject * Sender)
{
    if (LibHandle) {
	FreeLibrary(LibHandle);
    }

    bEndThread = true;
    Sleep(2500);
}
//---------------------------------------------------------------------------
void __fastcall TformMain::EnableStation(int com, int idx)
{
    char str[256];
    char t[32];

    cbPortName->Items->Clear();
    sbRunModule->Enabled = true;	// Разблокируем кнопку подклбючения
    sprintf(str, "GS%04d", dev_addr.addr);
    cbPortName->Items->Add(str);	// Добавляем станцию
    cbPortName->ItemIndex = idx;	// Индекс внутри combobox
    sec_to_str(dev_addr.time, t);
    sprintf(str, "Прибор обнаружен на порту COM%d. Версия ПО: %2d.%02d-%s", com, dev_addr.ver, dev_addr.rev, t);
    sbInfo->Panels->Items[0]->Text = str;
}

//---------------------------------------------------------------------------
bool __fastcall TformMain::EndThread(void)
{
    return bEndThread;
}

//---------------------------------------------------------------------------
// Выводим состояние полученного статуса
void __fastcall TformMain::CMUpdateStatus(TMessage & Message)
{
    char buf[128];
    u8 byte;
    u32 dig;

    sec_to_str(status.gs_time, buf);

    lbTimeGS->Caption = buf;
    byte = status.main_status;


    sprintf(buf,"CPU %d MHz, U pow %2.2f V",status.freq / 1000000, status.batt_pow / 1000.);
    formMain->sbInfo->Panels->Items[1]->Text = buf;

    pnTemp0->Caption = FloatToStrF(status.temperature / 10.0, ffFixed, 5, 1);
    pnPressure->Caption = (status.pressure);
    pnHumid->Caption = (status.humidity / 10);


////////////////////////////////////////////////////////////////////////////////
//              Главный cтатуc-0 байт
////////////////////////////////////////////////////////////////////////////////
    if (byte & 1) {
	lbStatus0_0->Font->Color = clRed;
    } else {
	lbStatus0_0->Font->Color = clGray;
    }

    if (byte & 2) {
	lbStatus0_1->Font->Color = clRed;
    } else {
	lbStatus0_1->Font->Color = clGray;
    }

    if (byte & 4) {
	lbStatus0_2->Font->Color = clGreen;
    } else {
	lbStatus0_2->Font->Color = clGray;
    }

    if (byte & 8) {
	lbStatus0_3->Font->Color = clRed;
    } else {
	lbStatus0_3->Font->Color = clGray;
    }

    if (byte & 16) {
	lbStatus0_4->Font->Color = clRed;
    } else {
	lbStatus0_4->Font->Color = clGray;
    }

    if (byte & 32) {
	lbStatus0_5->Font->Color = clRed;
    } else {
	lbStatus0_5->Font->Color = clGray;
    }

    if (byte & 64) {
	lbStatus0_6->Font->Color = clRed;
    } else {
	lbStatus0_6->Font->Color = clGray;
    }

    if (byte & 128) {
	lbStatus0_7->Font->Color = clBlue;
    } else {
	lbStatus0_7->Font->Color = clGray;
    }

////////////////////////////////////////////////////////////////////////////////
//              тестирование-1 байт
////////////////////////////////////////////////////////////////////////////////
    byte = status.self_test0;
    if (byte & 1) {
	lbStatus1_0->Font->Color = clRed;
    } else {
	lbStatus1_0->Font->Color = clGray;
    }

    if (byte & 2) {
	lbStatus1_1->Font->Color = clRed;
    } else {
	lbStatus1_1->Font->Color = clGray;
    }

    if (byte & 4) {
	lbStatus1_2->Font->Color = clRed;
    } else {
	lbStatus1_2->Font->Color = clGray;
    }

    if (byte & 8) {
	lbStatus1_3->Font->Color = clRed;
    } else {
	lbStatus1_3->Font->Color = clGray;
    }

    if (byte & 16) {
	lbStatus1_4->Font->Color = clRed;
    } else {
	lbStatus1_4->Font->Color = clGray;
    }

    if (byte & 32) {
	lbStatus1_5->Font->Color = clRed;
    } else {
	lbStatus1_5->Font->Color = clGray;
    }

    if (byte & 64) {
	lbStatus1_6->Font->Color = clRed;
    } else {
	lbStatus1_6->Font->Color = clGray;
    }

    if (byte & 128) {
	lbStatus1_7->Font->Color = clBlue;
    } else {
	lbStatus1_7->Font->Color = clGray;
    }


////////////////////////////////////////////////////////////////////////////////
//              прочие устройства -1 байт
////////////////////////////////////////////////////////////////////////////////
    // Насос
    byte = status.self_test1;
    if (byte & 1) {
	lbStatus2_0->Font->Color = clBlue;
	btnPumpOn->ImageIndex = 20;	// включен
    } else {
	lbStatus2_0->Font->Color = clGray;
	btnPumpOn->ImageIndex = 21;	// вЫключен            
    }

    if (byte & 2) {
	lbStatus2_1->Font->Color = clGreen;
    } else {
	lbStatus2_1->Font->Color = clGray;
    }

    if (byte & 4) {
	lbStatus2_2->Font->Color = clGreen;
    } else {
	lbStatus2_2->Font->Color = clGray;
    }

    if (byte & 8) {
	lbStatus2_3->Font->Color = clGreen;
    } else {
	lbStatus2_3->Font->Color = clGray;
    }

    if (byte & 16) {
	lbStatus2_4->Font->Color = clGreen;
    } else {
	lbStatus2_4->Font->Color = clGray;
    }

    if (byte & 32) {
	lbStatus2_5->Font->Color = clGreen;
    } else {
	lbStatus2_5->Font->Color = clGray;
    }

    if (byte & 64) {
	lbStatus2_6->Font->Color = clGreen;
    } else {
	lbStatus2_6->Font->Color = clGray;
    }

    if (byte & 128) {
	lbStatus2_7->Font->Color = clGreen;
    } else {
	lbStatus2_7->Font->Color = clGray;
    }
///////////////////////////////////////////////////////////////////////////////
//                           Статусы АЦП
//                      Пока показываем зеленым если они работают
///////////////////////////////////////////////////////////////////////////////
    dig = status.sensor_error.sens0;
    if (dig == SENS_SLEEP) {
	ltAdc0->State = rbSleep;
    } else if(dig == SENS_RUN){
       ltAdc0->State = rbRun;
     } else if(dig == SENS_STOPED) {
       ltAdc0->State = rbStop;
     } else {
       ltAdc0->State = rbError;
     }

    dig = status.sensor_error.sens1;
    if (dig == SENS_SLEEP) {
	ltAdc1->State = rbSleep;
    } else if(dig == SENS_RUN){
       ltAdc1->State = rbRun;
     } else if(dig == SENS_STOPED) {
       ltAdc1->State = rbStop;
     } else {
       ltAdc1->State = rbError;
     }

    dig = status.sensor_error.sens2;
    if (dig == SENS_SLEEP) {
	ltAdc2->State = rbSleep;
    } else if(dig == SENS_RUN){
       ltAdc2->State = rbRun;
     } else if(dig == SENS_STOPED) {
       ltAdc2->State = rbStop;
     } else {
       ltAdc2->State = rbError;
     }

    dig = status.sensor_error.sens3;
    if (dig == SENS_SLEEP) {
	ltAdc3->State = rbSleep;
    } else if(dig == SENS_RUN){
       ltAdc3->State = rbRun;
     } else if(dig == SENS_STOPED) {
       ltAdc3->State = rbStop;
     } else {
       ltAdc3->State = rbError;
     }

    dig = status.sensor_error.sens4;
    if (dig == SENS_SLEEP) {
	ltAdc4->State = rbSleep;
    } else if(dig == SENS_RUN){
       ltAdc4->State = rbRun;
     } else if(dig == SENS_STOPED) {
       ltAdc4->State = rbStop;
     } else {
       ltAdc4->State = rbError;
     }

    dig = status.sensor_error.sens5;
    if (dig == SENS_SLEEP) {
	ltAdc5->State = rbSleep;
    } else if(dig == SENS_RUN){
       ltAdc5->State = rbRun;
     } else if(dig == SENS_STOPED) {
       ltAdc5->State = rbStop;
     } else {
       ltAdc5->State = rbError;
     }

    dig = status.sensor_error.sens6;
    if (dig == SENS_SLEEP) {
	ltAdc6->State = rbSleep;
    } else if(dig == SENS_RUN){
       ltAdc6->State = rbRun;
     } else if(dig == SENS_STOPED) {
       ltAdc6->State = rbStop;
     } else {
       ltAdc6->State = rbError;
     }

    dig = status.sensor_error.sens7;
    if (dig == SENS_SLEEP) {
	ltAdc7->State = rbSleep;
    } else if(dig == SENS_RUN){
       ltAdc7->State = rbRun;
     } else if(dig == SENS_STOPED) {
       ltAdc7->State = rbStop;
     } else {
       ltAdc7->State = rbError;
     }
}

//---------------------------------------------------------------------------
// Какая то фоновая работа
void __fastcall TformMain::timIdleTimer(TObject * Sender)
{
     tick++;

    // Раз в секунду время
    if (tick % 10 == 0) {
	char str[128];
	time_t t;
	t = time(NULL);
	sec_to_str(t, str);
	lbUTCTime->Caption = str;
    }
}

//---------------------------------------------------------------------------
// Вывести щетчики обмена
void __fastcall TformMain::CMShowCounts(TMessage & Message)
{
     // счетчики PC
    pnPsRx->Caption = IntToStr(cnt0.rx_pack);
    pnPsTx->Caption = IntToStr(cnt0.tx_pack);
    pnPsRxCmdErr->Caption = IntToStr(cnt0.rx_cmd_err + cnt0.rx_stat_err + cnt0.rx_crc_err + cnt0.tx_err);

    // счетчики GS
    pnGsRx->Caption = IntToStr(cnt1.rx_pack);
    pnGsTx->Caption = IntToStr(cnt1.tx_pack);
    pnGsRxCmdErr->Caption = IntToStr(cnt1.rx_cmd_err + cnt1.rx_stat_err +cnt1.rx_crc_err +cnt1.tx_err);
}
//---------------------------------------------------------------------------
// Включить/выключить насос
void __fastcall TformMain::btnPumpOnClick(TObject *Sender)
{
    if(status.self_test1 & 0x01)// включен - выключить
      writeThread = new WriteThread(PumpOff, NULL, true);
    else
      writeThread = new WriteThread(PumpOn, NULL, true);

    writeThread->FreeOnTerminate = true;
    writeThread->Resume();

}
//---------------------------------------------------------------------------

void __fastcall TformMain::btnSyncClick(TObject *Sender)
{
   t0 = time(NULL);
   writeThread = new WriteThread(TimeSync, &t0, true);
   writeThread->FreeOnTerminate = true;
   writeThread->Resume();
}
//---------------------------------------------------------------------------
// Старт
void __fastcall TformMain::btnStartThreadClick(TObject *Sender)
{
    writeThread = new WriteThread(StartDevice, NULL, true);
    writeThread->FreeOnTerminate = true;
    writeThread->Resume();
}
//---------------------------------------------------------------------------
// Стоп
void __fastcall TformMain::btnStopClick(TObject *Sender)
{
    writeThread = new WriteThread(StopDevice, NULL, true);
    writeThread->FreeOnTerminate = true;
    writeThread->Resume();
    DisableStation();
}
//---------------------------------------------------------------------------
// Стереть показания
void __fastcall TformMain::DisableStation(void)
{
    pnRaw1->Caption = "";
    pnRaw2->Caption = "";
    pnRaw3->Caption = "";
    pnRaw4->Caption = "";
    pnRaw5->Caption = "";
    pnRaw6->Caption = "";
    pnRaw7->Caption = "";
    pnRaw8->Caption = "";

    pnConc1->Caption = "";
    pnConc2->Caption = "";
    pnConc3->Caption = "";
    pnConc4->Caption = "";
    pnConc5->Caption = "";
    pnConc6->Caption = "";
    pnConc7->Caption = "";
    pnConc8->Caption = "";
}
//---------------------------------------------------------------------------
// Настройки EEPROM
void __fastcall TformMain::btnConstClick(TObject *Sender)
{
        bGetAllEeprom = true;
        frmEepromParam->ShowModal();
}
//---------------------------------------------------------------------------
// Константы получить
void __fastcall TformMain::GetEepromParams(void)
{
    writeThread = new WriteThread(ConstGet, pack, true);
    writeThread->FreeOnTerminate = true;
    writeThread->Resume();
}
//---------------------------------------------------------------------------
// Выводим, в зависимости от частоты!
void __fastcall TformMain::CMReadData(TMessage & Message)
{
    int sec;
    u8  temp;
    int i, mv;
    FLT_2_INT zero, gain;

    double freq_s = 100.0; // всегда 100

    slsDisplay->Title->Text = "ADC ";

    sec = aquis.time;
    for (i = 0; i < NUM_AQUIS_IN_PACK; i++) {
	t[i] = sec % 86400 + i;;
        x0[i] = (float)aquis.channels[i].chan0.f_val;
        x1[i] = (float)aquis.channels[i].chan1.f_val;
#if 0
        x2[i] = (u16)pack->channels[i].chan2 - this->pack[2].payload.sens.zero;
        x3[i] = (u16)pack->channels[i].chan3 - this->pack[3].payload.sens.zero;
        x4[i] = (u16)pack->channels[i].chan4 - this->pack[4].payload.sens.zero;
        x5[i] = (u16)pack->channels[i].chan5 - this->pack[5].payload.sens.zero;
        x6[i] = (u16)pack->channels[i].chan6 - this->pack[6].payload.sens.zero;
        x7[i] = (u16)pack->channels[i].chan7 - this->pack[7].payload.sens.zero;
#endif
    }

    slsDisplay->Channels->Items[0]->Data->AddXYData(t, x0, NUM_AQUIS_IN_PACK);


    // температура
    temp = (this->pack[0].payload.sens.reg_set[MODECN_REG] & 0x07);
    if(temp >= 6)
    {
            mv = (u16)aquis.channels[0].chan0.u_val;
            mv = mv * 3000 / 0x1000;
    } else {
#if 0
            // минус смещение mv
            zero.u_val = this->pack[0].payload.sens.zero;
            gain.u_val = this->pack[0].payload.sens.gain;

            mv = x0[0] - zero.f_val;
            pnRaw1->Caption = IntToStr(mv) + " mV";
            mv *= gain.f_val;
            pnConc1->Caption = IntToStr(mv);
#endif

            pnRaw1->Caption = FloatToStrF(aquis.channels[0].chan0.f_val, ffFixed, 3,2);
            pnRaw2->Caption = FloatToStrF(aquis.channels[0].chan1.f_val, ffFixed, 3,2);

            pnRaw3->Caption = FloatToStrF(aquis.channels[0].chan2.f_val, ffFixed, 3,2) + " %";
            pnRaw4->Caption = FloatToStrF(aquis.channels[0].chan3.f_val, ffFixed, 3,2) + " %";
            pnRaw5->Caption = FloatToStrF(aquis.channels[0].chan4.f_val, ffFixed, 3,2) + " %";
            pnRaw6->Caption = FloatToStrF(aquis.channels[0].chan5.f_val, ffFixed, 3,2) + " %";
            pnRaw7->Caption = FloatToStrF(aquis.channels[0].chan6.f_val, ffFixed, 3,2) + " %";
            pnRaw8->Caption = FloatToStrF(aquis.channels[0].chan7.f_val, ffFixed, 3,2) + " %";

    }


}
//---------------------------------------------------------------------------
void __fastcall TformMain::slsDisplayXAxisCustomLabel(TObject *Sender,
      Real SampleValue, AnsiString &AxisLabel)
{
    char str[32];
    int i;
    i = SampleValue;

    sec_to_simple_str(i, str);
    AxisLabel = str;
}
//---------------------------------------------------------------------------
// О программе
void __fastcall TformMain::btnSetNameClick(TObject *Sender)
{
    frmAbout->ShowModal();
}
//---------------------------------------------------------------------------

