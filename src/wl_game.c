#include "include/wl_def.h"

/*
=============================================================================

						 GLOBAL VARIABLES

=============================================================================
*/

boolean ingame;
gametype gamestate;

#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION
int endtics, fizzlein;
#endif
long spearx, speary;
unsigned spearangle;
boolean spearflag;

/* ELEVATOR BACK MAPS - REMEMBER (-1)!! */
#if !defined(SPEAR) && !defined(GAMEVER_NOAH3D) // PSP_DEVIATION: S3DNA RESTORATION - no episodes
static int ElevatorBackTo[] = {1, 1, 7, 3, 5, 3};
#endif

//===========================================================================

/*
==========================
=
= ClearMemory
=
==========================
*/

void ClearMemory()
{
	MM_SortMem();
}

/*
==========================
=
= ScanInfoPlane
=
= Spawn all actors and mark down special places
=
==========================
*/

#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION
void ScanInfoPlane (void)
{
	unsigned	x,y; // PSP_DEVIATION: removed unused 'i','j'
	int			tile;
	word *start;

	start = mapsegs[1];
	for (y=0;y<mapheight;y++)
		for (x=0;x<mapwidth;x++)
		{
			tile = *start++;
			if (!tile)
				continue;

			switch (tile)
			{
			case 19:
			case 20:
			case 21:
			case 22:
				SpawnPlayer(x,y,NORTH+tile-19);
				break;

			case 23:
			case 24:
			case 25:
			case 26:
			case 27:
			case 28:
			case 29:
			case 30:

			case 31:
			case 32:
			case 33:
			case 34:
			case 35:
			case 36:
			case 37:
			case 38:

			case 39:
			case 40:
			case 41:
			case 42:
			case 43:
			case 44:
			case 45:
			case 46:

			case 47:
			case 48:
			case 49:
			case 50:
			case 51:
			case 52:
			case 53:
			case 54:

			case 55:
			case 56:
			case 57:
			case 58:
			case 59:
			case 60:
			case 61:
			case 62:

// *** ALPHA RESTORATION ***
			case 63:
			case 64:
			case 65:
			case 66:
			case 67:
			case 68:
			case 69:
			case 70:
// *** PRE-V1.4 APOGEE RESTORATION ***
			case 71:
			case 72:
			case 73:						// TRUCK AND SPEAR!
			case 74:

				SpawnStatic(x,y,tile-23);
				break;
// *** ALPHA RESTORATION ***
// Apparently we need the redundant break for code recreation

//
// P wall
//
			case 98:
				if (!loadedgame)
				  gamestate.secrettotal++;
				break;

			// *** S3DNA RESTORATION ***
			case 99:
				*(start-1) = 0;
				break;
			case 100:
			case 101:
				SpawnExit(x,y,tile==101);
				break;

//
// guard
//
			// *** S3DNA RESTORATION ***
			case 108:
			case 126:
				SpawnStand(en_guard,x,y,0,tile==126);
				break;
//
// officer
//
			// *** S3DNA RESTORATION ***
			case 109:
			case 127:
				SpawnStand(en_officer,x,y,0,tile==127);
				break;


//
// ss
//
			// *** S3DNA RESTORATION ***
			case 110:
			case 128:
				SpawnStand(en_ss,x,y,0,tile==128);
				break;

//
// dogs
//
			// *** S3DNA RESTORATION ***
			case 111:
			case 129:
				SpawnStand(en_dog,x,y,0,tile==129);
				break;

			// *** S3DNA RESTORATION ***
//
// mutants
//
			case 112:
			case 130:
				SpawnStand(en_mutant,x,y,0,tile==130);
				break;
//
// boss
//
			// *** S3DNA RESTORATION ***
			case 113:
			case 131:
				SpawnBoss (x,y);
				break;
			case 114:
			case 132:
				SpawnSchabbs (x,y);
				break;
			case 115:
			case 133:
				SpawnGretel (x,y);
				break;
			case 116:
			case 134:
				SpawnGift (x,y);
				break;
			case 117:
			case 135:
				SpawnFat (x,y);
				break;
			case 118:
			case 136:
				SpawnHitler (x,y);
				break;

			// *** S3DNA + ALPHA RESTORATION ***
			default:
				sprintf (str,"ScanInfoPlane: Invalid object $%02X at %d, %d!\n",
					tile,x,y);
				Quit (str);
			}

		}
}
#else
void ScanInfoPlane()
{
	unsigned x, y;
	int tile;
	word *start;

	start = mapsegs[1];
	for (y = 0; y < mapheight; y++)
		for (x = 0; x < mapwidth; x++)
		{
			tile = *start++;
			if (!tile)
				continue;

			switch (tile)
			{
			case 19:
			case 20:
			case 21:
			case 22:
				SpawnPlayer(x, y, tile - 19);
				break;

			case 23:
			case 24:
			case 25:
			case 26:
			case 27:
			case 28:
			case 29:
			case 30:

			case 31:
			case 32:
			case 33:
			case 34:
			case 35:
			case 36:
			case 37:
			case 38:

			case 39:
			case 40:
			case 41:
			case 42:
			case 43:
			case 44:
			case 45:
			case 46:

			case 47:
			case 48:
			case 49:
			case 50:
			case 51:
			case 52:
			case 53:
			case 54:

			case 55:
			case 56:
			case 57:
			case 58:
			case 59:
			case 60:
			case 61:
			case 62:

			case 63:
			case 64:
			case 65:
			case 66:
			case 67:
			case 68:
			case 69:
			case 70:
			case 71:
			case 72:
			case 73: // TRUCK AND SPEAR!
			case 74:

				SpawnStatic(x, y, tile - 23);
				break;

				//
				// P wall
				//
			case 98:
				if (!loadedgame)
					gamestate.secrettotal++;
				break;

				//
				// guard
				//
			case 180:
			case 181:
			case 182:
			case 183:
				if (gamestate.difficulty < gd_hard)
					break;
				tile -= 36;
			case 144:
			case 145:
			case 146:
			case 147:
				if (gamestate.difficulty < gd_medium)
					break;
				tile -= 36;
			case 108:
			case 109:
			case 110:
			case 111:
				SpawnStand(en_guard, x, y, tile - 108);
				break;

			case 184:
			case 185:
			case 186:
			case 187:
				if (gamestate.difficulty < gd_hard)
					break;
				tile -= 36;
			case 148:
			case 149:
			case 150:
			case 151:
				if (gamestate.difficulty < gd_medium)
					break;
				tile -= 36;
			case 112:
			case 113:
			case 114:
			case 115:
				SpawnPatrol(en_guard, x, y, tile - 112);
				break;

			case 124:
				SpawnDeadGuard(x, y);
				break;
				//
				// officer
				//
			case 188:
			case 189:
			case 190:
			case 191:
				if (gamestate.difficulty < gd_hard)
					break;
				tile -= 36;
			case 152:
			case 153:
			case 154:
			case 155:
				if (gamestate.difficulty < gd_medium)
					break;
				tile -= 36;
			case 116:
			case 117:
			case 118:
			case 119:
				SpawnStand(en_officer, x, y, tile - 116);
				break;

			case 192:
			case 193:
			case 194:
			case 195:
				if (gamestate.difficulty < gd_hard)
					break;
				tile -= 36;
			case 156:
			case 157:
			case 158:
			case 159:
				if (gamestate.difficulty < gd_medium)
					break;
				tile -= 36;
			case 120:
			case 121:
			case 122:
			case 123:
				SpawnPatrol(en_officer, x, y, tile - 120);
				break;

				//
				// ss
				//
			case 198:
			case 199:
			case 200:
			case 201:
				if (gamestate.difficulty < gd_hard)
					break;
				tile -= 36;
			case 162:
			case 163:
			case 164:
			case 165:
				if (gamestate.difficulty < gd_medium)
					break;
				tile -= 36;
			case 126:
			case 127:
			case 128:
			case 129:
				SpawnStand(en_ss, x, y, tile - 126);
				break;

			case 202:
			case 203:
			case 204:
			case 205:
				if (gamestate.difficulty < gd_hard)
					break;
				tile -= 36;
			case 166:
			case 167:
			case 168:
			case 169:
				if (gamestate.difficulty < gd_medium)
					break;
				tile -= 36;
			case 130:
			case 131:
			case 132:
			case 133:
				SpawnPatrol(en_ss, x, y, tile - 130);
				break;

				//
				// dogs
				//
			case 206:
			case 207:
			case 208:
			case 209:
				if (gamestate.difficulty < gd_hard)
					break;
				tile -= 36;
			case 170:
			case 171:
			case 172:
			case 173:
				if (gamestate.difficulty < gd_medium)
					break;
				tile -= 36;
			case 134:
			case 135:
			case 136:
			case 137:
				SpawnStand(en_dog, x, y, tile - 134);
				break;

			case 210:
			case 211:
			case 212:
			case 213:
				if (gamestate.difficulty < gd_hard)
					break;
				tile -= 36;
			case 174:
			case 175:
			case 176:
			case 177:
				if (gamestate.difficulty < gd_medium)
					break;
				tile -= 36;
			case 138:
			case 139:
			case 140:
			case 141:
				SpawnPatrol(en_dog, x, y, tile - 138);
				break;

//
// boss
//
#ifndef SPEAR
			case 214:
				SpawnBoss(x, y);
				break;
			case 197:
				SpawnGretel(x, y);
				break;
			case 215:
				SpawnGift(x, y);
				break;
			case 179:
				SpawnFat(x, y);
				break;
			case 196:
				SpawnSchabbs(x, y);
				break;
			case 160:
				SpawnFakeHitler(x, y);
				break;
			case 178:
				SpawnHitler(x, y);
				break;
#else
			case 106:
				SpawnSpectre(x, y);
				break;
			case 107:
				SpawnAngel(x, y);
				break;
			case 125:
				SpawnTrans(x, y);
				break;
			case 142:
				SpawnUber(x, y);
				break;
			case 143:
				SpawnWill(x, y);
				break;
			case 161:
				SpawnDeath(x, y);
				break;

#endif

				//
				// mutants
				//
			case 252:
			case 253:
			case 254:
			case 255:
				if (gamestate.difficulty < gd_hard)
					break;
				tile -= 18;
			case 234:
			case 235:
			case 236:
			case 237:
				if (gamestate.difficulty < gd_medium)
					break;
				tile -= 18;
			case 216:
			case 217:
			case 218:
			case 219:
				SpawnStand(en_mutant, x, y, tile - 216);
				break;

			case 256:
			case 257:
			case 258:
			case 259:
				if (gamestate.difficulty < gd_hard)
					break;
				tile -= 18;
			case 238:
			case 239:
			case 240:
			case 241:
				if (gamestate.difficulty < gd_medium)
					break;
				tile -= 18;
			case 220:
			case 221:
			case 222:
			case 223:
				SpawnPatrol(en_mutant, x, y, tile - 220);
				break;

//
// ghosts
//
#ifndef SPEAR
			case 224:
				SpawnGhosts(en_blinky, x, y);
				break;
			case 225:
				SpawnGhosts(en_clyde, x, y);
				break;
			case 226:
				SpawnGhosts(en_pinky, x, y);
				break;
			case 227:
				SpawnGhosts(en_inky, x, y);
				break;
#endif
			}
		}
}
#endif /* GAMEVER_NOAH3D */

