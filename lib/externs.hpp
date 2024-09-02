#ifndef _EXTERNS_HPP_
#define _EXTERNS_HPP_

//--------------------------------------------------------------------------------------------
//  Headers
//--------------------------------------------------------------------------------------------

#include "lib/opengl.hpp"
#include "lib/log.hpp"
#include "lib/timer.hpp"
#include "lib/sound.hpp"
#include "lib/font.hpp"
#include "lib/texturemanager.hpp"
#include "lib/effectmanager.hpp"

//--------------------------------------------------------------------------------------------
//  Link needed libraries
//--------------------------------------------------------------------------------------------

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "lib/demotractor.lib")
#pragma comment(lib, "lib/fmodvc.lib")
#pragma comment(lib, "lib/opengl32.lib")
#pragma comment(lib, "lib/glu32.lib")

//--------------------------------------------------------------------------------------------
//  Use our library namespace: TRACTION_DEMOTRACTOR
//--------------------------------------------------------------------------------------------

using namespace TRACTION_DEMOTRACTOR;

//--------------------------------------------------------------------------------------------
//  External Traction DEMOTRACTOR API functions
//--------------------------------------------------------------------------------------------

extern "C" __declspec(dllexport) void dmsMsg(const char *text, ...);
extern "C" __declspec(dllexport) bool dmsInit();
extern "C" __declspec(dllexport) bool dmsInitEx(int screenX, int screenY, int bpp, bool fullscreen, bool soundFlag, bool vsync);
extern "C" __declspec(dllexport) void dmsShutDown();
extern "C" __declspec(dllexport) void dmsAddTexture(const char *fileName, const char *name);
extern "C" __declspec(dllexport) void dmsUploadTextures();
extern "C" __declspec(dllexport) void dmsBindTexture(unsigned int mode, const char *name);

extern "C" __declspec(dllexport) void dmsWriteText(float x, float y, float z, float a, char *fontName, char *msg);

extern "C" __declspec(dllexport) void dmsEnable(int c);
extern "C" __declspec(dllexport) void dmsDisable(int c);
extern "C" __declspec(dllexport) void dmsBlendFunc(int src, int dst);

extern "C" __declspec(dllexport) Texture *dmsGetTexture(const char *name);
extern "C" __declspec(dllexport) Font *dmsGetFont(const char *name);
extern "C" __declspec(dllexport) HDC dmsGetHDC();

#endif