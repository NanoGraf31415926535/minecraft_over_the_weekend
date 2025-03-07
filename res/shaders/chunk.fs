#version 330

/**
 * @file textured_fragment.glsl
 * @brief Fragment shader for applying textures, colors, and fog.
 */

// Uniforms
uniform sampler2D textureSampler; ///< Texture sampler.
uniform vec4 fogColor;             ///< Fog color.
uniform float fogNear;            ///< Near fog distance.
uniform float fogFar;             ///< Far fog distance.

// Input variables
in vec4 vertexColor;    ///< Vertex color from vertex shader.
in vec2 vertexUV;       ///< Texture coordinates from vertex shader.
in vec3 viewPosition;   ///< View-space position from vertex shader.

// Output variable
out vec4 fragmentColor; ///< Final fragment color.

void main() {
    // Apply texture and vertex color
    fragmentColor = texture(textureSampler, vertexUV) * vertexColor;

    // Handle transparency (potential performance bottleneck)
    // TODO: Discarding fragments like this is bad for performance due to
    // potential early Z-test rejection and fragment shader invocations.
    // Consider using alpha blending or avoiding rendering transparent
    // fragments in the first place.
    if (fragmentColor.a == 0.0) {
        discard;
    }

    // Calculate fog factor using linear interpolation
    float fogFactor = smoothstep(fogNear, fogFar, length(viewPosition));

    // Apply fog using mix function
    fragmentColor = mix(fragmentColor, fogColor, fogFactor);

}
