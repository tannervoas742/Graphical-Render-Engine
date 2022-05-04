#ifndef _GRE_MODULE_INTERFACE_H_
#define _GRE_MODULE_INTERFACE_H_

#ifdef GRAPHICALRENDERENGINE_EXPORTS
// Do nothing if in DLL
#else

#include "GREGlobals.h"
#include <windows.h>
#include <libloaderapi.h>

typedef void (*VOID_FUNC_VOIDptr) (void*);
typedef GRESettings* (*GRESETTINGSptr_FUNC_VOID) (void);

namespace GRE
{
    extern "C" VOID_FUNC_VOIDptr LogGlobalSettings = NULL;
    extern "C" GRESETTINGSptr_FUNC_VOID GetGlobalSettings = NULL;

    bool LoadModule(bool LoadFlag)
    {
        if (LoadFlag) {
            COUT("WARNING: GRE Module already loaded");
        }

        COUT("Loading GRE Module");

        #define ERROR_CALL(HNDL, MSG) if (!HNDL) { COUT(MSG); goto exit_with_error; }

        HMODULE Status         =                           LoadLibraryA("GraphicalRenderEngine.dll");
        ERROR_CALL(Status,                "Load Failure: GraphicalRenderEngine.dll");
        GRE::LogGlobalSettings = (VOID_FUNC_VOIDptr)       GetProcAddress(GetModuleHandle(TEXT("GraphicalRenderEngine.dll")), "LogGREGlobalSettings");
        ERROR_CALL(GRE::LogGlobalSettings, "Load Failure: LogGREGlobalSettings");
        GRE::GetGlobalSettings = (GRESETTINGSptr_FUNC_VOID) GetProcAddress(GetModuleHandle(TEXT("GraphicalRenderEngine.dll")), "GetGREGlobalSettings");
        ERROR_CALL(GRE::GetGlobalSettings, "Load Failure: GetGREGlobalSettings");

        
    exit:
        COUT("Loaded GRE Module");
        #undef ERROR_CALL
        return true;
    exit_with_error:
        #undef ERROR_CALL
        return false;
    }

    void UnloadModule(bool LoadFlag)
    {
        if (!LoadFlag) {
            COUT("WARNING: GRE Module not loaded before calling unload module");
        }

        COUT("Unloading GRE Module");

        FreeLibrary(GetModuleHandle(TEXT("GraphicalRenderEngine.dll")));
        GRE::LogGlobalSettings = NULL;
        GRE::GetGlobalSettings = NULL;

        COUT("Unloaded GRE Module");
    }
}

#endif

#endif