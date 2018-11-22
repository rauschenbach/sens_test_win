//---------------------------------------------------------------------------

#ifndef frmMainH
#define frmMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "LPComponent.h"
#include "LPDrawLayers.h"
#include "SLComponentCollection.h"
#include "SLScope.h"
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <ToolWin.hpp>
#include <ImgList.hpp>
#include <Menus.hpp>
#include "ComPort.h"
#include "proto.h"
#include "globdefs.h"
#include "sensor.h"
#include "comm.h"
#include "PortThread.h"
#include "WriteThread.h"

#define CM_UPDATE_STATUS        WM_USER+401
#define CM_READ_DATA            WM_USER+402
#define CM_SHOW_COUNTS          WM_USER+403


typedef int (*pfn)(struct tm*, const long*);

//---------------------------------------------------------------------------
class TformMain : public TForm
{
__published:	// IDE-managed Components
        TToolBar *tbButtons;
        TToolButton *btnReset;
        TToolButton *btnPowerOff;
        TToolButton *ToolButton5;
        TToolButton *btnTest;
        TToolButton *btnExit;
        TToolButton *ToolButton6;
        TToolButton *btnStartThread;
        TToolButton *btnSync;
        TToolButton *btnConst;
        TToolButton *btnSetName;
        TPanel *pnUp;
        TGroupBox *gbTimes;
        TPanel *pnUTCTime;
        TPanel *pnTimeGNS110;
        TPanel *pnGpsTime;
        TPanel *lbGpsTime;
        TPanel *lbTimeGS;
        TPanel *lbUTCTime;
        TGroupBox *gbParams;
        TSpeedButton *sbRunModule;
        TListBox *cbPortName;
        TPanel *pnConfig1;
        TGroupBox *gbCounts;
        TPanel *pnGnsTotalRx;
        TPanel *pnGnsTotalTx;
        TPanel *pnGsTx;
        TPanel *pnGnsCmdError;
        TPanel *pnGsRxCmdErr;
        TPanel *pnGsRx;
        TGroupBox *gbMyCounts;
        TPanel *pnPcTotalRx;
        TPanel *pnPcTotalTx;
        TPanel *pnPsTx;
        TPanel *pnPcCmdError;
        TPanel *pnPsRxCmdErr;
        TPanel *pnPsRx;
        TGroupBox *gbEnviron;
        TPanel *pnStatus;
        TGroupBox *gbStatus1;
        TLabel *lbStatus1_0;
        TLabel *lbStatus1_1;
        TLabel *lbStatus1_2;
        TLabel *lbStatus1_3;
        TLabel *lbStatus1_4;
        TLabel *lbStatus1_5;
        TLabel *lbStatus1_6;
        TLabel *lbStatus1_7;
        TGroupBox *gbStatus0;
        TLabel *lbStatus0_3;
        TLabel *lbStatus0_4;
        TLabel *lbStatus0_5;
        TLabel *lbStatus0_6;
        TLabel *lbStatus0_7;
        TLabel *lbStatus0_1;
        TPanel *pnStatusAndErrors;
        TGroupBox *gbResetCause;
        TLabel *lbStatusReset_0;
        TLabel *lbStatusReset_1;
        TLabel *lbStatusReset_2;
        TLabel *lbStatusReset_3;
        TLabel *lbStatusReset_4;
        TLabel *lbStatusReset_5;
        TLabel *Label17;
        TLabel *Label18;
        TGroupBox *gbStatus2;
        TLabel *lbStatus2_7;
        TLabel *lbStatus2_4;
        TLabel *lbStatus2_5;
        TLabel *lbStatus2_0;
        TLabel *lbStatus2_1;
        TLabel *lbStatus2_2;
        TLabel *lbStatus2_3;
        TGroupBox *gbAdc;
        TPanel *pnAdcNum;
        TPanel *Panel3;
        TPanel *Panel6;
        TPanel *Panel9;
        TPanel *Panel11;
        TPanel *Panel13;
        TPanel *Panel14;
        TPanel *Panel15;
        TPanel *Panel16;
        TPanel *pnAdcStatus;
        TStatusBar *sbInfo;
        TImageList *imgList;
        TToolButton *btnStop;
        TMemo *memoLog;
        TLabel *lbStatus0_0;
        TLabel *lbStatus0_2;
        TLabel *lbStatus2_6;
        TTimer *timIdle;
        TToolButton *btnPumpOn;
        TToolButton *btnPumpOff;
        TToolButton *ToolButton1;
        TPanel *Panel4;
        TPanel *Panel5;
        TPanel *Panel7;
        TPanel *Panel8;
        TPanel *Panel10;
        TPanel *Panel12;
        TPanel *Panel17;
        TPanel *Panel18;
        TPanel *Panel19;
        TPanel *Panel20;
        TSLScope *slsDisplay;
        TProgressBar *pbLoad;
        TSensor *ltAdc1;
        TSensor *ltAdc2;
        TSensor *ltAdc3;
        TSensor *ltAdc4;
        TSensor *ltAdc5;
        TSensor *ltAdc6;
        TSensor *ltAdc7;
        TSensor *ltAdc0;
        TGroupBox *GroupBox1;
        TPanel *Panel21;
        TPanel *Panel23;
        TPanel *Panel24;
        TPanel *Panel25;
        TPanel *pnStuff1;
        TPanel *pnStuff2;
        TPanel *pnStuff4;
        TPanel *Panel26;
        TPanel *Panel34;
        TPanel *Panel35;
        TPanel *Panel36;
        TPanel *Panel37;
        TPanel *Panel38;
        TPanel *pnStuff5;
        TPanel *pnStuff6;
        TPanel *pnStuff7;
        TPanel *pnStuff8;
        TPanel *pnStuff3;
        TPanel *Panel27;
        TPanel *pnRaw1;
        TPanel *pnRaw2;
        TPanel *pnRaw3;
        TPanel *pnRaw4;
        TPanel *pnRaw5;
        TPanel *pnRaw6;
        TPanel *pnRaw7;
        TPanel *pnRaw8;
        TPanel *Panel45;
        TPanel *pnConc1;
        TPanel *pnConc2;
        TPanel *pnConc3;
        TPanel *pnConc4;
        TPanel *pnConc5;
        TPanel *pnConc6;
        TPanel *pnConc7;
        TPanel *pnConc8;
        TComm *comPort;
        TPanel *Panel30;
        TPanel *Panel32;
        TPanel *Panel33;
        TPanel *Panel39;
        TPanel *Panel40;
        TPanel *Panel41;
        TPanel *Panel43;
        TPanel *Panel44;
        TPanel *Panel46;
        TPanel *Panel42;
        TPanel *Panel31;
        TPanel *Panel29;
        TPanel *pnTemp0;
        TPanel *pnPressure;
        TPanel *pnHumid;
        void __fastcall btnExitClick(TObject *Sender);
        void __fastcall menuExitClick(TObject *Sender);
        void __fastcall FormActivate(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall timIdleTimer(TObject *Sender);
        void __fastcall btnPumpOnClick(TObject *Sender);
        void __fastcall btnSyncClick(TObject *Sender);
        void __fastcall btnStartThreadClick(TObject *Sender);
        void __fastcall btnStopClick(TObject *Sender);
        void __fastcall btnConstClick(TObject *Sender);
        void __fastcall slsDisplayXAxisCustomLabel(TObject *Sender,
          Real SampleValue, AnsiString &AxisLabel);
        void __fastcall btnSetNameClick(TObject *Sender);
public:
        DEV_ADDR_PACK     dev_addr;
        DEV_STATUS_PACK   status;
        UART_COUNTS_PACK cnt0, cnt1;
        EEPROM_VALUE_PACK pack[8]; // 8 значений сразу
        SENSOR_DATA_PACK  aquis;

        bool __fastcall EndThread(void);
        void __fastcall EnableStation(int, int);
        void __fastcall DisableStation(void);
        void __fastcall GetEepromParams(void);

        bool bGetAllEeprom;        
protected:
        double x0[10];
        double x1[10];
        double x2[10];
        double x3[10];
        double x4[10];
        double x5[10];
        double x6[10];
        double x7[10];
        double t[10];


        int tick;
        time_t t0;
private:	// User declarations
        bool bEndThread;

        void __fastcall   CMUpdateStatus(TMessage &Message);
        void __fastcall   CMReadData(TMessage &Message);
        void __fastcall   CMShowCounts(TMessage &Message);
        PortThread *portThread;
        WriteThread* writeThread;
public:		// User declarations
        __fastcall TformMain(TComponent* Owner);
        int __fastcall GetTimerTick(void) { return tick;}
       	HINSTANCE   LibHandle;
        pfn gmtime32_s;
BEGIN_MESSAGE_MAP
  MESSAGE_HANDLER(CM_UPDATE_STATUS, TMessage, CMUpdateStatus)
  MESSAGE_HANDLER(CM_READ_DATA,  TMessage, CMReadData)
  MESSAGE_HANDLER(CM_SHOW_COUNTS,  TMessage, CMShowCounts)
END_MESSAGE_MAP(TControl)
};
//---------------------------------------------------------------------------
extern PACKAGE TformMain *formMain;
//---------------------------------------------------------------------------
#endif

