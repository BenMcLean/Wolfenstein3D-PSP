#ifndef __VERSION_H__
#define __VERSION_H__

/* Change WMODE to point to the executable you would like to build: */
// Mode 0 = Wolf3D Shareware
// Mode 1 = Wolf3d Full
// Mode 2 = SOD Shareware
// Mode 3 = SOD Full / SOD Episode 1 & 2

#define WOLF3D_SHAREWARE 0
#define WOLF3D_FULL 1
#define WOLF3D_SOD_SHAREWARE 2
#define WOLF3D_SOD_FULL 3
#define WOLF3D_SOD_MISSION2 4 /* PSP_DEVIATION */
#define WOLF3D_SOD_MISSION3 5 /* PSP_DEVIATION */

#ifndef WMODE
#define WMODE WOLF3D_SHAREWARE
#endif

/* --- End of User-Modifiable Variables --- */

#if WMODE == 0
/* #define SPEAR */
/* #define SPEARDEMO */
#define UPLOAD
#define GAMENAME "Wolfenstein 3D Shareware"
#define GAMEEXT "wl1"
#define GAMETYPE "WL1\0"

#elif WMODE == 1
/* #define SPEAR */
/* #define SPEARDEMO */
/* #define UPLOAD */
#define GAMENAME "Wolfenstein 3D"
#define GAMEEXT "wl6"
#define GAMETYPE "WL6\0"

#elif WMODE == 2
#define SPEAR
#define SPEARDEMO
/* #define UPLOAD */
#define GAMENAME "Spear of Destiny Demo"
#define GAMEEXT "sdm"
#define GAMETYPE "SDM\0"

#elif WMODE == 3
#define SPEAR
/* #define SPEARDEMO */
/* #define UPLOAD */
#define GAMENAME "Spear of Destiny"
#define GAMEEXT "sod"
#define GAMETYPE "SOD\0"

/* PSP_DEVIATION: separate builds for the Spear of Destiny mission packs (*.sd2 / *.sd3 data files) */
#elif WMODE == 4
#define SPEAR
#define GAMENAME "Spear of Destiny: Return to Danger"
#define GAMEEXT "sd2"
#define GAMETYPE "SD2\0"

#elif WMODE == 5
#define SPEAR
#define GAMENAME "Spear of Destiny: Ultimate Challenge"
#define GAMEEXT "sd3"
#define GAMETYPE "SD3\0"

#else
#error "please edit version.h and fix WMODE"
#endif

#define GAMEHDR "WOLF3D\0\0"

#define SAVTYPE "SAV\0"
#define CFGTYPE "CFG\0"

#endif
