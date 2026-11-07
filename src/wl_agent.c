#include "include/wl_def.h"

/*
=============================================================================

						 LOCAL CONSTANTS

=============================================================================
*/

#define MOVESCALE 150
#define BACKMOVESCALE 100
#define ANGLESCALE 20

/*
=============================================================================

						 GLOBAL VARIABLES

=============================================================================
*/

//
// player state info
//
long thrustspeed;

unsigned plux, pluy; // player coordinates scaled to unsigned

int anglefrac;
int gotgatgun;

objtype *LastAttacker;

/*
=============================================================================

						 LOCAL VARIABLES

=============================================================================
*/

#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION - six weapons
struct atkinf
{
	char tics, attack, frame; // attack is 1 for gun, 2 for knife
} static attackinfo[6][14] =
	{
		{{6, 0, 1}, {6, 2, 2}, {6, 0, 3}, {6, -1, 4}},
		{{6, 0, 1}, {6, 1, 2}, {6, 0, 3}, {8, -1, 4}},
		{{6, 0, 1}, {6, 1, 2}, {6, 3, 3}, {6, -1, 4}},
		{{6, 0, 1}, {6, 1, 2}, {6, 4, 3}, {6, -1, 4}},
		{{7, 0, 1}, {7, 5, 2}, {7, 5, 3}, {7, -1, 4}},
		{{6, 0, 1}, {6, 0, 2}, {6, 6, 3}, {6, -1, 4}},
};

int MapEpisode[] = {1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 7};
int MapLevel[] = {1, 2, 3, 1, 2, 3, 4, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 6, 1, 2, 3, 4, 5, 6, 7, 1};
#else
struct atkinf
{
	char tics, attack, frame; // attack is 1 for gun, 2 for knife
} static attackinfo[4][14] =
	{
		{{6, 0, 1}, {6, 2, 2}, {6, 0, 3}, {6, -1, 4}},
		{{6, 0, 1}, {6, 1, 2}, {6, 0, 3}, {6, -1, 4}},
		{{6, 0, 1}, {6, 1, 2}, {6, 3, 3}, {6, -1, 4}},
		{{6, 0, 1}, {6, 1, 2}, {6, 4, 3}, {6, -1, 4}},
};
#endif /* GAMEVER_NOAH3D */

void DrawWeapon();
void GiveWeapon(int weapon);
void GiveAmmo(int ammo);

/*
=============================================================================

						CONTROL STUFF

=============================================================================
*/

/*
======================
=
= CheckWeaponChange
=
= Keys 1-4 change weapons
=
======================
*/

#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION
void CheckWeaponChange (void)
{
	int	i; // PSP_DEVIATION: removed unused 'buttons'

// *** S3DNA RESTORATION ***

// *** S3DNA RESTORATION ***
	for (i=wp_knife ; i<=wp_missile ; i++)
		if (buttonstate[bt_readyknife+i-wp_knife])
			// *** S3DNA RESTORATION ***
			switch (i)
			{
			case wp_knife:
				gamestate.weapon = gamestate.chosenweapon = i;
				DrawAmmo ();
				break;
			case wp_chaingun:
				if (!gamestate.chaingun)
					break;
			case wp_machinegun:
				if (!gamestate.machinegun)
					break;
			case wp_pistol:
				if (gamestate.ammo)
				{
					gamestate.weapon = gamestate.chosenweapon = i;
					DrawAmmo ();
					return;
				}
				break;
			case wp_flamethrower:
				if (gamestate.flamethrower && gamestate.gas)
				{
					gamestate.weapon = gamestate.chosenweapon = i;
					DrawAmmo ();
					return;
				}
				break;
			case wp_missile:
				if (gamestate.missile && gamestate.missiles)
				{
					gamestate.weapon = gamestate.chosenweapon = i;
					DrawAmmo ();
					return;
				}
				break;
			}
}
#else
void CheckWeaponChange()
{
	int i;

	if (!gamestate.ammo) // must use knife with no ammo
		return;

	for (i = wp_knife; i <= gamestate.bestweapon; i++)
		if (buttonstate[bt_readyknife + i - wp_knife])
		{
			gamestate.weapon = gamestate.chosenweapon = i;
			DrawWeapon();
			return;
		}
}
#endif /* GAMEVER_NOAH3D */

/*
=======================
=
= ControlMovement
=
= Takes controlx,controly, and buttonstate[bt_strafe]
=
= Changes the player's angle and position
=
= There is an angle hack because when going 70 fps, the roundoff becomes
= significant
=
=======================
*/

void ControlMovement(objtype *ob)
{
	int angle;
	int angleunits;
	int strafe; // PSP_DEVIATION

	thrustspeed = 0;
	//
	// side to side move
	//
	// BEGIN PSP_DEVIATION: strafing is its own axis (controlstrafe) so it can combine with turning.
	// The legacy bt_strafe mode (mouse button / Alt) still turns controlx into strafing; it is folded
	// into the same single Thrust so the two sources never double up. The clamp keeps the combined
	// value within the same bound PollControls applies to controlx/controly.
	// stock: if (buttonstate[bt_strafe]) { Thrust sideways by controlx } else { turn by controlx }
	strafe = controlstrafe;
	if (buttonstate[bt_strafe])
		strafe += controlx;

	if (strafe > 100 * tics)
		strafe = 100 * tics;
	else if (strafe < -100 * tics)
		strafe = -100 * tics;

	if (strafe > 0)
	{
		angle = ob->angle - ANGLES / 4;
		if (angle < 0)
			angle += ANGLES;
		Thrust(angle, strafe * MOVESCALE); // move to right
	}
	else if (strafe < 0)
	{
		angle = ob->angle + ANGLES / 4;
		if (angle >= ANGLES)
			angle -= ANGLES;
		Thrust(angle, -strafe * MOVESCALE); // move to left
	}
	// END PSP_DEVIATION

	if (!buttonstate[bt_strafe])
	{
		//
		// not strafing
		//
		anglefrac += controlx;
		angleunits = anglefrac / ANGLESCALE;
		anglefrac -= angleunits * ANGLESCALE;
		ob->angle -= angleunits;

		if (ob->angle >= ANGLES)
			ob->angle -= ANGLES;
		if (ob->angle < 0)
			ob->angle += ANGLES;
	}

	//
	// forward/backwards move
	//
	if (controly < 0)
	{
		Thrust(ob->angle, -controly * MOVESCALE); // move forwards
	}
	else if (controly > 0)
	{
		angle = ob->angle + ANGLES / 2;
		if (angle >= ANGLES)
			angle -= ANGLES;
		Thrust(angle, controly * BACKMOVESCALE); // move backwards
	}

	if (gamestate.victoryflag) // watching the BJ actor
		return;
}

/*
=============================================================================

					STATUS WINDOW STUFF

=============================================================================
*/

