#include "include/wl_def.h"

#include "SDL/SDL.h"
//#include <pthread.h>
//#include <sys/ioctl.h>
//#include <sys/soundcard.h>
#include <sys/types.h>
//#include <sys/stat.h>
//#include <fcntl.h>
//#include <unistd.h>

#include "include/fmopl.h"

#define PACKED __attribute__((packed))

typedef struct
{
	longword length;
	word priority;
} PACKED SoundCommon;

typedef struct
{
	SoundCommon common;
	byte data[1];
} PACKED PCSound;

typedef struct
{
	byte mChar, cChar, mScale, cScale, mAttack, cAttack, mSus, cSus,
		mWave, cWave, nConn, voice, mode, unused[3];
} PACKED Instrument;

typedef struct
{
	SoundCommon common;
	Instrument inst;
	byte block, data[1];
} PACKED AdLibSound;

typedef struct
{
	word length, values[1];
} PACKED MusicGroup;

boolean AdLibPresent = true, SoundBlasterPresent = true;

SDMode SoundMode;
SMMode MusicMode;
SDSMode DigiMode;

static int Volume;
static int leftchannel, rightchannel;
static int L, R;

static volatile boolean sqActive;

static word *DigiList;

static boolean SD_Started;

#define NUM_SFX 8

static volatile boolean SoundPositioned[NUM_SFX];
static volatile int SoundPlaying[NUM_SFX];
static volatile int SoundPlayPos[NUM_SFX];
static volatile int SoundPlayLen[NUM_SFX];
static volatile int SoundPage[NUM_SFX];
static volatile int SoundLen[NUM_SFX];
static volatile byte *SoundData[NUM_SFX];
static volatile fixed SoundGX[NUM_SFX];
static volatile fixed SoundGY[NUM_SFX];
static volatile int CurDigi[NUM_SFX];

static FM_OPL *OPL;
static MusicGroup *Music;
static volatile int NewMusic;
static volatile int NewAdlib;
static volatile int AdlibPlaying;
static volatile int CurAdlib;

static boolean SPHack;

#define NUM_SAMPS 512

static short int sdlbuf[NUM_SAMPS * 2];
static short int musbuf[NUM_SAMPS];
static int bufpos;

static int MusicLength;
static int MusicCount;
static word *MusicData;
static byte AdlibBlock;
static byte *AdlibData;
static int AdlibLength;

#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION - MIDI to AdLib translation, from ID_SD.C of the wolf3d recreation
// The recreated code wrote straight to the AdLib card, here it feeds the OPL emulator
#define alOut(n, b) OPLWrite(OPL, (n), (b))

#define alChar 0x20
#define alScale 0x40
#define alAttack 0x60
#define alSus 0x80
#define alWave 0xe0
#define alFreqL 0xa0
#define alFreqH 0xb0
#define alFeedCon 0xc0
#define alEffects 0xbd

typedef struct
{
	byte mChar, cChar,
		mScale, cScale,
		mAttack, cAttack,
		mSustain, cSustain,
		mWave, cWave,
		mFeedConn;
} inst_t;

static const byte carriers[9] = {3, 4, 5, 11, 12, 13, 19, 20, 21},
				  modifiers[9] = {0, 1, 2, 8, 9, 10, 16, 17, 18};

static int midiError = 0;
static float midiTimeScale = 1.86;
static byte *midiData, *midiDataStart;
static byte midiRunningStatus;
static longword midiLength, midiDeltaTime;
static boolean midiOn = false;

static longword
MIDI_VarLength(void)
{
	longword value = 0;
	while (*midiData & 0x80)
		value = (value << 7) + (*midiData++ & 0x7F);
	value = (value << 7) + *midiData++;
	return value;
}



static word	NoteTable[12] = {0x157,0x16b,0x181,0x198,0x1b0,0x1ca,0x1e5,0x202,0x220,0x241,0x263,0x287};

static byte	drums = 0;

static inst_t	instrument[14] = {
	{0x21, 0x31, 0x4f, 0x00, 0xf2, 0xd2, 0x52, 0x73, 0x00, 0x00, 0x06},
	{0x01, 0x31, 0x4f, 0x04, 0xf0, 0x90, 0xff, 0x0f, 0x00, 0x00, 0x06},
	{0x31, 0x22, 0x10, 0x04, 0x83, 0xf4, 0x9f, 0x78, 0x00, 0x00, 0x0a},
	{0x11, 0x31, 0x05, 0x00, 0xf9, 0xf1, 0x25, 0x34, 0x00, 0x00, 0x0a},
	{0x31, 0x61, 0x1c, 0x80, 0x41, 0x92, 0x0b, 0x3b, 0x00, 0x00, 0x0e},
	{0x21, 0x21, 0x19, 0x80, 0x43, 0x85, 0x8c, 0x2f, 0x00, 0x00, 0x0c},
	{0x21, 0x24, 0x94, 0x05, 0xf0, 0x90, 0x09, 0x0a, 0x00, 0x00, 0x0a},
	{0x21, 0xa2, 0x83, 0x8d, 0x74, 0x65, 0x17, 0x17, 0x00, 0x00, 0x07},
	{0x01, 0x01, 0x00, 0x00, 0xff, 0xff, 0x07, 0x07, 0x00, 0x00, 0x07},
	{0x10, 0x00, 0x00, 0x00, 0xd8, 0x87, 0x4a, 0x3c, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x11, 0x11, 0xfa, 0xfa, 0xb5, 0xb5, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x00, 0x00, 0xf8, 0xf8, 0x88, 0xb5, 0x00, 0x00, 0x00},
	{0x15, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01},
	{0x21, 0x11, 0x4c, 0x00, 0xf1, 0xf2, 0x63, 0x72, 0x00, 0x00, 0xc0}
};



