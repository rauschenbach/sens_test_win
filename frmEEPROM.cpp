//---------------------------------------------------------------------------
#include <stdio.h>
#include <vcl.h>
#pragma hdrstop
#include "frmMain.h"
#include "Utils.h"
#include "frmEeprom.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BMSpinEdit"
#pragma resource "*.dfm"
TfrmEepromParam *frmEepromParam;
//---------------------------------------------------------------------------
__fastcall TfrmEepromParam::TfrmEepromParam(TComponent * Owner)
:TForm(Owner)
{
}

//---------------------------------------------------------------------------
//Показать принятые параметры
void __fastcall TfrmEepromParam::ShowEepromParam(int num)
{
    FLT_2_INT val;
    u8 temp, temp0;
    if (num < 8) {
        char str[128];
        int temp;
	temp = formMain->pack[num].payload.sens.sens_type;
	cbType->ItemIndex = (temp > 2) ? 0 : temp;

        val.u_val = formMain->pack[num].payload.sens.gain;
	seGain->Value = val.f_val;

        val.u_val = formMain->pack[num].payload.sens.zero       ;
	seZero->Value = val.f_val;

        // регистр TIACN
        sprintf(str, "Настройки регистра TIACN: %02X",formMain->pack[num].payload.sens.reg_set[TIACN_REG]);
        cbTiaGain->ItemIndex = (formMain->pack[num].payload.sens.reg_set[TIACN_REG] >> 2) & 0x07;
        cbRload->ItemIndex = formMain->pack[num].payload.sens.reg_set[TIACN_REG] & 0x03;
        gbTiaCn->Caption = str;


        // Регистра REFCN
        sprintf(str, "Настройки регистра REFCN: %02X",formMain->pack[num].payload.sens.reg_set[REFCN_REG]);
        cbRefSource->ItemIndex = (formMain->pack[num].payload.sens.reg_set[REFCN_REG] >> 7) & 0x01;
        cbIntZ->ItemIndex = (formMain->pack[num].payload.sens.reg_set[REFCN_REG] >> 5) & 0x03;
        cbBiasSign->ItemIndex = (formMain->pack[num].payload.sens.reg_set[REFCN_REG] >> 4) & 0x01;
        temp = formMain->pack[num].payload.sens.reg_set[REFCN_REG] & 0x0f;
        cbBias->ItemIndex = (temp > 0x0d)?   0 : temp;
        cbFetShort->ItemIndex = (formMain->pack[num].payload.sens.reg_set[MODECN_REG] >> 7) & 0x01;
        gbRefCn->Caption = str;

        // Регистр MODECN
        sprintf(str, "Настройки регистра MODECN: %02X",formMain->pack[num].payload.sens.reg_set[MODECN_REG]);
        temp0 = (formMain->pack[num].payload.sens.reg_set[MODECN_REG]) & 0x07;
        if(temp0 == 7 || temp0 == 6) {
         temp0 -= 2;
        }
        cbOpMode->ItemIndex = temp0;
        gbModeCn->Caption = str;

	memcpy(str, formMain->pack[num].payload.sens.meas_gas, 32);
	edAgent->Text = koi8_to_win(str);
        udChanNum->Position = num;
    }
}

//---------------------------------------------------------------------------
void __fastcall TfrmEepromParam::btnReadClick(TObject * Sender)
{
    formMain->bGetAllEeprom = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmEepromParam::udChanNumClick(TObject *Sender, TUDBtnType Button)
{
        ShowEepromParam(udChanNum->Position);
}
//---------------------------------------------------------------------------
// Запись параметров EEPROM - последовательно от 0 до 7
// Записываем в pack и передаем COM порту на запись
void __fastcall TfrmEepromParam::btnWriteClick(TObject *Sender)
{
    int num;
    u8 temp, temp0;
    FLT_2_INT val;
    char str[32];


    // Номер ячейки EERPOM
    num = udChanNum->Position;

    if (num < 8) {
        // Тип сенсора
        temp = cbType->ItemIndex;
	formMain->pack[num].payload.sens.sens_type = temp;

        // Цена мл. разряда - храним побитно плавающее число
        val.f_val = seGain->Value;
       	formMain->pack[num].payload.sens.gain = val.u_val;

        // смещение нуля
        val.f_val = seZero->Value;
       	formMain->pack[num].payload.sens.zero = val.u_val;

        // Измеряемый газ
        edAgent->GetTextBuf(str, 31);
        str[31] = 0;
	strcpy(formMain->pack[num].payload.sens.meas_gas, str);

        // регистры
        // tiacn
        temp = 0;
        temp |= ((cbTiaGain->ItemIndex << 2) | (cbRload->ItemIndex & 0x03));
        formMain->pack[num].payload.sens.reg_set[TIACN_REG] = temp;

        // refcn
        temp = 0;
        temp |= ((cbRefSource->ItemIndex & 1) << 7) | ((cbIntZ->ItemIndex & 0x03) << 5) |
        ((cbBiasSign->ItemIndex  & 0x01) << 4) | (cbBias->ItemIndex & 0xf);
        formMain->pack[num].payload.sens.reg_set[REFCN_REG] = temp;

        // modecn
        temp = cbFetShort->ItemIndex << 7;
        temp0 = cbOpMode->ItemIndex;
        if(temp0 == 4 || temp == 5) {
         temp0 += 2;
        }

        temp =  temp | temp0;
        formMain->pack[num].payload.sens.reg_set[MODECN_REG] = temp;

        // и номер канала для модуля proto.cpp
	formMain->pack[num].payload.sens.reg_set[19] = num * 0x20;
//        formMain->iSetAllEeprom = num;
    }

}
//---------------------------------------------------------------------------
void __fastcall TfrmEepromParam::btnStartClick(TObject *Sender)
{
        formMain->btnStartThreadClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TfrmEepromParam::btnStopClick(TObject *Sender)
{
      formMain->btnStopClick(this);
}
//---------------------------------------------------------------------------

