# Yo dawg, I heard you like ARK, so I put [ARK](https://www.gog.com/en/game/super_3d_noahs_ark) in your [ARK](https://www.pspunk.com/psp-cfw/), so you can play [ARK](https://wisdomtree.itch.io/s3dna) while you run [ARK](https://github.com/PSP-Arkfive/ARK-5).

*Wolfenstein 3-D* for the Sony PSP by Ben McLean, forked from [the old PSP port by Zack & Chilly Willy](https://github.com/DanS2D/Wolfenstein3D-PSP). Fixes bugs and adds improvements, most notably *Super 3D Noah's Ark* support. The code is GPLv2 and the shareware game is included under its original 1992 redistribution permission from Apogee.

XMB music for *Super 3D Noah's Ark* is by [thebitterroost](https://ocremix.org/remix/OCR04183). For the other games, it uses [*Wonderin About My Remix* by Ben McLean](https://github.com/BenMcLean/WolfSharp/blob/master/godot/BenMcLean.Wolf3D.Shared/Resources/Wondering%20About%20My%20Remix.wlf).

## Building
Building this game requires the [PSP toolchain (pspdev)](https://pspdev.github.io/) to be installed, with `psp-config` on your `PATH` (`PSPDEV` set).

Each game is built separately and you have to say which one with `GAME=`:

```
WOLF3D_SHAREWARE      (the default if GAME is not given)
WOLF3D_FULL
WOLF3D_SOD_DEMO       (Commercial Demo)
WOLF3D_SOD_FULL
WOLF3D_SOD_MISSION2   (Return to Danger)
WOLF3D_SOD_MISSION3   (Ultimate Challenge)
WOLF3D_NOAH3D         (Super 3-D Noah's Ark)
```

For instance, to build *Super 3-D Noah's Ark*:

```
make GAME=WOLF3D_NOAH3D package
```

Everything ends up in `dist/<GAME>/`, ready to copy to your PSP (to `PSP/GAME/<GAME>/` on the memory stick): `EBOOT.PBP`, the game's data files if they are bundled in this repository (currently only the Wolfenstein 3D shareware data), and a `readme.txt` saying which data files you have to supply yourself for the other games. Intermediate files go to `build/<GAME>/`, so each game has its own build directory and nothing is written to the project root. Because of that you can build several games in a row without cleaning in between, and `make cleanall` deletes all of the `build/` directories. `dist/` is never deleted by the build, so data files you copy into `dist/<GAME>/` yourself survive rebuilds. Plain `make GAME=<name>` (without `package`) only builds `dist/<GAME>/EBOOT.PBP`.

Other ways to build:

* **Windows:** `build.cmd` runs the build inside WSL (a distro named `Ubuntu` with pspdev installed in `~/pspdev`). Without arguments it builds all seven games at the same time, or pass game names to build only those, e.g. `build.cmd WOLF3D_NOAH3D`. The output is in `dist\`.
* **Linux/WSL:** `./buildall.sh [GAME...]` does the same thing as `build.cmd` without the WSL wrapper (it expects pspdev in `~/pspdev`).
* **`makerelease.sh`:** builds every game except `WOLF3D_SOD_DEMO` and zips them into a single `Wolfenstein3D-PSP.zip` in the current directory (requires `zip`), containing one folder per game, ready to extract and run on your PSP.
* **GitHub Actions:** `.github/workflows/build.yml` builds each game in its own job (again without the Spear of Destiny demo) and zips the results into `Wolfenstein3D-PSP-<version>.zip`. Pushing a tag named like `v2026-11-06` also publishes that zip as a GitHub release; running the workflow manually only produces the zip as a build artifact.

## Controls
`O` acts as a modifier button: hold it and press another button for the alternate action in the "O +" column.

| Button | In a level | `O +` (in a level) | In menus |
|--------|-----------|--------------------|----------|
| Analog stick | Move forward/back and turn left/right. Speed scales with how far you push it (outside a dead zone); full deflection matches the D-pad | Up: toggle god mode, Down: full health/ammo/keys/weapons (debug cheats) | Move cursor |
| D-pad up/down | Walk forward/backward | | Move cursor |
| D-pad left/right | Turn left/right (strafe left/right with **Alt controls** on) | Previous / next weapon | Move cursor / change value |
| `L` / `R` | Strafe left/right, always at walking speed so you can aim while circle-strafing (turn left/right with **Alt controls** on) | Previous / next weapon | Alt + left / right |
| `X` | Fire | | Select |
| Triangle | Open doors / operate switches | | |
| Square (hold) | Run (the stick and D-pad move at running speed) | **Toggle auto-run on/off** (shows "Auto-run ON" / "Auto-run OFF") | |
| `Start` | Open the game menu | Cycle the CPU clock speed (133–333 MHz) | Back |
| `Select` | **Toggle Alt controls on/off** (shows "Alt controls ON" / "Alt controls OFF") | Backspace | Enter (`Select` + Square types "A") |
| `O` alone | | | Back / cancel |

### Notes
- **Auto-run:** press `O` + Square during a level to toggle it. While it is on, you run all the time (the stick, D-pad and turning all use running speed), and holding Square *walks* instead. Press `O` + Square again to turn it off. Strafing with `L`/`R` stays at walking speed either way. It can only be toggled during a level. It carries over between levels, but it isn't saved, so it starts off every time the game is launched.
- **Alt controls:** press `Select` during a level to toggle. With it on, the D-pad left/right strafe and `L`/`R` turn (instead of the D-pad turning and `L`/`R` strafing). Like auto-run, it carries over between levels, but it isn't saved and starts off every time the game is launched. The analog stick, auto-run and weapon cycling work the same either way.
- **Weapon switching:** hold `O` and press D-pad left/right or `L`/`R` to go to the previous/next weapon. It skips weapons you don't own or have no ammo for, which matters in Super 3-D Noah's Ark (cantaloupes and watermelons are separate from the normal ammo).

## Other Projects
- [WolfSharp](https://github.com/BenMcLean/WolfSharp) brings the Wolfenstein 3D, Spear of Destiny and Super 3D Noah's Ark to virtual reality!
