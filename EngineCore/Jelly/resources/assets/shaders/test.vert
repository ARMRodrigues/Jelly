#version 450

layout(set = 0, binding = 0) uniform MyUniforms {
    mat4 model;
    mat4 view;
    mat4 projection;
} uniforms;

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;

//layout(location = 0) out vec4 vColor;
layout(location = 0) out vec2 vUV;

void main() {
    vec4 pos = vec4(position, 1.0);
    vUV = uv;
    gl_Position = uniforms.projection * uniforms.view * uniforms.model * pos;
}
