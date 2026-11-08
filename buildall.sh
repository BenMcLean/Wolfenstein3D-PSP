#!/bin/bash
# Run inside WSL by build.cmd. Builds dist/<GAME>/ for every game, or only the games given as arguments.
# Objects go to build/<GAME>/ and the EBOOT straight to dist/<GAME>/, so the games share nothing and are
# built in parallel (CI does the same with one job per game; it calls make directly, not this script).
export PSPDEV=$HOME/pspdev
export PATH=$PSPDEV/bin:/usr/local/bin:/usr/bin:/bin
cd "$(dirname "$0")" || exit 1

if [ $# -gt 0 ]; then
	GAMES=("$@")
else
	GAMES=(WOLF3D_SHAREWARE WOLF3D_FULL WOLF3D_SOD_DEMO WOLF3D_SOD_FULL WOLF3D_SOD_MISSION2 WOLF3D_SOD_MISSION3 WOLF3D_NOAH3D)
fi

if [ ${#GAMES[@]} -eq 1 ]; then
	# One game: run in the foreground with all cores and normal output
	echo "=== ${GAMES[0]} ==="
	make GAME="${GAMES[0]}" -j"$(nproc)" package || { echo "FAILED: ${GAMES[0]}"; exit 1; }
	echo "=== Build succeeded; output is in dist/ ==="
	exit 0
fi

# Several games: one make per game at the same time, sharing the cores between them
JOBS=$(( ($(nproc) + ${#GAMES[@]} - 1) / ${#GAMES[@]} ))
mkdir -p build/logs
declare -A PIDS
for GAME in "${GAMES[@]}"; do
	echo "=== $GAME (started) ==="
	make GAME="$GAME" -j"$JOBS" package > "build/logs/$GAME.log" 2>&1 &
	PIDS[$GAME]=$!
done

FAILED=0
for GAME in "${GAMES[@]}"; do
	if wait "${PIDS[$GAME]}"; then
		echo "=== $GAME: OK ==="
	else
		echo "=== FAILED: $GAME (last lines of build/logs/$GAME.log) ==="
		tail -n 25 "build/logs/$GAME.log"
		FAILED=1
	fi
done

if [ $FAILED -ne 0 ]; then
	exit 1
fi
echo "=== All builds succeeded; output is in dist/ ==="
