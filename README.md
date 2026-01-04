# trafficjam

**Author:** Rayan BEN  
**Project:** PoPs_trafficjam  
**Date:** Dec 2025 - Jan 2026  

A terminal-based 6x6 sliding puzzle game inspired by "Rush Hour", written in C.  
Move vehicles around the grid to free the red car and reach the exit.

---

## Game Description

- Grid: 6x6  
- Vehicles: Cars (length 2) and Trucks (length 3)  
- Directions: Horizontal (`h`) or Vertical (`v`)  
- Max vehicles per game: 10  

**Objective:**  
Move the red car (ID 0) horizontally to the exit on the right side of the grid.  

---

## Features

- Real-time rendering in terminal using ASCII graphics  
- Colored vehicles using ANSI codes  
- HUD displaying:
  - Selected vehicle ID
  - Vehicle color
  - Direction
  - Move count
  - Help toggle  
- Controls (keyboard):
  - `Arrow Up / Down` → select vehicle  
  - `h / l / k / j` → move selected vehicle  
  - `Shift + H` → toggle help/controls  
  - `q` → quit game  

---

## Project Structure
PoPs_trafficjam/
│
├── trafficjam.c # full game logic and rendering
├── console.c # console utility functions (cursor, colors)
├── Makefile # Build instructions
├── README.md # Project description

---

## Branches

- `c99` → Modern C99 version (main branch)  
  - uses compound literals, inline `for` declarations, and C99 features  
  - For modern compilers  

- `c90` → ANSI C90 compatible branch   
  - Uses explicit struct initialization  
  - Declares variables at the top of blocks  
  - Compatible with strict embedded/legacy compilers  (sometimes we're bound to specific versions : this project was an opportunity to face off version constraints)  

---

## Compilation

**C99 version:**
```bash
gcc -std=c99 -Wall -Wextra -pedantic main.c -o trafficjam
./trafficjam
```
**C99 version:**
```bash
gcc -std=c90 -Wall -Wextra -pedantic main.c -o trafficjam_c90
./trafficjam_c90
```

## Notes

Ensure your terminal supports ANSI colors for best experience

Tested on Linux terminals

C90 branch may require additional fixes for more strict compilers
