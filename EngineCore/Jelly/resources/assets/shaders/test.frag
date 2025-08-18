#version 450

layout(location = 0) in vec2 fragUV;

layout(location = 0) out vec4 fragColor;

layout(set = 0, binding = 1) uniform sampler2D albedoTexture;

void main() {
    vec4 albedo = texture(albedoTexture, fragUV);
    //vec4 albedo = vec4(1.0, 0.0, 0.0, 1.0);
    vec4 color = vec4(1.0, 0.0, 0.0, 1.0); // vermelho

    //fragColor = albedo * color;
    fragColor = albedo;
}