static void
MIDI_SkipMetaEvent(void)
{
	longword length = MIDI_VarLength();
	midiData += length;
}

static void
MIDI_NoteOff(int channel, int note, int velocity)
{
	unsigned	fnumber;
	byte	octave;
	if (channel == 9)
	{
		switch (note)
		{
		case 0x23:
		case 0x24:
			drums &= 0xef;
			break;
		case 0x26:
		case 0x28:
			drums &= 0xf7;
			break;
		case 0x2a:
			drums &= 0xfe;
			break;
		default:
			midiError = -11;
		}
		alOut(alEffects,alChar|drums);
		return;
	}

	fnumber = NoteTable[note%12];
	octave = ((note/12)&7)<<2;
	alOut(alFreqL + 1 + channel, fnumber&0xFF);
	alOut(alFreqH + 1 + channel, octave+((fnumber>>8)&3));
}

static void
MIDI_NoteOn(int channel, byte note, byte velocity)
{
	unsigned	fnumber;
	int	octave;
	if (velocity)
	{
		if (channel == 9)
		{
			switch (note)
			{
			case 0x23:
			case 0x24:
				drums |= 0x10;
				break;
			case 0x26:
				drums |= 0x8;
				break;
			case 0x28:
				drums |= 0x4;
				break;
			case 0x2a:
				drums |= 1;
				break;
			default:
				midiError = -11;
			}
			alOut(alEffects,alChar|drums);
		}
		else
		{
			fnumber = NoteTable[note%12];
			octave = note/12;
			alOut(alFreqL + 1 + channel, fnumber&0xFF);
			alOut(alFreqH + 1 + channel, alChar|(octave<<2)|((fnumber>>8)&3));
		}
	}
	else
		MIDI_NoteOff (channel,note,velocity);
}

static void
MIDI_ControllerChange(int channel, int id, int value)
{
}

