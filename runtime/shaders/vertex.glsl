#version 330 core

layout(location = 0) in vec3 m_vec3InPosition;
layout(location = 1) in vec3 m_vec3Texcoord_in;
layout(location = 2) in vec3 m_vec3Normal;

out vec3 FragPos;       // Position of the fragment in world space
out vec3 m_vec3Colour;  // Pass the color to fragment shader
out vec3 m_vec3Texcoord;
out vec3 Normal;        // Pass the normal to fragment shader

uniform mat4 m_mat4Model;
uniform mat4 m_mat4View;
uniform mat4 m_mat4Projection;

void main() {
    // Calculate the vertex position in world space
    FragPos = vec3(m_mat4Model * vec4(m_vec3InPosition, 1.0));
    
    // Pass the color to the fragment shader
    m_vec3Colour = vec3(1.0,1.0,1.0);

    // Pass the texcoord.
    m_vec3Texcoord = m_vec3Texcoord_in;
    
    // Transform the normal vector with the transpose of the inverse of the model matrix
    // This handles any non-uniform scaling or other transformations correctly.
    Normal = mat3(transpose(inverse(m_mat4Model))) * m_vec3Normal;
    
    // Calculate the final position of the vertex in clip space
    gl_Position = m_mat4Projection * m_mat4View * vec4(FragPos, 1.0);
}