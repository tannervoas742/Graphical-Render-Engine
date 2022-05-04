#ifndef _GRE_GLOBAL_H_
#define _GRE_GLOBAL_H_

#ifdef GRAPHICALRENDERENGINE_EXPORTS
#define DLLAPI __declspec(dllexport)
#else
#define DLLAPI __declspec(dllimport)
#endif

#include <iostream>
#include <format>
#include <string>

#ifndef _MODULE_DEFINES_
#define _MODULE_DEFINES_

#define _COUT_PREFACE std::format("[{:<" + std::to_string(GREGlobalSettings._LogModulePadding) + "}] [{:<" + std::to_string(GREGlobalSettings._LogFunctionPadding) + "}]: ", "GRE", __FUNCTION__)
#define COUT(MSG) std::cout << std::format("{}{}\n", _COUT_PREFACE, MSG)



#define EXPORT __declspec(dllexport)
#define IMPORT __declspec(dllimport)

#define MAJOR_VERSION 0
#define MINOR_VERSION 1
#define VERSION MAJOR_VERSION.MINOR_VERSION

#endif

class GRESettings
{
public:
	GRESettings(void) {}
	~GRESettings(void) {}

	bool RendererStarted = false;
	bool TerminateRenderer = false;

	unsigned int _LogModulePadding = 6;
	unsigned int _LogFunctionPadding = 25;
};

extern "C" GRESettings GREGlobalSettings;

extern "C" DLLAPI GRESettings * GetGREGlobalSettings(void);
extern "C" DLLAPI void LogGREGlobalSettings(void* Stream);

#endif