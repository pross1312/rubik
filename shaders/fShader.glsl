#version 430

uniform vec2 resolution;
uniform float time;
in vec3 pos;
flat in uint c;
out vec4 color;

float map01(float x) {
    return (x + 1.0f) / 2.0f;
}
// enum COLOR { 
//     WHITE = 0,
//     YELLOW,
//     RED,
//     ORANGE,
//     BLUE,
//     GREEN,
//     NONE,
// };

void main() {
    // vec2 pos = gl_FragCoord.xy * 2 / resolution;
    // color = vec4(map01(sin(pos.z)), map01(cos(pos.z)),
    //             map01(sin(cos(132.021f + pos.z) * sin(resolution.x))), 1.0f);
    if (c == 0) {
        color = vec4(1.0f);
    }
    else if (c == 1) {
        color = vec4(1.0f, 1.0f, .0f, 1.0f);
    }
    else if (c == 2) {
        color = vec4(1.0f, .0f, .0f, 1.0f);
    }
    else if (c == 3) {
        color = vec4(1.0f, 165 / 255.0f, .0f, 1.0f);
    }
    else if (c == 4) {
        color = vec4(.0f, .0f, 1.0f, 1.0f);
    }
    else if (c == 5) {
        color = vec4(.0f, 1.0f, .0f, 1.0f);
    }
    else {
        color = vec4(.0f, .0f, .0f, .0f);
    }
    // color = vec4(1.0f);
}