/*
==================
=
= StatusDrawPic
=
==================
*/

void StatusDrawPic(unsigned x, unsigned y, unsigned picnum)
{
	VWB_DrawPic(x * 8, y + 160, picnum);
}

/*
==================
=
= DrawFace
=
==================
*/

#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION
void DrawFace (void)
{
	// *** SHAREWARE V1.0 APOGEE RESTORATION ***
	if (gamestate.health)
	{
		// *** S3DNA RESTORATION ***
		if (godmode)
			StatusDrawPic (19,4,GODMODEFACE1PIC+gamestate.faceframe);
		else
			StatusDrawPic (19,4,FACE1APIC+3*((100-gamestate.health)/16)+gamestate.faceframe);
	}
	else
	{
	// *** S3DNA RESTORATION ***
	   StatusDrawPic (19,4,FACE8APIC);
	}
	// *** SHAREWARE V1.0 APOGEE RESTORATION ***
}
#else
void DrawFace()
{
	if (gamestate.health)
	{
#ifdef SPEAR
		if (godmode)
			StatusDrawPic(17, 4, GODMODEFACE1PIC + gamestate.faceframe);
		else
#endif
			StatusDrawPic(17, 4, FACE1APIC + 3 * ((100 - gamestate.health) / 16) + gamestate.faceframe);
	}
	else
	{
#ifndef SPEAR
		if (LastAttacker->obclass == needleobj)
			StatusDrawPic(17, 4, MUTANTBJPIC);
		else
#endif
			StatusDrawPic(17, 4, FACE8APIC);
	}
}
#endif /* GAMEVER_NOAH3D */

/*
===============
=
= UpdateFace
=
= Calls draw face if time to change
=
===============
*/

int facecount;

#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION
void	UpdateFace (void)
{

	// *** S3DNA + ALPHA RESTORATION ***

	// *** S3DNA RESTORATION ***
	if (SD_SoundPlaying() == D_BONUSSND)
		facecount += tics;
	if (facecount > US_RndT())
	{
		gamestate.faceframe = (US_RndT()>>6);
		if (gamestate.faceframe==3)
			gamestate.faceframe = 1;

		facecount = 0;
		DrawFace ();
	}
}
#else
void UpdateFace()
{
	if (SD_SoundPlaying() == GETGATLINGSND)
		return;

	facecount += tics;
	if (facecount > US_RndT())
	{
		gamestate.faceframe = (US_RndT() >> 6);
		if (gamestate.faceframe == 3)
			gamestate.faceframe = 1;

		facecount = 0;
		DrawFace();
	}
}
#endif /* GAMEVER_NOAH3D */

/*
===============
=
= LatchNumber
=
= right justifies and pads with blanks
=
===============
*/

void LatchNumber(int x, int y, int width, long number)
{
	int length, c;

	ltoa(number, str, 10);

	length = strlen(str);

	while (length < width)
	{
		StatusDrawPic(x, y, N_BLANKPIC);
		x++;
		width--;
	}

	c = length <= width ? 0 : length - width;

	while (c < length)
	{
		StatusDrawPic(x, y, str[c] - '0' + N_0PIC);
		x++;
		c++;
	}
}

/*
===============
=
= DrawHealth
=
===============
*/

#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION
void DrawTreasure ()
{
	LatchNumber (35,16,2,gamestate.treasure);
}
#endif /* GAMEVER_NOAH3D */

#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION
void	DrawHealth (void)
{
	// *** S3DNA RESTORATION ***
	LatchNumber (23,16,3,gamestate.health);
}
#else
void DrawHealth()
{
	LatchNumber(21, 16, 3, gamestate.health);
}
#endif /* GAMEVER_NOAH3D */

/*
===============
=
= TakeDamage
=
===============
*/

#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION
void	TakeDamage (int points,objtype *attacker)
{
	// *** ALPHA RESTORATION ***
	LastAttacker = attacker;

	// *** SHAREWARE V1.0 APOGEE RESTORATION ***
	if (gamestate.victoryflag)
		return;
	// *** S3DNA + ALPHA RESTORATION ***
	switch (gamestate.difficulty)
	{
	case gd_baby: points>>=2;
		break;
	case gd_easy: points>>=1;
		break;
	case gd_medium: points-=(points>>2);
		break;
	}
	if (!points && (gamestate.difficulty != gd_baby))
		points++;

	if (!godmode)
		gamestate.health -= points;

	if (gamestate.health<=0)
	{
		gamestate.health = 0;
		playstate = ex_died;
		killerobj = attacker;
	}

	StartDamageFlash (points);
	// *** S3DNA RESTORATION ***
	SD_PlaySound (TAKEDAMAGESND);

	// *** ALPHA RESTORATION ***
	gotgatgun=0;

	DrawHealth ();
	DrawFace ();

	//
	// MAKE BJ'S EYES BUG IF MAJOR DAMAGE!
	//

}
#else
void TakeDamage(int points, objtype *attacker)
{
	LastAttacker = attacker;

	if (gamestate.victoryflag)
		return;
	if (gamestate.difficulty == gd_baby)
		points >>= 2;

	if (!godmode)
		gamestate.health -= points;

	if (gamestate.health <= 0)
	{
		gamestate.health = 0;
		playstate = ex_died;
		killerobj = attacker;
	}

	StartDamageFlash(points);

	gotgatgun = 0;

	DrawHealth();
	DrawFace();

//
// MAKE BJ'S EYES BUG IF MAJOR DAMAGE!
//
#ifdef SPEAR
	if (points > 30 && gamestate.health != 0 && !godmode)
	{
		StatusDrawPic(17, 4, BJOUCHPIC);
		facecount = 0;
	}
#endif
}
#endif /* GAMEVER_NOAH3D */

/*
===============
=
= HealSelf
=
===============
*/

void HealSelf(int points)
{
	gamestate.health += points;
	if (gamestate.health > 100)
		gamestate.health = 100;

	DrawHealth();
	gotgatgun = 0; // JR
	DrawFace();
}

//===========================================================================

/*
===============
=
= DrawLevel
=
===============
*/

#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION
void	DrawLevel (void)
{
	// *** S3DNA RESTORATION ***
	int 	level,episode;

	if (gamestate.mapon >= 31)
		Quit ("DrawLevel(): Bad level number!\n");

	episode = MapEpisode[gamestate.mapon];
	level = MapLevel[gamestate.mapon];
	LatchNumber (2,16,1,episode);
	LatchNumber (4,16,1,level);
}
#else
void DrawLevel()
{
#ifdef SPEAR
	if (gamestate.mapon == 20)
		LatchNumber(2, 16, 2, 18);
	else
#endif
		LatchNumber(2, 16, 2, gamestate.mapon + 1);
}
#endif /* GAMEVER_NOAH3D */

