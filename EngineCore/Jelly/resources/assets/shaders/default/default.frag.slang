struct FragmentOutput {
    float4 color : SV_TARGET;
};

FragmentOutput main() {
    FragmentOutput output;
    output.color = float4(1.0, 0.5, 0.2, 1.0); // Vermelho
    return output;
}
