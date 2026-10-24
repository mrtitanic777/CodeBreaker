float4x4 W;
row_major float4x4 V;
struct O { float4 P : POSITION0; float4 C : COLOR0; };
O VS(float4 p : POSITION0) { O o; o.P = mul(mul(p, W), V); o.C = mul(float4(0.3,0.6,0.9,1), V); return o; }
float4 PS(O i) : COLOR0 { return i.C; }
technique R { pass P0 { VertexShader = compile vs_3_0 VS(); PixelShader = compile ps_3_0 PS(); } }
