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
      void (*thread_func) (void*);	        /* ”казатель на функцыю, которую будем вызывать из потока */
protected:
        void __fastcall Execute();
public:
        __fastcall WriteThread(void *func, void* p, bool CreateSuspended);
};
//---------------------------------------------------------------------------
#endif
