#include "include/wl_def.h"
#include "SDL/SDL.h"
#include <pspctrl.h>
#include <psppower.h>
#include <pspdebug.h>
#include <pspkernel.h>

PSP_HEAP_SIZE_KB(-256);

byte *gfxbuf = NULL;
SDL_Surface *surface;
SDL_Joystick *joy;

extern void keyboard_handler(int code, int press);
extern boolean InternalKeyboard[NumCodes];

SceCtrlData pad;

int main(int argc, char *argv[])
{
	sceCtrlSetSamplingMode(1);
	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
	scePowerSetClockFrequency(333, 333, 166);
	return WolfMain(argc, argv);
}

void DisplayTextSplash(byte *text);

/* PSP_DEVIATION: wl_def.h declaration is commented out; implicit declarations are errors in modern GCC */
extern int WriteConfig();

// PSP_DEVIATION: stdout/stderr go nowhere on the PSP, so a fatal error looked like the game silently closing.
// Show it on screen with the SDK debug screen and wait for a button press before exiting.
static void ShowErrorScreen(const char *error)
{
	SceCtrlData ctrl;

	pspDebugScreenInit();
	pspDebugScreenSetBackColor(0x00000000);
	pspDebugScreenSetTextColor(0x000000FF);
	pspDebugScreenPrintf("\n  %s - ERROR\n\n", GAMENAME);
	pspDebugScreenSetTextColor(0x00FFFFFF);
	pspDebugScreenPrintf("  %s\n", error);
	pspDebugScreenPrintf("  The game data files (*.%s) must be in the same folder\n", GAMEEXT);
	pspDebugScreenPrintf("  as EBOOT.PBP, e.g. ms0:/PSP/GAME/<game folder>/\n\n");
	pspDebugScreenPrintf("  Press X or O to exit.\n");

	// wait for the button to be released, then pressed
	do
	{
		sceCtrlReadBufferPositive(&ctrl, 1);
		sceKernelDelayThread(20 * 1000);
	} while (ctrl.Buttons & (PSP_CTRL_CROSS | PSP_CTRL_CIRCLE));
	do
	{
		sceCtrlReadBufferPositive(&ctrl, 1);
		sceKernelDelayThread(20 * 1000);
	} while (!(ctrl.Buttons & (PSP_CTRL_CROSS | PSP_CTRL_CIRCLE)));
}

/*
==========================
=
= Quit
=
==========================
*/

void Quit(char *error)
{
	memptr screen = NULL;

	if (!error || !*error)
	{
		CA_CacheGrChunk(ORDERSCREEN);
		screen = grsegs[ORDERSCREEN];
		WriteConfig();
	}
	// PSP_DEVIATION: the ERRORSCREEN chunk was cached here but never displayed (DisplayTextSplash is disabled), and loading it
	// reads graphics data that may not exist yet when the error is missing data files. Show the error on screen instead.

	ShutdownId();

	if (screen)
	{
		//DisplayTextSplash(screen);
	}

	if (error && *error)
	{
		fprintf(stderr, "Quit: %s\n", error);
		ShowErrorScreen(error);
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}

void VL_WaitVBL(int vbls)
{
	long last = get_TimeCount() + vbls;
	while (last > get_TimeCount())
		;
}

void VW_UpdateScreen()
{
	//VL_WaitVBL(1);
	//memcpy(surface->pixels, gfxbuf, vstride*vheight);
	//SDL_UpdateRect(surface, 0, 0, 0, 0);

	SDL_Flip(surface);
}

/*
=======================
=
= VL_Startup
=
=======================
*/

void VL_Startup()
{
	vwidth = 512;
	vheight = 320;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_NOPARACHUTE) < 0)
	{
		Quit("Couldn't init SDL");
	}

	surface = SDL_SetVideoMode(vwidth, vheight, 8, SDL_SWSURFACE | SDL_HWPALETTE | SDL_FULLSCREEN);

	if (surface == NULL)
	{
		SDL_Quit();
		Quit("Couldn't set 512x320 mode");
	}
	gfxbuf = surface->pixels;
	gfxbuf = (byte *)((int)gfxbuf | 0x40000000);
	sceKernelDcacheWritebackAll();
	vstride = surface->pitch;

	if (surface->flags & SDL_FULLSCREEN)
		SDL_ShowCursor(0);

	SDL_WM_SetCaption(GAMENAME, GAMENAME);
}

