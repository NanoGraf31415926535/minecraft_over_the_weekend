// direction.c
/**
 * @file direction.c
 * @brief Provides functions and constants for working with 3D directions.
 */

#include "direction.h"
#include <assert.h>

const ivec3s DIRECTION_IVEC[6] = {
    { 0,  0, -1 }, // DIRECTION_NORTH
    { 0,  0,  1 }, // DIRECTION_SOUTH
    { 1,  0,  0 }, // DIRECTION_EAST
    {-1,  0,  0 }, // DIRECTION_WEST
    { 0,  1,  0 }, // DIRECTION_UP
    { 0, -1,  0 }  // DIRECTION_DOWN
};

const vec3s DIRECTION_VEC[6] = {
    { 0,  0, -1 }, // DIRECTION_NORTH
    { 0,  0,  1 }, // DIRECTION_SOUTH
    { 1,  0,  0 }, // DIRECTION_EAST
    {-1,  0,  0 }, // DIRECTION_WEST
    { 0,  1,  0 }, // DIRECTION_UP
    { 0, -1,  0 }  // DIRECTION_DOWN
};

/**
 * @brief Converts an ivec3s to a Direction enum value.
 * @param v The ivec3s to convert.
 * @return The corresponding Direction enum value, or -1 if invalid.
 */
enum Direction _ivec3s2dir(ivec3s v) {
    for (size_t i = 0; i < 6; i++) {
        if (DIRECTION_IVEC[i].x == v.x &&
            DIRECTION_IVEC[i].y == v.y &&
            DIRECTION_IVEC[i].z == v.z) {
            return (enum Direction)i;
        }
    }

    assert(0 && "Invalid ivec3s passed to _ivec3s2dir"); // More descriptive assert
    return (enum Direction)-1; // Return an invalid direction
}

// direction.h
#ifndef DIRECTION_H
#define DIRECTION_H

#include "../util/util.h" // Assuming ivec3s and vec3s are defined here.

typedef enum {
    DIRECTION_NORTH,
    DIRECTION_SOUTH,
    DIRECTION_EAST,
    DIRECTION_WEST,
    DIRECTION_UP,
    DIRECTION_DOWN
} Direction;

extern const ivec3s DIRECTION_IVEC[6];
extern const vec3s DIRECTION_VEC[6];

enum Direction _ivec3s2dir(ivec3s v);

#define DIR2IVEC3S(d) DIRECTION_IVEC[d]
#define DIR2VEC3S(d) DIRECTION_VEC[d]

#endif // DIRECTION_H
