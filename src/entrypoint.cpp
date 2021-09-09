#include "cpl_port.h"
#include "extradrivers.h"

extern "C" void CPL_DLL GDALRegisterMe();

void GDALRegisterMe()
{
    RegisterOGRHSS();
}
