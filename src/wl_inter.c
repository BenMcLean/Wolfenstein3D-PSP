#include "include/wl_def.h"

/*
==================
=
= CLearSplitVWB
=
==================
*/

void ClearSplitVWB()
{
	WindowX = 0;
	WindowY = 0;
	WindowW = 320;
	WindowH = 160;
}

/* ======================================================================== */

#if defined(SPEAR) && !defined(SPEARDEMO)
////////////////////////////////////////////////////////
//
// End of Spear of Destiny
//
////////////////////////////////////////////////////////

void EndScreen(int palette, int screen)
{
	VL_CacheScreen(screen);
	VW_UpdateScreen();
	CA_CacheGrChunk(palette);
	VL_FadeIn(0, 255, grsegs[palette], 30);
	CA_UnCacheGrChunk(palette);

	IN_ClearKeysDown();
	IN_Ack();
	VW_FadeOut();
}

void EndSpear()
{
	EndScreen(END1PALETTE, ENDSCREEN11PIC);

	VL_CacheScreen(ENDSCREEN3PIC);
	VW_UpdateScreen();
	CA_CacheGrChunk(END3PALETTE);
	VL_FadeIn(0, 255, grsegs[END3PALETTE], 30);
	CA_UnCacheGrChunk(END3PALETTE);

	fontnumber = 0;
	fontcolor = 0xd0;
	WindowX = 0;
	WindowW = 320;
	PrintX = 0;
	PrintY = 180;
	US_CPrint(STR_ENDGAME1 "\n");
	US_CPrint(STR_ENDGAME2);
	VW_UpdateScreen();

	IN_StartAck();
	set_TimeCount(0);
	while (!IN_CheckAck() && (get_TimeCount() < 700))
		;

	PrintX = 0;
	PrintY = 180;
	VW_Bar(0, 180, 320, 20, 0);
	US_CPrint(STR_ENDGAME3 "\n");
	US_CPrint(STR_ENDGAME4);
	VW_UpdateScreen();

	IN_StartAck();
	set_TimeCount(0);
	while (!IN_CheckAck() && (get_TimeCount() < 700))
		;

	VW_FadeOut();

	EndScreen(END4PALETTE, ENDSCREEN4PIC);
	EndScreen(END5PALETTE, ENDSCREEN5PIC);
	EndScreen(END6PALETTE, ENDSCREEN6PIC);
	EndScreen(END7PALETTE, ENDSCREEN7PIC);
	EndScreen(END8PALETTE, ENDSCREEN8PIC);
	EndScreen(END9PALETTE, ENDSCREEN9PIC);

	EndScreen(END2PALETTE, ENDSCREEN12PIC);

	MainMenu[savegame].active = 0;
}
#endif

/* ======================================================================== */

/*
==================
=
= Victory
=
==================
*/

#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION
// *** S3DNA RESTORATION ***
// PSP_DEVIATION: states are indices into gamestates[] and there is no page flipping
// or video hardware access, so this is adapted from the original CharacterCast
static const int caststate[] = {s_dogchase1, s_grdchase1, s_ofcchase1, s_sschase1,
								s_mutchase1, s_bosschase1, s_schabbchase1, s_gretelchase1,
								s_giftchase1, s_fatchase1, s_mechachase1, s_hitlerchase1};

static char *casttext[] = {"Goat", "Sheep", "Ostrich", "Antelope",
						   "Ox", "Carl the Camel", "Melvin the Monkey", "Ginny the Giraffe",
						   "Kerry the Kangaroo", "Ernie the Elephant", "Hiding Burt", "Burt the Bear"};

static const int castsound[] = {D_GOATSND, D_SHEEPSND, D_OSTRCHSND, D_ANTLPESND,
								D_OXSND, D_CAMELSND, D_MONKEYSND, D_GIRAFESND,
								D_KANGROSND, D_ELPHNTSND, D_BEARSND, D_BEARSND};

void CharacterCast(void)
{
	int en, count, cycle; // PSP_DEVIATION: removed unused 'i'
	int state;

	ClearMemory();
	StartCPMusic(NOAH02_MUS);
	gamestate.mapon = 30;
	SetupGameLevel();
	CA_CacheGrChunk(STARTFONT + 1);
	SETFONTCOLOR(READHCOLOR, BKGDCOLOR);
	PreloadGraphics();

	DrawPlayBorder();
	VW_Bar(0, 160, 320, 40, VIEWCOLOR);

	en = 0;
	anglefrac = 0;
	WindowX = 0;
	WindowW = 320 - 16;
	fontnumber = 1;

	lasttimecount = 0;
	set_TimeCount(0);
	IN_StartAck();

	do
	{
		state = caststate[en];
		count = gamestates[state].tictime;
		cycle = 0;

	nexttic:
		if (cycle == 0)
			SD_PlaySound(castsound[en]);

		CalcTics();
		cycle += 2 * tics;
		if (cycle > 224)
		{
			if (++en >= 12)
				en = 0;
			continue;
		}

		if (!(--count))
		{
			state = gamestates[state].next;
			count = gamestates[state].tictime;
		}

		memset(spotvis, 0, sizeof(spotvis));

		PollControls();
		WallRefresh();
		if (!nofloors)
			DrawPlanes();
		DrawScaleds();
		SimpleScaleShape(viewwidth / 2, gamestates[state].shapenum, cycle / 2 + 24);
		VW_Bar(0, 160, 320, 40, VIEWCOLOR);
		PrintY = 168;
		US_CPrintLine(casttext[en]);

		VW_UpdateScreen();
		frameon++;

		if (screenfaded)
			VW_FadeIn();

		if ((cycle >= 16) && (Keyboard[sc_Space] || buttonstate[bt_attack] || buttonstate[bt_use]))
		{
			if (++en >= 12)
				en = 0;
			continue;
		}

		if (Keyboard[sc_Escape] || Keyboard[sc_Q] || Keyboard[sc_Enter])
			break;

		goto nexttic;

	} while (en < 12);

	CA_UnCacheGrChunk(STARTFONT + 1);
	StopMusic();
	fontnumber = 0;
	VW_FadeOut();
}

#endif /* GAMEVER_NOAH3D */

