//---------------------------------------------------------------------------

#ifndef protoH
#define protoH

#include "globdefs.h"
#include "com_cmd.h"
#include "frmMain.h"
//---------------------------------------------------------------------------

bool PortInit(int, u32);
bool PortClose(void);
bool PortIsOpen(void);
bool PcCountsGet(void*);

bool WhoAreU(void *);
bool StatusGet(void *);
bool CountsGet(void *p);
bool PumpOn(void);
bool PumpOff(void);
bool TimeSync(void *);
bool StopDevice(void);
bool StartDevice(void);
bool ConstGet(void *, int);
bool DataGet(void *);
bool ConstSet(void*);

#endif