static void
MIDI_ProgramChange(int channel, int id)
{
	// S3DNA RESTORATION - While an inst_t pointer can be used with direct
	// access to all fields, based on generated machine code it looks like
	// this wasn't the way the code was written
	byte	*inst;
	if (channel == 9)
	{
		int	note;
		unsigned	fnumber;
		int	octave;

		inst = (byte *)&instrument[9];
		alOut(modifiers[6]+alChar, *inst++);
		alOut(carriers[6]+alChar, *inst++);
		alOut(modifiers[6]+alScale, *inst++);
		alOut(carriers[6]+alScale, *inst++);
		alOut(modifiers[6]+alAttack, *inst++);
		alOut(carriers[6]+alAttack, *inst++);
		alOut(modifiers[6]+alSus, *inst++);
		alOut(carriers[6]+alSus, *inst++);
		alOut(modifiers[6]+alWave, *inst++);
		alOut(carriers[6]+alWave, *inst++);

		alOut(alFeedCon+6, *inst);

		note = 24;
		fnumber = NoteTable[note%12];
		octave = ((note/12)&7)<<2;
		alOut(alFreqL+6,fnumber&0xFF);
		alOut(alFreqH+6,octave+((fnumber>>8)&3));
		note = 24;
		fnumber = NoteTable[note%12];
		octave = ((note/12)&7)<<2;
		alOut(alFreqL+7,fnumber&0xFF);
		alOut(alFreqH+7,octave+((fnumber>>8)&3));
		note = 24;
		fnumber = NoteTable[note%12];
		octave = ((note/12)&7)<<2;
		alOut(alFreqL+8,fnumber&0xFF);
		alOut(alFreqH+8,octave+((fnumber>>8)&3));

		inst = (byte *)&instrument[10];
		alOut(0x31,*inst); inst += 2;
		alOut(0x51,*inst); inst += 2;
		alOut(0x71,*inst); inst += 2;
		alOut(0x91,*inst); inst += 2;

		alOut(0xF1,*inst);
		alOut(0xC7,0);

		inst = (byte *)&instrument[12];
		alOut(0x32,*inst); inst += 2;
		alOut(0x52,*inst); inst += 2;
		alOut(0x72,*inst); inst += 2;
		alOut(0x92,*inst); inst += 2;

		alOut(0xF2,*inst);

		inst = (byte *)&instrument[11];
		alOut(0x34,*inst); inst += 2;
		alOut(0x54,*inst); inst += 2;
		alOut(0x74,*inst); inst += 2;
		alOut(0x94,*inst); inst += 2;

		alOut(0xF4,*inst);
		alOut(0xC8,0);

		inst = (byte *)&instrument[10];
		alOut(0x35,*inst); inst += 2;
		alOut(0x55,*inst); inst += 2;
		alOut(0x75,*inst); inst += 2;
		alOut(0x95,*inst); inst += 2;

		alOut(0xF5,*inst);

		return;
	}

	if (channel < 5)
	{
		switch (id & 0xF8)
		{
		case 0:
			inst = (byte *)&instrument[0];
			break;
		case 8:
			inst = (byte *)&instrument[8];
			break;
		case 16:
			inst = (byte *)&instrument[1];
			break;
		case 24:
			inst = (byte *)&instrument[0];
			break;
		case 32:
			inst = (byte *)&instrument[2];
			break;
		case 40:
		case 48:
			inst = (byte *)&instrument[0];
			break;
		case 56:
		case 64:
			inst = (byte *)&instrument[6];
			break;
		case 72:
			inst = (byte *)&instrument[7];
			break;
		case 80:
		case 88:
		case 96:
			inst = (byte *)&instrument[0];
			break;
		case 104:
		case 112:
		case 120:
			inst = (byte *)&instrument[8];
			break;
		default:
			midiError = -8;
			return;
		}
		
		alOut(modifiers[channel+1]+alChar, *inst++);
		alOut(carriers[channel+1]+alChar, *inst++);
		alOut(modifiers[channel+1]+alScale, *inst++);
		alOut(carriers[channel+1]+alScale, *inst++);
		alOut(modifiers[channel+1]+alAttack, *inst++);
		alOut(carriers[channel+1]+alAttack, *inst++);
		alOut(modifiers[channel+1]+alSus, *inst++);
		alOut(carriers[channel+1]+alSus, *inst++);
		alOut(modifiers[channel+1]+alWave, *inst++);
		alOut(carriers[channel+1]+alWave, *inst++);

		alOut(alFeedCon+channel, *inst);
	}
}

static void
MIDI_ChannelPressure(int channel, int id)
{
}

static void
MIDI_ProcessEvent(byte event)
{
	byte	note,velocity,id,value;
	switch (event&0xF0)
	{
	case 0x80:
		note = *midiData++;
		velocity = *midiData++;
		MIDI_NoteOff(event&0xF,note,velocity);
		break;
	case 0x90:
		note = *midiData++;
		velocity = *midiData++;
		MIDI_NoteOn(event&0xF,note,velocity);
		break;
	case 0xB0:
		id = *midiData++;
		value = *midiData++;
		MIDI_ControllerChange(event&0xF,id,value);
		break;
	case 0xC0:
		value = *midiData++;
		MIDI_ProgramChange(event&0xF,value);
		break;
	case 0xD0:
		value = *midiData++;
		MIDI_ChannelPressure(event&0xF,value);
		break;
	default:
		midiError = -7;
		break;
	}
}

static void
MIDI_DoEvent(void)
{
	byte	event;
	longword	length;
	longword	tempo;

	event = *midiData++;
	if (!(event & 0x80))
	{
		if (!(midiRunningStatus | 0x00))
			return;

		midiData--;
		MIDI_ProcessEvent(midiRunningStatus);
	}
	else if (event < 0xF0)
	{
		midiRunningStatus = event;
		MIDI_ProcessEvent(midiRunningStatus);
	}
	else if (event == 0xF0)
	{
		midiRunningStatus = 0;
		midiError = -4;
	}
	else if (event == 0xF7)
	{
		midiRunningStatus = 0;
		midiError = -5;
	}
	else if (event == 0xFF)
	{
		midiRunningStatus = 0;
		event = *midiData++;
		switch (event)
		{
		case 0x51:
			length = MIDI_VarLength();
			tempo = ((long)(*midiData)<<16) + (long)((*(midiData+1))<<8) + (*(midiData+2));
			midiTimeScale = (double)tempo/2.74176e5;
			midiTimeScale *= 1.1;
			midiData += length;
			break;
		case 0x2F:
			midiData = midiDataStart;
			midiDeltaTime = 0;
			break;
		default:
			MIDI_SkipMetaEvent();
			break;
		}
	}
	else
		midiError = -6;
}