#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION
void Victory (void)
{

	// *** SHAREWARE V1.0 APOGEE + S3DNA RESTORATION ***
	long	sec,parsec;
	// *** SHAREWARE V1.0 APOGEE + S3DNA RESTORATION ***
	int i,hr,min,kr,sr,tr,x;
	char tempstr[8];

// *** S3DNA RESTORATION ***
#define RATIOX	9
#define RATIOY	12
#define TIMEX	23
#define TIMEY	9
#define PARTIMEX	23
#define PARTIMEY	7



	// *** S3DNA RESTORATION ***
	StartCPMusic (NOAH07_MUS);
	ClearSplitVWB ();
	CacheLump(LEVELEND_LUMP_START,LEVELEND_LUMP_END);
	// *** S3DNA RESTORATION ***


	// *** S3DNA RESTORATION ***
	VW_Bar (0,0,320,200-STATUSLINES,218);

	// *** S3DNA RESTORATION ***
	VWB_DrawPic (8,8,L_BJWINSPIC);

	VWB_DrawPic (104,32,W_RATINGPIC);
	VWB_DrawPic (120,56,W_PARPIC);
	VWB_DrawPic (112,72,W_TIMEPIC);

	VWB_DrawPic (104,96,W_KILLSPIC);
	Write(33,12,"%");

	VWB_DrawPic (128,112,W_TREASURESPIC);
	Write(33,14,"%");

	VWB_DrawPic (96,128,W_SECRETSPIC);
	Write(33,16,"%");


	// *** S3DNA RESTORATION ***
	for (kr = sr = tr = sec = parsec = i = 0;i < 30;i++)
	{
		sec += LevelRatios[i].time;
		kr += LevelRatios[i].kill;
		sr += LevelRatios[i].secret;
		tr += LevelRatios[i].treasure;
		// *** S3DNA RESTORATION ***
		parsec += LevelRatios[i].par;
	}

	// *** S3DNA RESTORATION ***
	min = parsec/60;
	parsec %= 60;
	hr = min/60;
	min %= 60;
	if (hr > 9)
	{
		hr = 9;
		min = parsec = 99;
	}
	i = PARTIMEX*8;
	VWB_DrawPic (i,PARTIMEY*8,L_NUM0PIC+hr);
	i += 2*8;
	Write (i/8,PARTIMEY,":");
	i += 1*8;
	VWB_DrawPic (i,PARTIMEY*8,L_NUM0PIC+(min/10));
	i += 2*8;
	VWB_DrawPic (i,PARTIMEY*8,L_NUM0PIC+(min%10));
	i += 2*8;
	Write (i/8,PARTIMEY,":");
	i += 1*8;
	VWB_DrawPic (i,PARTIMEY*8,L_NUM0PIC+(parsec/10));
	i += 2*8;
	VWB_DrawPic (i,PARTIMEY*8,L_NUM0PIC+(parsec%10));
	VW_UpdateScreen ();

	min = sec/60;
	sec %= 60;
	// *** S3DNA RESTORATION ***
	hr = min/60;
	min %= 60;
	if (hr > 9)
	{
		hr = 9;
		min = sec = 99;
	}
	// *** SHAREWARE V1.0+1.1 APOGEE RESTORATION ***

	// *** S3DNA RESTORATION ***
	i = TIMEX*8;
	VWB_DrawPic(i,TIMEY*8,L_NUM0PIC+hr);
	i += 2*8;
	Write(i/8,TIMEY,":");
	i += 1*8;
	VWB_DrawPic(i,TIMEY*8,L_NUM0PIC+(min/10));
	i += 2*8;
	VWB_DrawPic(i,TIMEY*8,L_NUM0PIC+(min%10));
	i += 2*8;
	Write(i/8,TIMEY,":");
	i += 1*8;
	VWB_DrawPic(i,TIMEY*8,L_NUM0PIC+(sec/10));
	i += 2*8;
	VWB_DrawPic(i,TIMEY*8,L_NUM0PIC+(sec%10));
	VW_UpdateScreen ();
	// *** S3DNA RESTORATION ***
	kr /= 30;
	sr /= 30;
	tr /= 30;

	itoa(kr,tempstr,10);
	x=RATIOX+24-strlen(tempstr)*2;
	Write(x,RATIOY,tempstr);

	itoa(sr,tempstr,10);
	x=RATIOX+24-strlen(tempstr)*2;
	Write(x,RATIOY+2,tempstr);

	itoa(tr,tempstr,10);
	x=RATIOX+24-strlen(tempstr)*2;
	Write(x,RATIOY+4,tempstr);


	// *** SHAREWARE V1.0 APOGEE RESTORATION ***
//#ifndef UPLOAD
	// *** S3DNA RESTORATION ***


	// *** S3DNA RESTORATION ***

	VW_UpdateScreen ();
	VW_FadeIn ();

	IN_Ack();

	// *** SHAREWARE/REGISTERED APOGEE RESTORATION ***
	// This is also skipped in the Apogee EXEs
	//#ifndef SPEAR

	VW_FadeOut ();

	// *** S3DNA RESTORATION ***
	UnCacheLump(LEVELEND_LUMP_START,LEVELEND_LUMP_END);

	// *** S3DNA RESTORATION ***
	CharacterCast();


}
#else
void Victory()
{
#ifndef SPEARDEMO
	long sec;
	int i, min, kr, sr, tr, x;
	char tempstr[8];

#define RATIOX 6
#define RATIOY 14
#define TIMEX 14
#define TIMEY 8

#ifdef SPEAR
	StartCPMusic(XTHEEND_MUS);

	CA_CacheGrChunk(BJCOLLAPSE1PIC);
	CA_CacheGrChunk(BJCOLLAPSE2PIC);
	CA_CacheGrChunk(BJCOLLAPSE3PIC);
	CA_CacheGrChunk(BJCOLLAPSE4PIC);

	VW_Bar(0, 0, 320, 200, VIEWCOLOR);
	VWB_DrawPic(124, 44, BJCOLLAPSE1PIC);
	VW_UpdateScreen();
	VW_FadeIn();
	VW_WaitVBL(2 * 70);
	VWB_DrawPic(124, 44, BJCOLLAPSE2PIC);
	VW_UpdateScreen();
	VW_WaitVBL(105);
	VWB_DrawPic(124, 44, BJCOLLAPSE3PIC);
	VW_UpdateScreen();
	VW_WaitVBL(105);
	VWB_DrawPic(124, 44, BJCOLLAPSE4PIC);
	VW_UpdateScreen();
	VW_WaitVBL(3 * 70);

	CA_UnCacheGrChunk(BJCOLLAPSE1PIC);
	CA_UnCacheGrChunk(BJCOLLAPSE2PIC);
	CA_UnCacheGrChunk(BJCOLLAPSE3PIC);
	CA_UnCacheGrChunk(BJCOLLAPSE4PIC);
	VL_FadeOut(0, 255, 0, 17, 17, 5);
#endif

	StartCPMusic(URAHERO_MUS);
	ClearSplitVWB();
	CacheLump(LEVELEND_LUMP_START, LEVELEND_LUMP_END);
	CA_CacheGrChunk(STARTFONT);

	VW_Bar(0, 0, 320, 200 - STATUSLINES, 127);

	Write(18, 2, STR_YOUWIN);

	Write(TIMEX, TIMEY - 2, STR_TOTALTIME);

	Write(12, RATIOY - 2, "averages");

	Write(RATIOX + 8, RATIOY, STR_RATKILL);
	Write(RATIOX + 4, RATIOY + 2, STR_RATSECRET);
	Write(RATIOX, RATIOY + 4, STR_RATTREASURE);

	VWB_DrawPic(8, 4, L_BJWINSPIC);

#ifndef SPEAR
	for (kr = sr = tr = sec = i = 0; i < 8; i++)
#else
	for (kr = sr = tr = sec = i = 0; i < 20; i++)
#endif
	{
		sec += LevelRatios[i].time;
		kr += LevelRatios[i].kill;
		sr += LevelRatios[i].secret;
		tr += LevelRatios[i].treasure;
	}

#ifndef SPEAR
	kr /= 8;
	sr /= 8;
	tr /= 8;
#else
	kr /= 14;
	sr /= 14;
	tr /= 14;
#endif

	min = sec / 60;
	sec %= 60;

	if (min > 99)
		min = sec = 99;

	i = TIMEX * 8 + 1;
	VWB_DrawPic(i, TIMEY * 8, L_NUM0PIC + (min / 10));
	i += 2 * 8;
	VWB_DrawPic(i, TIMEY * 8, L_NUM0PIC + (min % 10));
	i += 2 * 8;
	Write(i / 8, TIMEY, ":");
	i += 1 * 8;
	VWB_DrawPic(i, TIMEY * 8, L_NUM0PIC + (sec / 10));
	i += 2 * 8;
	VWB_DrawPic(i, TIMEY * 8, L_NUM0PIC + (sec % 10));
	VW_UpdateScreen();

	itoa(kr, tempstr, 10);
	x = RATIOX + 24 - strlen(tempstr) * 2;
	Write(x, RATIOY, tempstr);

	itoa(sr, tempstr, 10);
	x = RATIOX + 24 - strlen(tempstr) * 2;
	Write(x, RATIOY + 2, tempstr);

	itoa(tr, tempstr, 10);
	x = RATIOX + 24 - strlen(tempstr) * 2;
	Write(x, RATIOY + 4, tempstr);

	fontnumber = 1;

	VW_UpdateScreen();
	VW_FadeIn();

	IN_Ack();

#ifndef SPEAR
	if (IN_KeyDown(sc_P) && MS_CheckParm("debugmode"))
		PicturePause();
#endif

	VW_FadeOut();

	UnCacheLump(LEVELEND_LUMP_START, LEVELEND_LUMP_END);

#ifndef SPEAR
	EndText();
#else
	EndSpear();
#endif

#endif /* SPEARDEMO */
}
#endif /* GAMEVER_NOAH3D */

