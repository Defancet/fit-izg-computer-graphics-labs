# Lab 05: 3D Displaying 2D Curves

## Author

- **Name:** Maksim Kalutski
- **Login:** xkalut00

## Introduction

This lab focuses on implementing methods to compute and display 2D curves, particularly Bezier cubic curves. These
curves are crucial in graphics for creating smooth, scalable shapes and animations.

## Tasks

1. **Bezier Cubic Trajectory**: Implement the function to compute a trajectory using Bezier cubic curves. You will need
   to retrieve control points and then use these points to calculate the curve points based on the Bezier cubic
   formula: `Q(t) = P0 * B03(t) + P1 * B13(t) + P2 * B23(t) + P3 * B33(t)`, where `Bij(t)` are the Bernstein
   polynomials.
2. **G1 Continuity**: Adjust the control points of your curve segments to ensure G1 continuity. G1 continuity means the
   curves are tangentially continuous, which is crucial for smooth transitions in curve segments. You might need to
   manually adjust or calculate new coordinates for these control points based on the techniques taught in class.
