TARGET = wolf3d
OBJS = 	Src/automap.o \
		Src/ff_psp.o \
		Src/id_ca.o \
		Src/id_us.o \
		Src/id_vh.o \
		Src/misc.o \
		Src/objs.o \
		Src/fmopl.o \
		Src/vi_comm.o \
		Src/vi_sdl.o \
		Src/wl_act1.o \
		Src/wl_act2.o \
		Src/wl_act3.o \
		Src/wl_agent.o \
		Src/wl_debug.o \
		Src/sd_comm.o \
		Src/sd_sdl.o \
		Src/wl_draw.o \
		Src/wl_game.o \
		Src/wl_inter.o \
		Src/wl_main.o \
		Src/wl_text.o \
		Src/wl_menu.o \
		Src/wl_play.o \
		Src/wl_state.o \
		Src/wl_map.o \
		Src/wl_quiz.o \

## version flags
#WOLF3D_SHAREWARE 0
#WOLF3D_FULL 1
#WOLF3D_SOD_SHAREWARE 2
#WOLF3D_SOD_FULL 3
#WOLF3D_SOD_MISSION2 4
#WOLF3D_SOD_MISSION3 5
#WOLF3D_NOAH3D 6

ifndef VERSION
    # default to WOLF3D shareware if no VERSION param is passed when building
    VERSION=0
endif

# PSP_DEVIATION: pin C standard; modern GCC defaults to C23 where false/true are keywords (breaks id_heads.h enum)
# PSP_DEVIATION: -fcommon; GCC 10+ defaults to -fno-common, but headers here contain tentative definitions (menuitems, pad)
CFLAGS = -std=gnu99 -fcommon -O2 -G0 -Wall -g -DHAVE_FFBLK -DDOSISM -DWMODE=$(VERSION)
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)
LIBDIR =
LIBS = -L/usr/local/pspsdk/lib
LDFLAGS =
BUILD_PRX = 0
PSP_FW_VERSION = 401
# PSP_DEVIATION: PSP_LARGE_MEMORY=1 removed; SDK warns it is unnecessary for PSP_FW_VERSION > 390 (SDK default expands memory)
EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_ICON= ICON0.png
PSP_EBOOT_PIC1= PIC1.png

ifeq ($(VERSION), 0)
	PSP_EBOOT_TITLE = Wolfenstein 3D Shareware
endif

ifeq ($(VERSION), 1)
	PSP_EBOOT_TITLE = Wolfenstein 3D
endif

ifeq ($(VERSION), 2)
	PSP_EBOOT_TITLE = Spear of Destiny Shareware
endif

ifeq ($(VERSION), 3)
	PSP_EBOOT_TITLE = Spear of Destiny
endif

# PSP_DEVIATION: Super 3-D Noah's Ark
ifeq ($(VERSION), 6)
	# PSP_DEVIATION: apostrophe must be shell-escaped because build.mak single-quotes the title
	PSP_EBOOT_TITLE = Super 3-D Noah'\''s Ark
endif

# PSP_DEVIATION: Spear of Destiny mission packs
ifeq ($(VERSION), 4)
	PSP_EBOOT_TITLE = Spear of Destiny: Return to Danger
endif

ifeq ($(VERSION), 5)
	PSP_EBOOT_TITLE = Spear of Destiny: Ultimate Challenge
endif

PSPSDK=$(shell psp-config --pspsdk-path)
PSPBIN = $(PSPSDK)/../bin
CFLAGS += $(shell $(PSPBIN)/sdl-config --cflags)
# PSP_DEVIATION: modern pspdev sdl-config --libs ends with -lc -lpspuser, which put SDK stub libs out of order (psp-fixup-imports failed -> black screen)
# PSP_DEVIATION: -lpspirkeyb -lpsppower already come from sdl-config --libs
LIBS += -lstdc++
LIBS += $(filter-out -lc -lpspuser,$(shell $(PSPBIN)/sdl-config --libs))
include $(PSPSDK)/lib/build.mak
