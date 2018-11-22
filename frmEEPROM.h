//---------------------------------------------------------------------------

#ifndef frmEepromH
#define frmEepromH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "BMSpinEdit.h"
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmEepromParam : public TForm
{
__published:	// IDE-managed Components
        TGroupBox *gbChanName;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label3;
        TLabel *Label7;
        TComboBox *cbType;
        TBMSpinEdit *seGain;
        TBMSpinEdit *seZero;
        TEdit *edAgent;
        TGroupBox *gbTiaCn;
        TLabel *Label5;
        TLabel *Label6;
        TComboBox *cbTiaGain;
        TComboBox *cbRload;
        TBitBtn *btnRead;
        TBitBtn *btnWrite;
        TGroupBox *gbRefCn;
        TLabel *Label8;
        TLabel *Label9;
        TComboBox *cbRefSource;
        TComboBox *cbIntZ;
        TLabel *Label10;
        TLabel *Label11;
        TComboBox *cbBiasSign;
        TComboBox *cbBias;
        TGroupBox *GroupBox3;
        TEdit *edChanNum;
        TUpDown *udChanNum;
        TGroupBox *gbModeCn;
        TLabel *Label4;
        TLabel *Label12;
        TComboBox *cbFetShort;
        TComboBox *cbOpMode;
        TBitBtn *btnStart;
        TBitBtn *btnStop;
        void __fastcall btnReadClick(TObject *Sender);
        void __fastcall udChanNumClick(TObject *Sender, TUDBtnType Button);
        void __fastcall btnWriteClick(TObject *Sender);
        void __fastcall btnStartClick(TObject *Sender);
        void __fastcall btnStopClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        void __fastcall ShowEepromParam(int num);
        __fastcall TfrmEepromParam(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmEepromParam *frmEepromParam;
//---------------------------------------------------------------------------
#endif
