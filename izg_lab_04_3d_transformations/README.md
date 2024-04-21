# Lab 04: 3D Transformations

## Author

- **Name:** Maksim Kalutski
- **Login:** xkalut00

In this lab, we focus on the implementation of basic 3D transformations using the OpenSceneGraph (OSG) framework. The
lab includes tasks on translation, scaling, and rotation around arbitrary axes.

## Introduction

The OpenSceneGraph is an open-source high performance 3D graphics toolkit, utilized in projects from visual simulation,
games, virtual reality, scientific visualization and modelling. Written entirely in Standard C++ and OpenGL, it employs
the scene graph data structure to render real-time graphics depending on depth coordinates.

## Tasks

1. **Translation and Scaling**: Implement functions to manually create scaling and translation matrices.
2. **Rotation**: Implement functions for rotation around the X, Y, and Z axes.
3. **Arbitrary Axis Rotation**: Develop a function to compute the rotation matrix for a rotation around an arbitrary
   axis defined by two given vectors.

## Useful Functions

- `osg::Matrix::scale()`
- `osg::Matrix::translate()`
- `osg::Matrix::rotate()`
- `osg::Vec3d` for vector operations.