/* ======================================================================== */

#ifndef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION - not in S3DNA
/*
==================
=
= PG13
=
==================
*/

void PG13()
{
	VW_FadeOut();
	VW_Bar(0, 0, 320, 200, 0x82); // background

	CA_CacheGrChunk(PG13PIC);
	VWB_DrawPic(216, 110, PG13PIC);
	VW_UpdateScreen();

	CA_UnCacheGrChunk(PG13PIC);

	VW_FadeIn();
	IN_UserInput(TickBase * 7);

	VW_FadeOut();
}
#endif /* GAMEVER_NOAH3D */

//==========================================================================

#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION
void Write(int x,int y,char *string)
{
 // *** S3DNA RESTORATION ***
 int alpha[]={L_NUM0PIC,L_NUM1PIC,L_NUM2PIC,L_NUM3PIC,L_NUM4PIC,L_NUM5PIC,
	L_NUM6PIC,L_NUM7PIC,L_NUM8PIC,L_NUM9PIC,L_COLONPIC,0,0,0,0,0};

 int i,ox,nx,ny;
 char ch;


 ox=nx=x*8;
 ny=y*8;
 for (i=0;i<strlen(string);i++)
   if (string[i]=='\n')
   {
	nx=ox;
	ny+=16;
   }
   else
   {
	ch=string[i];
	if (ch>='a')
	  ch-=('a'-'A');
	ch-='0';

	switch(string[i])
	{
	 case '!':
	   VWB_DrawPic(nx,ny,L_EXPOINTPIC);
	   nx+=8;
	   continue;

	 // *** SHAREWARE V1.0 APOGEE RESTORATION ***
	 case '\'':
	   VWB_DrawPic(nx,ny,L_APOSTROPHEPIC);
	   nx+=8;
	   continue;

	 case ' ': break;
	 case 0x3a:	// ':'

	   VWB_DrawPic(nx,ny,L_COLONPIC);
	   nx+=8;
	   continue;

	 case '%':
	   VWB_DrawPic(nx,ny,L_PERCENTPIC);
	   break;

	 // *** S3DNA RESTORATION ***
	 case '-':
	   VWB_DrawPic(nx,ny,L_MINUSPIC);
	   nx+=8;
	   continue;

	 default:
	  if (ch >= 16)
	  {
		sprintf(str,"Write(): Invalid character $%02X!",ch);
		Quit(str);
	  }
	   VWB_DrawPic(nx,ny,alpha[(unsigned char)ch]); // PSP_DEVIATION: cast avoids negative char index
	}
	nx+=16;
   }
}
#else
void Write(int x, int y, char *string)
{
	int alpha[] = {L_NUM0PIC, L_NUM1PIC, L_NUM2PIC, L_NUM3PIC, L_NUM4PIC, L_NUM5PIC,
				   L_NUM6PIC, L_NUM7PIC, L_NUM8PIC, L_NUM9PIC, L_COLONPIC, 0, 0, 0, 0, 0, 0, L_APIC, L_BPIC,
				   L_CPIC, L_DPIC, L_EPIC, L_FPIC, L_GPIC, L_HPIC, L_IPIC, L_JPIC, L_KPIC,
				   L_LPIC, L_MPIC, L_NPIC, L_OPIC, L_PPIC, L_QPIC, L_RPIC, L_SPIC, L_TPIC,
				   L_UPIC, L_VPIC, L_WPIC, L_XPIC, L_YPIC, L_ZPIC};

	int i, ox, nx, ny;
	char ch;

	ox = nx = x * 8;
	ny = y * 8;
	for (i = 0; i < strlen(string); i++)
		if (string[i] == '\n')
		{
			nx = ox;
			ny += 16;
		}
		else
		{
			ch = string[i];
			if (ch >= 'a')
				ch -= ('a' - 'A');
			ch -= '0';

			switch (string[i])
			{
			case '!':
				VWB_DrawPic(nx, ny, L_EXPOINTPIC);
				nx += 8;
				continue;

			case '\'':
				VWB_DrawPic(nx, ny, L_APOSTROPHEPIC);
				nx += 8;
				continue;

			case ' ':
				break;
			case 0x3a: // ':'

				VWB_DrawPic(nx, ny, L_COLONPIC);
				nx += 8;
				continue;

			case '%':
				VWB_DrawPic(nx, ny, L_PERCENTPIC);
				break;

			default:
				VWB_DrawPic(nx, ny, alpha[(int)ch]);
			}
			nx += 16;
		}
}
#endif /* GAMEVER_NOAH3D */

//
// Breathe Mr. BJ!!!
//
#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION
void BJ_Breathe(void)
{
	static int which=0,max=10;
	int pics[2]={L_GUYPIC,L_GUY2PIC};


	if (get_TimeCount()>max)
	{
		which^=1;
		// *** S3DNA RESTORATION ***
		VWB_DrawPic(8,8,pics[which]);
		VW_UpdateScreen();
		set_TimeCount(0);
		max=35;
	}
}
#else
void BJ_Breathe()
{
	static int which = 0, max = 10;
	int pics[2] = {L_GUYPIC, L_GUY2PIC};

	if (get_TimeCount() > max)
	{
		which ^= 1;
		VWB_DrawPic(0, 16, pics[which]);
		VW_UpdateScreen();
		set_TimeCount(0);
		max = 35;
	}
}
#endif /* GAMEVER_NOAH3D */

/*
==================
=
= LevelCompleted
=
= Entered with the screen faded out
= Still in split screen mode with the status bar
=
= Exit with the screen faded out
=
==================
*/

#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION
#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION - Additional functions for LevelCompleted
void ShowBonus (long bonus)
{
	char tempstr[10];
	char	ch;
	int	x,i;

	ltoa(bonus,tempstr,10);
	x=8*(36-strlen(tempstr)*2);

	for (i=0;i<strlen(tempstr);i++)
	{
		ch = tempstr[i];
		if ((ch >= '0') && (ch <= '9'))
		{
			VWB_DrawPic(x,72,ch+(Y_NUM0PIC-'0'));
			x += 16;
		}
	}
}

