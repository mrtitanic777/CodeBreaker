// The scalar and vector operations a pixel shader is made of, on interpolated values.
float Scale = 1.5;
float3 Axis = float3(0.2, 0.7, 0.4);

struct VSOut { float4 Position : POSITION0; float4 A : TEXCOORD0; float4 B : TEXCOORD1; float4 C : COLOR0; };

VSOut VS(float4 p : POSITION0, float4 uv : TEXCOORD0, float4 b : TEXCOORD1, float4 c : COLOR0)
{
    VSOut o;
    o.Position = p;
    o.A = uv;
    o.B = b;
    o.C = c;
    return o;
}

float4 PS(VSOut i) : COLOR0
{
    float x = i.A.x * Scale, y = i.A.y;
    float s = sin(x * 3.1) * 0.5 + 0.5;
    float c = cos(y * 2.3) * 0.5 + 0.5;
    float e = exp2(-x) * 0.5;
    float l = log2(1.0 + y) * 0.7;
    float p = pow(saturate(i.B.x), 2.2);
    float q = sqrt(y) * rsqrt(1.0 + x);
    float f = frac(x * 4.0) + floor(y * 3.0) / 6.0;
    float3 n = normalize(cross(i.B.xyz + 0.1, Axis));
    float3 r = reflect(-Axis, n);
    float m = max(min(x, y), abs(i.B.y - 0.5));
    float st = step(0.4, y) * 0.3 + smoothstep(0.2, 0.8, x) * 0.7;
    float4 lerped = lerp(i.C, float4(r * 0.5 + 0.5, 1), st);
    return float4(frac(s + e + l), frac(c + p + q), frac(f + m + dot(n, Axis)), 1) * 0.5 + lerped * 0.5;
}

technique Math { pass P0 { VertexShader = compile vs_3_0 VS(); PixelShader = compile ps_3_0 PS(); } }