/* ======================================================================== */

/*
==================
=
= SetupGameLevel
=
==================
*/

#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION
void SetupGameLevel (void)
{
	int	x,y; // PSP_DEVIATION: removed unused 'i','spot'
	word *map,tile;


	// *** ALPHA RESTORATION ***
	if (!loadedgame)
	{
	 gamestate.TimeCount=
	 gamestate.secrettotal=
	 gamestate.killtotal=
	 gamestate.treasuretotal=
	 gamestate.secretcount=
	 gamestate.killcount=
	 gamestate.treasurecount=0;
	 // *** S3DNA RESTORATION ***
	 pwallstate=0;
	}

	if (demoplayback || demorecord)
		US_InitRndT (false);
	else
		US_InitRndT (true);

//
// load the level
//
	// *** S3DNA + ALPHA RESTORATION ***
	CA_CacheMap (gamestate.mapon);

	// PSP_DEVIATION: mapwidth and mapheight are constants (64) on the PSP, checked below
	if (mapwidth != 64 || mapheight != 64)
		// *** S3DNA RESTORATION ***
		Quit ("SetupGameLevel(): Map not 64*64!");


//
// copy the wall data to a data segment array
//
	memset (tilemap,0,sizeof(tilemap));
	memset (actorat,0,sizeof(actorat));
	map = mapsegs[0];
	for (y=0;y<mapheight;y++)
		for (x=0;x<mapwidth;x++)
		{
			tile = *map++;
			if (tile<AREATILE)
			{
				// *** S3DNA RESTORATION ***
				if (tile == 89)
					tile = 31;
				else
					tile &= 31;
			// solid wall
				tilemap[x][y] = tile;
				actorat[x][y] = tile;
			}
			else
			{
			// area floor
				tilemap[x][y] = 0;
				actorat[x][y] = 0;
			}
		}

//
// spawn doors
//
	InitActorList ();			// start spawning things with a clean slate
	InitDoorList ();
	InitStaticList ();

	map = mapsegs[0];
	for (y=0;y<mapheight;y++)
		for (x=0;x<mapwidth;x++)
		{
			tile = *map++;
			if (tile >= 90 && tile <= 101)
			{
			// door
				switch (tile)
				{
				// *** S3DNA RESTORATION ***
				case 90:
				case 91:
					SpawnDoor (x,y,!(tile&1),0);
					break;
				case 92:
				case 93:
					SpawnDoor (x,y,!(tile&1),1);
					break;
				case 94:
				case 95:
					SpawnDoor (x,y,!(tile&1),2);
					break;
				case 96:
				case 97:
					SpawnDoor (x,y,!(tile&1),5);
					break;
				}
			}
		}

//
// spawn actors
//
	ScanInfoPlane ();

	// *** S3DNA RESTORATION ***



//
// have the caching manager load and purge stuff to make sure all marks
// are in memory
//
	CA_LoadAllSounds ();

}
#else
void SetupGameLevel()
{
	int x, y;
	word *map, tile;

	if (!loadedgame)
	{
		gamestate.TimeCount =
			gamestate.secrettotal =
				gamestate.killtotal =
					gamestate.treasuretotal =
						gamestate.secretcount =
							gamestate.killcount =
								gamestate.treasurecount = 0;
	}

	if (demoplayback || demorecord)
		US_InitRndT(false);
	else
		US_InitRndT(true);

	/* load the level */
	CA_CacheMap(gamestate.mapon + 10 * gamestate.episode);

	if ((mapheaderseg[mapon]->width != 64) || (mapheaderseg[mapon]->height != 64))
		Quit("Map not 64*64!");

	mapon -= gamestate.episode * 10;

	memset(tilemap, 0, sizeof(tilemap));
	memset(actorat, 0, sizeof(actorat));

	/* copy the wall data to a data segment array */
	map = mapsegs[0];
	for (y = 0; y < mapheight; y++)
		for (x = 0; x < mapwidth; x++)
		{
			tile = *map++;
			if (tile < AREATILE)
			{ /* solid wall */
				tilemap[x][y] = tile;
				actorat[x][y] = tile;
			}
			else
			{ /* area floor */
				tilemap[x][y] = 0;
				actorat[x][y] = 0;
			}
		}

	InitActorList(); /* start spawning things with a clean slate */
	InitDoorList();
	InitStaticList();

	/* spawn doors */
	map = mapsegs[0];
	for (y = 0; y < mapheight; y++)
		for (x = 0; x < mapwidth; x++)
		{
			tile = *map++;
			if (tile >= 90 && tile <= 101)
			{
				/* door */
				switch (tile)
				{
				case 90:
				case 92:
				case 94:
				case 96:
				case 98:
				case 100:
					SpawnDoor(x, y, 1, (tile - 90) / 2);
					break;
				case 91:
				case 93:
				case 95:
				case 97:
				case 99:
				case 101:
					SpawnDoor(x, y, 0, (tile - 91) / 2);
					break;
				}
			}
		}

	/* spawn actors */
	ScanInfoPlane();

	/* take out the ambush markers */
	map = mapsegs[0];
	for (y = 0; y < mapheight; y++)
		for (x = 0; x < mapwidth; x++)
		{
			tile = *map++;
			if (tile == AMBUSHTILE)
			{
				tilemap[x][y] = 0;
				if (actorat[x][y] == AMBUSHTILE)
					actorat[x][y] = 0;

				if (*map >= AREATILE)
					tile = *map;
				if (*(map - 1 - mapwidth) >= AREATILE)
					tile = *(map - 1 - mapwidth);
				if (*(map - 1 + mapwidth) >= AREATILE)
					tile = *(map - 1 + mapwidth);
				if (*(map - 2) >= AREATILE)
					tile = *(map - 2);

				*(map - 1) = tile;
			}
		}

	CA_LoadAllSounds();
}
#endif /* GAMEVER_NOAH3D */