void DrawEndLevelScreen (int secretlvl)
{
	char tempstr[10];

	VW_Bar (0,0,320,160,VIEWCOLOR);
	VWB_DrawPic (8,8,L_GUYPIC);
	if (secretlvl)
	{
		VWB_DrawPic (104,8,W_SECRETPIC);
		VWB_DrawPic (104,24,W_LEVELPIC);
		VWB_DrawPic (104,40,W_FINISHPIC);
		VWB_DrawPic (184,24,secretlvl+L_NUM0PIC);
	}
	else
	{
		VWB_DrawPic (104,8,W_LEVELPIC);
		VWB_DrawPic (104,24,W_FLOORPIC);
		VWB_DrawPic (104,40,W_FINISHPIC);
		sprintf (tempstr,"%d",MapEpisode[gamestate.mapon]);
		Write (23,1,tempstr);
		sprintf (tempstr,"%d",MapLevel[gamestate.mapon]);
		Write (23,3,tempstr);
	}
	VWB_DrawPic (104,72,W_BONUSPIC);
}

void RollDelay (void)
{
	unsigned long lasttime = get_TimeCount();
	while (get_TimeCount() == lasttime)
		;
}
#endif /* GAMEVER_NOAH3D */

LRstruct LevelRatios[30];
#elif !defined(SPEAR)
LRstruct LevelRatios[8];
#else
LRstruct LevelRatios[20];
#endif

