//-------------------------------------------------------
//	Defines
//-------------------------------------------------------

// This extracts MFC stuff off
#define WIN32_LEAN_AND_MEAN
#define VC_LEANMEAN           

//-------------------------------------------------------
//	Headers
//-------------------------------------------------------

#include <windows.h>
#include <stdio.h>

#ifdef _DEBUG
	#include <stdlib.h>
	#include "mmgr.h"
#endif

#include "externs.hpp"
#include "config.hpp"

#include "effects/bands.hpp"
#include "effects/spark.hpp"
#include "effects/starfield.hpp"
#include "effects/rings.hpp"
#include "effects/wiretunnel.hpp"
#include "effects/tetris.hpp"
#include "effects/pong.hpp"
#include "effects/tunnel.hpp"
#include "effects/tron.hpp"
#include "effects/laby.hpp"
#include "effects/asteroids.hpp"
#include "effects/city.hpp"
#include "effects/alku.hpp"

using namespace TRACTION_DEMOTRACTOR;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{	
	// Stores messages that windows sends to the application
	MSG msg = {0};			
	HDC hdc = NULL;
	bool done = false;
		
	try
	{
		// Tee PAK-tiedosto käsin
//		dmsMakePAK("resources", "data.pak");
		dmsSetLogging(false);
/*
		FILE *fp = fopen("C:\\loki.txt", "wt");
		fprintf(fp, "ennen fonttia");
		fprintf(fp, "fontin jälkeen");
		fclose(fp);
*/
		Config cfg;	

		cfg.run();
		
		if(cfg.getRunFlag())
		{
			// Lukee setup.ini:n
			//dmsInit();

			// Tää on tällä hetkellä testatuin.. hoitaa suunilleen kaiken tarvittavan
			// Lukee datat .PAK-tiedostosta ja lisää ne managereille
			dmsInitEx(cfg.getScreenX(), cfg.getScreenY(), cfg.getBpp(), cfg.getFullscreen(), 
					  cfg.getSound(), cfg.getVsync(), "data.pak");
//			dmsInitEx(cfg.getScreenX(), cfg.getScreenY(), cfg.getBpp(), cfg.getFullscreen(), 
//					  true, cfg.getVsync(), "data.pak");

			dmsAddFont(32, 32, "arcade.pcx");
			dmsAddRenderToTexture(512, 512, "render0");
			dmsAddRenderToTexture(512, 512, "render1");
			dmsAddRenderToTexture(512, 512, "render2");
#define COMPLETEDEMO

#ifdef COMPLETEDEMO
			dmsAddEffect(0, 16000, 0, "alku", new Alku());
			dmsAddEffect(16000, 32000, 0, "rings", new Rings());
			dmsAddEffect(32000, 48000, 0, "Bands", new Bands());
			dmsAddEffect(48000, 64200, 0, "Tunnel", new Tunnel());
			dmsAddEffect(64200, 80000, 100, "starfield", new Starfield());
			dmsAddEffect(79800, 96000, 0, "asteroids", new Asteroids());
			dmsAddEffect(96000, 114000, 100, "pong", new Laby());
			dmsAddEffect(114000, 130000, 0, "wiretunnel", new Wiretunnel());
			dmsAddEffect(130000, 146000, 0, "tetris", new Tetris());
			dmsAddEffect(146000, 162000, 0, "tron", new Tron());
			dmsAddEffect(162000, 178000, 0, "asteroids", new Pong());
			dmsAddEffect(178000, 198000, 0, "city", new City());
#else
			dmsAddEffect(0, 16000, 0, "testi", new Laby());
#endif

			dmsInitEffects();
		
			// Tää bugaa
			//dmsSetGamma(cfg.getGamma());

			dmsUploadTextures();
			dmsInitTimer();
			dmsPlaySong(true);
		}
		else
		{
			done = true;
		}
		
		// Demo loop
		while(!done)
		{		
			// Check for Windows messages in queue
			if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				//If app was closed send a note to the demo to stop the Demo loop
				if(msg.message == WM_QUIT)
				{
					done = true;
				}
				else
				{
					// Other messages are handled by Windows own handlers
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
			// No messages left, we can do some own code now which means
			// actually running the demo :)
			else
			{	
				dmsUpdateTimer();
				dmsCheckControls(1000);
				 
				if(GetAsyncKeyState(VK_ESCAPE)) done = true;

				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			

				dmsRun();

				#ifdef _DEBUG
					char buf[256] = {0};
					sprintf(buf, "Time: %f s", dmsGetTime() / 1000.0f);
					dmsSetWindowTitle(buf);				
				#endif
					char buf[256] = {0};
					sprintf(buf, "Time: %f s", dmsGetTime() / 1000.0f);
					dmsSetWindowTitle(buf);				

				if (dmsGetTime() > 198000)
					done = true;

				// Update the demo window's screen			
				hdc = dmsGetHDC();
				SwapBuffers(hdc);					
			}
		}	
	}
	catch(const char *msg)
	{
		dmsMsg("Exception: %s\n", msg);
	}
		
	dmsSetGamma(0);
	dmsShutDown();

	// This application is done and we let the Windows know that
	return msg.wParam;
}

