float4x4 World;
float4x4 ViewProj;
float4x3 Bones[20];
float3 LightDir = float3(0, 1, 0);
bool UseFog;
texture Tex;
sampler S1 = sampler_state { Texture = <Tex>; };
sampler S2 = sampler_state { Texture = <Tex>; };

struct VSIn { float4 Position : POSITION0; float3 Normal : NORMAL0; float2 Uv0 : TEXCOORD0; float2 Uv3 : TEXCOORD3; float4 Color1 : COLOR1; float4 Indices : BLENDINDICES0; float4 Weights : BLENDWEIGHT0; };
struct VSOut { float4 Position : POSITION0; float4 Color : COLOR0; float2 UvB : TEXCOORD5; float3 N : TEXCOORD1; float2 UvA : TEXCOORD0; float Fog : TEXCOORD2; };

VSOut VS(VSIn i)
{
    VSOut o;
    float4x3 skin = Bones[i.Indices.x] * i.Weights.x + Bones[i.Indices.y] * i.Weights.y;
    float3 p = mul(i.Position, skin);
    o.Position = mul(mul(float4(p, 1), World), ViewProj);
    o.N = mul(i.Normal, (float3x3)World);
    o.UvA = i.Uv0;
    o.UvB = i.Uv3;
    o.Color = i.Color1;
    o.Fog = UseFog ? o.Position.z * 0.01 : 0;
    return o;
}

float4 PS(VSOut i) : COLOR0
{
    float l = saturate(dot(normalize(i.N), LightDir));
    float4 c = tex2D(S1, i.UvA) * tex2D(S2, i.UvB) * i.Color * l;
    clip(c.a - 0.02);
    return lerp(c, float4(1, 1, 1, 1), i.Fog);
}

technique T { pass P { VertexShader = compile vs_3_0 VS(); PixelShader = compile ps_3_0 PS(); } }
