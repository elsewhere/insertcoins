#ifndef _EXTERNS_HPP_
#define _EXTERNS_HPP_

//--------------------------------------------------------------------------------------------
//  Headers
//--------------------------------------------------------------------------------------------

#include "lib/opengl.hpp"
//#include "lib/log.hpp"
//#include "lib/timer.hpp"
//#include "lib/sound.hpp"
#include "lib/font.hpp"
#include "lib/texturemanager.hpp"
#include "lib/objectmanager.hpp"
#include "lib/effectmanager.hpp"
#include "lib/beziercurve.hpp"
#include "lib/hermitecurve.hpp"
#include "lib/path.hpp"
#include "lib/t3d.hpp"

//--------------------------------------------------------------------------------------------
//  Link needed libraries
//--------------------------------------------------------------------------------------------

#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "lib/demotractor.lib")
#pragma comment(lib, "lib/opengl32.lib")
#pragma comment(lib, "lib/glu32.lib")
#pragma comment(lib, "lib/fmodvc.lib")
#pragma comment(lib, "lib/jpeg.lib")
//#pragma comment(lib, "lib/libpng.lib")
//#pragma comment(lib, "lib/libz.lib")

//--------------------------------------------------------------------------------------------
//  Use our library namespace: TRACTION_DEMOTRACTOR
//--------------------------------------------------------------------------------------------

using namespace TRACTION_DEMOTRACTOR;

//--------------------------------------------------------------------------------------------
//  External Traction DEMOTRACTOR API functions
//--------------------------------------------------------------------------------------------

extern "C" __declspec(dllexport) void dmsSetLogging(bool f);
extern "C" __declspec(dllexport) void dmsMsg(const char *text, ...);
extern "C" __declspec(dllexport) bool dmsInit();
extern "C" __declspec(dllexport) bool dmsInitEx(int screenX, int screenY, int bpp, bool fullscreen, bool soundFlag, bool vsync, char *pakFile);
extern "C" __declspec(dllexport) bool dmsInitDummyEx(int screenX, int screenY, int bpp, bool fullscreen, bool soundFlag, bool vsync);
extern "C" __declspec(dllexport) bool dmsOnFocus();
extern "C" __declspec(dllexport) void dmsSetGamma(int gamma);
extern "C" __declspec(dllexport) void dmsShutDown();
extern "C" __declspec(dllexport) void dmsCheckControls(float jump);
extern "C" __declspec(dllexport) void dmsPerspective2D(int w, int h);
extern "C" __declspec(dllexport) void dmsPerspective3D();
extern "C" __declspec(dllexport) void dmsSetFOV(float f);

extern "C" __declspec(dllexport) void dmsInitTimer();
extern "C" __declspec(dllexport) void dmsUpdateTimer();
extern "C" __declspec(dllexport) void dmsAdjustTime(float f);

extern "C" __declspec(dllexport) void dmsInitSound();
extern "C" __declspec(dllexport) void dmsLoadSong(const char *fileName);
extern "C" __declspec(dllexport) void dmsPlaySong(bool f);
extern "C" __declspec(dllexport) void dmsPauseSong(bool f);
extern "C" __declspec(dllexport) bool dmsCheckSyncPoint();

extern "C" __declspec(dllexport) void dmsAddTexture(const char *fileName, const char *name);
extern "C" __declspec(dllexport) void dmsAddRenderToTexture(int width, int height, const char *name);
extern "C" __declspec(dllexport) void dmsUploadTextures();
extern "C" __declspec(dllexport) void dmsBindTexture(unsigned int mode, const char *name);

extern "C" __declspec(dllexport) void dmsAddFont(int x, int y, const char *name);

extern "C" __declspec(dllexport) void dmsInitObjects();
extern "C" __declspec(dllexport) void dmsAddObject(const char *fileName, const char *name);
extern "C" __declspec(dllexport) void dmsDrawObject(const char *name);

extern "C" __declspec(dllexport) void dmsInitEffects();
extern "C" __declspec(dllexport) void dmsAddEffect(unsigned long st, unsigned long et, unsigned int prio, const char *name, Base *effect);
extern "C" __declspec(dllexport) void dmsSetEffectPriority(unsigned int prio, const char *name);

extern "C" __declspec(dllexport) void dmsRun();

extern "C" __declspec(dllexport) void dmsWrite2D(float x, float y, float a, float scale, char *fontName, char *msg);
extern "C" __declspec(dllexport) void dmsWrite3D(float x, float y, float z, float a, char *fontName, char *msg);
extern "C" __declspec(dllexport) void dmsSetWindowTitle(const char *msg);
extern "C" __declspec(dllexport) void dmsDrawQuad2D();
extern "C" __declspec(dllexport) void dmsDrawQuad2DEx(Vector p1, Vector p2, Vector p3, Vector p4);
extern "C" __declspec(dllexport) void dmsDrawTexturedQuad2D();
extern "C" __declspec(dllexport) void dmsDrawTexturedQuad2DEx(Vector p1, Vector p2, Vector p3, Vector p4);
extern "C" __declspec(dllexport) void dmsBlending(bool blending, unsigned int src, unsigned int dst);

extern "C" __declspec(dllexport) float dmsGetTime();
extern "C" __declspec(dllexport) float dmsGetDeltaTime();
extern "C" __declspec(dllexport) float *dmsGetSpectrum();
extern "C" __declspec(dllexport) T3D *dmsGetObject(const char *name);
extern "C" __declspec(dllexport) Texture *dmsGetTexture(const char *name);
extern "C" __declspec(dllexport) Font *dmsGetFont(const char *name);
extern "C" __declspec(dllexport) HDC dmsGetHDC();
extern "C" __declspec(dllexport) int dmsGetWindowWidth();
extern "C" __declspec(dllexport) int dmsGetWindowHeight();

extern "C" __declspec(dllexport) int dmsCalcPos(float value, float limit1, float limit2);
extern "C" __declspec(dllexport) float dmsCalcSaturate(float value, float limit1, float limit2, float multiply);
extern "C" __declspec(dllexport) float dmsRandFloat();

extern "C" __declspec(dllexport) void myEnable(int c);
extern "C" __declspec(dllexport) void myDisable(int c);
extern "C" __declspec(dllexport) void myBlendFunc(int src, int dst);
extern "C" __declspec(dllexport) bool dmsMakePAK(const char *src, const char *dst);

#endif