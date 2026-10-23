float4x4 WorldViewProj;
float4 Tint = float4(1, 0.5, 0.25, 1);
float Brightness = 2;
texture Diffuse;
sampler DiffuseSampler = sampler_state { Texture = <Diffuse>; MinFilter = Linear; MagFilter = Linear; AddressU = Clamp; AddressV = Wrap; };

struct VSIn { float4 Position : POSITION0; float2 Uv : TEXCOORD0; float4 Color : COLOR0; };
struct VSOut { float4 Position : POSITION0; float2 Uv : TEXCOORD0; float4 Color : COLOR0; };

VSOut VS(VSIn i)
{
    VSOut o;
    o.Position = mul(i.Position, WorldViewProj);
    o.Uv = i.Uv * 2;
    o.Color = i.Color * Tint;
    return o;
}

float4 PS(VSOut i) : COLOR0
{
    float4 c = tex2D(DiffuseSampler, i.Uv) * i.Color;
    return c * Brightness + float4(0.1, 0.2, 0.3, 0);
}

technique Probe
{
    pass P0
    {
        AlphaBlendEnable = true;
        SrcBlend = SrcAlpha;
        DestBlend = InvSrcAlpha;
        CullMode = None;
        VertexShader = compile vs_3_0 VS();
        PixelShader = compile ps_3_0 PS();
    }
}