//===========================================================================

/*
===============
=
= DrawLives
=
===============
*/

#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION
void	DrawLives (void)
{
	// *** S3DNA RESTORATION ***
	LatchNumber (16,16,1,gamestate.lives);
}
#else
void DrawLives()
{
	LatchNumber(14, 16, 1, gamestate.lives);
}
#endif /* GAMEVER_NOAH3D */

/*
===============
=
= GiveExtraMan
=
===============
*/

void GiveExtraMan()
{
	if (gamestate.lives < 9)
		gamestate.lives++;
	DrawLives();
	SD_PlaySound(BONUS1UPSND);
}

//===========================================================================

/*
===============
=
= DrawScore
=
===============
*/

#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION
void	DrawScore (void)
{
	// *** S3DNA RESTORATION ***
	LatchNumber (7,16,7,gamestate.score);
}
#else
void DrawScore()
{
	LatchNumber(6, 16, 6, gamestate.score);
}
#endif /* GAMEVER_NOAH3D */

/*
===============
=
= GivePoints
=
===============
*/

void GivePoints(long points)
{
	gamestate.score += points;
	while (gamestate.score >= gamestate.nextextra)
	{
		gamestate.nextextra += EXTRAPOINTS;
		GiveExtraMan();
	}
	DrawScore();
}

//===========================================================================

#ifndef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION - not in S3DNA
/*
==================
=
= DrawWeapon
=
==================
*/

void DrawWeapon()
{
	StatusDrawPic(32, 8, KNIFEPIC + gamestate.weapon);
}
#endif /* GAMEVER_NOAH3D */

/*
==================
=
= DrawKeys
=
==================
*/

#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION
void DrawKeys (void)
{
	// *** SHAREWARE V1.0 APOGEE RESTORATION ***
	// *** S3DNA RESTORATION ***
	if (gamestate.keys & 1)
		StatusDrawPic (32,4,GOLDKEYPIC);
	else
		StatusDrawPic (32,4,NOKEYPIC);

	if (gamestate.keys & 2)
		StatusDrawPic (32,20,SILVERKEYPIC);
	else
		StatusDrawPic (32,20,NOKEYPIC);
	// *** SHAREWARE V1.0 APOGEE RESTORATION ***
}
#else
void DrawKeys()
{
	if (gamestate.keys & 1)
		StatusDrawPic(30, 4, GOLDKEYPIC);
	else
		StatusDrawPic(30, 4, NOKEYPIC);

	if (gamestate.keys & 2)
		StatusDrawPic(30, 20, SILVERKEYPIC);
	else
		StatusDrawPic(30, 20, NOKEYPIC);
}
#endif /* GAMEVER_NOAH3D */

/*
==================
=
= GiveWeapon
=
==================
*/

#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION
void GiveWeapon (int weapon)
{
	GiveAmmo (6);

	if (gamestate.bestweapon<weapon)
		gamestate.bestweapon = gamestate.weapon
		= gamestate.chosenweapon = weapon;

	// *** S3DNA RESTORATION ***
	DrawAmmo ();
}
#else
void GiveWeapon(int weapon)
{
	GiveAmmo(6);

	if (gamestate.bestweapon < weapon)
		gamestate.bestweapon = gamestate.weapon = gamestate.chosenweapon = weapon;

	DrawWeapon();
}
#endif /* GAMEVER_NOAH3D */

//===========================================================================

/*
===============
=
= DrawAmmo
=
===============
*/

#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION
void	DrawAmmo (void)
{
	// *** S3DNA RESTORATION ***
	int ammo;
	switch (gamestate.weapon)
	{
	case wp_pistol:
	case wp_machinegun:
	case wp_chaingun:
		ammo = gamestate.ammo;
		break;
	case wp_flamethrower:
		ammo = gamestate.gas;
		break;
	case wp_missile:
		ammo = gamestate.missiles;
		break;
	default:
		ammo = 0;
	}
	LatchNumber (28,16,3,
		ammo);
}
#else
void DrawAmmo()
{
	LatchNumber(27, 16, 2, gamestate.ammo);
}
#endif /* GAMEVER_NOAH3D */

/*
===============
=
= GiveAmmo
=
===============
*/

#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION
void	GiveAmmo (int ammo)
{
	// *** S3DNA RESTORATION ***
	gamestate.ammo += ammo;
	if (gamestate.ammo > gamestate.maxammo)
		gamestate.ammo = gamestate.maxammo;
	if (gamestate.weapon == wp_knife)
		if (!gamestate.attackframe &&
			((gamestate.chosenweapon == wp_chaingun) || (gamestate.chosenweapon == wp_machinegun) || (gamestate.chosenweapon == wp_pistol)))
			gamestate.weapon = gamestate.chosenweapon;
	DrawAmmo ();
}
#else
void GiveAmmo(int ammo)
{
	if (!gamestate.ammo) // knife was out
	{
		if (!gamestate.attackframe)
		{
			gamestate.weapon = gamestate.chosenweapon;
			DrawWeapon();
		}
	}
	gamestate.ammo += ammo;
	if (gamestate.ammo > 99)
		gamestate.ammo = 99;
	DrawAmmo();
}
#endif /* GAMEVER_NOAH3D */

//===========================================================================

#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION
/*
==================
=
= GiveTreasure
=
==================
*/
void GiveTreasure (int treasure)
{
	gamestate.treasure += treasure;
	while (gamestate.treasure >= 50)
	{
		gamestate.treasure -= 50;
		GiveExtraMan ();
	}
	gamestate.treasurecount++;
	DrawTreasure ();
}

/*
===============
=
= GiveGas
=
===============
*/
void	GiveGas (int ammo)
{
	gamestate.gas += ammo;
	if (gamestate.gas > 99)
		gamestate.gas = 99;
	if ((gamestate.weapon == wp_knife) && gamestate.flamethrower)
	{
		if (!gamestate.attackframe)
		{
			gamestate.weapon = wp_flamethrower;
		}
		else
		{
			gamestate.chosenweapon = wp_flamethrower;
		}
	}
	DrawAmmo ();
}

/*
===============
=
= GiveMissile
=
===============
*/
void	GiveMissile (int ammo)
{
	gamestate.missiles += ammo;
	if (gamestate.missiles > 99)
		gamestate.missiles = 99;
	if ((gamestate.weapon == wp_knife) && gamestate.missile)
	{
		if (!gamestate.attackframe)
		{
			gamestate.weapon = wp_missile;
		}
		else
		{
			gamestate.chosenweapon = wp_missile;
		}
	}
	DrawAmmo ();
}
#endif /* GAMEVER_NOAH3D */

/*
==================
=
= GiveKey
=
==================
*/

void GiveKey(int key)
{
	gamestate.keys |= (1 << key);
	DrawKeys();
}

