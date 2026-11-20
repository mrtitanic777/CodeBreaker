// sha1 c4dfa6a607b6014aefbfca5e554c47ed72f247b0
// MotionBlurBlend: an Xbox 360 effect, recovered as HLSL by CodeBreaker (decompile/xbox_effect.py) from its microcode.
// The shaders read as the compiled programs did: registers r0..rN, the constants by their names.

texture TargetTexture;
texture MaskTexture;
texture ScreenTexture;

sampler screenSampler = sampler_state
{
    Texture = <ScreenTexture>;
};

sampler2D targetSampler = sampler_state
{
    Texture = <TargetTexture>;
};

sampler2D maskSampler = sampler_state
{
    Texture = <MaskTexture>;
};

struct PS_Technique1_Pass1_Input
{
    float4 TexCoord0 : TEXCOORD0;
};

struct PS_Technique1_Pass1_Output
{
    float4 Color0 : COLOR0;
};

PS_Technique1_Pass1_Output PS_Technique1_Pass1(PS_Technique1_Pass1_Input input)
{
    PS_Technique1_Pass1_Output output = (PS_Technique1_Pass1_Output)0;
    float4 r0 = 0.0, r1 = 0.0;
    float ps = 0.0;
    float4 t;
    r0.xy = input.TexCoord0.xy;
    t = tex2D(maskSampler, r0.xy);
    r0.w = t.x;
    t = tex2D(targetSampler, r0.xy);
    r1.xyz = t.xyz;
    t = tex2D(screenSampler, r0.xy);
    r0.xyz = t.xyz;
    r1.xyz = r1.xyz - r0.xyz;
    output.Color0.xyz = r1.xyz * r0.www + r0.xyz;
    output.Color0.w = 1.0;
    return output;
}

technique Technique1
{
    pass Pass1
    {
        PixelShader = compile ps_3_0 PS_Technique1_Pass1();
    }
}
