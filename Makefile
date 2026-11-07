TARGET = wolf3d
OBJS = 	src/automap.o \
		src/ff_psp.o \
		src/id_ca.o \
		src/id_us.o \
		src/id_vh.o \
		src/misc.o \
		src/objs.o \
		src/fmopl.o \
		src/vi_comm.o \
		src/vi_sdl.o \
		src/wl_act1.o \
		src/wl_act2.o \
		src/wl_act3.o \
		src/wl_agent.o \
		src/wl_debug.o \
		src/sd_comm.o \
		src/sd_sdl.o \
		src/wl_draw.o \
		src/wl_game.o \
		src/wl_inter.o \
		src/wl_main.o \
		src/wl_text.o \
		src/wl_menu.o \
		src/wl_play.o \
		src/wl_state.o \
		src/wl_map.o \
		src/wl_quiz.o \

## game selection: make GAME=<name>
# GAME is the WMODE macro from src/include/version.h, and also names the data directory:
#   WOLF3D_SHAREWARE, WOLF3D_FULL, WOLF3D_SOD_SHAREWARE, WOLF3D_SOD_FULL,
#   WOLF3D_SOD_MISSION2, WOLF3D_SOD_MISSION3, WOLF3D_NOAH3D
# The legacy numeric VERSION=<n> (the same macros' values) is still accepted.
LEGACY_GAME_0 = WOLF3D_SHAREWARE
LEGACY_GAME_1 = WOLF3D_FULL
LEGACY_GAME_2 = WOLF3D_SOD_SHAREWARE
LEGACY_GAME_3 = WOLF3D_SOD_FULL
LEGACY_GAME_4 = WOLF3D_SOD_MISSION2
LEGACY_GAME_5 = WOLF3D_SOD_MISSION3
LEGACY_GAME_6 = WOLF3D_NOAH3D

ifndef GAME
    ifdef VERSION
        GAME = $(LEGACY_GAME_$(VERSION))
    else
        # default to WOLF3D shareware if no GAME param is passed when building
        GAME = WOLF3D_SHAREWARE
    endif
endif

TITLE_WOLF3D_SHAREWARE = Wolfenstein 3D Shareware
TITLE_WOLF3D_FULL = Wolfenstein 3D
TITLE_WOLF3D_SOD_SHAREWARE = Spear of Destiny Shareware
TITLE_WOLF3D_SOD_FULL = Spear of Destiny
TITLE_WOLF3D_SOD_MISSION2 = Spear of Destiny: Return to Danger
TITLE_WOLF3D_SOD_MISSION3 = Spear of Destiny: Ultimate Challenge
# PSP_DEVIATION: apostrophe must be shell-escaped because build.mak single-quotes the title
TITLE_WOLF3D_NOAH3D = Super 3-D Noah'\''s Ark

ifndef TITLE_$(GAME)
    $(error Unknown GAME '$(GAME)'. See the game selection comment in the Makefile)
endif
PSP_EBOOT_TITLE = $(TITLE_$(GAME))

# PSP_DEVIATION: pin C standard; modern GCC defaults to C23 where false/true are keywords (breaks id_heads.h enum)
# PSP_DEVIATION: -fcommon; GCC 10+ defaults to -fno-common, but headers here contain tentative definitions (menuitems, pad)
CFLAGS = -std=gnu99 -fcommon -O2 -G0 -Wall -g -DHAVE_FFBLK -DDOSISM -DWMODE=$(GAME)
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)
LIBDIR =
LIBS = -L/usr/local/pspsdk/lib
LDFLAGS =
# PSP_DEVIATION: build as a relocatable PRX. Modern pspdev links a fixed-address ELF at 0x08804000, which the Vita (Adrenaline)
# refuses to load with 800200D9 (SCE_KERNEL_ERROR_MEMBLOCK_ALLOC_FAILED); the 2020 build was linked at 0x08900000.
# A PRX lets the loader choose the address. Verified on a Vita running Adrenaline 6.61.
BUILD_PRX = 1
PSP_FW_VERSION = 401
# PSP_DEVIATION: PSP_LARGE_MEMORY=1 kept as in the original build (MEMSIZE=1 in PARAM.SFO). Untested without it on a Vita.
PSP_LARGE_MEMORY = 1
EXTRA_TARGETS = EBOOT.PBP

