//---------------------------------------------------------------------------

#ifndef PortThreadH
#define PortThreadH
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
class PortThread : public TThread
{            
private:
protected:
        void __fastcall Execute();
public:
        __fastcall PortThread(bool CreateSuspended);
};
//---------------------------------------------------------------------------
#endif