/* ======================================================================== */

/*
===================
=
= DrawPlayBorderSides
=
= To fix window overwrites
=
===================
*/

void DrawPlayBorderSides()
{
	int xl, yl;

	xl = 160 - viewwidthwin / 2;
	yl = (200 - STATUSLINES - viewheightwin) / 2;

	if (xl == 0)
	{
		return;
	}
	else
	{
#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION
		VW_Bar(0, 0, xl - 1, 200 - STATUSLINES, VIEWCOLOR);
		VW_Bar(xl + viewwidthwin + 1, 0, xl - 2, 200 - STATUSLINES, VIEWCOLOR);

		VW_Vlin(yl - 1, yl + viewheightwin, xl - 1, 220);
		VW_Vlin(yl - 1, yl + viewheightwin, xl + viewwidthwin, 216);
#else
		VW_Bar(0, 0, xl - 1, 200 - STATUSLINES, 127);
		VW_Bar(xl + viewwidthwin + 1, 0, xl - 2, 200 - STATUSLINES, 127);

		VW_Vlin(yl - 1, yl + viewheightwin, xl - 1, 0);
		VW_Vlin(yl - 1, yl + viewheightwin, xl + viewwidthwin, 125);
#endif
	}
}

/*
===================
=
= DrawPlayBorder
=
===================
*/