/*
=============================================================================

							MOVEMENT

=============================================================================
*/

/*
===================
=
= GetBonus
=
===================
*/
#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION
void GetBonus (statobj_t *check)
{
	// *** SHAREWARE V1.0 APOGEE RESTORATION ***
	switch (check->itemnumber)
	{
	case	bo_firstaid:
		if (gamestate.health == 100)
			return;

		SD_PlaySound (HEALTH2SND);
		HealSelf (25);
		break;

	case	bo_key1:
	case	bo_key2:
	// *** S3DNA RESTORATION ***
		GiveKey (check->itemnumber - bo_key1);
		SD_PlaySound (GETKEYSND);
		break;

	// *** S3DNA RESTORATION ***
	case	bo_cross:
		SD_PlaySound (D_BONUSSND);
		GiveTreasure (1);
		break;
	case	bo_chalice:
		SD_PlaySound (D_BONUSSND);
		GiveTreasure (1);
		break;
	case	bo_bible:
		SD_PlaySound (D_BONUSSND);
		GiveTreasure (1);
		break;
	case	bo_crown:
		SD_PlaySound (D_BONUSSND);
		GiveTreasure (1);
		break;
	case	bo_chest:
		SD_PlaySound (D_BONUSSND);
		GiveTreasure (1);
		break;

	case	bo_clip:
		// *** S3DNA RESTORATION ***
		if (gamestate.ammo >= gamestate.maxammo)
			return;

		SD_PlaySound (GETAMMOSND);
		// *** S3DNA RESTORATION ***
		GiveAmmo (5);
		break;
	// *** S3DNA RESTORATION ***

		// *** S3DNA RESTORATION ***
//#ifdef SPEAR
	case	bo_25clip:
		// *** S3DNA RESTORATION ***
		if (gamestate.ammo >= gamestate.maxammo)
		  return;

		SD_PlaySound (GETAMMOBOXSND);
		GiveAmmo (25);
		break;

	case	bo_machinegun:
		// *** S3DNA RESTORATION ***
		gamestate.machinegun = 1;
		// *** ALPHA RESTORATION ***
		SD_PlaySound (GETMACHINESND);
		GiveWeapon (wp_machinegun);
		break;
	case	bo_chaingun:
		// *** S3DNA RESTORATION ***
		gamestate.chaingun = 1;
		// *** ALPHA RESTORATION ***
		SD_PlaySound (GETGATLINGSND);
		GiveWeapon (wp_chaingun);

		// *** ALPHA RESTORATION ***
		// *** SHAREWARE V1.0 APOGEE RESTORATION ***
		// *** S3DNA RESTORATION ***
		StatusDrawPic (19,4,GOTGATLINGPIC);
		// *** SHAREWARE V1.0 APOGEE RESTORATION ***
		facecount = 0;
		gotgatgun = 1;
		break;

		// *** ALPHA RESTORATION ***
	case	bo_fullheal:
		SD_PlaySound (BONUS1UPSND);
		HealSelf (99);
		// *** S3DNA RESTORATION ***
		GiveExtraMan ();
		gamestate.treasurecount++;
		break;

	case	bo_food:
		if (gamestate.health == 100)
			return;

		SD_PlaySound (HEALTH1SND);
		HealSelf (10);
		break;

	// *** S3DNA RESTORATION ***
	case	bo_alpo:
		if (demoplayback)
		{
			HealSelf (4); break;
		}
		if ((gamestate.health >= 100) && (gamestate.ammo >= gamestate.maxammo))
			return;

		if (noquestions)
		{
			HealSelf (4);
			StartBonusFlash ();
		}
		else
		{
			if (AskQuestion (questionnum))
				StartBonusFlash ();
			if (++questionnum >= 99)
				questionnum = 0;
		}
		check->shapenum = -1;
		return;

	case	bo_bandolier:
		gamestate.maxammo += 100;
		if (gamestate.maxammo > 299)
			gamestate.maxammo = 299;
		GiveAmmo (10);
		GiveGas (1);
		GiveMissile (1);
		SD_PlaySound (D_BONUSSND);
		break;

	case	bo_flamethrower:
		SD_PlaySound (D_BONUSSND);
		GiveGas (20);
		GiveWeapon (wp_flamethrower);
		StatusDrawPic (19,4,GOTGATLINGPIC);
		facecount = 0;
		gotgatgun = 1;
		if (!gamestate.flamethrower)
		{
			gamestate.flamethrower = 1;
			gamestate.bestweapon = gamestate.weapon = gamestate.chosenweapon
				= wp_flamethrower;
		}
		break;

	case	bo_gascan:
		if (gamestate.gas >= 99)
			return;

		SD_PlaySound (D_BONUSSND);
		GiveGas (14);
		break;

	case	bo_launcher:
		SD_PlaySound (D_BONUSSND);
		GiveMissile (5);
		GiveWeapon (wp_missile);
		StatusDrawPic (19,4,GOTGATLINGPIC);
		facecount = 0;
		gotgatgun = 1;
		if (!gamestate.missile)
		{
			gamestate.missile = 1;
			gamestate.bestweapon = gamestate.weapon = gamestate.chosenweapon
				= wp_missile;
		}
		break;

	case	bo_missiles:
		if (gamestate.missiles >= 99)
			return;

		SD_PlaySound (D_BONUSSND);
		GiveMissile (5);
		break;

	case	bo_map:
		SD_PlaySound (GETKEYSND);
		gamestate.automap = true;
		break;

	case bo_exit:
		playstate = ex_completed;
		break;

	case bo_secret:
		playstate = ex_secretlevel;
		break;

	// *** PRE-V1.4 APOGEE + S3DNA RESTORATION ***
	// *** S3DNA RESTORATION ***
	default:
		sprintf (str,"GetBonus: Invalid item $%02X!\n",check->itemnumber);
		Quit (str);
	}

	StartBonusFlash ();
	check->shapenum = -1;			// remove from list
}
#else
void GetBonus(statobj_t *check)
{
	switch (check->itemnumber)
	{
	case bo_firstaid:
		if (gamestate.health == 100)
			return;

		SD_PlaySound(HEALTH2SND);
		HealSelf(25);
		break;

	case bo_key1:
	case bo_key2:
	case bo_key3:
	case bo_key4:
		GiveKey(check->itemnumber - bo_key1);
		SD_PlaySound(GETKEYSND);
		break;

	case bo_cross:
		SD_PlaySound(BONUS1SND);
		GivePoints(100);
		gamestate.treasurecount++;
		break;
	case bo_chalice:
		SD_PlaySound(BONUS2SND);
		GivePoints(500);
		gamestate.treasurecount++;
		break;
	case bo_bible:
		SD_PlaySound(BONUS3SND);
		GivePoints(1000);
		gamestate.treasurecount++;
		break;
	case bo_crown:
		SD_PlaySound(BONUS4SND);
		GivePoints(5000);
		gamestate.treasurecount++;
		break;

	case bo_clip:
		if (gamestate.ammo == 99)
			return;

		SD_PlaySound(GETAMMOSND);
		GiveAmmo(8);
		break;
	case bo_clip2:
		if (gamestate.ammo == 99)
			return;

		SD_PlaySound(GETAMMOSND);
		GiveAmmo(4);
		break;

#ifdef SPEAR
	case bo_25clip:
		if (gamestate.ammo == 99)
			return;

		SD_PlaySound(GETAMMOBOXSND);
		GiveAmmo(25);
		break;
#endif

	case bo_machinegun:
		SD_PlaySound(GETMACHINESND);
		GiveWeapon(wp_machinegun);
		break;
	case bo_chaingun:
		SD_PlaySound(GETGATLINGSND);
		GiveWeapon(wp_chaingun);

		StatusDrawPic(17, 4, GOTGATLINGPIC);
		facecount = 0;
		gotgatgun = 1;
		break;

	case bo_fullheal:
		SD_PlaySound(BONUS1UPSND);
		HealSelf(99);
		GiveAmmo(25);
		GiveExtraMan();
		gamestate.treasurecount++;
		break;

	case bo_food:
		if (gamestate.health == 100)
			return;

		SD_PlaySound(HEALTH1SND);
		HealSelf(10);
		break;

	case bo_alpo:
		if (gamestate.health == 100)
			return;

		SD_PlaySound(HEALTH1SND);
		HealSelf(4);
		break;

	case bo_gibs:
		if (gamestate.health > 10)
			return;

		SD_PlaySound(SLURPIESND);
		HealSelf(1);
		break;

	case bo_spear:
		spearflag = true;
		spearx = player->x;
		speary = player->y;
		spearangle = player->angle;
		playstate = ex_completed;
	}

	StartBonusFlash();
	check->shapenum = -1; // remove from list
}
#endif /* GAMEVER_NOAH3D */

