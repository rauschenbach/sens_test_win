//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <stdio.h>
#include "About.h"
#include "frmMain.h"
//--------------------------------------------------------------------- 
#pragma resource "*.dfm"
TfrmAbout *frmAbout;
//--------------------------------------------------------------------- 
__fastcall TfrmAbout::TfrmAbout(TComponent* AOwner)
	: TForm(AOwner)
{
}
//---------------------------------------------------------------------


void __fastcall TfrmAbout::FormShow(TObject *Sender)
{
   // Выведем версию на форме
   char buf[128];
   char str[32];

   sprintf(str,"%s", __TIME__);
   str[5] = 0;
   sprintf(buf,"Compilation time:\t\t%s  %s", __DATE__, str);
   Comments->Caption = buf;
}
//---------------------------------------------------------------------------