void DrawPlayBorder()
{
	int xl, yl;

#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION
	VW_Bar(0, 0, 320, 200 - STATUSLINES + 1, VIEWCOLOR);
#else
	VW_Bar(0, 0, 320, 200 - STATUSLINES + 1, 127);
#endif
	xl = 160 - viewwidthwin / 2;
	yl = (200 - STATUSLINES - viewheightwin) / 2;
	VW_Bar(xl, yl, viewwidthwin, viewheightwin + 1, 0);

	if (xl == 0)
	{
		VW_Hlin(0, viewwidthwin - 1, viewheightwin, 125);
	}
	else
	{
#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION
		VW_Hlin(xl - 1, xl + viewwidthwin, yl - 1, 220);
		VW_Hlin(xl - 1, xl + viewwidthwin, yl + viewheightwin, 216);
		VW_Vlin(yl - 1, yl + viewheightwin, xl - 1, 220);
		VW_Vlin(yl - 1, yl + viewheightwin, xl + viewwidthwin, 216);
		VW_Plot(xl - 1, yl + viewheightwin, 214);
#else
		VW_Hlin(xl - 1, xl + viewwidthwin, yl - 1, 0);
		VW_Hlin(xl - 1, xl + viewwidthwin, yl + viewheightwin, 125);
		VW_Vlin(yl - 1, yl + viewheightwin, xl - 1, 0);
		VW_Vlin(yl - 1, yl + viewheightwin, xl + viewwidthwin, 125);
		VW_Plot(xl - 1, yl + viewheightwin, 124);
#endif
	}
}