/*
===================
=
= TryMove
=
= returns true if move ok
=
===================
*/

boolean TryMove(objtype *ob)
{
	int xl, yl, xh, yh, x, y;
	objtype *check;
	long deltax, deltay;

	xl = (ob->x - PLAYERSIZE) >> TILESHIFT;
	yl = (ob->y - PLAYERSIZE) >> TILESHIFT;

	xh = (ob->x + PLAYERSIZE) >> TILESHIFT;
	yh = (ob->y + PLAYERSIZE) >> TILESHIFT;

	//
	// check for solid walls
	//
	for (y = yl; y <= yh; y++)
		for (x = xl; x <= xh; x++)
		{
			if (actorat[x][y] && !(actorat[x][y] & 0x8000))
				return false;
		}

	//
	// check for actors
	//
	if (yl > 0)
		yl--;
	if (yh < MAPSIZE - 1)
		yh++;
	if (xl > 0)
		xl--;
	if (xh < MAPSIZE - 1)
		xh++;

	for (y = yl; y <= yh; y++)
		for (x = xl; x <= xh; x++)
		{
			if (actorat[x][y] & 0x8000)
			{
				check = &objlist[actorat[x][y] & ~0x8000];

				if (check->flags & FL_SHOOTABLE)
				{
					deltax = ob->x - check->x;
					if (deltax < -MINACTORDIST || deltax > MINACTORDIST)
						continue;
					deltay = ob->y - check->y;
					if (deltay < -MINACTORDIST || deltay > MINACTORDIST)
						continue;

					return false;
				}
			}
		}

	return true;
}

/*
===================
=
= ClipMove
=
===================
*/

#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION
void ClipMove (objtype *ob, long xmove, long ymove)
{
	long	basex,basey;

	basex = ob->x;
	basey = ob->y;

	ob->x = basex+xmove;
	ob->y = basey+ymove;
	if (TryMove (ob))
		return;

	if (noclip && ob->x > 2*TILEGLOBAL && ob->y > 2*TILEGLOBAL &&
	ob->x < (((long)(mapwidth-1))<<TILESHIFT)
	&& ob->y < (((long)(mapheight-1))<<TILESHIFT) )
		return;		// walk through walls

	// *** S3DNA RESTORATION ***

	ob->x = basex+xmove;
	ob->y = basey;
	if (TryMove (ob))
		return;

	ob->x = basex;
	ob->y = basey+ymove;
	if (TryMove (ob))
		return;

	ob->x = basex;
	ob->y = basey;
}
#else
void ClipMove(objtype *ob, long xmove, long ymove)
{
	long basex, basey;

	basex = ob->x;
	basey = ob->y;

	ob->x = basex + xmove;
	ob->y = basey + ymove;
	if (TryMove(ob))
		return;

	if (noclip && ob->x > 2 * TILEGLOBAL && ob->y > 2 * TILEGLOBAL &&
		ob->x < (((long)(mapwidth - 1)) << TILESHIFT) && ob->y < (((long)(mapheight - 1)) << TILESHIFT))
		return; // walk through walls

	if (!SD_SoundPlaying())
		SD_PlaySound(HITWALLSND);

	ob->x = basex + xmove;
	ob->y = basey;
	if (TryMove(ob))
		return;

	ob->x = basex;
	ob->y = basey + ymove;
	if (TryMove(ob))
		return;

	ob->x = basex;
	ob->y = basey;
}
#endif /* GAMEVER_NOAH3D */

//==========================================================================

/*
===================
=
= VictoryTile
=
===================
*/

#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION
void VictoryTile (void)
{
	// *** S3DNA RESTORATION ***

	gamestate.victoryflag = true;
}
#else
void VictoryTile()
{
#ifndef SPEAR
	SpawnBJVictory();
#endif
	gamestate.victoryflag = true;
}
#endif /* GAMEVER_NOAH3D */

/*
===================
=
= Thrust
=
===================
*/

void Thrust(int angle, long speed)
{
	long xmove, ymove;
	unsigned offset;

#ifdef SPEAR
	//
	// ZERO FUNNY COUNTER IF MOVED!
	//
	if (speed)
		funnyticount = 0;
#endif

	thrustspeed += speed;
	//
	// moving bounds speed
	//
	if (speed >= MINDIST * 2)
		speed = MINDIST * 2 - 1;

	xmove = FixedByFrac(speed, costable[angle]);
	ymove = -FixedByFrac(speed, sintable[angle]);

	ClipMove(player, xmove, ymove);

	player->tilex = player->x >> TILESHIFT; // scale to tile values
	player->tiley = player->y >> TILESHIFT;

	offset = farmapylookup[player->tiley] + player->tilex;
	player->areanumber = *(mapsegs[0] + offset) - AREATILE;

	if (*(mapsegs[1] + offset) == EXITTILE)
		VictoryTile();
}