static void
MIDI_IRQService(void)
{
	int	maxevent = 0;

	if (!midiOn)
		return;

	if (midiDeltaTime)
	{
		midiDeltaTime--;
		return;
	}

	while (!midiDeltaTime && (maxevent++ < 32))
	{
		MIDI_DoEvent();
		midiDeltaTime = MIDI_VarLength();
	}

	if (maxevent >= 32)
		midiError = -1;
	else if (midiDeltaTime & 0xFFFF0000)
	{
		midiError = -2;
		return;
	}

	midiDeltaTime = midiDeltaTime * midiTimeScale;
}

// PSP_DEVIATION: reads of the (big endian) MIDI header, byte by byte since the chunk isn't necessarily aligned
static unsigned int MIDI_ReadBE(const byte *p, int bytes)
{
	unsigned int v = 0;

	while (bytes--)
		v = (v << 8) | *p++;
	return v;
}

// Called (from the audio thread) when new music has been selected
static void MIDI_Start(MusicGroup *music)
{
	byte *seqPtr = (byte *)music->values;
	unsigned int i;

	midiOn = false;
	midiError = 0;

	if (memcmp(seqPtr, "MThd", 4))
		return; // MIDI header expected
	if (MIDI_ReadBE(seqPtr + 8, 2) != 0 || MIDI_ReadBE(seqPtr + 10, 2) != 1)
		return; // invalid or unsupported MIDI file format

	seqPtr += MIDI_ReadBE(seqPtr + 4, 4) + 8;
	if (memcmp(seqPtr, "MTrk", 4))
		return; // MIDI track header expected
	i = MIDI_ReadBE(seqPtr + 4, 4);
	if (!i)
		return; // MIDI track is 0 length

	seqPtr += 8;
	midiData = seqPtr;
	midiDataStart = seqPtr;
	midiLength = i;
	midiDeltaTime = 0;
	midiDeltaTime = MIDI_VarLength();
	if (midiDeltaTime & 0xFFFF0000)
	{
		midiError = -2;
		return;
	}

	midiRunningStatus = 0;
	MIDI_ProgramChange(9, 0);
	alOut(alEffects, alChar);
	drums = 0;
	midiOn = true;
}
#endif /* GAMEVER_NOAH3D */

static void SetSoundLoc(fixed gx, fixed gy);
static boolean SD_PlayDirSound(soundnames sound, fixed gx, fixed gy);

void SD_SetVolume(int vol)
{
	Volume = vol;
}

int SD_GetVolume()
{
	return Volume;
}

void InitSoundBuff(void)
{
	MusicLength = 0;
	MusicCount = 0;
	MusicData = NULL;
	AdlibBlock = 0;
	AdlibData = NULL;
	AdlibLength = -1;

	OPLWrite(OPL, 0x01, 0x20); /* Set WSE=1 */
	OPLWrite(OPL, 0x08, 0x00); /* Set CSM=0 & SEL=0 */
}

