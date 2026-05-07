# Overcooked Qt6 Prototype

Pure `Qt6 Core + Gui + Widgets` local co-op prototype built with `QGraphicsView`.

## Current Gameplay

- Two-player local keyboard play
- Smooth 8-direction movement with collision
- Tomato box, chopping board, pot, plate box, counter, delivery zone
- Full tomato soup loop:
  - Take tomato
  - Chop on board
  - Put 3 chopped tomatoes into pot
  - Wait for soup to cook
  - Use a plate to serve
  - Deliver for score

## Controls

- `Player 1`: `W A S D` move, `F` interact
- `Player 2`: `Arrow Keys` move, `/` interact

## Build

Open this folder directly in `Qt Creator` as a `CMake` project, then build with `Qt 6.0+`.

## Source Files

- `main.cpp`: application entry
- `gamewindow.*`: top-level window
- `gameview.*`: graphics view setup and rendering optimization
- `gamescene.*`: scene, level construction, input, gameplay loop
- `player.*`: chef movement, collision, carrying and drawing
- `station.*`: counters and all kitchen interaction stations
- `tileitem.*`: floor tiles
- `huditem.*`: score, timer and control overlay
- `gameconstants.h`: enums, colors, shared drawing helpers and constants
