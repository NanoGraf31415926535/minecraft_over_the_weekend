#ifndef DIRECTION_H
#define DIRECTION_H

/**
 * @file direction.h
 * @brief Provides an enum and functions for working with 3D directions.
 */

#include <cglm/cglm.h>
#include <cglm/struct.h>

/**
 * @brief Represents 3D directions.
 */
typedef enum {
    NORTH = 0, ///< North direction.
    SOUTH = 1, ///< South direction.
    EAST = 2,  ///< East direction.
    WEST = 3,  ///< West direction.
    UP = 4,    ///< Up direction.
    DOWN = 5   ///< Down direction.
} Direction;

extern const ivec3s DIRECTION_IVEC[6]; ///< Array of direction vectors as ivec3s.
extern const vec3s DIRECTION_VEC[6]; ///< Array of direction vectors as vec3s.

/**
 * @brief Converts an ivec3s to a Direction enum value.
 * @param v The ivec3s to convert.
 * @return The corresponding Direction enum value, or -1 if invalid.
 */
Direction _ivec3s2dir(ivec3s v);

/**
 * @brief Gets the vec3s representation of a direction.
 * @param d The direction.
 * @return The vec3s representation.
 */
#define DIR2VEC3S(d) (DIRECTION_VEC[d])

/**
 * @brief Gets the ivec3s representation of a direction.
 * @param d The direction.
 * @return The ivec3s representation.
 */
#define DIR2IVEC3S(d) (DIRECTION_IVEC[d])

/**
 * @brief Converts an ivec3s to a Direction enum value.
 * @param v The ivec3s to convert.
 * @return The corresponding Direction enum value.
 */
#define IVEC3S2DIR(v) (_ivec3s2dir(v))

#endif // DIRECTION_H
