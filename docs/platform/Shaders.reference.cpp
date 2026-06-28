#include "stdafx.h"
#include "Shaders.h"
#include "Common.h"

// ---------------------------------------------------------------------------
// Shaders are RUNTIME-compiled from the embedded HLSL below via D3DXCompileShader,
// matching the proven-working indev port. (Earlier we shipped offline-precompiled
// Xenos microcode to dodge a 360 D3DX fault on the 'discard' statement -- but the
// pixel shader now uses clip() (NOT discard), which the runtime compiler accepts
// fine, so the precompiled path is no longer needed. The precompiled microcode was
// also a silent hazard: if the HLSL is edited but the bytecode isn't regenerated,
// the shader creates OK yet renders garbage. Runtime compile guarantees the VS/PS
// always match the source + the constant register layout below.)
// ---------------------------------------------------------------------------
static const char* g_src_textured_vs =
    "float4x4 g_WorldViewProj : register(c0);\n"
    "float4 g_FogParams : register(c8);\n"
    "struct VS_IN { float3 pos:POSITION; float4 col:COLOR0; float2 tex:TEXCOORD0; };\n"
    "struct VS_OUT { float4 pos:POSITION; float4 col:COLOR0; float2 tex:TEXCOORD0; float fog:TEXCOORD1; };\n"
    "VS_OUT main(VS_IN i) {\n"
    "  VS_OUT o;\n"
    "  o.pos = mul(float4(i.pos,1), g_WorldViewProj);\n"
    "  o.col = i.col;\n"
    "  o.tex = i.tex;\n"
    "  o.fog = saturate((g_FogParams.y - o.pos.w) / max(g_FogParams.y - g_FogParams.x, 0.001f));\n"
    "  return o;\n"
    "}\n";

static const char* g_src_textured_ps =
    "sampler s0 : register(s0);\n"
    "float4 g_FogColor : register(c0);\n"
    "float4 g_FogParams : register(c1);\n"
    "float4 g_Gamma : register(c3);\n"
    "struct PS_IN { float4 col:COLOR0; float2 tex:TEXCOORD0; float fog:TEXCOORD1; };\n"
    "float4 main(PS_IN i) : COLOR {\n"
    "  float4 texel = tex2D(s0, i.tex) * i.col;\n"
    "  clip(texel.a - g_FogParams.z);\n"
    "  float3 fogCol = g_FogColor.rgb;\n"
    "  float vis = lerp(1.0, i.fog, g_FogParams.w);\n"
    "  texel.rgb = lerp(fogCol, texel.rgb, vis);\n"
    "  texel.rgb = pow(texel.rgb, g_Gamma.x);\n"
    "  return texel;\n"
    "}\n";

static const char* g_src_untextured_vs =
    "float4x4 g_WorldViewProj : register(c0);\n"
    "struct VS_IN { float3 pos:POSITION; float4 col:COLOR0; float2 tex:TEXCOORD0; };\n"
    "struct VS_OUT { float4 pos:POSITION; float4 col:COLOR0; };\n"
    "VS_OUT main(VS_IN i) {\n"
    "  VS_OUT o;\n"
    "  o.pos = mul(float4(i.pos,1), g_WorldViewProj);\n"
    "  o.col = i.col;\n"
    "  return o;\n"
    "}\n";

static const char* g_src_untextured_ps =
    "float4 g_Gamma : register(c3);\n"
    "struct PS_IN { float4 col:COLOR0; };\n"
    "float4 main(PS_IN i) : COLOR {\n"
    "  float4 o = i.col;\n"
    "  o.rgb = pow(o.rgb, g_Gamma.x);\n"
    "  return o;\n"
    "}\n";

