#version 330

/**
 * @file simple_vertex.glsl
 * @brief Simple vertex shader for rendering textured geometry.
 */

// Input attributes
layout (location = 0) in vec3 position; ///< Vertex position.
layout (location = 1) in vec2 uv;       ///< Texture coordinates (UVs).

// Uniform matrices
uniform mat4 modelMatrix;      ///< Model matrix (object transformation).
uniform mat4 viewMatrix;       ///< View matrix (camera transformation).
uniform mat4 projectionMatrix; ///< Projection matrix (perspective/orthographic).

// Output variables
out vec2 vertexUV;    ///< Texture coordinates passed to fragment shader.

void main() {
    // Transform vertex position
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);

    // Pass texture coordinates to fragment shader
    vertexUV = uv;
}