void DrawStatusBar()
{
#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION - treasure counter, ammo drawing replaces the weapon picture
	DrawTreasure();
#endif
	DrawFace();
	DrawHealth();
	DrawLives();
	DrawLevel();
	DrawAmmo();
	DrawKeys();
#ifndef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION
	DrawWeapon();
#endif
	DrawScore();
}

/*
===================
=
= DrawPlayScreen
=
===================
*/

void DrawPlayScreen()
{
	VW_FadeOut();

	CA_CacheGrChunk(STATUSBARPIC);

	DrawPlayBorder();
	VWB_DrawPic(0, 200 - STATUSLINES, STATUSBARPIC);

	CA_UnCacheGrChunk(STATUSBARPIC);

	DrawStatusBar();
}

/* ======================================================================= */

/*
==================
=
= StartDemoRecord
=
==================
*/

#define MAXDEMOSIZE 8192

void StartDemoRecord(int levelnumber)
{
	MM_GetPtr(&demobuffer, MAXDEMOSIZE);
	MM_SetLock(&demobuffer, true);
	demoptr = (byte *)demobuffer;
	lastdemoptr = demoptr + MAXDEMOSIZE;

	*demoptr = levelnumber;
	demoptr += 4; // leave space for length
	demorecord = true;
}

/*
==================
=
= FinishDemoRecord
=
==================
*/

char demoname[13] = "demo?.dem";

void FinishDemoRecord()
{
	long length, level;

	demorecord = false;

	length = (byte *)demoptr - (byte *)demobuffer;

	demoptr = ((byte *)demobuffer) + 1;

	demoptr[0] = length & 0xFF;
	demoptr[1] = (length >> 8) & 0xFF;

	CenterWindow(24, 3);
	PrintY += 6;
	VW_FadeIn();
	US_Print(" Demo number (0-9):");
	VW_UpdateScreen();

	if (US_LineInput(px, py, str, NULL, true, 2, 0))
	{
		level = atoi(str);
		if (level >= 0 && level <= 9)
		{
			demoname[4] = '0' + level;
			CA_WriteFile(demoname, (void *)demobuffer, length);
		}
	}

	MM_FreePtr(&demobuffer);
}

/*
==================
=
= RecordDemo
=
= Fades the screen out, then starts a demo.  Exits with the screen faded
=
==================
*/

void RecordDemo()
{
	int level, esc;

	CenterWindow(26, 3);
	PrintY += 6;
	CA_CacheGrChunk(STARTFONT);
	fontnumber = 0;
#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION
	US_Print("  Demo which level(1-30):");
#else
	US_Print("  Demo which level(1-10):");
#endif
	VW_UpdateScreen();
	VW_FadeIn();

	esc = !US_LineInput(px, py, str, NULL, true, 2, 0);
	if (esc)
		return;

	level = atoi(str);
	level--;

	SETFONTCOLOR(0, 15);
	VW_FadeOut();

#if !defined(SPEAR) && !defined(GAMEVER_NOAH3D) // PSP_DEVIATION: S3DNA RESTORATION - no episodes
	NewGame(gd_hard, level / 10);
	gamestate.mapon = level % 10;
#else
	NewGame(gd_hard, 0);
	gamestate.mapon = level;
#endif

	StartDemoRecord(level);

	DrawPlayScreen();
	VW_UpdateScreen();

	VW_FadeIn();

	startgame = false;
	demorecord = true;
	SetupGameLevel();
	StartMusic();
#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION
	fizzlein = 3;
	LoadFloorTiles(FloorTile[gamestate.mapon]);
#endif
	PlayLoop();
	demoplayback = false;

	StopMusic();
	VW_FadeOut();
	ClearMemory();

	FinishDemoRecord();
}

