#version 330

/**
 * @file basic_vertex.glsl
 * @brief Basic vertex shader for rendering textured and colored geometry.
 */

// Input attributes
layout (location = 0) in vec3 position; ///< Vertex position.
layout (location = 1) in vec2 uv;       ///< Texture coordinates (UVs).
layout (location = 2) in vec4 color;    ///< Vertex color.

// Uniform matrices
uniform mat4 modelMatrix;      ///< Model matrix (object transformation).
uniform mat4 viewMatrix;       ///< View matrix (camera transformation).
uniform mat4 projectionMatrix; ///< Projection matrix (perspective/orthographic).

// Output variables
out vec4 vertexColor; ///< Vertex color passed to fragment shader.
out vec2 vertexUV;    ///< Texture coordinates passed to fragment shader.

void main() {
    // Transform vertex position
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);

    // Pass vertex color and UVs to fragment shader
    vertexColor = color;
    vertexUV = uv;
}
