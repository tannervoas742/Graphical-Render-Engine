#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "GREGlobals.h"
#include <iostream>
#include <windows.h>
#include <processthreadsapi.h>
#include "GREUtilities.h"
#include "OpenGLWindow.h"
//------- Ignore this ----------
#include<filesystem>
namespace fs = std::filesystem;
//------------------------------

#include"Model.h"

bool LaunchRenderer(void);
void TerminateRenderer(void);

#endif