/*
==================
=
= PlayDemo
=
= Fades the screen out, then starts a demo.  Exits with the screen faded
=
==================
*/

void PlayDemo(int demonumber)
{
	int length;

#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION - three demos
	int dems[3] = {T_DEMO0, T_DEMO1, T_DEMO2};
#elif !defined(SPEARDEMO)
	int dems[4] = {T_DEMO0, T_DEMO1, T_DEMO2, T_DEMO3};
#else
	int dems[1] = {T_DEMO0};
#endif

	CA_CacheGrChunk(dems[demonumber]);
	demoptr = grsegs[dems[demonumber]];

	NewGame(1, 0);

	gamestate.mapon = demoptr[0];
	gamestate.difficulty = gd_hard;
	length = demoptr[1] | (demoptr[2] << 8);
	demoptr += 4;

	lastdemoptr = demoptr + length - 4;

	VW_FadeOut();

	SETFONTCOLOR(0, 15);
	DrawPlayScreen();
	VW_UpdateScreen(); /* force redraw */

	VW_FadeIn();

	loadedgame = false;
	startgame = false;
	demoplayback = true;
	SetupGameLevel();
	StartMusic();
#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION
	LoadFloorTiles(FloorTile[gamestate.mapon]);
	fizzlein = 3;
#endif
	PlayLoop();
	CA_UnCacheGrChunk(dems[demonumber]);

	demoplayback = false;

	StopMusic();
	VW_FadeOut();
	ClearMemory();
}

int PlayDemoFromFile(char *demoname)
{
	int length;

	CA_LoadFile(demoname, &demobuffer);
	if (demobuffer == NULL)
	{
		fprintf(stderr, "Unable to load demo: %s\n", demoname);
		return 0;
	}

	MM_SetLock(&demobuffer, true);
	demoptr = (byte *)demobuffer;

	NewGame(1, 0);
	gamestate.mapon = demoptr[0];
	gamestate.difficulty = gd_hard;
	length = demoptr[1] | (demoptr[2] << 8);
	demoptr += 4;
	lastdemoptr = demoptr - 4 + length;

	VW_FadeOut();

	SETFONTCOLOR(0, 15);
	DrawPlayScreen();
	VW_UpdateScreen(); /* force redraw */

	VW_FadeIn();

	startgame = false;
	demoplayback = true;
	SetupGameLevel();
	StartMusic();
#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION
	LoadFloorTiles(FloorTile[gamestate.mapon]);
	fizzlein = 3;
#endif
	PlayLoop();
	MM_FreePtr(&demobuffer);

	demoplayback = false;

	StopMusic();
	ClearMemory();

	return 1;
}

//==========================================================================

/*
==================
=
= Died
=
==================
*/

#define DEATHROTATE 2