// Compile one HLSL source at runtime and create the shader. Returns NULL on failure,
// logging the compiler error (failure path only) so a broken shader is still diagnosable.
static IDirect3DVertexShader9* _compileVS(const char* src, const char* tag) {
    ID3DXBuffer* code = NULL; ID3DXBuffer* err = NULL;
    HRESULT hr = D3DXCompileShader(src, (UINT)strlen(src), NULL, NULL, "main", "vs_3_0", 0, &code, &err, NULL);
    if (FAILED(hr)) {
        DbgPrintF("[SHADER] %s VS compile FAILED hr=0x%08X: %s\n", tag, (unsigned)hr,
                  err ? (const char*)err->GetBufferPointer() : "(no message)");
        if (err) err->Release();
        return NULL;
    }
    IDirect3DVertexShader9* vs = NULL;
    hr = g_pd3dDevice->CreateVertexShader((const DWORD*)code->GetBufferPointer(), &vs);
    code->Release(); if (err) err->Release();
    if (FAILED(hr)) { DbgPrintF("[SHADER] %s VS create FAILED hr=0x%08X\n", tag, (unsigned)hr); return NULL; }
    return vs;
}
static IDirect3DPixelShader9* _compilePS(const char* src, const char* tag) {
    ID3DXBuffer* code = NULL; ID3DXBuffer* err = NULL;
    HRESULT hr = D3DXCompileShader(src, (UINT)strlen(src), NULL, NULL, "main", "ps_3_0", 0, &code, &err, NULL);
    if (FAILED(hr)) {
        DbgPrintF("[SHADER] %s PS compile FAILED hr=0x%08X: %s\n", tag, (unsigned)hr,
                  err ? (const char*)err->GetBufferPointer() : "(no message)");
        if (err) err->Release();
        return NULL;
    }
    IDirect3DPixelShader9* ps = NULL;
    hr = g_pd3dDevice->CreatePixelShader((const DWORD*)code->GetBufferPointer(), &ps);
    code->Release(); if (err) err->Release();
    if (FAILED(hr)) { DbgPrintF("[SHADER] %s PS create FAILED hr=0x%08X\n", tag, (unsigned)hr); return NULL; }
    return ps;
}

