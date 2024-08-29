#version 330 core

out vec4 FragColor;
in vec2 fragCoord;

/*
 * Input
 */
uniform vec2 u_center;  // Center of the view in the complex plane
uniform float u_zoom;   // Zoom factor
uniform float m_iTime;  // Time uniform to animate the shader
uniform vec2 m_vec2Resolution;  // Resolution of the screen
uniform vec2 m_vec2MousePosition;
uniform vec3 m_vec3AudioAmplitude; // x = low. y = mid. z = high.

void main() {
    // Map the pixel coordinates to the complex plane
    vec2 c = (fragCoord - 0.5 * m_vec2Resolution) / u_zoom + u_center;

    // Initialize the complex number z
    vec2 z = vec2(0.0, 0.0);

    // Maximum iterations
    const int maxIterations = 1000;
    int i;

    for (i = 0; i < maxIterations; i++) {
        // Compute z = z^2 + c in the complex plane
        z = vec2(z.x*z.x - z.y*z.y, 2.0*z.x*z.y) + c;
        
        // Check if the magnitude of z has exceeded 2
        if (dot(z, z) > 4.0) break;
    }

    // Map the number of iterations to a colour
    float t = float(i) / float(maxIterations);
    vec3 color = vec3(t, t * 0.5, t * 0.25);

    // Output the final color
    FragColor = vec4(color, 1.0);
}

#version 330 core

/*
 * Input
 */
uniform float m_iTime;  // Time uniform to animate the shader
uniform vec2 m_vec2Resolution;  // Resolution of the screen
uniform vec2 m_vec2MousePosition;

uniform float m_fLowAmplitude;
uniform float m_fMidAmplitude;
uniform float m_fHighAmplitude;

/*
 * Output
 */
out vec4 FragColor;

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

//https://www.shadertoy.com/view/mtyGWy
void main() {
    vec2 uv = (gl_FragCoord.xy * 2.0 - m_vec2Resolution.xy) / m_vec2Resolution.y;
    vec2 uv0 = uv;
    vec3 finalColor = vec3(0.0);

    float T = m_iTime;
    
    for (float i = 0.0; i < 4.0; i++) {
        uv = fract(uv * 1.5) - 0.5;

        float d = length(uv) * exp(-length(uv0));

        vec3 col = palette(length(uv0) + i*.4 + T*.4);

        d = sin(d*8. + T)/8.;
        d = abs(d);

        d = pow(0.01 / d, 1.2);

        finalColor += col * d;
    }
        
    FragColor = vec4(finalColor * (m_fMidAmplitude*800), 1.0);
}