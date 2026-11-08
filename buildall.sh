#!/bin/bash
# Run inside WSL by build.cmd. Builds dist/<GAME>/ for every game, or only the games given as arguments.
export PSPDEV=$HOME/pspdev
export PATH=$PSPDEV/bin:/usr/local/bin:/usr/bin:/bin
cd "$(dirname "$0")" || exit 1

if [ $# -gt 0 ]; then
	GAMES="$*"
else
	GAMES="WOLF3D_SHAREWARE WOLF3D_FULL WOLF3D_SOD_SHAREWARE WOLF3D_SOD_FULL WOLF3D_SOD_MISSION2 WOLF3D_SOD_MISSION3 WOLF3D_NOAH3D"
fi

for GAME in $GAMES; do
	echo "=== $GAME ==="
	# Objects are compiled with -DWMODE=<GAME>, so a clean is required between games
	make clean >/dev/null 2>&1
	make GAME=$GAME -j8 package || { echo "FAILED: $GAME"; exit 1; }
done
# Remove the intermediate EBOOT.PBP/.elf/.prx/.o files from the project root; dist/ is kept
make clean >/dev/null 2>&1
echo "=== All builds succeeded; output is in dist/ ==="
