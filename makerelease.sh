# Builds every release package into <GAME>.tar.gz in the current directory
for GAME in WOLF3D_SHAREWARE WOLF3D_FULL WOLF3D_SOD_FULL WOLF3D_SOD_MISSION2 WOLF3D_SOD_MISSION3; do
	rm -f $GAME.tar.gz
	make clean && make GAME=$GAME package && tar -C dist -zcvf $GAME.tar.gz $GAME || exit 1
done
