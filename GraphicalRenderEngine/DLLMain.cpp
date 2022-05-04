// dllmain.cpp : Defines the entry point for the DLL application.

#include "PCH.h"
#include "GREGlobals.h"
#include "Renderer.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        LaunchRenderer();
        return TRUE;
    case DLL_THREAD_ATTACH:
        return TRUE;
    case DLL_THREAD_DETACH:
        if (GREGlobalSettings.TerminateRenderer == true && GREGlobalSettings.RendererStarted == true) {
            COUT("Signaling renderer to halt due to thread detachment");
            TerminateRenderer();
        }
        return TRUE;
    case DLL_PROCESS_DETACH:
        TerminateRenderer();
        return TRUE;
    }
    return TRUE;
}