void FillSoundBuff(void)
{
	int i, j, snd;
	short int samp;
#ifndef GAMEVER_NOAH3D // PSP_DEVIATION: 'dat' is only used by the AdLib music path (Noah3D uses MIDI)
	word dat;
#endif

	AdLibSound *AdlibSnd;
	Instrument *inst;

	// start by computing next buffer worth of music
	if ((MusicMode != smm_Off) || (SoundMode != sdm_Off))
	{
		if (NewAdlib != -1)
		{
			AdlibPlaying = NewAdlib;
			AdlibSnd = (AdLibSound *)audiosegs[STARTADLIBSOUNDS + AdlibPlaying];
			inst = (Instrument *)&AdlibSnd->inst;
#define alChar 0x20
#define alScale 0x40
#define alAttack 0x60
#define alSus 0x80
#define alFeedCon 0xc0 // PSP_DEVIATION: lowercase hex to match earlier identical definition (no redefinition warning)
#define alWave 0xe0

			// PSP_DEVIATION: OPLResetChip (called by SD_MusicOff on every music start/stop) clears the
			// waveform select enable bit (register 0x01 bit 5), after which the chip ignores the alWave
			// registers and every instrument plays as a plain sine. InitSoundBuff only sets it once at
			// startup, so re-enable it before each sound effect.
			OPLWrite(OPL, 0x01, 0x20);

			OPLWrite(OPL, 0 + alChar, 0);
			OPLWrite(OPL, 0 + alScale, 0);
			OPLWrite(OPL, 0 + alAttack, 0);
			OPLWrite(OPL, 0 + alSus, 0);
			OPLWrite(OPL, 0 + alWave, 0);
			OPLWrite(OPL, 3 + alChar, 0);
			OPLWrite(OPL, 3 + alScale, 0);
			OPLWrite(OPL, 3 + alAttack, 0);
			OPLWrite(OPL, 3 + alSus, 0);
			OPLWrite(OPL, 3 + alWave, 0);
			OPLWrite(OPL, 0xA0, 0);
			OPLWrite(OPL, 0xB0, 0);

			OPLWrite(OPL, 0 + alChar, inst->mChar);
			OPLWrite(OPL, 0 + alScale, inst->mScale);
			OPLWrite(OPL, 0 + alAttack, inst->mAttack);
			OPLWrite(OPL, 0 + alSus, inst->mSus);
			OPLWrite(OPL, 0 + alWave, inst->mWave);
			OPLWrite(OPL, 3 + alChar, inst->cChar);
			OPLWrite(OPL, 3 + alScale, inst->cScale);
			OPLWrite(OPL, 3 + alAttack, inst->cAttack);
			OPLWrite(OPL, 3 + alSus, inst->cSus);
			OPLWrite(OPL, 3 + alWave, inst->cWave);

			//OPLWrite(OPL, alFeedCon, inst->nConn);
			OPLWrite(OPL, alFeedCon, 0);

			AdlibBlock = ((AdlibSnd->block & 7) << 2) | 0x20;
			AdlibData = (byte *)&AdlibSnd->data;
			AdlibLength = AdlibSnd->common.length * 5;
			//OPLWrite(OPL, 0xB0, AdlibBlock);
			NewAdlib = -1;
		}

		if (NewMusic != -1)
		{
			NewMusic = -1;
#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION - the music is MIDI
			MIDI_Start(Music);
#else
			MusicLength = Music->length;
			MusicData = (word *)((byte *)Music + offsetof(MusicGroup, values)); // PSP_DEVIATION: avoids -Waddress-of-packed-member
			MusicCount = 0;
#endif
		}
		for (i = 0; i < 4; i++)
		{
#ifdef GAMEVER_NOAH3D // PSP_DEVIATION: S3DNA RESTORATION - two ticks of the 700Hz sequencer per step
			if (sqActive)
			{
				MIDI_IRQService();
				MIDI_IRQService();
			}
#else
			if (sqActive)
			{
				while (MusicCount <= 0)
				{
					dat = *MusicData++;
					MusicCount = *MusicData++;
					MusicLength -= 4;
					OPLWrite(OPL, dat & 0xFF, dat >> 8);
				}
				if (MusicLength <= 0)
				{
					NewMusic = 1;
				}
				MusicCount -= 2;
			}
#endif
			if (AdlibPlaying != -1)
			{
				// PSP_DEVIATION: AdLib sound effect playback rewritten to match the original id ALSoundService
				// (ID_SD.C). Each note byte is one 140Hz step (5 ticks of 700Hz; two ticks are processed per pass).
				// - A note byte of 0 is a KEY-OFF (0xB0 = 0), not "frequency 0 with the key held". The following
				//   non-zero note then keys the channel back on, which restarts the envelope. Without this the
				//   instruments' percussive envelopes decayed during the first burst and the rest of the effect
				//   was inaudible, since the key was never released and so never re-triggered.
				// - Rewriting 0xB0 with the key already on is harmless (the OPL only retriggers on a key transition).
				// - The sound ends with a key-off so the channel is not left keyed.
				int t;
				for (t = 0; t < 2 && AdlibPlaying != -1; t++)
				{
					if (AdlibLength <= 0)
					{
						OPLWrite(OPL, 0xB0, 0);
						AdlibPlaying = -1;
						break;
					}
					if ((AdlibLength % 5) == 0)
					{
						if (*AdlibData)
						{
							OPLWrite(OPL, 0xA0, *AdlibData);
							OPLWrite(OPL, 0xB0, AdlibBlock);
						}
						else
							OPLWrite(OPL, 0xB0, 0);
						AdlibData++;
					}
					AdlibLength--;
				}
			}

			YM3812UpdateOne(OPL, &musbuf[i * NUM_SAMPS / 4], NUM_SAMPS / 4);
		}
	}

	//prefill audio buffer with music
	for (i = 0; i < NUM_SAMPS * 2; i++)
		sdlbuf[i] = musbuf[i / 2];

	// now add in sound effects
	for (j = 0; j < NUM_SFX; j++)
	{
		if (SoundPlaying[j] == -1)
			continue;

		// compute L/R scale based on player position and stored sound position
		if (SoundPositioned[j])
		{
			SetSoundLoc(SoundGX[j], SoundGY[j]);
			L = leftchannel;
			R = rightchannel;
		}

		// now add sound effect data to buffer
		for (i = 0; i < NUM_SAMPS * 2; i += 2)
		{
			samp = (SoundData[j][(SoundPlayPos[j] >> 16)] << 8) ^ 0x8000;
			if (SoundPositioned[j])
			{
				snd = samp * (16 - L) >> 5;
				sdlbuf[i + 0] += snd;
				snd = samp * (16 - R) >> 5;
				sdlbuf[i + 1] += snd;
			}
			else
			{
				snd = samp >> 2;
				sdlbuf[i + 0] += snd;
				sdlbuf[i + 1] += snd;
			}
			SoundPlayPos[j] += 10402; // 7000 / 44100 * 65536
			if ((SoundPlayPos[j] >> 16) >= SoundPlayLen[j])
			{
				SoundPlayPos[j] -= (SoundPlayLen[j] << 16);
				SoundLen[j] -= 4096;
				SoundPlayLen[j] = (SoundLen[j] < 4096) ? SoundLen[j] : 4096;
				if (SoundLen[j] <= 0)
				{
					SoundPlaying[j] = -1;
					CurDigi[j] = -1;
					i = NUM_SAMPS * 2;
				}
				else
				{
					SoundPage[j]++;
					SoundData[j] = PM_GetSoundPage(SoundPage[j]);
				}
			}
		}
	}
}

