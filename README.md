# main.cpp

A terminal 3D torus renderer in C++. A spinning ASCII donut rendered directly in the console — no libraries, no GPU, no dependencies.

```
         $$$$$$$
      $$$$$$$$$$$$$$
    $$$$$$!!*##$$$$$$$
   $$$$$!*#@@@#*!$$$$$$
  $$$$$$*@@@@@@@*!$$$$$$$
  $$$$$!#@@@@@@@#!!$$$$$$
  $$$$$$*#@@@@@#*!$$$$$$$
   $$$$$$!!*##*!!$$$$$$$
    $$$$$$$$$$$$$$$$$$
      $$$$$$$$$$$$
```

## How it works

### 1. Torus geometry

The torus is built parametrically using two angles:

- `theta` (0..2π) — angle around the tube cross-section (inner circle)
- `phi` (0..2π) — angle around the torus axis (outer circle)

A point on the surface before rotation:
```
circleX = R + r·cos(θ)   // R=2, r=1 — torus radii
circleY = r·sin(θ)
```

### 2. Rotation matrices

Two animation angles `a` (pitch) and `b` (yaw) produce the final 3D coordinates via a combined rotation matrix around the X and Z axes.

### 3. Perspective projection

```
ooz = 1 / z          // inverse depth (perspective divide)
xp  = cx + k·ooz·x  // screen X
yp  = cy - k·ooz·y  // screen Y (Y-axis inverted)
```

### 4. Z-buffer

Before writing a character, `ooz > zbuffer[idx]` is checked. The closest fragment wins — standard depth test.

### 5. Lighting

The dot product of the surface normal and the light vector gives luminance `L`. A character is selected from the gradient:

```
. , - ~ : ; = ! * # $ @
```

Darkest to brightest.

### 6. Render loop

Each frame:
1. `output` and `zbuffer` are cleared
2. All torus points are computed
3. Cursor is moved to `(0,0)` via `\x1b[H` — no flicker from `clear`
4. Buffer is flushed to `stdout`
5. Angles advance: `a += 0.05`, `b += 0.03`
6. `sleep(30ms)`

## Build & run

```bash
# GCC / Clang
g++ -O2 -o donut donut.cpp
./donut

# MSVC (Windows)
cl /O2 donut.cpp
donut.exe
```

Requirements: C++11 or later. No external dependencies.

## Parameters (hardcoded)

| Parameter | Value | Description |
|---|---|---|
| `donut_scale` | `15.0f` | Projection scale |
| `theta` step | `0.07` | Tube detail |
| `phi` step | `0.02` | Revolution detail |
| `a` increment | `0.05` | Pitch speed |
| `b` increment | `0.03` | Yaw speed |
| Frame delay | `30 ms` | ~33 FPS |

## Compatibility

| Platform | Status |
|---|---|
| Linux / macOS | ✓ (`ioctl` + `TIOCGWINSZ`) |
| Windows | ✓ (`GetConsoleScreenBufferInfo`) |

Terminal size is queried dynamically every frame — the render adapts when the window is resized.

## Code structure

```
donut.cpp
├── Platform detection (_WIN32 / POSIX)
├── Animation loop
│   ├── Terminal size detection
│   ├── Buffer init (output, zbuffer)
│   ├── Double loop over theta/phi
│   │   ├── Point rotation (matrices a, b)
│   │   ├── Perspective projection
│   │   ├── Z-test
│   │   └── Character write by luminance
│   ├── Frame output
│   └── sleep(30ms)
```

## Ideas for extension

- Colored output via ANSI escape codes (256 colors / truecolor)
- Keyboard-controlled rotation speed
- Render other primitives — sphere, cube, Clifford torus
- Frame export to file for GIF generation
