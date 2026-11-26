// GpuShaders.h -- GL's fixed-function pipeline as HLSL, the part every Direct3D backend shares: the vertex stage (transform, the
// texture matrices, two-light lighting with the colour material, flat shading's provoking values) and the fragment stage (two
// texture environments, the alpha test, fog). A backend wraps these in its own entry points -- Direct3D 9's shader model 3
// registers and semantics, Direct3D 11's constant buffers and SV_ semantics -- so the maths is written once.
//
// The constants are named as GL11.cpp fills them (gpu::Constants): the vertex stage's 28 float4s, the fragment stage's 4.
#ifndef GPU_SHADERS_H
#define GPU_SHADERS_H

namespace j2xgl {
namespace gpu {

// The constants (gpu::Constants), as each backend declares them -- Direct3D 9 as c-registers, Direct3D 11 as a constant buffer,
// both in this order (a float4x4 is four float4s):
//   vertex stage   c0 gWVP (model-view-projection, GL's clip space mapped to the API's)  c4 gMV (the modelview)
//                  c8 gT0, c12 gT1 (texture matrices)  c16 gLit (x lighting, y/z lights 0/1 on, w colour material: 0 off
//                  1 AMBIENT 2 DIFFUSE 3 both)  c17 gAmb (the model ambient; w normalize)  c18-20 gL0p/gL0d/gL0a (light 0:
//                  eye position, w 0 a direction; diffuse; ambient)  c21-23 gL1p/gL1d/gL1a  c24 gMa, c25 gMd (the material)
//                  c26 gTint (a colourless list: the colour at glCallList)  c27 gShade (x flat shading)
//   fragment stage c0 gTex (x/y units on, z/w environments: 0 MODULATE 1 REPLACE 2 ADD 3 DECAL)  c1 gAlpha (x on, y the
//                  comparison 0..7 = GL_NEVER..GL_ALWAYS, z the reference)  c2 gFog (x on, y 0 LINEAR 1 EXP 2 EXP2, z start,
//                  w end)  c3 gFogC (rgb the colour, a the density)

// The vertex stage. glVertex fills the clip position, the lit colour, both texture coordinates and the fog distance.
static const char* const kGlVertexStage =
	"float3 toLight(float4 lp, float3 e) { float3 d = lp.w == 0 ? lp.xyz : lp.xyz - e; float l = dot(d, d); return l > 1e-12 ? d * rsqrt(l) : float3(0, 0, 0); }\n"
	"void glVertex(float3 p, float3 vn, float4 vc, float2 t0, float2 t1, float3 fn, float4 fc,\n"
	"              out float4 op, out float4 oc, out float2 ot0, out float2 ot1, out float ofz) {\n"
	"  float3 n = gShade.x > 0.5 ? fn : vn;\n"
	"  float4 c = gShade.x > 0.5 ? fc : vc;\n"
	"  float4 P = float4(p, 1);\n"
	"  op = mul(P, gWVP);\n"
	"  float4 e = mul(P, gMV);\n"
	"  ofz = abs(e.z);\n"                 // GL's fog coordinate: the eye-plane distance
	"  float4 a = mul(float4(t0, 0, 1), gT0); ot0 = a.xy / a.w;\n"
	"  float4 b = mul(float4(t1, 0, 1), gT1); ot1 = b.xy / b.w;\n"
	"  float4 col = c * gTint;\n"
	"  if (gLit.x > 0.5) {\n"
	"    float3 N = mul(float4(n, 0), gMV).xyz;\n"
	"    if (gAmb.w > 0.5) { float l = dot(N, N); N = l > 1e-12 ? N * rsqrt(l) : float3(0, 0, 0); }\n"
	"    float4 ma = (gLit.w > 0.5 && gLit.w < 1.5) || gLit.w > 2.5 ? col : gMa;\n"
	"    float4 md = gLit.w > 1.5 ? col : gMd;\n"
	"    float3 s = gAmb.rgb * ma.rgb;\n"
	"    if (gLit.y > 0.5) s += gL0a.rgb * ma.rgb + max(dot(N, toLight(gL0p, e.xyz)), 0) * gL0d.rgb * md.rgb;\n"
	"    if (gLit.z > 0.5) s += gL1a.rgb * ma.rgb + max(dot(N, toLight(gL1p, e.xyz)), 0) * gL1d.rgb * md.rgb;\n"
	"    col = float4(s, md.a);\n"
	"  }\n"
	"  oc = saturate(col);\n"
	"}\n";

// The fragment stage, given the colour and the two units' samples (taken by the backend's entry point). Returns false where
// the alpha test discards (the entry point clips).
static const char* const kGlFragmentStage =
	"float4 env(float4 o, float4 t, float m) {\n"
	"  if (m < 0.5) return o * t;\n"
	"  if (m < 1.5) return t;\n"
	"  if (m < 2.5) return float4(saturate(o.rgb + t.rgb), o.a * t.a);\n"
	"  return float4(lerp(o.rgb, t.rgb, t.a), o.a);\n"
	"}\n"
	"float4 glFragment(float4 c, float4 s0, float4 s1, float fz, out bool keep) {\n"
	"  float4 o = c;\n"
	"  if (gTex.x > 0.5) o = env(o, s0, gTex.z);\n"
	"  if (gTex.y > 0.5) o = env(o, s1, gTex.w);\n"
	"  keep = true;\n"
	"  if (gAlpha.x > 0.5) {\n"
	"    float a = o.a, r = gAlpha.z, f = gAlpha.y;\n"
	"    keep = f < 0.5 ? false : f < 1.5 ? a < r : f < 2.5 ? a == r : f < 3.5 ? a <= r : f < 4.5 ? a > r : f < 5.5 ? a != r : f < 6.5 ? a >= r : true;\n"   // (`pass` is reserved in HLSL)
	"  }\n"
	"  if (gFog.x > 0.5) {\n"
	"    float d = gFogC.a * fz;\n"
	"    float k = gFog.y < 0.5 ? (gFog.w - fz) / max(gFog.w - gFog.z, 1e-6) : gFog.y < 1.5 ? exp(-d) : exp(-d * d);\n"
	"    o.rgb = lerp(gFogC.rgb, o.rgb, saturate(k));\n"
	"  }\n"
	"  return o;\n"
	"}\n";

}  // namespace gpu
}  // namespace j2xgl

#endif // GPU_SHADERS_H