void Died()
{
	float fangle;
	long dx, dy;
	int iangle, curangle, clockwise, counter, change;

	gamestate.weapon = -1; // take away weapon
	SD_PlaySound(PLAYERDEATHSND);

	/* swing around to face attacker (if any) */
	if (killerobj)
	{
		dx = killerobj->x - player->x;
		dy = player->y - killerobj->y;
	}
	else
	{
		dx = -player->x;
		dy = player->y;
	}

	fangle = atan2(dy, dx); // returns -pi to pi
	if (fangle < 0)
		fangle = PI * 2 + fangle;

	iangle = fangle / (PI * 2) * ANGLES;

	if (player->angle > iangle)
	{
		counter = player->angle - iangle;
		clockwise = ANGLES - player->angle + iangle;
	}
	else
	{
		clockwise = iangle - player->angle;
		counter = player->angle + ANGLES - iangle;
	}

	curangle = player->angle;

	if (clockwise < counter)
	{
		//
		// rotate clockwise
		//
		if (curangle > iangle)
			curangle -= ANGLES;
		do
		{
			change = tics * DEATHROTATE;
			if (curangle + change > iangle)
				change = iangle - curangle;

			curangle += change;
			player->angle += change;
			if (player->angle >= ANGLES)
				player->angle -= ANGLES;

			ThreeDRefresh();
			CalcTics();
		} while (curangle != iangle);
	}
	else
	{
		//
		// rotate counterclockwise
		//
		if (curangle < iangle)
			curangle += ANGLES;
		do
		{
			change = -tics * DEATHROTATE;
			if (curangle + change < iangle)
				change = iangle - curangle;

			curangle += change;
			player->angle += change;
			if (player->angle < 0)
				player->angle += ANGLES;

			ThreeDRefresh();
			CalcTics();
		} while (curangle != iangle);
	}

	//
	// fade to red
	//
	FinishPaletteShifts();

#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION - no fizzle fade, no wait for a key
	// PSP_DEVIATION: xoffset, yoffset, viewwidth and viewheight are already in screen pixels (see SetViewSize),
	// but VW_Bar takes 320x200 coordinates and scales them by xfrac/yfrac. Passing screen pixels scaled the bar
	// a second time (about 1.6x), so it was drawn far past the end of gfxbuf and overwrote the heap (corrupting
	// the OPL tables, graphics and malloc's free list). VL_Bar takes screen pixels, like ClearScreen does.
	VL_Bar(xoffset, yoffset, viewwidth, viewheight, 4);
	IN_ClearKeysDown();

	gamestate.lives--;

	if (gamestate.lives > -1)
	{
		VL_FadeOut(0, 255, 0, 0, 0, 64);
		SD_WaitSoundDone();
		SD_StopSound();
		gamestate.health = 100;
		gamestate.weapon = gamestate.bestweapon = gamestate.chosenweapon = wp_pistol;
		gamestate.ammo = STARTAMMO;
		gamestate.keys = 0;
		gamestate.attackframe = gamestate.attackcount =
			gamestate.weaponframe = 0;

		gamestate.maxammo = 99;
		gamestate.gas = gamestate.missiles = 0;
		gamestate.machinegun = 0;
		gamestate.chaingun = 0;
		gamestate.flamethrower = 0;
		gamestate.missile = 0;
		gamestate.automap = false;
		DrawTreasure();

		DrawKeys();
		DrawAmmo();
		DrawHealth();
		DrawFace();
		DrawLives();
		return;
	}

	endtics = 0;
	NewState(player, s_gameover);
	while (endtics < 192)
	{
		UpdatePaletteShifts();
		ThreeDRefresh();
		CalcTics();
	}
#else
	FizzleFade(false, 70, 4);

	IN_ClearKeysDown();

	IN_UserInput(140);
	SD_WaitSoundDone();

	gamestate.lives--;

	if (gamestate.lives > -1)
	{
		gamestate.health = 100;
		gamestate.weapon = gamestate.bestweapon = gamestate.chosenweapon = wp_pistol;
		gamestate.ammo = STARTAMMO;
		gamestate.keys = 0;
		gamestate.attackframe = gamestate.attackcount =
			gamestate.weaponframe = 0;

		DrawKeys();
		DrawWeapon();
		DrawAmmo();
		DrawHealth();
		DrawFace();
		DrawLives();
	}
#endif /* GAMEVER_NOAH3D */
}

//==========================================================================

/*
===================
=
= GameLoop
=
===================
*/

