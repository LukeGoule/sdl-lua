#version 330 core

layout(location = 0) in vec3 m_vec3InPosition;
layout(location = 1) in vec3 m_vec3InColour;

out vec3 m_vec3Colour;

uniform mat4 m_mat4Model;
uniform mat4 m_mat4View;
uniform mat4 m_mat4Projection;

void main() {
    gl_Position = m_mat4Projection * m_mat4View * m_mat4Model * vec4(m_vec3InPosition, 1.0);
    m_vec3Colour = m_vec3InColour;
}