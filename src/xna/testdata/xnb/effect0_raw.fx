// sha1 a3b2e8b0cce4a9624f56ce3a279f5ac2ac763f15
// effect0_raw: an Xbox 360 effect, recovered as HLSL by CodeBreaker (decompile/xbox_effect.py) from its microcode.
// The shaders read as the compiled programs did: registers r0..rN, the constants by their names.


sampler ColorSampler;

struct PS_Technique1_Pass1_Input
{
    float4 TexCoord0 : TEXCOORD0;
    float4 Color0 : COLOR0;
};

struct PS_Technique1_Pass1_Output
{
    float4 Color0 : COLOR0;
};

PS_Technique1_Pass1_Output PS_Technique1_Pass1(PS_Technique1_Pass1_Input input)
{
    PS_Technique1_Pass1_Output output = (PS_Technique1_Pass1_Output)0;
    float4 r0 = 0.0, r1 = 0.0;
    float4 t;
    r0.xy = input.TexCoord0.xy;
    r1.xyzw = input.Color0.xyzw;
    t = tex2D(ColorSampler, r0.xy);
    r0.xyzw = float4(t.x, t.y, t.z, 1.0);
    output.Color0.xyzw = r0.xyzw * r1.xyzw;
    return output;
}

technique Technique1
{
    pass Pass1
    {
        PixelShader = compile ps_3_0 PS_Technique1_Pass1();
    }
}
