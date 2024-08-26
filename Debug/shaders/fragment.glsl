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

#define r(a) mat2(cos(a + vec4(0,33,11,0))) 

#define s(p) ( q = p,                                    \
    d = length(vec2(length(q.xy += .5)-.5, q.z)) - .01,  \
    q.yx *= r(round((atan(q.y,q.x)-mod_time) * 3.8) / 3.8 + mod_time), \
    q.x -= .5,                                           \
    FragColor += (sin(t+mod_time)*.1+.1)*(1.+cos(t+mod_time*.5+vec4(0,1,2,0))) \
         / (.5 + pow(length(q)*50., 1.3))            , d ) // return d

vec2 currentPixel() {
    return gl_FragCoord.xy / m_vec2Resolution;
}

void main1() {
    // Normalised pixel coordinates (from 0 to 1)
    vec2 uv = gl_FragCoord.xy / m_vec2Resolution;

    // Generate a color pattern based on sin and cos functions
    float r = 0.5 + 0.5 * cos(m_iTime + uv.x * 20.0);
    float g = 0.5 + 0.5 * sin(m_iTime + uv.y * 20.0);
    float b = 0.5 + 0.5 * cos(uv.x * 10.0 + uv.y * 100.0 + m_iTime);

    // Combine the colors
    vec3 color = vec3(r, g, b);

    // Output the final color
    FragColor = vec4(color, 1.0);
}

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

// https://www.shadertoy.com/view/DsBczR
void main111() {
    vec3  p, q,    R = vec3(m_vec2Resolution,0);
    float i, t, d;
    
    vec2 F = gl_FragCoord.xy;

    float space_warp_factor = sin(m_iTime);
    float mod_time = m_iTime + (m_vec3AudioAmplitude.y * 100); 
    
    for (FragColor *= i, F += F - R.xy; i++ < 28.;          // raymarch for 28 iterations
        
        p = t * normalize( vec3( F * r( t * space_warp_factor ), R.y ) ),    // ray position
        p.zx *= r( mod_time /4. ), p.zy *= r( mod_time / 3.), p.x += mod_time, // camera movement
                   
        t += min(min(s( p = fract(p) - .5 ),        // distance to torus + color (x3)
                     s( vec3(-p.y, p.zx)  )),
                     s( -p.zxy            ))
    );

    FragColor = FragColor;
}