#include "PCH.h"
#include "GREGlobals.h"

GRESettings GREGlobalSettings = GRESettings();

GRESettings* GetGREGlobalSettings(void)
{
	return &GREGlobalSettings;
}

DLLAPI void LogGREGlobalSettings(void* Stream)
{
	std::string FormatSize = std::to_string(6 + GREGlobalSettings._LogModulePadding + GREGlobalSettings._LogFunctionPadding);
	std::ostream* OStream = ((std::ostream*)Stream);
	std::string Outstring = std::format("{}", "GlobalSettings");
	Outstring += std::format("\n{:<" + FormatSize + "} - RendererStarted: {}", "", GREGlobalSettings.RendererStarted);
	Outstring += std::format("\n{:<" + FormatSize + "} - TerminateRenderer: {}", "", GREGlobalSettings.TerminateRenderer);
	COUT(Outstring);
}