void SoundCallBack(void *unused, Uint8 *stream, int len)
{
	Uint8 *waveptr;
	int waveleft;

	waveptr = (Uint8 *)sdlbuf + bufpos;
	waveleft = sizeof(sdlbuf) - bufpos;

	SDL_MixAudio(stream, waveptr, len, Volume * SDL_MIX_MAXVOLUME / 10);
	bufpos += len;
	if (len >= waveleft)
		bufpos = 0;

	if (bufpos == 0)
		FillSoundBuff();
}

void Blah()
{
	memptr list;
	word *p, pg;
	int i;

	if (DigiList != NULL)
		free(DigiList);

	MM_GetPtr(&list, PMPageSize);
	p = PM_GetPage(ChunksInFile - 1);
	memcpy((void *)list, (void *)p, PMPageSize);

	pg = PMSoundStart;
	for (i = 0; i < PMPageSize / (sizeof(word) * 2); i++, p += 2)
	{
		if (pg >= ChunksInFile - 1)
			break;
		pg += (p[1] + (PMPageSize - 1)) / PMPageSize;
	}
	MM_GetPtr((memptr *)&DigiList, i * sizeof(word) * 2);
	memcpy((void *)DigiList, (void *)list, i * sizeof(word) * 2);
	MM_FreePtr(&list);
}

void SD_Startup()
{
	SDL_AudioSpec aspec;
	int i;

	if (SD_Started)
		return;

	Blah();
	InitDigiMap();
	OPL = OPLCreate(OPL_TYPE_YM3812, 3579545, 44100);

	for (i = 0; i < NUM_SFX; i++)
	{
		SoundPlaying[i] = -1;
		CurDigi[i] = -1;
	}

	CurAdlib = -1;
	NewAdlib = -1;
	NewMusic = -1;
	AdlibPlaying = -1;
	sqActive = false;

	if (SDL_Init(SDL_INIT_AUDIO) < 0)
	{
		printf("failed to init audio\n");
		return;
	}

	aspec.freq = 44100;
	aspec.format = AUDIO_S16SYS;
	aspec.channels = 2;
	aspec.samples = NUM_SAMPS;
	aspec.callback = SoundCallBack;
	if (SDL_OpenAudio(&aspec, NULL) < 0)
	{
		printf("couldn't open audio with desired format\n");
		return;
	}

	printf("Configured audio device with %d samples/slice\n", aspec.samples);
	InitSoundBuff();
	SDL_PauseAudio(0);

	SD_Started = true;
}

void SD_Shutdown()
{
	if (!SD_Started)
		return;

	SD_MusicOff();
	SD_StopSound();

	SD_Started = false;
	OPLDestroy(OPL);
}

///////////////////////////////////////////////////////////////////////////
//
//	SD_PlaySound() - plays the specified sound on the appropriate hardware
//
///////////////////////////////////////////////////////////////////////////
boolean SD_PlaySound(soundnames sound)
{
	SPHack = false;
	return SD_PlayDirSound(sound, 0, 0);
}

