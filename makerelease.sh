# Builds every game and packages them all into one Wolfenstein3D-PSP.zip in the current directory
# (WOLF3D_SOD_DEMO is intentionally not included)
GAMES="WOLF3D_SHAREWARE WOLF3D_FULL WOLF3D_SOD_FULL WOLF3D_SOD_MISSION2 WOLF3D_SOD_MISSION3 WOLF3D_NOAH3D"
rm -f Wolfenstein3D-PSP.zip
for GAME in $GAMES; do
	make clean && make GAME=$GAME package || exit 1
done
(cd dist && zip -r ../Wolfenstein3D-PSP.zip $GAMES) || exit 1