static const DWORD g_unused_textured_vs_bytecode[] = {
    0x102a1101, 0x00000130, 0x0000010c, 0x00000000, 0x00000024, 0x000000bc, 
    0x000000e4, 0x00000000, 0x00000000, 0x00000094, 0x0000001c, 0x00000087, 
    0xfffe0300, 0x00000002, 0x0000001c, 0x00000000, 0x00000080, 0x00000044, 
    0x00020008, 0x00010000, 0x00000050, 0x00000000, 0x00000060, 0x00020000, 
    0x00040000, 0x00000070, 0x00000000, 0x675f466f, 0x67506172, 0x616d7300, 
    0x00010003, 0x00010004, 0x00010000, 0x00000000, 0x675f576f, 0x726c6456, 
    0x69657750, 0x726f6a00, 0x00030003, 0x00040004, 0x00010000, 0x00000000, 
    0x76735f33, 0x5f300032, 0x2e302e32, 0x31323536, 0x2e3000ab, 0x00000000, 
    0x00000001, 0x00000000, 0x00000000, 0x00000014, 0x00fc0010, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000040, 0x000000cc, 0x00210003, 
    0x00000000, 0x00000000, 0x00001c63, 0x00000001, 0x00000003, 0x00000003, 
    0x00000290, 0x00100003, 0x0000a004, 0x00305005, 0x00003050, 0x00011151, 
    0x0002f2a0, 0x0000100e, 0x0000100d, 0x0000100f, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x3fb8aa3b, 0x00000000, 
    0x00000000, 0x00000000, 0x70153003, 0x00001200, 0xc2000000, 0x00005006, 
    0x00001200, 0xc4000000, 0x0000500b, 0x00002200, 0x00000000, 0x05f83000, 
    0x00000a88, 0x00000000, 0x05f81000, 0x00000688, 0x00000000, 0x05f80000, 
    0x00000fc8, 0x00000000, 0xc8080002, 0x003e3e00, 0x6f030300, 0xc8040002, 
    0x003e3e00, 0x6f020300, 0xc8020002, 0x003e3e00, 0x6f010300, 0xc8010002, 
    0x003e3e00, 0x6f000300, 0xc80f803e, 0x00000000, 0xe2020200, 0xa8400000, 
    0x00000042, 0xe20000ff, 0xc8040000, 0x02c66c00, 0xa1000800, 0x3a108001, 
    0x000000c6, 0xe2000000, 0xc8038000, 0x00b0b000, 0xe2000000, 0xc80f8002, 
    0x00000000, 0xe2010100, 0x00000000, 0x00000000, 0x00000000
};
static const DWORD g_textured_ps_bytecode[] = {
    0x102a1100, 0x00000124, 0x000000c4, 0x00000000, 0x00000024, 0x000000d0, 
    0x000000f8, 0x00000000, 0x00000000, 0x000000a8, 0x0000001c, 0x0000009b, 
    0xffff0300, 0x00000003, 0x0000001c, 0x00000000, 0x00000094, 0x00000058, 
    0x00020000, 0x00010000, 0x00000064, 0x00000000, 0x00000074, 0x00020001, 
    0x00010000, 0x00000064, 0x00000000, 0x00000080, 0x00030000, 0x00010000, 
    0x00000084, 0x00000000, 0x675f466f, 0x67436f6c, 0x6f7200ab, 0x00010003, 
    0x00010004, 0x00010000, 0x00000000, 0x675f466f, 0x67506172, 0x616d7300, 
    0x733000ab, 0x0004000c, 0x00010001, 0x00010000, 0x00000000, 0x70735f33, 
    0x5f300032, 0x2e302e32, 0x31323536, 0x2e3000ab, 0x00000000, 0x00000001, 
    0x00000000, 0x00000000, 0x00000014, 0x01fc0010, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000040, 0x00000084, 0x10000200, 0x00000008, 
    0x00000000, 0x00001c63, 0x00030007, 0x00000021, 0x00003050, 0x00001151, 
    0x0000f2a0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x3f000000, 0x00000000, 0x3f800000, 0x00000000, 0x00011002, 
    0x00001200, 0xc4000000, 0x00006003, 0x10091200, 0x22000000, 0x10080001, 
    0x1f1ff688, 0x00004000, 0xc8080202, 0x001b1b00, 0xe1000200, 0xc80e0200, 
    0x01fcfcfc, 0xcb000200, 0xb8110002, 0x036cc643, 0xa001ffff, 0xc8010202, 
    0x006cb1c6, 0x8b0201ff, 0xc8070202, 0x00156cc0, 0xcb000200, 0xc8000202, 
    0x00b16c00, 0x79ff0000, 0xc80f8000, 0x00000000, 0xe2020200, 0x00000000, 
    0x00000000, 0x00000000
};
static const DWORD g_untextured_vs_bytecode[] = {
    0x102a1101, 0x000000c4, 0x00000084, 0x00000000, 0x00000024, 0x00000000, 
    0x0000008c, 0x00000000, 0x00000000, 0x00000064, 0x0000001c, 0x00000057, 
    0xfffe0300, 0x00000001, 0x0000001c, 0x00000000, 0x00000050, 0x00000030, 
    0x00020000, 0x00040000, 0x00000040, 0x00000000, 0x675f576f, 0x726c6456, 
    0x69657750, 0x726f6a00, 0x00030003, 0x00040004, 0x00010000, 0x00000000, 
    0x76735f33, 0x5f300032, 0x2e302e32, 0x31323536, 0x2e3000ab, 0x00000000, 
    0x00000084, 0x00010001, 0x00000000, 0x00000000, 0x00001021, 0x00000001, 
    0x00000002, 0x00000001, 0x00000290, 0x00100003, 0x0030a004, 0x0000f0a0, 
    0x00001009, 0x30052003, 0x00001200, 0xc2000000, 0x00004005, 0x00001200, 
    0xc4000000, 0x00001009, 0x00002200, 0x00000000, 0x05f81000, 0x00000a88, 
    0x00000000, 0x05f80000, 0x00000688, 0x00000000, 0xc801803e, 0x003e3e00, 
    0x6f000100, 0xc802803e, 0x003e3e00, 0x6f010100, 0xc804803e, 0x003e3e00, 
    0x6f020100, 0xc808803e, 0x003e3e00, 0x6f030100, 0xc80f8000, 0x00000000, 
    0xe2000000, 0x00000000, 0x00000000, 0x00000000
};
static const DWORD g_untextured_ps_bytecode[] = {
    0x102a1100, 0x0000007c, 0x00000024, 0x00000000, 0x00000024, 0x00000000, 
    0x00000058, 0x00000000, 0x00000000, 0x00000030, 0x0000001c, 0x00000023, 
    0xffff0300, 0x00000000, 0x00000000, 0x00000000, 0x0000001c, 0x70735f33, 
    0x5f300032, 0x2e302e32, 0x31323536, 0x2e3000ab, 0x00000000, 0x00000024, 
    0x10000000, 0x00000000, 0x00000000, 0x00001021, 0x00000001, 0x00000001, 
    0x0000f0a0, 0x00000000, 0x1001c400, 0x22000000, 0xc80f8000, 0x00000000, 
    0xe2000000, 0x00000000, 0x00000000, 0x00000000
};

