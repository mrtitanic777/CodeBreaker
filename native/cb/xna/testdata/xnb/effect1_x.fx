// sha1 9c77b71e40c3f95aa8d9426ad29b40863a70bdaf
// effect1_x: an Xbox 360 effect, recovered as HLSL by CodeBreaker (decompile/xbox_effect.py) from its microcode.
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
    float s;
    r0.xy = input.TexCoord0.xy;
    r1.xyzw = input.Color0.xyzw;
    t = tex2D(ColorSampler, r0.xy);
    r0.yzw = t.xyz;
    output.Color0.xyz = r0.yzw * r1.xyz;
    s = 0.33333334 * r1.w;
    r1.x = dot(r0.wyz, (1.0).xxx);
    r0.x = s;
    output.Color0.w = r0.x * r1.x;
    return output;
}

technique Technique1
{
    pass Pass1
    {
        PixelShader = compile ps_3_0 PS_Technique1_Pass1();
    }
}
