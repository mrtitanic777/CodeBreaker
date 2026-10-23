// Loops: one of a fixed count (the compiler may unroll it) and one whose count is a parameter (a loop in the
// microcode, which the translator does not do yet: it must say so, not get it wrong).
int Steps = 3;
float4 Weights[4];

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
    float4 sum = 0;
    for (int k = 0; k < 4; k++)
        sum += Weights[k] * (i.Uv.x + k * 0.1);
    [loop] for (int j = 0; j < Steps; j++)
        sum.y += 0.05 * i.Uv.y;
    return frac(sum);
}

technique Loop { pass P0 { VertexShader = compile vs_3_0 VS(); PixelShader = compile ps_3_0 PS(); } }
