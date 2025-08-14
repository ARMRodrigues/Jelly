struct MyUniforms
{
    float4x4 model;
    float4x4 view;
    float4x4 projection;
};

uniform ConstantBuffer<MyUniforms> uniforms;

// Entrada do vertex buffer
struct VertexInput {
    float3 position : POSITION;
};

struct VertexOutput {
    float4 position : SV_POSITION;
};

VertexOutput main(VertexInput input) {
    VertexOutput output;
    float4 pos = float4(input.position, 1.0);

    output.position = mul(uniforms.projection, mul(uniforms.view, mul(uniforms.model, pos)));

    return output;
}