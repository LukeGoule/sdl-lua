#version 330 core

out vec4 FragColor;

/*
 * Input
 */
uniform vec2 u_center;  // Center of the view in the complex plane
uniform float u_zoom;   // Zoom factor
uniform float m_iTime;  // Time uniform to animate the shader
uniform vec2 m_vec2Resolution;  // Resolution of the screen
uniform vec2 m_vec2MousePosition;
uniform int u_iterations;

uniform float m_fLowAmplitude;
uniform float m_fMidAmplitude;
uniform float m_fHighAmplitude;

void main() {
    // Map the pixel coordinates to the complex plane
    vec2 c = (gl_FragCoord.xy - 0.5 * m_vec2Resolution) / u_zoom + u_center;

    // Initialize the complex number z
    vec2 z = vec2(0.0, 0.0);

    // Maximum iterations and glow factor
    float glowIntensity = -1;

    int i;
    float smoothColor = 0.0;

    for (i = 0; i < u_iterations; i++) {
        // Compute z = z^2 + c in the complex plane
        z = vec2(z.x*z.x - z.y*z.y, 2.0*z.x*z.y) + c;
        
        // Check if the magnitude of z has exceeded 2
        if (dot(z, z) > 4.0) break;
        
        // Calculate smooth color based on the escape time
        smoothColor += exp(-dot(z, z) * glowIntensity);
    }

    // Normalize the smoothColor
    smoothColor = smoothColor / float(u_iterations);

    // Map the smoothColor to a glowing color gradient
    vec3 color = vec3(smoothColor, smoothColor * 0.7, smoothColor * 0.4);


    // Output the final color
    FragColor = vec4(color, 1.0);
}