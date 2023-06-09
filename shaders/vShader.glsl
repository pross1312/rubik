#version 430

uniform float time;
uniform vec2 resolution;
uniform mat4 proj_mat;
uniform mat4 view_mat;
layout(location = 0) in vec3 position;
layout(location = 1) in int color;

flat out uint c;
out vec3 pos;

mat4 buildRotateX(float rad); // declaration of matrix transformation utility functions
mat4 buildRotateY(float rad); // (GLSL requires functions to be declared prior to invocation)
mat4 buildRotateZ(float rad);
mat4 buildTranslate(float x, float y, float z);
mat4 buildScale(float x, float t, float z);

void main() {
    // vec2 quad = vec2(float(gl_VertexID & 1), float((gl_VertexID >> 1) & 1));
    // gl_Position = vec4(quad, 1.0f, 1.0f);
    // gl_Position = proj_mat * view_mat * buildRotateZ(65 * 3.1415f / 180 + time ) * buildRotateY(75 * 3.1415f / 180 + time ) * buildRotateX(85 * 3.1415f / 180 + time) * buildScale(.4f, .5f, .5f) * vec4(position, 1.0f);
    gl_Position =  proj_mat * view_mat * buildScale(.5f, .5f, .5f) * vec4(position, 1.0f);
    pos = position;
    c = color;
}

mat4 buildTranslate(float x, float y, float z) {
  mat4 trans = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,
                    x, y, z, 1.0);
  return trans;
}

mat4 buildRotateX(float rad) {
  mat4 xrot = mat4(1.0, 0.0, 0.0, 0.0, 0.0, cos(rad), -sin(rad), 0.0, 0.0,
                   sin(rad), cos(rad), 0.0, 0.0, 0.0, 0.0, 1.0);
  return xrot;
}

mat4 buildRotateY(float rad) {
  mat4 yrot = mat4(cos(rad), 0.0, sin(rad), 0.0, 0.0, 1.0, 0.0, 0.0, -sin(rad),
                   0.0, cos(rad), 0.0, 0.0, 0.0, 0.0, 1.0);
  return yrot;
}

// builds and returns a matrix that performs a rotation around the Z axis
mat4 buildRotateZ(float rad) {
  mat4 zrot = mat4(cos(rad), -sin(rad), 0.0, 0.0, sin(rad), cos(rad), 0.0, 0.0,
                   0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
  return zrot;
}

mat4 buildScale(float x, float y, float z) {
  mat4 scale = mat4(x, 0.0, 0.0, 0.0,
                    0.0, y, 0.0, 0.0,
                    0.0, 0.0, z, 0.0,
                    0.0, 0.0, 0.0, 1.0);
  return scale;
}
