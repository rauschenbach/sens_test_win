//---------------------------------------------------------------------------

#ifndef WriteThreadH
#define WriteThreadH
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
class WriteThread : public TThread
{            
private:
      void* param;
      bool  app;
      void (*thread_func) (void*);	        /* ��������� �� �������, ������� ����� �������� �� ������ */
protected:
        void __fastcall Execute();
public:
        __fastcall WriteThread(void *func, void* p, bool CreateSuspended);
};
//---------------------------------------------------------------------------
#endif