IDirect3DVertexDeclaration9* Shaders::s_vertexDecl = NULL;
IDirect3DVertexShader9*  Shaders::s_texturedVS = NULL;
IDirect3DPixelShader9*   Shaders::s_texturedPS = NULL;
IDirect3DVertexShader9*  Shaders::s_untexturedVS = NULL;
IDirect3DPixelShader9*   Shaders::s_untexturedPS = NULL;
D3DXMATRIX Shaders::s_worldMatrix;
D3DXMATRIX Shaders::s_viewMatrix;
D3DXMATRIX Shaders::s_projMatrix;
float Shaders::s_fogColor[3] = {0.5f, 0.8f, 1.0f};
float Shaders::s_fogDensity = 0.02f;
float Shaders::s_fogStart = 0.0f;       // GL_FOG_START (world units) -> VS fog factor
float Shaders::s_fogEnd   = 1000.0f;    // GL_FOG_END
bool  Shaders::s_fogEnabled = false;
float Shaders::s_alphaRef = 0.0f;  // PORT(★ alpha cutout): PS clip threshold (g_FogParams.z). See header.
float Shaders::s_gamma = 1.0f;   // PORT(★ brightness): PS gamma exponent = pow(rgb, g_Gamma.x). 1.0 = IDENTITY
                                 // (texture shown at its TRUE/default brightness, like Java's gamma-less GL).
                                 // Was 1.19 which DARKENED everything (the user reported the menu/dirt far too
                                 // dark on the emulator); the working indev port uses 1.0 too. Keep at 1.0 so
                                 // no global gamma can dim a texture below its authored color.

