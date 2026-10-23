// Vertex shader work: matrices, an array read through the address register, a 3x3 normal
// transform, several interpolators, and a texture read with an explicit level of detail and a bias.
float4x4 World;
float4x4 View;
float3x3 NormalMatrix;
float4 Palette[8];
float2 Offset = float2(0.05, -0.03);
texture Tex;
sampler S = sampler_state { Texture = <Tex>; MinFilter = Linear; MagFilter = Linear; MipFilter = Point; };

struct VSIn { float4 Position : POSITION0; float3 Normal : NORMAL0; float4 Uv : TEXCOORD0; float4 Indices : BLENDINDICES0; };
struct VSOut { float4 Position : POSITION0; float3 Normal : TEXCOORD0; float4 Tint : TEXCOORD1; float2 Uv : TEXCOORD2; };

VSOut VS(VSIn i)
{
    VSOut o;
    o.Position = mul(mul(i.Position, World), View);
    o.Position.xy += Offset;
    o.Normal = normalize(mul(i.Normal, NormalMatrix));
    o.Tint = Palette[(int)i.Indices.x] * 0.5 + Palette[(int)i.Indices.y + 4] * 0.5;
    o.Uv = i.Uv.xy;
    return o;
}

float4 PS(VSOut i) : COLOR0
{
    float4 a = tex2Dlod(S, float4(i.Uv, 0, 0));
    float4 b = tex2Dbias(S, float4(i.Uv * 2.0, 0, 1.0));
    return float4(i.Normal * 0.5 + 0.5, 1) * 0.4 + i.Tint * 0.3 + (a + b) * 0.15;
}

technique Vertex { pass P0 { VertexShader = compile vs_3_0 VS(); PixelShader = compile ps_3_0 PS(); } }
