//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("frmMain.cpp", formMain);
USEFORM("About.cpp", frmAbout);
USEFORM("frmEEPROM.cpp", frmEepromParam);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
  HANDLE hWnd = NULL;
  DecimalSeparator = '.';


  hWnd = FindWindow(NULL, "Gas Sensor");
  if(hWnd){
           ShowWindow(hWnd, SW_SHOWNORMAL);
           return 0;
  } else 
        try
        {
                 Application->Initialize();
                 Application->CreateForm(__classid(TformMain), &formMain);
                 Application->CreateForm(__classid(TfrmAbout), &frmAbout);
                 Application->CreateForm(__classid(TfrmEepromParam), &frmEepromParam);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        catch (...)
        {
                 try
                 {
                         throw Exception("");
                 }
                 catch (Exception &exception)
                 {
                         Application->ShowException(&exception);
                 }
        }
        return 0;
}
//---------------------------------------------------------------------------
