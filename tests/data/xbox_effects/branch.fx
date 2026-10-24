// Branches: dynamic ones on interpolated values (the compiler's jumps and predicated code), a bool parameter's static
// one, and selects.
bool Invert = true;
float Threshold = 0.45;
texture Tex;
sampler S = sampler_state { Texture = <Tex>; MinFilter = Point; MagFilter = Point; AddressU = Wrap; AddressV = Wrap; };

struct VSOut { float4 Position : POSITION0; float4 Uv : TEXCOORD0; float4 C : COLOR0; };

VSOut VS(float4 p : POSITION0, float4 uv : TEXCOORD0, float4 c : COLOR0)
{
    VSOut o;
    o.Position = p;
    o.Uv = uv;
    o.C = c;
    return o;
}

float4 PS(VSOut i) : COLOR0
{
    float4 result = i.C;
    [branch] if (i.Uv.x > Threshold)
    {
        float4 t = tex2Dlod(S, float4(i.Uv.xy * 3.0, 0, 0));
        result = t * float4(1.0, 0.8, 0.6, 1.0) + sin(i.Uv.y * 9.0) * 0.1;
        [branch] if (i.Uv.y > 0.7)
            result.rgb = 1.0 - result.rgb;
    }
    else
    {
        result.rg = i.Uv.yx;
        result.b = i.Uv.x < 0.2 ? 0.9 : 0.1;
    }
    if (Invert)
        result.a = 1.0 - result.a * 0.5;
    return result;
}

technique Branch { pass P0 { VertexShader = compile vs_3_0 VS(); PixelShader = compile ps_3_0 PS(); } }
