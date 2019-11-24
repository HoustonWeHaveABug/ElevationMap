# ElevationMap
Elevation Map generator

This program creates a rectangular elevation map using a C port of OpenSimplexNoise 2D functions (https://gist.github.com/KdotJPG/b1270127455a94ac5d19), to get more realistic data. It generates integer values in a user-provided elevation range.

It expects the below parameters on standard input:

- Height (> 1)
- Width (> 1)
- Elevation Min. (may be negative)
- Elevation Max. (>= Elevation Min., may be negative)
- Seed (64 bits integer)
- HTML Output (0/1)

If HTML output is activated, it will represent elevation data as a colored grid on a green/red scale (green: low, red: high).
