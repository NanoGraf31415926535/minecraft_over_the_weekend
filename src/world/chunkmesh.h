#ifndef CHUNKMESH_H
#define CHUNKMESH_H

#include "../gfx/gfx.h"
#include "../gfx/vao.h"
#include "../gfx/vbo.h"
#include "../util/util.h"
#include <stdbool.h> // For bool

// Forward declaration
struct Chunk;

/**
 * @brief Represents different parts of a chunk mesh.
 */
typedef enum {
    BASE,
    TRANSPARENT
} ChunkMeshPart;

/**
 * @brief Represents different buffer types used in a chunk mesh.
 */
typedef enum {
    DATA = 0,
    INDICES,
    FACES,
    BUFFER_TYPE_LAST = FACES // Last buffer type
} BufferType;

/**
 * @brief Represents a face in the chunk mesh.
 */
struct Face {
    size_t indices_base; ///< Index of face indices in 'indices' ChunkMeshBuffer.
    vec3s position;      ///< Position of the face (used for sorting).
    f32 distance;        ///< Precomputed distance (used for sorting).
};

/**
 * @brief Represents a buffer used in the chunk mesh.
 */
struct ChunkMeshBuffer {
    BufferType type;    ///< Type of the buffer.
    void *data;         ///< Data store for this buffer, NULL if not allocated.
    size_t capacity;    ///< Capacity (in bytes) of data.
    size_t index;       ///< Current index (in bytes) into data.
    size_t count;       ///< Final count (in bytes) in data.
    size_t elements;    ///< Current count (in elements) of data.
};

/**
 * @brief Represents a chunk mesh instance.
 */
struct ChunkMesh {
    struct Chunk *chunk; ///< Pointer to the chunk this mesh belongs to.

    struct ChunkMeshBuffer buffers[BUFFER_TYPE_LAST + 1]; ///< Data, indices, faces buffers.

    size_t vertex_count; ///< Total number of vertices in this mesh.

    struct {
        struct {
            size_t offset; ///< Offset of indices.
            size_t count;  ///< Count of indices.
        } base, transparent;
    } indices; ///< Indices offsets and counts.

    struct {
        bool finalize : 1;   ///< If true, mesh needs to be finalized (uploaded).
        bool dirty : 1;      ///< If true, mesh will be rebuilt next render.
        bool depth_sort : 1; ///< If true, mesh will be depth sorted next render.
        bool destroy : 1;    ///< If true, mesh will be destroyed when data is accessible.
        bool persist : 1;    ///< If true, index and face buffers are kept in memory.
    } flags; ///< Mesh flags.

    struct VAO vao; ///< Vertex array object.
    struct VBO vbo; ///< Vertex buffer object.
    struct VBO ibo; ///< Index buffer object.
};

/**
 * @brief Creates a new chunk mesh.
 * @param chunk Pointer to the chunk this mesh belongs to.
 * @return Pointer to the created ChunkMesh, or NULL on failure.
 */
struct ChunkMesh *chunkmesh_create(struct Chunk *chunk);

/**
 * @brief Destroys a chunk mesh.
 * @param self Pointer to the ChunkMesh to destroy.
 */
void chunkmesh_destroy(struct ChunkMesh *self);

/**
 * @brief Prepares a chunk mesh for rendering.
 * @param self Pointer to the ChunkMesh to prepare.
 */
void chunkmesh_prepare_render(struct ChunkMesh *self);

/**
 * @brief Renders a chunk mesh part.
 * @param self Pointer to the ChunkMesh to render.
 * @param part The part of the mesh to render (BASE or TRANSPARENT).
 */
void chunkmesh_render(struct ChunkMesh *self, ChunkMeshPart part);

/**
 * @brief Sets the persist flag for a chunk mesh.
 * @param self Pointer to the ChunkMesh.
 * @param persist True to persist buffers, false otherwise.
 */
void chunkmesh_set_persist(struct ChunkMesh *self, bool persist);

#endif // CHUNKMESH_H