#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION
void LevelCompleted (void)
{
	#define VBLWAIT	30
	#define PAR_AMOUNT	500
	#define PERCENT100AMT	10000
	typedef struct {
	// *** SHAREWARE V1.0 APOGEE RESTORATION ***
			float time;
			char timestr[6];
			} times;

	// *** S3DNA  RESTORATION ***
	int	x,i,min,sec,secret; // PSP_DEVIATION: removed unused 'ratio'
	boolean perfect;
	long bonus,timeleft;
	int	kr,sr,tr;
	char tempstr[10];
	// *** S3DNA  RESTORATION ***
	unsigned long	lasttime;
	times parTimes[]=
	{
	 // *** SHAREWARE V1.0 APOGEE RESTORATION ***
	 //
	 // Episode One Par Times
	 //
	 {0.5,  "00:30"},
	 {1,	"01:00"},
	 {1,	"01:00"},

	 //
	 // Episode Two Par Times
	 //
	 {2,	"02:00"},
	 {1.75,	"01:45"},
	 {1.5,  "01:30"},
	 {1.75,	"01:45"},

	 //
	 // Episode Three Par Times
	 //
	 {2.75,	"01:45"}, // S3DNA RESTORATION - Looks like a typo
	 {2,	"02:00"},
	 {2.25,	"02:15"},
	 {1,	"01:00"},
	 {2,	"02:00"},

	 //
	 // Episode Four Par Times
	 //
	 {3.5,	"03:30"},
	 {2.5,	"02:30"},
	 {4,	"04:00"},
	 {2.25,	"02:15"},
	 {3.5,	"03:30"},

	 //
	 // Episode Five Par Times
	 //
	 {2.5,	"02:30"},
	 {2.75,	"02:45"},
	 {3,	"03:00"},
	 {3.5,	"03:30"},
	 {3,	"03:00"},
	 {3,	"03:00"},

	 //
	 // Episode Six Par Times
	 //
	 {1.75,	"01:45"},
	 {3,	"03:00"},
	 {3,	"03:00"},
	 {1.5,  "01:30"},
	 {2.5,	"02:30"},
	 {3.1900001,	"03:11"},
	 {2.5,	"02:00"} // S3DNA RESTORATION - Another typo
	};



	// *** S3DNA  RESTORATION ***
	StartCPMusic(NOAH05_MUS);
	CacheLump(LEVELEND_LUMP_START,LEVELEND_LUMP_END);
	ClearSplitVWB ();			// set up for double buffering in split screen
	ClearMemory();
	CA_LoadAllSounds();

	switch (gamestate.mapon)
	{
		case 11: secret = 1;
			break;
		case 29: secret = 2;
			break;
		default: secret = 0;
	}
	DrawEndLevelScreen (secret);
	bonus = 0;
	ShowBonus (0);
	IN_ClearKeysDown ();
	IN_StartAck ();

	kr = sr = tr
		= 0;
	if (gamestate.killtotal)
		kr=(gamestate.killcount*100)/gamestate.killtotal;
	if (gamestate.secrettotal)
		sr=(gamestate.secretcount*100)/gamestate.secrettotal;
	if (gamestate.treasuretotal)
		tr=(gamestate.treasurecount*100)/gamestate.treasuretotal;

	if (secret)
	{
		VW_UpdateScreen();
		VW_FadeIn();
		VL_WaitVBL(VBLWAIT);
		SD_PlaySound(PERCENT100SND);
		bonus = 15000;
		ShowBonus(15000);
		set_TimeCount(0);
		IN_StartAck();
		lasttime = get_TimeCount();
		do
		{
			BJ_Breathe();
			VW_UpdateScreen();
			if (IN_CheckAck())
				break;
		} while (get_TimeCount() - lasttime < 2*TickBase);

	
		while (SD_SoundPlaying())
			BJ_Breathe();
	}
	VWB_DrawPic(96,112,W_PARPIC);
	Write(19,14,parTimes[gamestate.mapon].timestr);
	VWB_DrawPic(88,128,W_TIMEPIC);

	//
	// PRINT TIME
	//
	sec=gamestate.TimeCount/70;

	if (sec > 99*60)		// 99 minutes max
		sec = 99*60;

	if (gamestate.TimeCount<parTimes[gamestate.mapon].time*4200)
		timeleft=parTimes[gamestate.mapon].time*4200/70-sec;
	else
		timeleft=0;

	min=sec/60;
	sec%=60;

	i=19*8;
	VWB_DrawPic(i,16*8,L_NUM0PIC+(min/10));
	i+=2*8;
	VWB_DrawPic(i,16*8,L_NUM0PIC+(min%10));
	i+=2*8;
	Write(i/8,16,":");
	i+=1*8;
	VWB_DrawPic(i,16*8,L_NUM0PIC+(sec/10));
	i+=2*8;
	VWB_DrawPic(i,16*8,L_NUM0PIC+(sec%10));

	VW_UpdateScreen ();
	VW_FadeIn ();

	set_TimeCount(0);
	//
	// PRINT TIME BONUS
	//
	if (timeleft)
	{
		for (i=0;i<=timeleft;i++)
		{
			ShowBonus(bonus+(long)i*PAR_AMOUNT);
			SD_PlaySound(D_INCSND);
			VW_UpdateScreen();
			BJ_Breathe();
			if (IN_CheckAck())
				break;
			RollDelay();
		}
		VW_UpdateScreen();
		SD_PlaySound(D_INCSND);
		while (SD_SoundPlaying())
			BJ_Breathe();
	}
	else
		SD_PlaySound(NOBONUSSND);

	bonus+=timeleft*PAR_AMOUNT;
	IN_StartAck();
	lasttime = get_TimeCount();
	do
	{
		BJ_Breathe();
		VW_UpdateScreen();
		if (IN_CheckAck())
			break;
	} while (get_TimeCount() - lasttime < 2*TickBase);


	DrawEndLevelScreen(secret);
	ShowBonus(bonus);
	VWB_DrawPic(80,104,W_KILLSPIC);
	Write(27,13,"0%");
	VWB_DrawPic(104,120,W_TREASURESPIC);
	Write(27,15,"0%");
	VWB_DrawPic(72,136,W_SECRETSPIC);
	Write(27,17,"0%");

	VW_UpdateScreen();
	IN_StartAck();

	#define RATIOXX		29
	for (i=0;i<=kr;i++)
	{
		itoa(i,tempstr,10);
		x=RATIOXX-strlen(tempstr)*2;
		Write(x,13,tempstr);
		SD_PlaySound(D_INCSND);
		VW_UpdateScreen();
		BJ_Breathe();
		if (IN_CheckAck())
			goto done;
		RollDelay();
	}

	if (kr==100)
	{
		VW_WaitVBL(VBLWAIT);
		SD_StopSound();
		bonus+=PERCENT100AMT;
		ShowBonus(bonus);
		SD_PlaySound(PERCENT100SND);
	}
	else if (kr==0)
	{
		VW_WaitVBL(VBLWAIT);
		SD_StopSound();
		SD_PlaySound(NOBONUSSND);
	}
	else
		SD_PlaySound(D_INCSND);

	VW_UpdateScreen();
	while (SD_SoundPlaying())
		BJ_Breathe();

	for (i=0;i<=tr;i++)
	{
		itoa(i,tempstr,10);
		x=RATIOXX-strlen(tempstr)*2;
		Write(x,15,tempstr);
		SD_PlaySound(D_INCSND);
		VW_UpdateScreen();
		BJ_Breathe();
		if (IN_CheckAck())
			goto done;
		RollDelay();
	}

	if (tr==100)
	{
		VW_WaitVBL(VBLWAIT);
		SD_StopSound();
		bonus+=PERCENT100AMT;
		ShowBonus(bonus);
		SD_PlaySound(PERCENT100SND);
	}
	else if (tr==0)
	{
		VW_WaitVBL(VBLWAIT);
		SD_StopSound();
		SD_PlaySound(NOBONUSSND);
	}
	else
		SD_PlaySound(D_INCSND);

	VW_UpdateScreen();
	while (SD_SoundPlaying())
		BJ_Breathe();

	for (i=0;i<=sr;i++)
	{
		itoa(i,tempstr,10);
		x=RATIOXX-strlen(tempstr)*2;
		Write(x,17,tempstr);
		SD_PlaySound(D_INCSND);
		VW_UpdateScreen();
		BJ_Breathe();
		if (IN_CheckAck())
			goto done;
		RollDelay();
	}

	if (sr==100)
	{
		VW_WaitVBL(VBLWAIT);
		SD_StopSound();
		bonus+=PERCENT100AMT;
		ShowBonus(bonus);
		SD_PlaySound(PERCENT100SND);
	}
	else if (sr==0)
	{
		VW_WaitVBL(VBLWAIT);
		SD_StopSound();
		SD_PlaySound(NOBONUSSND);
	}
	else
		SD_PlaySound(D_INCSND);

	VW_UpdateScreen();
	while (SD_SoundPlaying())
		BJ_Breathe();

	//
	// JUMP STRAIGHT HERE IF KEY PRESSED
	//
	done:

	perfect = (kr==100)&&(tr==100)&&(sr==100);

	itoa(kr,tempstr,10);
	x=RATIOXX-strlen(tempstr)*2;
	Write(x,13,tempstr);

	itoa(tr,tempstr,10);
	x=RATIOXX-strlen(tempstr)*2;
	Write(x,15,tempstr);

	itoa(sr,tempstr,10);
	x=RATIOXX-strlen(tempstr)*2;
	Write(x,17,tempstr);

	//
	// SAVE RATIO INFORMATION FOR ENDGAME
	//
	LevelRatios[gamestate.mapon].kill=kr;
	LevelRatios[gamestate.mapon].secret=sr;
	LevelRatios[gamestate.mapon].treasure=tr;
	LevelRatios[gamestate.mapon].time=min*60+sec;
	LevelRatios[gamestate.mapon].par=parTimes[gamestate.mapon].time*(4200/70);


	GivePoints(bonus);
	ShowBonus(bonus);
	DrawScore();
	VW_UpdateScreen();


	if (perfect)
	{
		DrawEndLevelScreen(secret);
		ShowBonus(bonus);
		VWB_DrawPic(96,120,W_PERFECTPIC);
		VW_UpdateScreen();
		SD_StopSound();
		SD_PlaySound(PERCENT100SND);
	}
	IN_StartAck();
	set_TimeCount(0);
	while (!IN_CheckAck())
		BJ_Breathe();


	VW_FadeOut();
	DrawPlayBorder ();

	UnCacheLump(LEVELEND_LUMP_START,LEVELEND_LUMP_END);

	// PSP_DEVIATION: the audio chunks are not freed here
	SD_WaitSoundDone();

}
#else
void LevelCompleted()
{
#define VBLWAIT 30
#define PAR_AMOUNT 500
#define PERCENT100AMT 10000
	typedef struct
	{
		float time;
		char timestr[6];
	} times;

	int x, i, min, sec, ratio, kr, sr, tr;

	char tempstr[10];
	long bonus, timeleft = 0;
	times parTimes[] =
		{
#ifndef SPEAR
			//
			// Episode One Par Times
			//
			{1.5, "01:30"},
			{2, "02:00"},
			{2, "02:00"},
			{3.5, "03:30"},
			{3, "03:00"},
			{3, "03:00"},
			{2.5, "02:30"},
			{2.5, "02:30"},
			{0, "??:??"}, // Boss level
			{0, "??:??"}, // Secret level

			//
			// Episode Two Par Times
			//
			{1.5, "01:30"},
			{3.5, "03:30"},
			{3, "03:00"},
			{2, "02:00"},
			{4, "04:00"},
			{6, "06:00"},
			{1, "01:00"},
			{3, "03:00"},
			{0, "??:??"},
			{0, "??:??"},

			//
			// Episode Three Par Times
			//
			{1.5, "01:30"},
			{1.5, "01:30"},
			{2.5, "02:30"},
			{2.5, "02:30"},
			{3.5, "03:30"},
			{2.5, "02:30"},
			{2, "02:00"},
			{6, "06:00"},
			{0, "??:??"},
			{0, "??:??"},

			//
			// Episode Four Par Times
			//
			{2, "02:00"},
			{2, "02:00"},
			{1.5, "01:30"},
			{1, "01:00"},
			{4.5, "04:30"},
			{3.5, "03:30"},
			{2, "02:00"},
			{4.5, "04:30"},
			{0, "??:??"},
			{0, "??:??"},

			//
			// Episode Five Par Times
			//
			{2.5, "02:30"},
			{1.5, "01:30"},
			{2.5, "02:30"},
			{2.5, "02:30"},
			{4, "04:00"},
			{3, "03:00"},
			{4.5, "04:30"},
			{3.5, "03:30"},
			{0, "??:??"},
			{0, "??:??"},

			//
			// Episode Six Par Times
			//
			{6.5, "06:30"},
			{4, "04:00"},
			{4.5, "04:30"},
			{6, "06:00"},
			{5, "05:00"},
			{5.5, "05:30"},
			{5.5, "05:30"},
			{8.5, "08:30"},
			{0, "??:??"},
			{0, "??:??"}
#else
			//
			// SPEAR OF DESTINY TIMES
			//
			{1.5, "01:30"},
			{3.5, "03:30"},
			{2.75, "02:45"},
			{3.5, "03:30"},
			{0, "??:??"}, // Boss 1
			{4.5, "04:30"},
			{3.25, "03:15"},
			{2.75, "02:45"},
			{4.75, "04:45"},
			{0, "??:??"}, // Boss 2
			{6.5, "06:30"},
			{4.5, "04:30"},
			{2.75, "02:45"},
			{4.5, "04:30"},
			{6, "06:00"},
			{0, "??:??"}, // Boss 3
			{6, "06:00"},
			{0, "??:??"}, // Boss 4
			{0, "??:??"}, // Secret level 1
			{0, "??:??"}, // Secret level 2
#endif
		};

	CacheLump(LEVELEND_LUMP_START, LEVELEND_LUMP_END);
	ClearSplitVWB(); // set up for double buffering in split screen
	VW_Bar(0, 0, 320, 200 - STATUSLINES, 127);
	StartCPMusic(ENDLEVEL_MUS);

	//
	// do the intermission
	//
	IN_ClearKeysDown();
	IN_StartAck();

	VWB_DrawPic(0, 16, L_GUYPIC);

#ifndef SPEAR
	if (mapon < 8)
#else
	if (mapon != 4 &&
		mapon != 9 &&
		mapon != 15 &&
		mapon < 17)
#endif
	{
		Write(14, 2, "floor\ncompleted");

		Write(14, 7, STR_BONUS "     0");
		Write(16, 10, STR_TIME);
		Write(16, 12, STR_PAR);

		Write(9, 14, STR_RAT2KILL);
		Write(5, 16, STR_RAT2SECRET);
		Write(1, 18, STR_RAT2TREASURE);

		Write(26, 2, itoa(gamestate.mapon + 1, tempstr, 10));

		Write(26, 12, parTimes[gamestate.episode * 10 + mapon].timestr);

		//
		// PRINT TIME
		//
		sec = gamestate.TimeCount / 70;

		if (sec > 99 * 60) // 99 minutes max
			sec = 99 * 60;

		if (gamestate.TimeCount < parTimes[gamestate.episode * 10 + mapon].time * 4200)
			timeleft = (parTimes[gamestate.episode * 10 + mapon].time * 4200) / 70 - sec;

		min = sec / 60;
		sec %= 60;

		i = 26 * 8;
		VWB_DrawPic(i, 10 * 8, L_NUM0PIC + (min / 10));
		i += 2 * 8;
		VWB_DrawPic(i, 10 * 8, L_NUM0PIC + (min % 10));
		i += 2 * 8;
		Write(i / 8, 10, ":");
		i += 1 * 8;
		VWB_DrawPic(i, 10 * 8, L_NUM0PIC + (sec / 10));
		i += 2 * 8;
		VWB_DrawPic(i, 10 * 8, L_NUM0PIC + (sec % 10));

		VW_UpdateScreen();
		VW_FadeIn();

		//
		// FIGURE RATIOS OUT BEFOREHAND
		//
		kr = sr = tr = 0;
		if (gamestate.killtotal)
			kr = (gamestate.killcount * 100) / gamestate.killtotal;
		if (gamestate.secrettotal)
			sr = (gamestate.secretcount * 100) / gamestate.secrettotal;
		if (gamestate.treasuretotal)
			tr = (gamestate.treasurecount * 100) / gamestate.treasuretotal;

		//
		// PRINT TIME BONUS
		//
		bonus = timeleft * PAR_AMOUNT;
		if (bonus)
		{
			for (i = 0; i <= timeleft; i++)
			{
				ltoa((long)i * PAR_AMOUNT, tempstr, 10);
				x = 36 - strlen(tempstr) * 2;
				Write(x, 7, tempstr);
				if (!(i % (PAR_AMOUNT / 10)))
					SD_PlaySound(ENDBONUS1SND);
				VW_UpdateScreen();
				while (SD_SoundPlaying())
					BJ_Breathe();
				if (IN_CheckAck())
					goto done;
			}

			VW_UpdateScreen();
			SD_PlaySound(ENDBONUS2SND);
			while (SD_SoundPlaying())
				BJ_Breathe();
		}

#define RATIOXX 37
		//
		// KILL RATIO
		//
		ratio = kr;
		for (i = 0; i <= ratio; i++)
		{
			itoa(i, tempstr, 10);
			x = RATIOXX - strlen(tempstr) * 2;
			Write(x, 14, tempstr);
			if (!(i % 10))
				SD_PlaySound(ENDBONUS1SND);
			VW_UpdateScreen();
			while (SD_SoundPlaying())
				BJ_Breathe();

			if (IN_CheckAck())
				goto done;
		}
		if (ratio == 100)
		{
			VW_WaitVBL(VBLWAIT);
			SD_StopSound();
			bonus += PERCENT100AMT;
			ltoa(bonus, tempstr, 10);
			x = (RATIOXX - 1) - strlen(tempstr) * 2;
			Write(x, 7, tempstr);
			VW_UpdateScreen();
			SD_PlaySound(PERCENT100SND);
		}
		else if (!ratio)
		{
			VW_WaitVBL(VBLWAIT);
			SD_StopSound();
			SD_PlaySound(NOBONUSSND);
		}
		else
			SD_PlaySound(ENDBONUS2SND);

		VW_UpdateScreen();
		while (SD_SoundPlaying())
			BJ_Breathe();

		//
		// SECRET RATIO
		//
		ratio = sr;
		for (i = 0; i <= ratio; i++)
		{
			itoa(i, tempstr, 10);
			x = RATIOXX - strlen(tempstr) * 2;
			Write(x, 16, tempstr);
			if (!(i % 10))
				SD_PlaySound(ENDBONUS1SND);
			VW_UpdateScreen();
			while (SD_SoundPlaying())
				BJ_Breathe();
			BJ_Breathe();

			if (IN_CheckAck())
				goto done;
		}
		if (ratio == 100)
		{
			VW_WaitVBL(VBLWAIT);
			SD_StopSound();
			bonus += PERCENT100AMT;
			ltoa(bonus, tempstr, 10);
			x = (RATIOXX - 1) - strlen(tempstr) * 2;
			Write(x, 7, tempstr);
			VW_UpdateScreen();
			SD_PlaySound(PERCENT100SND);
		}
		else if (!ratio)
		{
			VW_WaitVBL(VBLWAIT);
			SD_StopSound();
			SD_PlaySound(NOBONUSSND);
		}
		else
			SD_PlaySound(ENDBONUS2SND);
		VW_UpdateScreen();
		while (SD_SoundPlaying())
			BJ_Breathe();

		//
		// TREASURE RATIO
		//
		ratio = tr;
		for (i = 0; i <= ratio; i++)
		{
			itoa(i, tempstr, 10);
			x = RATIOXX - strlen(tempstr) * 2;
			Write(x, 18, tempstr);
			if (!(i % 10))
				SD_PlaySound(ENDBONUS1SND);
			VW_UpdateScreen();
			while (SD_SoundPlaying())
				BJ_Breathe();
			if (IN_CheckAck())
				goto done;
		}
		if (ratio == 100)
		{
			VW_WaitVBL(VBLWAIT);
			SD_StopSound();
			bonus += PERCENT100AMT;
			ltoa(bonus, tempstr, 10);
			x = (RATIOXX - 1) - strlen(tempstr) * 2;
			Write(x, 7, tempstr);
			VW_UpdateScreen();
			SD_PlaySound(PERCENT100SND);
		}
		else if (!ratio)
		{
			VW_WaitVBL(VBLWAIT);
			SD_StopSound();
			SD_PlaySound(NOBONUSSND);
		}
		else
			SD_PlaySound(ENDBONUS2SND);
		VW_UpdateScreen();
		while (SD_SoundPlaying())
			BJ_Breathe();

	//
	// JUMP STRAIGHT HERE IF KEY PRESSED
	//
	done:

		itoa(kr, tempstr, 10);
		x = RATIOXX - strlen(tempstr) * 2;
		Write(x, 14, tempstr);

		itoa(sr, tempstr, 10);
		x = RATIOXX - strlen(tempstr) * 2;
		Write(x, 16, tempstr);

		itoa(tr, tempstr, 10);
		x = RATIOXX - strlen(tempstr) * 2;
		Write(x, 18, tempstr);

		bonus = (long)timeleft * PAR_AMOUNT +
				(PERCENT100AMT * (kr == 100)) +
				(PERCENT100AMT * (sr == 100)) +
				(PERCENT100AMT * (tr == 100));

		GivePoints(bonus);
		ltoa(bonus, tempstr, 10);
		x = 36 - strlen(tempstr) * 2;
		Write(x, 7, tempstr);

		//
		// SAVE RATIO INFORMATION FOR ENDGAME
		//
		LevelRatios[mapon].kill = kr;
		LevelRatios[mapon].secret = sr;
		LevelRatios[mapon].treasure = tr;
		LevelRatios[mapon].time = min * 60 + sec;
	}
	else
	{
#ifdef SPEAR
#ifndef SPEARDEMO
		switch (mapon)
		{
		case 4:
			Write(14, 4, " trans\n"
						 " grosse\n" STR_DEFEATED);
			break;
		case 9:
			Write(14, 4, "barnacle\n"
						 "wilhelm\n" STR_DEFEATED);
			break;
		case 15:
			Write(14, 4, "ubermutant\n" STR_DEFEATED);
			break;
		case 17:
			Write(14, 4, " death\n"
						 " knight\n" STR_DEFEATED);
			break;
		case 18:
			Write(13, 4, "secret tunnel\n"
						 "    area\n"
						 "  completed!");
			break;
		case 19:
			Write(13, 4, "secret castle\n"
						 "    area\n"
						 "  completed!");
			break;
		}
#endif
#else
		Write(14, 4, "secret floor\n completed!");
#endif

		Write(10, 16, "15000 bonus!");

		VW_UpdateScreen();
		VW_FadeIn();

		GivePoints(15000);
	}

	DrawScore();
	VW_UpdateScreen();

	set_TimeCount(0);
	IN_StartAck();
	while (!IN_CheckAck())
		BJ_Breathe();

//
// done
//
#ifdef SPEARDEMO
	if (gamestate.mapon == 1)
	{
		SD_PlaySound(BONUS1UPSND);

		CA_CacheGrChunk(STARTFONT + 1);
		Message("This concludes your demo\n"
				"of Spear of Destiny! Now,\n"
				"go to your local software\n"
				"store and buy it!");
		CA_UnCacheGrChunk(STARTFONT + 1);

		IN_ClearKeysDown();
		IN_Ack();
	}
#endif

#ifndef SPEAR
	if (IN_KeyDown(sc_P) && MS_CheckParm("debugmode"))
		PicturePause();
#endif

	VW_FadeOut();
	DrawPlayBorder();

	UnCacheLump(LEVELEND_LUMP_START, LEVELEND_LUMP_END);
}
#endif /* GAMEVER_NOAH3D */

