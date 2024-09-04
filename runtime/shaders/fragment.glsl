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

uniform sampler2D textureS; // Texture sampler

void main() {
    // Ambient lighting
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse lighting
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular lighting
    float specularStrength = 0.1;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    // Combine lighting results
    vec3 result = ambient + diffuse + specular;
    
    // Sample the texture
    //vec4 texColor = texture(textureS, m_vec3Texcoord);

    // Final colour: combine texture colour with lighting
    //FragColor = vec4(result * texColor.rgb, texColor.a);
    FragColor = vec4(result, 1.f);
}