/*
=======================
=
= VL_Shutdown
=
=======================
*/

void VL_Shutdown()
{
	//if (gfxbuf != NULL) {
	//	free(gfxbuf);
	//	gfxbuf = NULL;
	//}
	SDL_Quit();
}

/* ======================================================================== */

/*
=================
=
= VL_SetPalette
=
=================
*/

void VL_SetPalette(const byte *palette)
{
	SDL_Color colors[256];
	int i;

	VL_WaitVBL(1);

	for (i = 0; i < 256; i++)
	{
		colors[i].r = palette[i * 3 + 0] << 2;
		colors[i].g = palette[i * 3 + 1] << 2;
		colors[i].b = palette[i * 3 + 2] << 2;
	}
	SDL_SetColors(surface, colors, 0, 256);
	// Should also do a flip here.
	SDL_Flip(surface);
}

/*
=================
=
= VL_GetPalette
=
=================
*/

void VL_GetPalette(byte *palette)
{
	int i;
	for (i = 0; i < 256; i++)
	{
		palette[i * 3 + 0] = surface->format->palette->colors[i].r >> 2;
		palette[i * 3 + 1] = surface->format->palette->colors[i].g >> 2;
		palette[i * 3 + 2] = surface->format->palette->colors[i].b >> 2;
	}
}

/*
=================
=
= INL_Update
=
=================
*/

void ToggleSpeed()
{
	static int speed = 0;
	int freq[] = {133, 180, 222, 266, 300, 333};
	char temp[8];

	scePowerSetClockFrequency(freq[speed], freq[speed], freq[speed] >> 1);

	sprintf(temp, "%d MHz", freq[speed]);
	Message(temp);

	sceKernelDelayThread(500 * 1000);
	speed = (speed + 1) % 6;
}

// PSP_DEVIATION: auto-run toggle (not in the original game)
void ToggleAutoRun()
{
	autorun = !autorun;
	Message(autorun ? "Auto-run ON" : "Auto-run OFF");
	sceKernelDelayThread(500 * 1000);
}

static int mx = 0;
static int my = 0;