static boolean SD_PlayDirSound(soundnames sound, fixed gx, fixed gy)
{
	SoundCommon *s;

	s = (SoundCommon *)audiosegs[STARTADLIBSOUNDS + sound];

	if (DigiMode != sds_Off)
	{
		if (DigiMap[sound] != -1)
		{
			int i;

			for (i = 0; i < NUM_SFX; i++)
				if (SoundPlaying[i] == -1)
					break;
			if (i == NUM_SFX)
				for (i = 0; i < NUM_SFX; i++)
					if (s->priority >= ((SoundCommon *)audiosegs[STARTADLIBSOUNDS + CurDigi[i]])->priority)
						break;
			if (i == NUM_SFX)
				return false; // no free channels, and priority not high enough to preempt a playing sound

			SoundPage[i] = DigiList[(DigiMap[sound] * 2) + 0];
			SoundLen[i] = DigiList[(DigiMap[sound] * 2) + 1];
			/* PSP_DEVIATION: load every page of the digitized sound here, on the game thread.
			   Digi sounds longer than one page (PMPageSize) used to load their later pages
			   lazily from FillSoundBuff, which runs in SDL's audio callback thread. That
			   thread then called PM_GetPage concurrently with the game thread, racing on
			   the shared VSWAP file handle (seek+read) and on malloc. A read could land at
			   the wrong offset, and the wrong data stayed cached forever, which showed up
			   as swapped E/W vs N/S wall textures, randomly corrupted sprites
			   and bursts of static in the audio. Only the Vita (Adrenaline)
			   reproduced it because its slow storage widens the race window; PPSSPP's
			   instant I/O hid it. With every page cached before playback starts, the audio
			   thread only reads already-cached pages and never touches file I/O or malloc.
			   Do not call PM_* or any file/allocation code from the audio callback. */
			{
				int k, pages = (SoundLen[i] + (PMPageSize - 1)) / PMPageSize;
				for (k = 0; k < pages; k++)
					PM_GetSoundPage(SoundPage[i] + k);
			}
			SoundData[i] = PM_GetSoundPage(SoundPage[i]);
			SoundPlayLen[i] = (SoundLen[i] < 4096) ? SoundLen[i] : 4096;
			SoundPlayPos[i] = 0;
			if (SPHack)
			{
				SPHack = false;
				SoundGX[i] = gx;
				SoundGY[i] = gy;
				SoundPositioned[i] = true;
			}
			else
			{
				SoundPositioned[i] = false;
			}
			CurDigi[i] = sound;
			SoundPlaying[i] = DigiMap[sound];
			return true;
		}
	}

	if (SoundMode != sdm_Off)
	{
		if ((AdlibPlaying == -1) || (CurAdlib == -1) ||
			(s->priority >= ((SoundCommon *)audiosegs[STARTADLIBSOUNDS + CurAdlib])->priority))
		{
			CurAdlib = sound;
			NewAdlib = sound;
			return true;
		}
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////
//
//	SD_SoundPlaying() - returns the sound number of first playing sound,
//		or 0 if nothing playing
//
///////////////////////////////////////////////////////////////////////////
word SD_SoundPlaying()
{
	int i;

	for (i = 0; i < NUM_SFX; i++)
		if (SoundPlaying[i] != -1)
			return CurDigi[i];

	if (AdlibPlaying != -1)
		return CurAdlib;

	return 0;
}

///////////////////////////////////////////////////////////////////////////
//
//	SD_StopSound() - if a sound is playing, stops it
//
///////////////////////////////////////////////////////////////////////////
void SD_StopSound()
{
	int i;

	for (i = 0; i < NUM_SFX; i++)
	{
		SoundPlaying[i] = -1;
		CurDigi[i] = -1;
	}
}

///////////////////////////////////////////////////////////////////////////
//
//	SD_WaitSoundDone() - waits until the current sound is done playing
//
///////////////////////////////////////////////////////////////////////////
void SD_WaitSoundDone()
{
	while (SD_SoundPlaying())
		sceKernelDelayThread(1000);
}

/*
==========================
=
= SetSoundLoc - Given the location of an object, munges the values
=	for an approximate distance from the left and right ear, and puts
=	those values into leftchannel and rightchannel.
=
= JAB
=
==========================
*/

#define ATABLEMAX 15
static const byte righttable[ATABLEMAX][ATABLEMAX * 2] = {
	{8, 8, 8, 8, 8, 8, 8, 7, 7, 7, 7, 7, 7, 6, 0, 0, 0, 0, 0, 1, 3, 5, 8, 8, 8, 8, 8, 8, 8, 8},
	{8, 8, 8, 8, 8, 8, 8, 7, 7, 7, 7, 7, 6, 4, 0, 0, 0, 0, 0, 2, 4, 6, 8, 8, 8, 8, 8, 8, 8, 8},
	{8, 8, 8, 8, 8, 8, 8, 7, 7, 7, 7, 6, 6, 4, 1, 0, 0, 0, 1, 2, 4, 6, 8, 8, 8, 8, 8, 8, 8, 8},
	{8, 8, 8, 8, 8, 8, 8, 7, 7, 7, 7, 6, 5, 4, 2, 1, 0, 1, 2, 3, 5, 7, 8, 8, 8, 8, 8, 8, 8, 8},
	{8, 8, 8, 8, 8, 8, 8, 8, 7, 7, 7, 6, 5, 4, 3, 2, 2, 3, 3, 5, 6, 8, 8, 8, 8, 8, 8, 8, 8, 8},
	{8, 8, 8, 8, 8, 8, 8, 8, 7, 7, 7, 6, 6, 5, 4, 4, 4, 4, 5, 6, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8},
	{8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 7, 7, 6, 6, 5, 5, 5, 6, 6, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
	{8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 7, 7, 6, 6, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
	{8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
	{8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
	{8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
	{8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
	{8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
	{8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
	{8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8}};
static const byte lefttable[ATABLEMAX][ATABLEMAX * 2] = {
	{8, 8, 8, 8, 8, 8, 8, 8, 5, 3, 1, 0, 0, 0, 0, 0, 6, 7, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8},
	{8, 8, 8, 8, 8, 8, 8, 8, 6, 4, 2, 0, 0, 0, 0, 0, 4, 6, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8},
	{8, 8, 8, 8, 8, 8, 8, 8, 6, 4, 2, 1, 0, 0, 0, 1, 4, 6, 6, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8},
	{8, 8, 8, 8, 8, 8, 8, 8, 7, 5, 3, 2, 1, 0, 1, 2, 4, 5, 6, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8},
	{8, 8, 8, 8, 8, 8, 8, 8, 8, 6, 5, 3, 3, 2, 2, 3, 4, 5, 6, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8},
	{8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 6, 5, 4, 4, 4, 4, 5, 6, 6, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8},
	{8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 6, 6, 5, 5, 5, 6, 6, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8},
	{8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 7, 6, 6, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
	{8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
	{8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
	{8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
	{8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
	{8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
	{8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
	{8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8}};

static void SetSoundLoc(fixed gx, fixed gy)
{
	fixed xt, yt;
	int x, y;

	// translate point to view centered coordinates
	//
	gx -= viewx;
	gy -= viewy;

	//
	// calculate newx
	//
	xt = FixedByFrac(gx, viewcos);
	yt = FixedByFrac(gy, viewsin);
	x = (xt - yt) >> TILESHIFT;

	//
	// calculate newy
	//
	xt = FixedByFrac(gx, viewsin);
	yt = FixedByFrac(gy, viewcos);
	y = (yt + xt) >> TILESHIFT;

	if (y >= ATABLEMAX)
		y = ATABLEMAX - 1;
	else if (y <= -ATABLEMAX)
		y = -ATABLEMAX;
	if (x < 0)
		x = -x;
	if (x >= ATABLEMAX)
		x = ATABLEMAX - 1;

	leftchannel = lefttable[x][y + ATABLEMAX];
	rightchannel = righttable[x][y + ATABLEMAX];
}

/*
==========================
=
= SetSoundLocGlobal - Sets up globalsoundx & globalsoundy and then calls
=	UpdateSoundLoc() to transform that into relative channel volumes. Those
=	values are then passed to the Sound Manager so that they'll be used for
=	the next sound played (if possible).
=
==========================
*/

void PlaySoundLocGlobal(word s, int id, fixed gx, fixed gy)
{
	SPHack = true;
	SD_PlayDirSound(s, gx, gy);
}

// tell sound driver the current player moved/turned
void UpdateSoundLoc(fixed x, fixed y, int angle)
{
	// SetSoundLoc uses viewx, viewy, viewcos, viewsin in FillBuffer
}

///////////////////////////////////////////////////////////////////////////
//
//	SD_MusicOn() - turns on the sequencer
//
///////////////////////////////////////////////////////////////////////////
void SD_MusicOn()
{
	sqActive = true;
}

///////////////////////////////////////////////////////////////////////////
//
//	SD_MusicOff() - turns off the sequencer and any playing notes
//
///////////////////////////////////////////////////////////////////////////
void SD_MusicOff()
{
	int j;

	sqActive = false;
	OPLResetChip(OPL);
	OPLWrite(OPL, 0x01, 0x20); // PSP_DEVIATION: OPLResetChip clears WSE (waveform select); restore it for music and effects

	for (j = 0; j < NUM_SAMPS; j++)
		musbuf[j] = 0;
}

///////////////////////////////////////////////////////////////////////////
//
//	SD_StartMusic() - starts playing the music pointed to
//
///////////////////////////////////////////////////////////////////////////
void SD_StartMusic(int music)
{
	music += STARTMUSIC;

	CA_CacheAudioChunk(music);

	if (MusicMode != smm_Off) // PSP_DEVIATION: was sdm_Off (wrong enum, same value 0)
	{
		SD_MusicOff();
		SD_MusicOn();
	}

	Music = (MusicGroup *)audiosegs[music];
	NewMusic = 1;
}

void SD_SetDigiDevice(SDSMode mode)
{
	if ((mode == sds_Off) || (mode == sds_SoundBlaster))
	{
		DigiMode = mode;
	}
}

///////////////////////////////////////////////////////////////////////////
//
//	SD_SetSoundMode() - Sets which sound hardware to use for sound effects
//
///////////////////////////////////////////////////////////////////////////
boolean SD_SetSoundMode(SDMode mode)
{
	if ((mode == sdm_Off) || (mode == sdm_AdLib))
	{
		SoundMode = mode;
		return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////
//
//	SD_SetMusicMode() - sets the device to use for background music
//
///////////////////////////////////////////////////////////////////////////
boolean SD_SetMusicMode(SMMode mode)
{
	if (mode == smm_Off)
	{
		MusicMode = mode;
		SD_MusicOff();
		return true;
	}
	if (mode == smm_AdLib)
	{
		MusicMode = mode;
		//SD_MusicOn();
		return true;
	}
	return false;
}
