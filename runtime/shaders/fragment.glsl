#version 330 core

out vec4 FragColor;

/*
 * Input
 */

in vec3 FragPos;  // Position of the fragment
in vec3 Normal;   // Normal of the fragment
in vec3 m_vec3Colour;
in vec3 m_vec3Texcoord;

uniform vec2 u_center;  // Center of the view in the complex plane
uniform float u_zoom;   // Zoom factor
uniform float m_iTime;  // Time uniform to animate the shader
uniform vec2 m_vec2Resolution;  // Resolution of the screen
uniform vec2 m_vec2MousePosition;
uniform int u_iterations;

uniform float m_fLowAmplitude;
uniform float m_fMidAmplitude;
uniform float m_fHighAmplitude;

uniform vec3 lightPos;   // Position of the light source
uniform vec3 viewPos;    // Position of the camera/viewer
uniform vec3 lightColor; // Color of the light source

uniform vec3 m_vec3Ambient;
uniform vec3 m_vec3Diffuse;
uniform vec3 m_vec3Specular;
uniform vec3 m_vec3Emission;
uniform float m_fShininess;
uniform float m_fRefractoryIndex;
uniform float m_fDisolve;
uniform int m_iIllumination;

uniform sampler2D textureS; // Texture sampler

const float PI = 3.14159265359;
const float E = 2.718281828459;

//https://iquilezles.org/articles/palettes/
vec3 palette( float t ) {
    vec3 a = vec3(0.5, 0.5, 0.5);
    vec3 b = vec3(0.5, 0.5, 0.5);
    vec3 c = vec3(1.0, 1.0, 1.0);
    vec3 d = vec3(0.263, 0.416, 0.557);

    return a + b*cos( ( PI * 2 ) * ( c * t + d ) );
}


void main() {
    
    // Normalize the normal
    vec3 norm = normalize(Normal);

    // Light direction and view direction
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);

    // Ambient lighting (pre-clamped)
    vec3 ambient = m_vec3Ambient * lightColor;

    // Diffuse lighting (Lambertian reflection, max to avoid negative lighting)
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = m_vec3Diffuse * diff * lightColor;

    // Specular lighting (Phong reflection model)
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), m_fShininess);
    vec3 specular = m_vec3Specular * spec * lightColor;

    // Emission (if any)
    vec3 emission = m_vec3Emission;

    // Combine all the lighting components
    vec3 result = ambient + diffuse + specular;// + emission;

    // Ensure the result does not go over 1.0 for each color channel
    result = clamp(result, 0.0, 1.0);

    // Apply the fragment's base color (m_vec3Colour) as a multiplier
    result *= m_vec3Colour;

    // Apply dissolve (opacity)
    float alpha = mix(1.0, 0.0, m_fDisolve);

    // Output the final color with alpha
    FragColor = vec4(result, 1);
}