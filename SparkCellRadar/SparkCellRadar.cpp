#include "Helpers/initpdk.h"
#include <gauges.h>
#include "RadarPanelCallback.h"

static const char RADAR_CALLBACK_NAME[] = "SparkCellRadar";

GAUGE_CALLBACK gauge_callback;

void RadarPanelCallbackInit();
void RadarPanelCallbackDeInit();

PPANELS Panels = NULL;

GAUGESIMPORT    ImportTable =
{
    { 0x0000000F, (PPANELS)NULL },
    { 0x00000000, NULL }
};

void FSAPI  module_init(__in __notnull P3D::IPdk* pPdk)
{
    P3D::PdkServices::Init(pPdk);

    if (P3D::PdkServices::GetPanelSystem())
    {
        ImportTable.PANELSentry.fnptr = (PPANELS)Panels;
        RadarPanelCallbackInit();
    }
}

void FSAPI  module_deinit(void) {
    RadarPanelCallbackDeInit();
    P3D::PdkServices::Shutdown();
}

BOOL WINAPI DllMain(HINSTANCE hDLL, DWORD dwReason, LPVOID lpReserved)
{
    return TRUE;
}

// This is the module's export table.
GAUGESLINKAGE   Linkage =
{
    0x00000013,
    0,
    0,
    0,
    0,

    FS9LINK_VERSION,{ 0 }
};

void RadarPanelCallbackInit()
{
    auto pRadarPanelCallback = new RadarPanelCallback();

    if (pRadarPanelCallback)
    {
        auto b = P3D::PdkServices::GetPanelSystem()->PanelRegisterCCallback(RADAR_CALLBACK_NAME, pRadarPanelCallback);
        pRadarPanelCallback->Release();
    }
}

void RadarPanelCallbackDeInit()
{
    P3D::PdkServices::GetPanelSystem()->PanelRegisterCCallback(RADAR_CALLBACK_NAME, nullptr);
}