/*
=============================================================================

								ACTIONS

=============================================================================
*/

/*
===============
=
= Cmd_Fire
=
===============
*/

void Cmd_Fire()
{
	buttonheld[bt_attack] = true;

	gamestate.weaponframe = 0;

	player->state = s_attack;

	gamestate.attackframe = 0;
	gamestate.attackcount =
		attackinfo[gamestate.weapon][gamestate.attackframe].tics;
	gamestate.weaponframe =
		attackinfo[gamestate.weapon][gamestate.attackframe].frame;
}

//===========================================================================

/*
===============
=
= Cmd_Use
=
===============
*/

#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION
void Cmd_Use (void)
{
	// PSP_DEVIATION: removed unused 'check'
	// *** S3DNA + ALPHA RESTORATION ***
	int			checkx,checky,doornum,dir;


	// *** ALPHA RESTORATION ***
//
// find which cardinal direction the player is facing
//
	if (player->angle < ANGLES/8 || player->angle > 7*ANGLES/8)
	{
		checkx = player->tilex + 1;
		checky = player->tiley;
		// *** S3DNA + ALPHA RESTORATION ***
		dir = di_east;
	}
	else if (player->angle < 3*ANGLES/8)
	{
		checkx = player->tilex;
		checky = player->tiley-1;
		// *** S3DNA + ALPHA RESTORATION ***
		dir = di_north;
	}
	else if (player->angle < 5*ANGLES/8)
	{
		checkx = player->tilex - 1;
		checky = player->tiley;
		// *** S3DNA + ALPHA RESTORATION ***
		dir = di_west;
	}
	else
	{
		checkx = player->tilex;
		checky = player->tiley + 1;
		// *** S3DNA + ALPHA RESTORATION ***
		dir = di_south;
	}

	doornum = tilemap[checkx][checky];
	// *** S3DNA RESTORATION ***
	doornum &= 0xFFDF;
	// *** ALPHA RESTORATION **/
	// Somewhat different code
	if (*(mapsegs[1]+farmapylookup[checky]+checkx) == PUSHABLETILE)
	{
	//
	// pushable wall
	//

		PushWall (checkx,checky,dir);
		return;
	}
	// *** S3DNA RESTORATION ***
		if (!buttonheld[bt_use] && doornum & 0x80)
	{
		buttonheld[bt_use] = true;
		OperateDoor (doornum & ~0x80);
	}
	else
		// *** S3DNA RESTORATION ***
		SD_PlaySound (NOWAYSND);

}
#else
void Cmd_Use()
{
	int checkx, checky, doornum, dir;
	boolean elevatorok;

	//
	// find which cardinal direction the player is facing
	//
	if (player->angle < ANGLES / 8 || player->angle > 7 * ANGLES / 8)
	{
		checkx = player->tilex + 1;
		checky = player->tiley;
		dir = di_east;
		elevatorok = true;
	}
	else if (player->angle < 3 * ANGLES / 8)
	{
		checkx = player->tilex;
		checky = player->tiley - 1;
		dir = di_north;
		elevatorok = false;
	}
	else if (player->angle < 5 * ANGLES / 8)
	{
		checkx = player->tilex - 1;
		checky = player->tiley;
		dir = di_west;
		elevatorok = true;
	}
	else
	{
		checkx = player->tilex;
		checky = player->tiley + 1;
		dir = di_south;
		elevatorok = false;
	}

	doornum = tilemap[checkx][checky];
	if (*(mapsegs[1] + farmapylookup[checky] + checkx) == PUSHABLETILE)
	{
		//
		// pushable wall
		//

		PushWall(checkx, checky, dir);
		return;
	}
	if (!buttonheld[bt_use] && doornum == ELEVATORTILE && elevatorok)
	{
		//
		// use elevator
		//
		buttonheld[bt_use] = true;

		tilemap[checkx][checky]++; // flip switch
		if (*(mapsegs[0] + farmapylookup[player->tiley] + player->tilex) == ALTELEVATORTILE)
			playstate = ex_secretlevel;
		else
			playstate = ex_completed;
		SD_PlaySound(LEVELDONESND);
		SD_WaitSoundDone();
	}
	else if (!buttonheld[bt_use] && doornum & 0x80)
	{
		buttonheld[bt_use] = true;
		OperateDoor(doornum & ~0x80);
	}
	else
		SD_PlaySound(DONOTHINGSND);
}
#endif /* GAMEVER_NOAH3D */

/*
=============================================================================

						   PLAYER CONTROL

=============================================================================
*/

/*
===============
=
= SpawnPlayer
=
===============
*/

void SpawnPlayer(int tilex, int tiley, int dir)
{
	player->obclass = playerobj;
	player->active = true;
	player->tilex = tilex;
	player->tiley = tiley;
	player->areanumber =
		*(mapsegs[0] + farmapylookup[player->tiley] + player->tilex);
	player->x = ((long)tilex << TILESHIFT) + TILEGLOBAL / 2;
	player->y = ((long)tiley << TILESHIFT) + TILEGLOBAL / 2;
	player->state = s_player;
	player->angle = (1 - dir) * 90;
	if (player->angle < 0)
		player->angle += ANGLES;
	player->flags = FL_NEVERMARK;
	Thrust(0, 0); // set some variables

	InitAreas();
}

//===========================================================================

/*
===============
=
= T_KnifeAttack
=
= Update player hands, and try to do damage when the proper frame is reached
=
===============
*/

#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION
void	KnifeAttack (objtype *ob)
{
	objtype *check,*closest;
	long	dist;

	SD_PlaySound (ATKKNIFESND);
// actually fire
	dist = 0x7fffffff;
	closest = NULL;
	for (check=ob->next ; check ; check=check->next)
		if ( (check->flags & FL_SHOOTABLE)
		&& (check->flags & FL_VISABLE)
		&& abs (check->viewx-centerx) < shootdelta
		)
		{
			if (check->transx < dist)
			{
				dist = check->transx;
				closest = check;
			}
		}

	if (!closest || dist> 0x18000l)
	{
	// missed

		return;
	}

// hit something
	// *** S3DNA RESTORATION ***
	DamageActor (closest,US_RndT()&3);
}
#else
void KnifeAttack(objtype *ob)
{
	objtype *check, *closest;
	long dist;

	SD_PlaySound(ATKKNIFESND);
	// actually fire
	dist = 0x7fffffff;
	closest = NULL;
	for (check = ob->next; check; check = check->next)
		if ((check->flags & FL_SHOOTABLE) && (check->flags & FL_VISABLE) && abs(check->viewx - centerx) < shootdelta)
		{
			if (check->transx < dist)
			{
				dist = check->transx;
				closest = check;
			}
		}

	if (!closest || dist > 0x18000l)
	{
		// missed

		return;
	}

	// hit something
	DamageActor(closest, US_RndT() >> 4);
}
#endif /* GAMEVER_NOAH3D */

