#ifndef PLAYER_H
#define PLAYER_H

#include "../util/util.h"
#include "../util/camera.h"
#include "../block/block.h"
#include <stdbool.h> // For bool

// Forward declaration
struct World;

/**
 * @brief Represents the player entity.
 */
struct EntityPlayer {
    struct World *world; ///< Pointer to the world this player belongs to.
    struct PerspectiveCamera camera; ///< Player's camera.

    bool has_look_block; ///< True if the player is looking at a block.
    ivec3s look_block; ///< Position of the block the player is looking at.
    enum Direction look_face; ///< Face of the block the player is looking at.

    ivec3s offset; ///< Player's current chunk offset.
    ivec3s block_pos; ///< Player's current block position within the chunk.

    bool offset_changed; ///< True if the offset changed since the last update.
    bool block_pos_changed; ///< True if the block position changed since the last update.

    enum BlockId selected_block; ///< Currently selected block in the player's hand.
};

/**
 * @brief Initializes the player entity.
 * @param self Pointer to the EntityPlayer structure to initialize.
 * @param world Pointer to the world this player belongs to.
 */
void player_init(struct EntityPlayer *self, struct World *world);

/**
 * @brief Destroys the player entity.
 * @param self Pointer to the EntityPlayer structure to destroy.
 */
void player_destroy(struct EntityPlayer *self);

/**
 * @brief Renders the player entity.
 * @param self Pointer to the EntityPlayer structure to render.
 */
void player_render(struct EntityPlayer *self);

/**
 * @brief Updates the player entity.
 * @param self Pointer to the EntityPlayer structure to update.
 */
void player_update(struct EntityPlayer *self);

/**
 * @brief Ticks the player entity.
 * @param self Pointer to the EntityPlayer structure to tick.
 */
void player_tick(struct EntityPlayer *self);

#endif // PLAYER_H
