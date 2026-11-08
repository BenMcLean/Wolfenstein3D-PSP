Wolfenstein 3D for the Sony PSP by Ben McLean, forked from [the old PSP port by Zack & Chilly Willy](https://github.com/DanS2D/Wolfenstein3D-PSP). Fixes bugs and adds improvements, most notably Super 3D Noah's Ark support.

# Building
Building this game requires you to have the PSP Toolchain installed.

As for building the game itself, you have two options:

1) Run the makerelease.sh shell script. 
2) Manually build.

For option 1, it will build each version of Wolf3D and package them in tar.gz files (in the root directory), ready to extract and run on your PSP.
For option 2, you need to specify which version of Wolf3D you intend to build:

```
WOLF3D_SHAREWARE
WOLF3D_FULL
WOLF3D_SOD_SHAREWARE
WOLF3D_SOD_FULL
WOLF3D_SOD_MISSION2  (Return to Danger)
WOLF3D_SOD_MISSION3  (Ultimate Challenge)
WOLF3D_NOAH3D        (Super 3-D Noah's Ark)
```

For instance, to build the Super 3-D Noah's Ark run:

```
make GAME=WOLF3D_NOAH3D
```

# Controls
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

## Notes
- **Auto-run:** press `O` + Square during a level to toggle it. While it is on, you run all the time (the stick, D-pad and turning all use running speed), and holding Square *walks* instead. Press `O` + Square again to turn it off. Strafing with `L`/`R` stays at walking speed either way. It can only be toggled during a level. It carries over between levels, but it isn't saved, so it starts off every time the game is launched.
- **Alt controls:** press `Select` during a level to toggle. With it on, the D-pad left/right strafe and `L`/`R` turn (instead of the D-pad turning and `L`/`R` strafing). Like auto-run, it carries over between levels, but it isn't saved and starts off every time the game is launched. The analog stick, auto-run and weapon cycling work the same either way.
- **Weapon switching:** hold `O` and press D-pad left/right or `L`/`R` to go to the previous/next weapon. It skips weapons you don't own or have no ammo for, which matters in Super 3-D Noah's Ark (cantaloupes and watermelons are separate from the normal ammo).

# Other Projects
- [WolfSharp](https://github.com/BenMcLean/WolfSharp) brings the Wolfenstein 3D, Spear of Destiny and Super 3D Noah's Ark to virtual reality!
