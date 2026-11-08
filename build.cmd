@echo off
rem Builds the PSP packages (via the pspdev toolchain in WSL Ubuntu) into dist\<GAME>\
rem   build.cmd                     builds all seven games
rem   build.cmd WOLF3D_NOAH3D       builds only the named game(s)
rem Games: WOLF3D_SHAREWARE WOLF3D_FULL WOLF3D_SOD_SHAREWARE WOLF3D_SOD_FULL
rem        WOLF3D_SOD_MISSION2 WOLF3D_SOD_MISSION3 WOLF3D_NOAH3D
wsl.exe -d Ubuntu --cd "%~dp0." -- bash ./buildall.sh %*
if errorlevel 1 (
	echo.
	echo BUILD FAILED
	exit /b 1
)
echo.
echo Done. Packages are in %~dp0dist