//==========================================================================

/*
=================
=
= PreloadGraphics
=
=================
*/

#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION
int	FloorTile[] = {0,0,2,1,1,1,0,2,2,2,1,3,0,0,0,0,2,1,1,1,1,1,0,2,2,2,2,2,1,3,3};

char	mission1text[] = {
	"You'll be out of the ark in six\n"
	"days, Noah. Unfortunately, the\n"
	"animals are a tad bit restless and\n"
	"want to get out now. Good thing\n"
	"you brought all that food with\n"
	"you. You'll need it to put the\n"
	"busy ones to sleep.\n"
	"\n"
	"At the end of the first day, be\n"
	"prepared to deal with Carl the\n"
	"Camel. He's been real cranky lately\n"
	"and is a bit out of control.\n"
	"\n"
	"Good luck and be careful...\n"
},

mission2text[] = {
	"Wow, Noah! Carl sure was hard to\n"
	"calm down. But remember, you\n"
	"were chosen to guide this ark to\n"
	"safety because you know how to\n"
	"get the job done.\n"
	"\n"
	"The closing of day two ends with\n"
	"irritable Ginny the Giraffe. She\n"
	"may be tall and quick, but give\n"
	"her enough food and it's off to\n"
	"sleep.\n"
	"\n"
	"Oh, and Noah, remember to keep\n"
	"an eye out for hidden rooms...\n"
},