#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION
void FlameAttack (objtype *ob)
{
	int x, y;

	SD_PlaySound (D_COCTHRSND);
	madenoise = true;
	gamestate.gas--;
	DrawAmmo ();
	GetNewActor ();
	new->state = s_flame;
	new->obclass = flameobj;
	new->ticcount = 1;
	x = costable[ob->angle];
	y = -sintable[ob->angle];
	new->x = ob->x + (x>>2);
	new->y = ob->y + (y>>2);
	new->tilex = new->x >> TILESHIFT;
	new->tiley = new->y >> TILESHIFT;
	new->areanumber = ob->areanumber;
	new->dir = ob->dir;
	new->angle = ob->angle;
	new->speed = 0x3000;
	new->flags = FL_NEVERMARK;
	new->active = ac_yes;
}

void MissileAttack (objtype *ob)
{
	int x, y;

	SD_PlaySound (D_WATTHRSND);
	madenoise = true;
	gamestate.missiles--;
	DrawAmmo ();
	GetNewActor ();
	new->state = s_missile;
	new->obclass = missileobj;
	new->ticcount = 1;
	x = costable[ob->angle];
	y = -sintable[ob->angle];
	new->x = ob->x + (x>>2);
	new->y = ob->y + (y>>2);
	new->tilex = new->x >> TILESHIFT;
	new->tiley = new->y >> TILESHIFT;
	new->areanumber = ob->areanumber;
	new->dir = ob->dir;
	new->angle = ob->angle;
	new->speed = 0x3000;
	new->flags = FL_NEVERMARK;
	new->active = ac_yes;
}
#endif /* GAMEVER_NOAH3D */

#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION
void	GunAttack (objtype *ob)
{
	objtype *check,*closest,*oldclosest;
	int		damage;
	int		dx,dy,dist;
	long	viewdist;

	switch (gamestate.weapon)
	{
	case wp_pistol:
		SD_PlaySound (ATKPISTOLSND);
		break;
	case wp_machinegun:
		SD_PlaySound (ATKMACHINEGUNSND);
		break;
	case wp_chaingun:
		SD_PlaySound (ATKGATLINGSND);
		break;
	default: // PSP_DEVIATION: silence -Wswitch for other weapons
		break;
	}

	madenoise = true;

//
// find potential targets
//
	viewdist = 0x7fffffffl;
	closest = NULL;

	while (1)
	{
		oldclosest = closest;

		for (check=ob->next ; check ; check=check->next)
			if ( (check->flags & FL_SHOOTABLE)
			&& (check->flags & FL_VISABLE)
			&& abs (check->viewx-centerx) < shootdelta
			)
			{
				if (check->transx < viewdist)
				{
					viewdist = check->transx;
					closest = check;
				}
			}

		if (closest == oldclosest)
			return;						// no more targets, all missed

	//
	// trace a line from player to enemey
	//
		if (CheckLine(closest))
			break;

	}

//
// hit something
//
	dx = abs(closest->tilex - player->tilex);
	dy = abs(closest->tiley - player->tiley);
	dist = dx>dy ? dx:dy;

	// *** S3DNA RESTORATION ***
	if (dist<2)
		damage = US_RndT() & 0xf;
	else
	{
		damage = US_RndT() & 7;

		if ((dist >= 4) && ( (US_RndT() / 12) < dist))		// missed
			return;
	}

	DamageActor (closest,damage);
}
#else
void GunAttack(objtype *ob)
{
	objtype *check, *closest, *oldclosest;
	int damage;
	int dx, dy, dist;
	long viewdist;

	switch (gamestate.weapon)
	{
	case wp_pistol:
		SD_PlaySound(ATKPISTOLSND);
		break;
	case wp_machinegun:
		SD_PlaySound(ATKMACHINEGUNSND);
		break;
	case wp_chaingun:
		SD_PlaySound(ATKGATLINGSND);
		break;
	default:
		break;
	}

	madenoise = true;

	//
	// find potential targets
	//
	viewdist = 0x7fffffffl;
	closest = NULL;

	while (1)
	{
		oldclosest = closest;

		for (check = ob->next; check; check = check->next)
			if ((check->flags & FL_SHOOTABLE) && (check->flags & FL_VISABLE) && abs(check->viewx - centerx) < shootdelta)
			{
				if (check->transx < viewdist)
				{
					viewdist = check->transx;
					closest = check;
				}
			}

		if (closest == oldclosest)
			return; // no more targets, all missed

		//
		// trace a line from player to enemey
		//
		if (CheckLine(closest))
			break;
	}

	//
	// hit something
	//
	dx = abs(closest->tilex - player->tilex);
	dy = abs(closest->tiley - player->tiley);
	dist = dx > dy ? dx : dy;

	if (dist < 2)
		damage = US_RndT() / 4;
	else if (dist < 4)
		damage = US_RndT() / 6;
	else
	{
		if ((US_RndT() / 12) < dist) // missed
			return;
		damage = US_RndT() / 6;
	}

	DamageActor(closest, damage);
}
#endif /* GAMEVER_NOAH3D */

//===========================================================================

/*
===============
=
= VictorySpin
=
===============
*/

void VictorySpin()
{
	long desty;

	if (player->angle > 270)
	{
		player->angle -= tics * 3;
		if (player->angle < 270)
			player->angle = 270;
	}
	else if (player->angle < 270)
	{
		player->angle += tics * 3;
		if (player->angle > 270)
			player->angle = 270;
	}

	desty = (((long)player->tiley - 5) << TILESHIFT) - 0x3000;

	if (player->y > desty)
	{
		player->y -= tics * 4096;
		if (player->y < desty)
			player->y = desty;
	}
}

//===========================================================================

/*
===============
=
= T_Attack
=
===============
*/

