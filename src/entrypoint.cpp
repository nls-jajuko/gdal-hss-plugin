#include "cpl_port.h"
#include "gdal_hss_plugin.h"

extern "C" void CPL_DLL GDALRegisterMe();

void GDALRegisterMe()
{
    RegisterOGRHSS();
}