void GameLoop()
{
	boolean died;

restartgame:
	ClearMemory();
	SETFONTCOLOR(0, 15);
	DrawPlayScreen();
	died = false;

	do
	{
#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION - mission briefings
		endtics = 0;
		if (!died && !loadedgame)
			switch (gamestate.mapon)
			{
			case 0:
				Briefing(0, false);
				break;
			case 3:
				Briefing(1, false);
				break;
			case 7:
				Briefing(2, false);
				break;
			case 12:
				Briefing(3, false);
				break;
			case 17:
				Briefing(4, false);
				break;
			case 23:
				Briefing(5, false);
				break;
			}
#endif
		if (!loadedgame)
			gamestate.score = gamestate.oldscore;
		DrawScore();
		startgame = false;
		if (loadedgame)
			loadedgame = false;
		else
			SetupGameLevel();

#ifdef SPEAR
		if (gamestate.mapon == 20) /* give them the key always */
		{
			gamestate.keys |= 1;
			DrawKeys();
		}
#endif

		ingame = true;
		StartMusic();

		if (!died)
			PreloadGraphics();
		else
			died = false;

#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION
		fizzlein = 3;
#endif
		DrawLevel();
#ifdef SPEAR
	startplayloop:
#endif
		PlayLoop();

#ifdef SPEAR
		if (spearflag)
		{
			SD_StopSound();
			SD_PlaySound(GETSPEARSND);

			if (DigiMode == sds_Off)
			{
				long lasttimecount = get_TimeCount();

				while (get_TimeCount() < (lasttimecount + 150))
					;
			}
			else
				SD_WaitSoundDone();

			ClearMemory();
			gamestate.oldscore = gamestate.score;
			gamestate.mapon = 20;
			SetupGameLevel();
			StartMusic();
			player->x = spearx;
			player->y = speary;
			player->angle = spearangle;
			spearflag = false;
			Thrust(0, 0);
			goto startplayloop;
		}
#endif

		StopMusic();
		ingame = false;

		if (demorecord && playstate != ex_warped)
			FinishDemoRecord();

		if (startgame || loadedgame)
			goto restartgame;

#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION - the last level always ends the game
		if ((gamestate.mapon == 28) &&
			((playstate == ex_completed) || (playstate == ex_secretlevel)))
			playstate = ex_victorious;
#endif

		switch (playstate)
		{
		case ex_completed:
		case ex_secretlevel:
			gamestate.keys = 0;
			DrawKeys();
#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION
			gamestate.automap = false;
#endif
			VW_FadeOut();

			ClearMemory();

			LevelCompleted(); // do the intermission
#ifdef SPEARDEMO
			if (gamestate.mapon == 1)
			{
				died = true; // don't "get psyched!"

				VW_FadeOut();

				ClearMemory();

				CheckHighScore(gamestate.score, gamestate.mapon + 1);

				strcpy(MainMenu[viewscores].string, STR_VS);
				MainMenu[viewscores].routine = (MenuFunc)CP_ViewScores;

				return;
			}
#endif

			gamestate.oldscore = gamestate.score;
#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION
#define FROMSECRET1 7
#define FROMSECRET2 25
#define GAMEVER_SECRET1 11
#define GAMEVER_SECRET2 29
			//
			// GOING TO SECRET LEVEL
			//
			if (playstate == ex_secretlevel)
				switch (gamestate.mapon)
				{
				case FROMSECRET1:
					gamestate.mapon = GAMEVER_SECRET1;
					break;
				case FROMSECRET2:
					gamestate.mapon = GAMEVER_SECRET2;
					break;
				default:
					sprintf(str, "GameLoop(): Invalid secret exit on level %d!\n", gamestate.mapon);
					Quit(str);
				}
			else
				//
				// COMING BACK FROM SECRET LEVEL
				//
				if (gamestate.mapon == GAMEVER_SECRET1 || gamestate.mapon == GAMEVER_SECRET2)
				switch (gamestate.mapon)
				{
				case GAMEVER_SECRET1:
					gamestate.mapon = FROMSECRET1 + 1;
					break;
				case GAMEVER_SECRET2:
					gamestate.mapon = FROMSECRET2 + 1;
					break;
				}
			else
				//
				// SKIPPING SECRET LEVEL
				//
				if (gamestate.mapon == 10)
				gamestate.mapon = 12;
#elif !defined(SPEAR)
			//
			// COMING BACK FROM SECRET LEVEL
			//
			if (gamestate.mapon == 9)
				gamestate.mapon = ElevatorBackTo[gamestate.episode]; // back from secret
			else
				//
				// GOING TO SECRET LEVEL
				//
				if (playstate == ex_secretlevel)
				gamestate.mapon = 9;
#else

#define FROMSECRET1 3
#define FROMSECRET2 11

			//
			// GOING TO SECRET LEVEL
			//
			if (playstate == ex_secretlevel)
				switch (gamestate.mapon)
				{
				case FROMSECRET1:
					gamestate.mapon = 18;
					break;
				case FROMSECRET2:
					gamestate.mapon = 19;
					break;
				}
			else
				//
				// COMING BACK FROM SECRET LEVEL
				//
				if (gamestate.mapon == 18 || gamestate.mapon == 19)
				switch (gamestate.mapon)
				{
				case 18:
					gamestate.mapon = FROMSECRET1 + 1;
					break;
				case 19:
					gamestate.mapon = FROMSECRET2 + 1;
					break;
				}
#endif
			else
				//
				// GOING TO NEXT LEVEL
				//
				gamestate.mapon++;

			break;

		case ex_died:
			Died();
			died = true; // don't "get psyched!"

			if (gamestate.lives > -1)
				break; // more lives left

			VW_FadeOut();

			ClearMemory();

			CheckHighScore(gamestate.score, gamestate.mapon + 1);

			strcpy(MainMenu[viewscores].string, STR_VS);
			MainMenu[viewscores].routine = (MenuFunc)CP_ViewScores;

			return;

		case ex_victorious:
#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION
			endtics = 0;
#endif
#ifndef SPEAR
			VW_FadeOut();
#else
			VL_FadeOut(0, 255, 0, 17, 17, 300);
#endif
			ClearMemory();
#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION
			LevelCompleted();
			ClearMemory();
#endif
			Victory();

			ClearMemory();

			CheckHighScore(gamestate.score, gamestate.mapon + 1);

			strcpy(MainMenu[viewscores].string, STR_VS);
			MainMenu[viewscores].routine = (MenuFunc)CP_ViewScores;

			return;

		default:
			ClearMemory();
			break;
		}

	} while (1);
}
