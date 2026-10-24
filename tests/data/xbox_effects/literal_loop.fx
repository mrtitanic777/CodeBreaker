// Loops whose counts are numbers: the compiler makes their loop constants itself (defi), as literals.
float4 W[4];

struct VSOut { float4 Position : POSITION0; float4 Uv : TEXCOORD0; };

VSOut VS(float4 p : POSITION0, float4 uv : TEXCOORD0)
{
    VSOut o;
    o.Position = p;
    o.Uv = uv;
    return o;
}

float4 PS(VSOut i) : COLOR0
{
    float4 s = 0;
    [loop] for (int k = 0; k < 10; k++) s += W[k % 4] * i.Uv.x * 0.1;
    [loop] for (int j = 2; j < 9; j += 3) s.y += i.Uv.y * j * 0.05;
    return frac(s * 0.5);
}

technique LiteralLoop { pass P0 { VertexShader = compile vs_3_0 VS(); PixelShader = compile ps_3_0 PS(); } }