#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION
void	T_Attack (objtype *ob)
{
	struct	atkinf	*cur;

	UpdateFace ();

	// *** ALPHA RESTORATION ***
	if (gamestate.victoryflag)		// watching the BJ actor
	{
		VictorySpin ();
		return;
	}

	if ( buttonstate[bt_use] && !buttonheld[bt_use] )
		buttonstate[bt_use] = false;

	if ( buttonstate[bt_attack] && !buttonheld[bt_attack])
		buttonstate[bt_attack] = false;

	ControlMovement (ob);
	// *** ALPHA RESTORATION ***
	if (gamestate.victoryflag)		// watching the BJ actor
		return;

	// *** SHAREWARE V1.0 APOGEE RESTORATION ***
	plux = player->x >> UNSIGNEDSHIFT;			// scale to fit in unsigned
	pluy = player->y >> UNSIGNEDSHIFT;
	player->tilex = player->x >> TILESHIFT;		// scale to tile values
	player->tiley = player->y >> TILESHIFT;

//
// change frame and fire
//
	gamestate.attackcount -= tics;
	while (gamestate.attackcount <= 0)
	{
		cur = &attackinfo[gamestate.weapon][gamestate.attackframe];
		switch (cur->attack)
		{
		case -1:
			ob->state = s_player;
			// *** S3DNA RESTORATION ***
			if ((!gamestate.gas && (gamestate.weapon == wp_flamethrower))
			    || (!gamestate.missiles && (gamestate.weapon == wp_missile)))
			{
				if (gamestate.ammo)
				{
					if (gamestate.chaingun)
						gamestate.weapon = gamestate.chosenweapon = wp_chaingun;
					else if (gamestate.machinegun)
						gamestate.weapon = gamestate.chosenweapon = wp_machinegun;
					else
						gamestate.weapon = gamestate.chosenweapon = wp_pistol;
					DrawAmmo ();
				}
				else
					gamestate.weapon = wp_knife;
				DrawAmmo ();
			}
			else if (!gamestate.ammo)
			{
				if (gamestate.missile && gamestate.missiles)
					gamestate.weapon = gamestate.chosenweapon = wp_missile;
				else if (gamestate.flamethrower && gamestate.gas)
					gamestate.weapon = gamestate.chosenweapon = wp_flamethrower;
				else
					gamestate.weapon = wp_knife;
				DrawAmmo ();
			}
			else
			{
				if (gamestate.weapon != gamestate.chosenweapon)
				{
					gamestate.weapon = gamestate.chosenweapon;
					DrawAmmo ();
				}
			};
			gamestate.attackframe = gamestate.weaponframe = 0;
			return;

		case 4:
			if (!gamestate.ammo)
				break;
			if (buttonstate[bt_attack])
				gamestate.attackframe -= 2;
		case 1:
			if (!gamestate.ammo)
			{	// can only happen with chain gun
				gamestate.attackframe++;
				break;
			}
			GunAttack (ob);
			gamestate.ammo--;
			DrawAmmo ();
			break;

		case 2:
			KnifeAttack (ob);
			break;

		case 3:
			if (gamestate.ammo && buttonstate[bt_attack])
				gamestate.attackframe -= 2;
			break;
		// *** S3DNA RESTORATION ***
		case 5:
			if (gamestate.gas)
				FlameAttack (ob);
			break;
		case 6:
			if (gamestate.missiles)
				MissileAttack (ob);
			break;
		}

		gamestate.attackcount += cur->tics;
		gamestate.attackframe++;
		gamestate.weaponframe =
			attackinfo[gamestate.weapon][gamestate.attackframe].frame;
	}

}
#else
void T_Attack(objtype *ob)
{
	struct atkinf *cur;

	UpdateFace();

	if (gamestate.victoryflag) // watching the BJ actor
	{
		VictorySpin();
		return;
	}

	if (buttonstate[bt_use] && !buttonheld[bt_use])
		buttonstate[bt_use] = false;

	if (buttonstate[bt_attack] && !buttonheld[bt_attack])
		buttonstate[bt_attack] = false;

	ControlMovement(ob);
	if (gamestate.victoryflag) // watching the BJ actor
		return;

	plux = player->x >> UNSIGNEDSHIFT; // scale to fit in unsigned
	pluy = player->y >> UNSIGNEDSHIFT;
	player->tilex = player->x >> TILESHIFT; // scale to tile values
	player->tiley = player->y >> TILESHIFT;

	//
	// change frame and fire
	//
	gamestate.attackcount -= tics;
	while (gamestate.attackcount <= 0)
	{
		cur = &attackinfo[gamestate.weapon][gamestate.attackframe];
		switch (cur->attack)
		{
		case -1:
			ob->state = s_player;
			if (!gamestate.ammo)
			{
				gamestate.weapon = wp_knife;
				DrawWeapon();
			}
			else
			{
				if (gamestate.weapon != gamestate.chosenweapon)
				{
					gamestate.weapon = gamestate.chosenweapon;
					DrawWeapon();
				}
			};
			gamestate.attackframe = gamestate.weaponframe = 0;
			return;

		case 4:
			if (!gamestate.ammo)
				break;
			if (buttonstate[bt_attack])
				gamestate.attackframe -= 2;
		case 1:
			if (!gamestate.ammo)
			{ // can only happen with chain gun
				gamestate.attackframe++;
				break;
			}
			GunAttack(ob);
			gamestate.ammo--;
			DrawAmmo();
			break;

		case 2:
			KnifeAttack(ob);
			break;

		case 3:
			if (gamestate.ammo && buttonstate[bt_attack])
				gamestate.attackframe -= 2;
			break;
		}

		gamestate.attackcount += cur->tics;
		gamestate.attackframe++;
		gamestate.weaponframe =
			attackinfo[gamestate.weapon][gamestate.attackframe].frame;
	}
}
#endif /* GAMEVER_NOAH3D */

/*
===============
=
= T_Player
=
===============
*/

#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION
void	T_Player (objtype *ob)
{
	// *** ALPHA RESTORATION ***
	if (gamestate.victoryflag)		// watching the BJ actor
	{
		VictorySpin ();
		return;
	}

	UpdateFace ();
	CheckWeaponChange ();

	// *** ALPHA RESTORATION ***
	if ( buttonstate[bt_use] )
		Cmd_Use ();

	// *** S3DNA RESTORATION ***
	if (buttonstate[bt_attack])

	    if (!buttonheld[bt_attack] || (gamestate.weapon == wp_knife) || (gamestate.weapon == wp_pistol))
		Cmd_Fire ();

	ControlMovement (ob);
	if (gamestate.victoryflag)		// watching the BJ actor
		return;


	// *** ALPHA RESTORATION ***
	plux = player->x >> UNSIGNEDSHIFT;			// scale to fit in unsigned
	pluy = player->y >> UNSIGNEDSHIFT;
	player->tilex = player->x >> TILESHIFT;		// scale to tile values
	player->tiley = player->y >> TILESHIFT;
}
#else
void T_Player(objtype *ob)
{
	if (gamestate.victoryflag) // watching the BJ actor
	{
		VictorySpin();
		return;
	}

	UpdateFace();
	CheckWeaponChange();

	if (buttonstate[bt_use])
		Cmd_Use();

	if (buttonstate[bt_attack] && !buttonheld[bt_attack])
		Cmd_Fire();

	ControlMovement(ob);
	if (gamestate.victoryflag) // watching the BJ actor
		return;

	plux = player->x >> UNSIGNEDSHIFT; // scale to fit in unsigned
	pluy = player->y >> UNSIGNEDSHIFT;
	player->tilex = player->x >> TILESHIFT; // scale to tile values
	player->tiley = player->y >> TILESHIFT;
}
#endif /* GAMEVER_NOAH3D */
