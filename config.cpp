//-------------------------------------------------------
// Includes
//-------------------------------------------------------

#include <windows.h>
#include <commctrl.h>
#include <stdio.h>

#ifdef _DEBUG
	#include <stdlib.h>
	#include "mmgr.h"
#endif

#include "resource.h"
#include "config.hpp"

//-------------------------------------------------------
//	Globals and static members
//-------------------------------------------------------

int Config::screenX;
int Config::screenY;
int Config::bpp;
int Config::gamma;
bool Config::fullscreen;
bool Config::sound;
bool Config::vsync;
bool Config::runFlag;

//-------------------------------------------------------
//	Constructors and Destructor
//-------------------------------------------------------

Config::Config()
{
	screenX = 640;
	screenY = 480;
	bpp = 32;
	gamma = 0;
	fullscreen = false;
	sound = false;
	vsync = false;
	runFlag = false;
}

Config::~Config()
{
}

//-------------------------------------------------------
//	Public functions
//-------------------------------------------------------

bool Config::run()
{	
	if(DialogBox(GetModuleHandle(0), MAKEINTRESOURCE(IDD_SETUPDLG), NULL, (DLGPROC)ConfigProc) == 1)
	{		
		return false;
	}

	if(!getRunFlag()) return false;

	return true;
}

int Config::getScreenX()
{
	return screenX;
}

int Config::getScreenY()
{
	return screenY;
}

int Config::getBpp()
{
	return bpp;
}

int Config::getGamma()
{
	return gamma;
}

bool Config::getFullscreen()
{
	return fullscreen;
}

bool Config::getSound()
{
	return sound;
}

bool Config::getVsync()
{
	return vsync;
}

bool Config::getRunFlag()
{
	return runFlag;
}

//-------------------------------------------------------
//	Dialog procedure - handles inputs
//-------------------------------------------------------

CALLBACK Config::ConfigProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{	
	switch(uMsg)
	{				
		case WM_INITDIALOG:
		{				
			InitCommonControls();
			
			RECT rect, dlgRect;
			HWND dlg;
			
			// Center the dialog box
			dlg = GetDesktopWindow();			
			if(dlg)
			{				
				GetWindowRect(dlg, &rect);
				GetWindowRect(hwnd, &dlgRect);

				SetWindowPos(hwnd, HWND_TOP, (rect.right/2) - ((dlgRect.right - dlgRect.left) / 2), (rect.bottom/2) - ((dlgRect.bottom - dlgRect.top) / 2), 0, 0, SWP_NOSIZE);
			}			

			// Show the dialog
			ShowWindow(hwnd, SW_SHOW);
			SetActiveWindow(hwnd);
			SetForegroundWindow(hwnd);
			SetFocus(hwnd);

			// Check default values
			SendDlgItemMessage(hwnd, IDC_1024, BM_SETCHECK, wParam, 0);
			SendDlgItemMessage(hwnd, IDC_FULLSCREEN, BM_SETCHECK, wParam, 0);
			SendDlgItemMessage(hwnd, IDC_SOUND, BM_SETCHECK, wParam, 0);
			SendDlgItemMessage(hwnd, IDC_VSYNC, BM_SETCHECK, wParam, 0);
			
			// Setup gamma slider values [-255..255]
			SendDlgItemMessage(hwnd, IDC_GAMMA, TBM_SETRANGE, true, MAKELONG(-255,255));

		} break;

		case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
				case IDOK:
				{					
					if(SendDlgItemMessage(hwnd, IDC_1280, BM_GETSTATE, 0, 0) == BST_CHECKED) 
					{
						screenX = 1280;
						screenY = 1024;
						bpp = 32;
					}

					if(SendDlgItemMessage(hwnd, IDC_1024, BM_GETSTATE, 0, 0) == BST_CHECKED) 
					{
						screenX = 1024;
						screenY = 768;
						bpp = 32;
					}

					if(SendDlgItemMessage(hwnd, IDC_800, BM_GETSTATE, 0, 0) == BST_CHECKED) 
					{
						screenX = 800;
						screenY = 600;
						bpp = 32;
					}

					if(SendDlgItemMessage(hwnd, IDC_640, BM_GETSTATE, 0, 0) == BST_CHECKED) 
					{
						screenX = 640;
						screenY = 480;
						bpp = 32;
					}
					
					if(SendDlgItemMessage(hwnd, IDC_FULLSCREEN, BM_GETSTATE, 0, 0) == BST_CHECKED) 
					{
						fullscreen = true;
					}

					if(SendDlgItemMessage(hwnd, IDC_SOUND, BM_GETSTATE, 0, 0) == BST_CHECKED) 
					{
						sound = true;
					}

					if(SendDlgItemMessage(hwnd, IDC_VSYNC, BM_GETSTATE, 0, 0) == BST_CHECKED) 
					{
						vsync = true;
					}

					gamma = SendDlgItemMessage(hwnd, IDC_GAMMA, TBM_GETPOS, 0, 0);
					
					Config::runFlag = true;
					EndDialog(hwnd, 0);
				} break;
				
				case IDCANCEL:
				{
					Config::runFlag = false;
					SendMessage(hwnd, WM_CLOSE, 0, 0);
				} break;

			} break;

		} break;

		case WM_DESTROY:
		{
			SendMessage(hwnd, WM_CLOSE, 0, 0);
		} break;
		
		case WM_CLOSE:
		{						
			EndDialog(hwnd, 0);
		} 
		break;		
	}
		
	return 0;
}