void INL_Update()
{
	static u32 buttons = 0;
	u32 new_buttons;
	static int mu = 0;
	static int md = 0;

	sceCtrlReadBufferPositive(&pad, 1);
	// PSP_DEVIATION: removed the per-call d-pad/shoulder masking; it made held buttons flicker
	new_buttons = pad.Buttons ^ buttons; // set if button changed
	buttons = pad.Buttons;

	if (new_buttons & PSP_CTRL_CROSS)
	{
		if (!(buttons & PSP_CTRL_CROSS))
		{
			// X just released
			keyboard_handler(sc_Control, 0); // FIRE not pressed
			keyboard_handler(sc_Y, 0);		 // Y not pressed
		}
		else
		{
			// X just pressed
			keyboard_handler(sc_Control, 1); // FIRE pressed
			keyboard_handler(sc_Y, 1);		 // Y pressed
		}
	}
	if (new_buttons & PSP_CTRL_TRIANGLE)
	{
		if (!(buttons & PSP_CTRL_TRIANGLE))
		{
			// <| just released
			keyboard_handler(sc_Space, 0); // open/operate not pressed
		}
		else
		{
			// <| just pressed
			keyboard_handler(sc_Space, 1); // open/operate pressed
		}
	}
	if (new_buttons & PSP_CTRL_SQUARE)
	{
		if (!(buttons & PSP_CTRL_SQUARE))
		{
			// [] just released
			keyboard_handler(sc_RShift, 0); // run not pressed
		}
		else
		{
			// [] just pressed
			// BEGIN PSP_DEVIATION: O + [] toggles auto-run (in a level only)
			if ((buttons & PSP_CTRL_CIRCLE) && ingame)
				ToggleAutoRun();
			else
				keyboard_handler(sc_RShift, 1); // run pressed
			// END PSP_DEVIATION
		}
	}

	if (new_buttons & PSP_CTRL_UP)
	{
		if (!(buttons & PSP_CTRL_UP))
		{
			// up just released
			keyboard_handler(sc_1, 0);		 // weapon 1 not pressed
			keyboard_handler(sc_UpArrow, 0); // up not pressed
		}
		else
		{
			// up just pressed
			if (buttons & PSP_CTRL_CIRCLE)
				keyboard_handler(sc_1, 1); // weapon 1 pressed
			else
				keyboard_handler(sc_UpArrow, 1); // up pressed
		}
	}
	if (new_buttons & PSP_CTRL_RIGHT)
	{
		if (!(buttons & PSP_CTRL_RIGHT))
		{
			// right just released
			keyboard_handler(sc_2, 0);			// weapon 2 not pressed
			keyboard_handler(sc_RightArrow, 0); // right not pressed
		}
		else
		{
			// right just pressed
			if (buttons & PSP_CTRL_CIRCLE)
				keyboard_handler(sc_2, 1); // weapon 2 pressed
			else
				keyboard_handler(sc_RightArrow, 1); // right pressed
		}
	}
	if (new_buttons & PSP_CTRL_DOWN)
	{
		if (!(buttons & PSP_CTRL_DOWN))
		{
			// down just released
			keyboard_handler(sc_3, 0);		   // weapon 3 not pressed
			keyboard_handler(sc_DownArrow, 0); // down not pressed
		}
		else
		{
			// down just pressed
			if (buttons & PSP_CTRL_CIRCLE)
				keyboard_handler(sc_3, 1); // weapon 3 pressed
			else
				keyboard_handler(sc_DownArrow, 1); // down pressed
		}
	}
	if (new_buttons & PSP_CTRL_LEFT)
	{
		if (!(buttons & PSP_CTRL_LEFT))
		{
			// left just released
			keyboard_handler(sc_4, 0);		   // weapon 4 not pressed
			keyboard_handler(sc_LeftArrow, 0); // left not pressed
		}
		else
		{
			// left just pressed
			if (buttons & PSP_CTRL_CIRCLE)
				keyboard_handler(sc_4, 1); // weapon 4 pressed
			else
				keyboard_handler(sc_LeftArrow, 1); // left pressed
		}
	}
	// BEGIN PSP_DEVIATION: shoulder buttons strafe in a level.
	// The strafe keys follow the physical buttons on every update (level-triggered) and are written
	// straight into the key arrays, so LastScan is not disturbed and IN_ClearKeysDown cannot leave
	// them stuck off. Outside a level the shoulders keep their old menu behaviour (Alt + arrow).
	Keyboard[sc_StrafeLeft] = InternalKeyboard[sc_StrafeLeft] = (pad.Buttons & PSP_CTRL_LTRIGGER) != 0;
	Keyboard[sc_StrafeRight] = InternalKeyboard[sc_StrafeRight] = (pad.Buttons & PSP_CTRL_RTRIGGER) != 0;
#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION
	// BEGIN PSP_DEVIATION: S3DNA RESTORATION - weapons 5 (O + L) and 6 (O + R), the D-pad only has room for 1-4
	if (new_buttons & PSP_CTRL_LTRIGGER)
		keyboard_handler(sc_5, (buttons & PSP_CTRL_LTRIGGER) && (buttons & PSP_CTRL_CIRCLE) && ingame);
	if (new_buttons & PSP_CTRL_RTRIGGER)
		keyboard_handler(sc_6, (buttons & PSP_CTRL_RTRIGGER) && (buttons & PSP_CTRL_CIRCLE) && ingame);
	// END PSP_DEVIATION
#endif
	if (new_buttons & PSP_CTRL_RTRIGGER)
	{
		if (!(buttons & PSP_CTRL_RTRIGGER))
		{
			// rtrg just released
			keyboard_handler(sc_Alt, 0); // alt not pressed
			if (!(buttons & PSP_CTRL_RIGHT))
				keyboard_handler(sc_RightArrow, 0); // right not pressed (unless the d-pad holds it)
		}
		else if (!ingame)
		{
			// rtrg just pressed (menus only)
			keyboard_handler(sc_Alt, 1);		// alt pressed
			keyboard_handler(sc_RightArrow, 1); // right pressed
		}
	}
	if (new_buttons & PSP_CTRL_LTRIGGER)
	{
		if (!(buttons & PSP_CTRL_LTRIGGER))
		{
			// ltrg just released
			keyboard_handler(sc_Alt, 0); // alt not pressed
			if (!(buttons & PSP_CTRL_LEFT))
				keyboard_handler(sc_LeftArrow, 0); // left not pressed (unless the d-pad holds it)
		}
		else if (!ingame)
		{
			// ltrg just pressed (menus only)
			keyboard_handler(sc_Alt, 1);	   // alt pressed
			keyboard_handler(sc_LeftArrow, 1); // left pressed
		}
	}
	// END PSP_DEVIATION
	if (new_buttons & PSP_CTRL_START)
	{
		if (!(buttons & PSP_CTRL_START))
		{
			// START just released
			keyboard_handler(sc_Escape, 0); // MENU not pressed
		}
		else
		{
			// START just pressed
			if (buttons & PSP_CTRL_CIRCLE)
				ToggleSpeed();
			else
				keyboard_handler(sc_Escape, 1); // MENU pressed
		}
	}
	if (new_buttons & PSP_CTRL_SELECT)
	{
		if (!(buttons & PSP_CTRL_SELECT))
		{
			// SELECT just released
			keyboard_handler(sc_BackSpace, 0); // BackSpace not pressed
			keyboard_handler(sc_Enter, 0);	 // Enter not pressed
			keyboard_handler(sc_A, 0);		   // A not pressed
		}
		else
		{
			// SELECT just pressed
			if (buttons & PSP_CTRL_CIRCLE)
				keyboard_handler(sc_BackSpace, 1); // BackSpace pressed
			else if (buttons & PSP_CTRL_SQUARE)
				keyboard_handler(sc_A, 1); // A pressed
			else
				keyboard_handler(sc_Enter, 1); // Enter pressed
		}
	}
	if (new_buttons & PSP_CTRL_CIRCLE && !StartGame)
	{
		if (!(buttons & PSP_CTRL_CIRCLE))
		{
			// O just released
			keyboard_handler(sc_Escape, 0); // MENU not pressed
		}
		else
		{
			// O just pressed
			keyboard_handler(sc_Escape, 1); // MENU pressed
		}
	}

	// handle analog stick
	if (buttons & PSP_CTRL_CIRCLE)
	{
		if (pad.Ly < 64 && !mu)
		{
			// just pressed stick up
			mu = 1;
			keyboard_handler(sc_Tab, 1);
			keyboard_handler(sc_G, 1);
			keyboard_handler(sc_F10, 1); // toggle god-mode
		}
		else if (pad.Ly > 64 && mu)
		{
			// just released stick up
			mu = 0;
			keyboard_handler(sc_Tab, 0);
			keyboard_handler(sc_G, 0);
			keyboard_handler(sc_F10, 0); // toggle god-mode
		}
		if (pad.Ly > 192 && !md)
		{
			// just pressed stick down
			md = 1;
			keyboard_handler(sc_M, 1);
			keyboard_handler(sc_I, 1);
			keyboard_handler(sc_L, 1); // Full Health, Ammo, Keys, Weapons, Zero score
		}
		else if (pad.Ly < 192 && md)
		{
			// just released stick down
			md = 0;
			keyboard_handler(sc_M, 0);
			keyboard_handler(sc_I, 0);
			keyboard_handler(sc_L, 0);
		}
	}
	else
	{
		if (abs(pad.Lx - 128) > 32)
			mx = (pad.Lx - 128) >> 1;
		else
			mx = 0;
		if (abs(pad.Ly - 128) > 32)
			my = (pad.Ly - 128) >> 2;
		else
			my = 0;
	}
}

void IN_GetMouseDelta(int *dx, int *dy)
{
	if (dx)
		*dx = mx;
	if (dy)
		*dy = my;
}

byte IN_MouseButtons()
{
	return 0;
}
