#version 330 core

/*
 * Input
 */
uniform float m_iTime;  // Time uniform to animate the shader
uniform vec2 m_vec2Resolution;  // Resolution of the screen
uniform vec2 m_vec2MousePosition;
uniform vec3 m_vec3AudioAmplitude; // x = low. y = mid. z = high.

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

    float T = m_iTime;// + m_vec3AudioAmplitude.y;
    
    for (float i = 0.0; i < 4.0; i++) {
        uv = fract(uv * 1.5) - 0.5;

        float d = length(uv) * exp(-length(uv0));

        vec3 col = palette(length(uv0) + i*.4 + T*.4);

        d = sin(d*8. + T)/8.;
        d = abs(d);

        d = pow(0.01 / d, 1.2);

        finalColor += col * d;
    }
        
    FragColor = vec4(finalColor * (m_vec3AudioAmplitude.y*800), 1.0);
}