mission3text[] = {
	"ZZZZZ... Great job, Noah!. Ginny is\n"
	"snoring away.\n"
	"\n"
	"Boy, it seems the animals never\n"
	"seem to stay asleep very long.\n"
	"\n"
	"Keep your eye out for Melvin the\n"
	"Monkey. He can be pretty tricky\n"
	"and may try distracting you with\n"
	"coconuts, but it's your job to get\n"
	"him settled down for the rest of\n"
	"your time aboard the ark."
},

mission4text[] = {
	"You're doing a great job, Noah!\n"
	"You took care of that Melvin like\n"
	"a real pro.\n"
	"\n"
	"I hope you haven't worn yourself\n"
	"out yet, because more challenges\n"
	"are ahead.\n"
	"\n"
	"Kerry the Kangaroo awaits you,\n"
	"and she doesn't look the least\n"
	"bit tired. Are you ready for her?\n"
	"She needs to rest like the other\n"
	"animals."
},

mission5text[] = {
	"Wow! Kerry was no challenge for\n"
	"you! Can you keep up the pace\n"
	"when faced with Ernie the\n"
	"Elephant?\n"
	"\n"
	"Don't worry. Very soon you will\n"
	"be on dry land, and you won't\n"
	"have to chase the animals around\n"
	"the ark anymore."
},

mission6text[] = {
	"You are almost there! If you can\n"
	"get through one more day, you\n"
	"will be out of here for good!\n"
	"\n"
	"Rumor has it one of the bears\n"
	"does not want to be found. In\n"
	"fact he may try to hide from\n"
	"you. Be on the alert for Burt\n"
	"the Bear.\n"
	"\n"
	"Get him to sleep and your work\n"
	"is done!"
},

creditstext[] = {
	"\n"
	"Super 3-D Noah's Ark\n"
	"\n"
	"\n"
	"(C)1995 Wisdom Tree, Inc.\n"
	"All rights reserved.\n"
	"\n"
	"\n"
	"This game contains copyrighted\n"
	"software code owned by\n"
	"Id Software, Inc\n"
	"(C)1992 Id Software, Inc.\n"
	"All rights reserved.\n"
};


char *missiontext[] =
	{mission1text,mission2text,mission3text,mission4text,mission5text,mission6text,creditstext};



void Briefing (int mission, boolean credits)
{
	ClearMemory ();
	if (mission >= 7)
	{
		sprintf (str,"Briefing(): Invalid briefing number %d!",mission);
		Quit (str);
	}
	CacheLump (LEVELEND_LUMP_START,LEVELEND_LUMP_END);
	ClearSplitVWB ();
	ClearMScreen ();
	CA_CacheGrChunk (STARTFONT+1);
	fontnumber = 1;
	WindowX = PrintX = 8;
	WindowY = PrintY = 8;
	WindowW = 320-16;
	WindowH = 200-16;
	SETFONTCOLOR (READHCOLOR,BKGDCOLOR);

	if (credits)
		US_CPrint (missiontext[mission]);
	else
		US_Print (missiontext[mission]);

	VW_UpdateScreen ();
	VW_FadeIn ();

	if (credits)
		UnCacheLump (LEVELEND_LUMP_START,LEVELEND_LUMP_END);
	else
	{
		StartCPMusic (NOAH09_MUS);
		IN_ClearKeysDown();
		IN_Ack();
		StopMusic();
		UnCacheLump (LEVELEND_LUMP_START,LEVELEND_LUMP_END);
		DrawPlayScreen();
	}
	CA_UnCacheGrChunk (STARTFONT+1);
	ClearMemory ();
}
#endif /* GAMEVER_NOAH3D */

