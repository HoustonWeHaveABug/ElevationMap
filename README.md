# ElevationMap
Elevation Map generator

This program generates a rectangular elevation map using a C port of OpenSimplexNoise 2D functions (https://gist.github.com/KdotJPG/b1270127455a94ac5d19), to get more realistic data.

It expects the below parameters on standard input:

- Height
- Width
- Elevation Min.
- Elevation Max.
- Seed
- HTML Output (0/1)

If HTML output is activated, it will represent elevation data as a colored grid on a green/red scale (green: low, red: high).