bool Shaders::init() {
    D3DVERTEXELEMENT9 decl[] = {
        { 0, 0,  D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
        { 0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,    0 },
        { 0, 16, D3DDECLTYPE_FLOAT2,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
        D3DDECL_END()
    };
    if (FAILED(g_pd3dDevice->CreateVertexDeclaration(decl, &s_vertexDecl)))
        s_vertexDecl = NULL;

    // Compile the four shaders on-device from the embedded HLSL (matches indev).
    s_texturedVS   = _compileVS(g_src_textured_vs,   "textured");
    s_texturedPS   = _compilePS(g_src_textured_ps,   "textured");
    s_untexturedVS = _compileVS(g_src_untextured_vs, "untextured");
    s_untexturedPS = _compilePS(g_src_untextured_ps, "untextured");

    D3DXMatrixIdentity(&s_worldMatrix);
    D3DXMatrixIdentity(&s_viewMatrix);
    D3DXMatrixIdentity(&s_projMatrix);
    return (s_texturedVS && s_texturedPS && s_untexturedVS && s_untexturedPS);
}

void Shaders::shutdown() {
    if (s_vertexDecl)   { s_vertexDecl->Release();   s_vertexDecl = NULL; }
    if (s_texturedVS)   { s_texturedVS->Release();   s_texturedVS = NULL; }
    if (s_texturedPS)   { s_texturedPS->Release();    s_texturedPS = NULL; }
    if (s_untexturedVS) { s_untexturedVS->Release();  s_untexturedVS = NULL; }
    if (s_untexturedPS) { s_untexturedPS->Release();   s_untexturedPS = NULL; }
}

void Shaders::useTexturedShader() {
    g_pd3dDevice->SetVertexShader(s_texturedVS);
    g_pd3dDevice->SetPixelShader(s_texturedPS);
    g_pd3dDevice->SetVertexDeclaration(s_vertexDecl);
}

void Shaders::useUntexturedShader() {
    g_pd3dDevice->SetVertexShader(s_untexturedVS);
    g_pd3dDevice->SetPixelShader(s_untexturedPS);
    g_pd3dDevice->SetVertexDeclaration(s_vertexDecl);
}

void Shaders::updateShaderConstants() {
    D3DXMATRIX wvp;
    D3DXMatrixMultiply(&wvp, &s_worldMatrix, &s_viewMatrix);
    D3DXMatrixMultiply(&wvp, &wvp, &s_projMatrix);
    D3DXMatrixTranspose(&wvp, &wvp);
    g_pd3dDevice->SetVertexShaderConstantF(0, (float*)&wvp, 4);
    // Layout matches BOTH shaders' g_FogParams usage: VS reads .x=fogStart .y=fogEnd for the linear
    // fog factor (g_FogParams.y - w)/(g_FogParams.y - g_FogParams.x); PS reads .z=alphaRef for the
    // cutout clip and .w=fogEnabled for the lerp(1, fog, .w). Previously this packed {density, enabled,
    // alphaRef, 0} -> .w was always 0 so distance fog was silently disabled (terrain never faded to the
    // sky/fog color = part of the hard black-vs-Java horizon).
    float fogParams[4] = { s_fogStart, s_fogEnd, s_alphaRef, s_fogEnabled ? 1.0f : 0.0f };
    g_pd3dDevice->SetVertexShaderConstantF(8, fogParams, 1);
    float fogCol[4] = { s_fogColor[0], s_fogColor[1], s_fogColor[2], 1.0f };
    g_pd3dDevice->SetPixelShaderConstantF(0, fogCol, 1);
    g_pd3dDevice->SetPixelShaderConstantF(1, fogParams, 1);
    // PORT(★ CRITICAL): both the textured AND untextured pixel shaders read g_Gamma at
    // register c3 (texel.rgb = pow(texel.rgb, g_Gamma.x)). If c3 is never set it is an
    // UNINITIALIZED shader constant -> pow(rgb, garbage) -> corrupted/black output -> the
    // whole scene renders invisible (blue clear shows through). MUST upload c3 every draw.
    // Default 1.0 = identity (no change), matching Java's gamma-less OpenGL + the indev port.
    float gammaVec[4] = { s_gamma, s_gamma, s_gamma, 1.0f };
    g_pd3dDevice->SetPixelShaderConstantF(3, gammaVec, 1);
}

void Shaders::setWorldMatrix(const D3DXMATRIX& m) { s_worldMatrix = m; }
void Shaders::setViewMatrix(const D3DXMATRIX& m)  { s_viewMatrix = m; }
void Shaders::setProjectionMatrix(const D3DXMATRIX& m) { s_projMatrix = m; }
D3DXMATRIX& Shaders::getWorldMatrix()  { return s_worldMatrix; }
D3DXMATRIX& Shaders::getViewMatrix()   { return s_viewMatrix; }
D3DXMATRIX& Shaders::getProjectionMatrix() { return s_projMatrix; }
void Shaders::setFogColor(float r, float g, float b) { s_fogColor[0]=r; s_fogColor[1]=g; s_fogColor[2]=b; }
void Shaders::setFogDensity(float d) { s_fogDensity = d; }
void Shaders::setFogStart(float s)   { s_fogStart = s; }
void Shaders::setFogEnd(float e)     { s_fogEnd = e; }
void Shaders::setFogEnabled(bool e)  { s_fogEnabled = e; }
void Shaders::setGamma(float g)      { s_gamma = g; }   // PORT(brightness): PS gamma exponent for the next draw(s)
void Shaders::setAlphaRef(float a)   { s_alphaRef = a; } // PORT(alpha cutout): PS clip threshold for the next draw(s)