#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION
boolean PreloadUpdate(unsigned current, unsigned total)
{
	unsigned w = WindowW - 10;


	// *** S3DNA RESTORATION ***
	w = ((long)w * current) / total;
	if (w)
	{
	 // *** S3DNA RESTORATION ***
	 VW_Bar(WindowX + 5,WindowY + WindowH,w,2,0x55);
	 VW_Bar(WindowX + 5,WindowY + WindowH,w-1,1,0x52);

	}
	VW_UpdateScreen();
		return(false);
}
#else
boolean PreloadUpdate(int current, int total)
{
	int w = WindowW - 10;

	VW_Bar(WindowX + 5, WindowY + WindowH - 3, w, 2, BLACK);
	w = (w * current) / total;
	if (w)
	{
		VW_Bar(WindowX + 5, WindowY + WindowH - 3, w, 2, 0x37);
		VW_Bar(WindowX + 5, WindowY + WindowH - 3, w - 1, 1, 0x32);
	}
	VW_UpdateScreen();

	return false;
}
#endif /* GAMEVER_NOAH3D */

void PreloadGraphics()
{
	int i;

	DrawLevel();
	ClearSplitVWB();

#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION
	VW_Bar(0, 0, 320, 200 - STATUSLINES, VIEWCOLOR);
#else
	VW_Bar(0, 0, 320, 200 - STATUSLINES, 127);
#endif

	VWB_DrawPic(48, 56, GETPSYCHEDPIC);

	WindowX = 160 - 14 * 8;
	WindowY = 80 - 3 * 8;
	WindowW = 28 * 8;
	WindowH = 48;
	VW_UpdateScreen();
	VW_FadeIn();

	for (i = 0; i <= 40; i++)
		PreloadUpdate(i, 50);
	PreloadUpdate(50, 50);

#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION
	LoadFloorTiles(FloorTile[gamestate.mapon]);
#endif
	IN_UserInput(70);
	VW_FadeOut();

	DrawPlayBorder();
	VW_UpdateScreen();
}

//==========================================================================

/*
==================
=
= DrawHighScores
=
==================
*/

void DrawHighScores()
{
	char buffer[16];
	char buffer1[16], *str;
	word i, w, h;
	HighScore *s;

	buffer1[0] = 0;
	str = 0;
	(void)buffer1; // PSP_DEVIATION: silence set-but-unused
	(void)str;

#ifndef SPEAR
	CA_CacheGrChunk(HIGHSCORESPIC);
	CA_CacheGrChunk(STARTFONT);
	CA_CacheGrChunk(C_LEVELPIC);
	CA_CacheGrChunk(C_SCOREPIC);
	CA_CacheGrChunk(C_NAMEPIC);

	ClearMScreen();
	DrawStripes(10);

	VWB_DrawPic(48, 0, HIGHSCORESPIC);
	CA_UnCacheGrChunk(HIGHSCORESPIC);

	VWB_DrawPic(4 * 8, 68, C_NAMEPIC);
#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION
	VWB_DrawPic(21 * 8, 68, C_LEVELPIC);
	VWB_DrawPic(32 * 8, 68, C_SCOREPIC);
#else
	VWB_DrawPic(20 * 8, 68, C_LEVELPIC);
	VWB_DrawPic(28 * 8, 68, C_SCOREPIC);
#endif

	fontnumber = 0;
#else
	CacheLump(BACKDROP_LUMP_START, BACKDROP_LUMP_END);
	ClearMScreen();
	DrawStripes(10);
	UnCacheLump(BACKDROP_LUMP_START, BACKDROP_LUMP_END);

	CacheLump(HIGHSCORES_LUMP_START, HIGHSCORES_LUMP_END);
	CA_CacheGrChunk(STARTFONT + 1);
	VWB_DrawPic(0, 0, HIGHSCORESPIC);

	fontnumber = 1;
#endif

#ifndef SPEAR
	SETFONTCOLOR(15, 0x29);
#else
	SETFONTCOLOR(HIGHLIGHT, 0x29);
#endif

	for (i = 0, s = Scores; i < MaxScores; i++, s++)
	{
		PrintY = 76 + (16 * i);

		//
		// name
		//
#ifndef SPEAR
		PrintX = 4 * 8;
#else
		PrintX = 16;
#endif
		US_Print(s->name);

		//
		// level
		//
#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION - show as episode-level
		if (s->completed >= 30)
			s->completed = 30;
		sprintf(buffer, "%d-%d", MapEpisode[s->completed], MapLevel[s->completed]);
		USL_MeasureString(buffer, &w, &h);
		PrintX = (24 * 8) - w;
#else
		ultoa(s->completed, buffer, 10);
#endif
#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION
#elif !defined(SPEAR)
		for (str = buffer; *str; str++)
			*str = *str + (129 - '0'); // Used fixed-width numbers (129...)
		USL_MeasureString(buffer, &w, &h);
		PrintX = (22 * 8) - w;
#else
		USL_MeasureString(buffer, &w, &h);
		PrintX = 194 - w;
#endif

#if (!defined(UPLOAD) || !defined(SPEAR)) && !defined(GAMEVER_NOAH3D) // PSP_DEVIATION: S3DNA RESTORATION
		PrintX -= 6;
		itoa(s->episode + 1, buffer1, 10);
		US_Print("E");
		US_Print(buffer1);
		US_Print("/L");
#endif

#ifdef SPEAR
		if (s->completed == 21)
			VWB_DrawPic(PrintX + 8, PrintY - 1, C_WONSPEARPIC);
		else
#endif
			US_Print(buffer);

		//
		// score
		//
		ultoa(s->score, buffer, 10);
#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION
		USL_MeasureString(buffer, &w, &h);
		PrintX = 288 - w;
#elif !defined(SPEAR)
		for (str = buffer; *str; str++)
			*str = *str + (129 - '0'); // Used fixed-width numbers (129...)
		USL_MeasureString(buffer, &w, &h);
		PrintX = (34 * 8) - 8 - w;
#else
		USL_MeasureString(buffer, &w, &h);
		PrintX = 292 - w;
#endif
		US_Print(buffer);
	}

	VW_UpdateScreen();

#ifdef SPEAR
	UnCacheLump(HIGHSCORES_LUMP_START, HIGHSCORES_LUMP_END);
	fontnumber = 0;
#endif
}

/*
=======================
=
= CheckHighScore
=
=======================
*/

void CheckHighScore(long score, word other)
{
	word i, j;
	int n;
	HighScore myscore;

	// PSP has no text entry: use a default name
	strcpy(myscore.name, "PSP Enjoyer");
	myscore.score = score;
#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION
	myscore.episode = gamestate.mapon;
#else
	myscore.episode = gamestate.episode;
#endif
	myscore.completed = other;

	for (i = 0, n = -1; i < MaxScores; i++)
	{
		if ((myscore.score > Scores[i].score) ||
			((myscore.score == Scores[i].score) &&
			 (myscore.completed > Scores[i].completed)))
		{
			for (j = MaxScores; --j > i;)
				Scores[j] = Scores[j - 1];
			Scores[i] = myscore;
			n = i;
			break;
		}
	}

#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION
	StartCPMusic(NOAH11_MUS);
#elif defined(SPEAR)
	StartCPMusic(XAWARD_MUS);
#else
	StartCPMusic(ROSTER_MUS);
#endif
	DrawHighScores();

	VW_FadeIn();

	// PSP has no text entry: a qualifying score is recorded under the default
	// name "PSP Enjoyer", and we wait the same as for a non-qualifying score.
	IN_ClearKeysDown();
	IN_UserInput(500);
}
