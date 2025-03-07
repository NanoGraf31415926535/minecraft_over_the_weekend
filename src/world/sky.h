#ifndef SKY_H
#define SKY_H

#include "../gfx/gfx.h"
#include "../util/util.h"
#include "../gfx/vao.h"
#include "../gfx/vbo.h"
#include <stdbool.h> // For bool

// Forward declaration
struct World;

/**
 * @brief Represents celestial bodies.
 */
typedef enum {
    SUN,
    MOON
} CelestialBody;

/**
 * @brief Represents the state of the sky.
 */
typedef enum {
    DAY,
    NIGHT,
    SUNRISE,
    SUNSET
} SkyState;

/**
 * @brief Represents the sky and its properties.
 */
struct Sky {
    struct World *world; ///< Pointer to the world this sky belongs to.

    f32 fog_near; ///< Minimum fog distance.
    f32 fog_far;  ///< Maximum fog distance.

    vec4s fog_color;   ///< Color of the fog.
    vec4s clear_color; ///< Clear color of the sky.
    vec4s sunlight_color; ///< Color of the sunlight.

    SkyState state; ///< Current sky state (DAY, NIGHT, etc.).
    SkyState state_day_night; ///< current day or night state
    f32 sky_state_progress; ///< Progress of the current sky state (0.0 to 1.0).
    f32 day_night_progress; ///< Progress of the day/night cycle (0.0 to 1.0).

    struct VBO ibo; ///< Index buffer object for the sky.
    struct VBO vbo; ///< Vertex buffer object for the sky.
    struct VAO vao; ///< Vertex array object for the sky.
};

/**
 * @brief Initializes the sky.
 * @param self Pointer to the Sky structure to initialize.
 * @param world Pointer to the world this sky belongs to.
 */
void sky_init(struct Sky *self, struct World *world);

/**
 * @brief Destroys the sky and its resources.
 * @param self Pointer to the Sky structure to destroy.
 */
void sky_destroy(struct Sky *self);

/**
 * @brief Renders the sky.
 * @param self Pointer to the Sky structure to render.
 */
void sky_render(struct Sky *self);

#endif // SKY_H
