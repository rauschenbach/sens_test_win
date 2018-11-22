//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "WriteThread.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall WriteThread::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall WriteThread::WriteThread(void *func, void* p, bool CreateSuspended)
        : TThread(CreateSuspended)
{
     param = p;
     thread_func = (void (*)(void*)) func;
}
//---------------------------------------------------------------------------
// Вызываем thread_func, если он установлена
void __fastcall WriteThread::Execute()
{
    if (thread_func != NULL)
	thread_func(param);
}
//---------------------------------------------------------------------------