# PSP_DEVIATION: XMB menu assets live in menu/<family>/: ICON0.png (144x80), PIC1.png (480x272) and
# SND0.AT3 (ATRAC3, 44.1kHz, <= 55s). Each game maps to one family folder; any asset a family does not
# provide falls back to menu/WOLF3D/.
MENU_WOLF3D_SHAREWARE = WOLF3D
MENU_WOLF3D_FULL = WOLF3D
MENU_WOLF3D_SOD_SHAREWARE = SOD
MENU_WOLF3D_SOD_FULL = SOD
MENU_WOLF3D_SOD_MISSION2 = SOD
MENU_WOLF3D_SOD_MISSION3 = SOD
MENU_WOLF3D_NOAH3D = NOAH3D
MENUDIR = menu/$(MENU_$(GAME))
DEFAULT_MENUDIR = menu/WOLF3D
menu_asset = $(firstword $(wildcard $(MENUDIR)/$(1) $(DEFAULT_MENUDIR)/$(1)))
PSP_EBOOT_ICON = $(call menu_asset,ICON0.png)
PSP_EBOOT_PIC1 = $(call menu_asset,PIC1.png)
ifneq ($(call menu_asset,SND0.AT3),)
	PSP_EBOOT_SND0 = $(call menu_asset,SND0.AT3)
endif

PSPSDK=$(shell psp-config --pspsdk-path)
PSPBIN = $(PSPSDK)/../bin
CFLAGS += $(shell $(PSPBIN)/sdl-config --cflags)
# PSP_DEVIATION: modern pspdev sdl-config --libs ends with -lc -lpspuser, which put SDK stub libs out of order (psp-fixup-imports failed -> black screen)
# PSP_DEVIATION: -lpspirkeyb -lpsppower already come from sdl-config --libs
LIBS += -lstdc++
LIBS += $(filter-out -lc -lpspuser,$(shell $(PSPBIN)/sdl-config --libs))
include $(PSPSDK)/lib/build.mak

# Readme written next to the EBOOT for games whose data files are not in the repo (see the package target)
README_WOLF3D_FULL = In order to run the full version of the game, you need to place the wolf3d game data files here.
README_WOLF3D_SOD_FULL = In order to run Spear of Destiny, you need to place the Spear of Destiny game data files (*.sod) here.
README_WOLF3D_SOD_MISSION2 = In order to run Spear of Destiny Mission 2: Return to Danger, you need to place the mission pack game data files (*.sd2, or *.sod as sold by GOG) here.
README_WOLF3D_SOD_MISSION3 = In order to run Spear of Destiny Mission 3: Ultimate Challenge, you need to place the mission pack game data files (*.sd3, or *.sod as sold by GOG) here.
# PSP_DEVIATION: apostrophe is shell-escaped because the text is single-quoted in the package recipe
README_WOLF3D_NOAH3D = In order to run Super 3-D Noah'\''s Ark, you need to place the game data files (*.N3D) here.

# make package GAME=<name>: assemble dist/<GAME>/ ready to copy to the PSP (EBOOT, bundled data files, generated readme)
.PHONY: package
package: EBOOT.PBP
	rm -rf dist/$(GAME)
	mkdir -p dist/$(GAME)
	if [ -d $(GAME) ]; then cp $(GAME)/* dist/$(GAME)/; fi
	$(if $(README_$(GAME)),printf '%s\n' '$(README_$(GAME))' > dist/$(GAME)/readme.txt)
	cp EBOOT.PBP dist/$(GAME)/
