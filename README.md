ASCII Donut C++ (3D Torus Renderer)

This project is a C++ implementation of the famous Donut.c algorithm. It renders a rotating 3D torus (donut) directly in the terminal using only ASCII characters and standard mathematical functions.
Technical Overview

The program simulates a three-dimensional object by calculating the coordinates of points on a torus and projecting them onto a two-dimensional grid (the terminal window). It handles depth through a Z-buffer and shading through a light-source vector calculation.
Key Features

    Pure Mathematics: No external graphical libraries (OpenGL, DirectX) are required. All rendering is done via trigonometric functions.

    Dynamic Scaling: The code automatically detects terminal dimensions and adjusts the donut's position and scale to remain centered.

    Z-Buffering: A depth-testing algorithm ensures that closer parts of the donut correctly obscure the parts behind them.

    Luminance Shading: Surface normals are calculated to determine light intensity, mapped to a character set from low to high density.

    Cross-Platform: Compatible with Windows (via windows.h) and Linux/macOS (via sys/ioctl.h).

The Mathematics of the Donut

The project uses the parametric equation for a torus:
x=(R1+R2cosθ)cosϕ
y=(R1+R2cosθ)sinϕ
z=R2sinθ

To project these points onto the screen:

    The points are rotated around the X and Z axes using rotation matrices.

    The 3D coordinates (x,y,z) are projected into 2D (xp,yp) using a perspective projection formula:
    xp=center_x+K1⋅zx​

    Light intensity L is calculated using the dot product of the surface normal and a predefined light vector. This value L maps to the character string ".,-~:;=!*#$@".

Compilation and Usage
Prerequisites

    A C++ compiler (g++, clang, or MSVC).

    A terminal supporting ANSI escape sequences (modern CMD, PowerShell, iTerm, or Bash).

Linux and macOS
Bash

g++ main.cpp -o donut
./donut

Windows
Bash

g++ main.cpp -o donut.exe
./donut.exe
