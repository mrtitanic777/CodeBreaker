#include "stdafx.h"
#include "Shaders.h"
#include "Common.h"

// ---------------------------------------------------------------------------
// Shaders are precompiled OFFLINE to Xenos microcode with the Xbox 360 XDK
// shader compiler (fxc /Tvs_3_0 | /Tps_3_0). We deliberately do NOT call
// D3DXCompileShader at runtime:
//   * The 360 D3DX microcode compiler rejects some HLSL the PC compiler accepts
//     (notably the 'discard' statement -- the textured pixel shader therefore
//     uses clip(), which compiles to kill_gt). Compiling the 'discard' shader at
//     runtime faulted the 360 D3DX compiler; the CRT then tried to unwind via
//     XexPcToFileHeader and the title died with a black screen.
//   * Shipping 360 titles embed precompiled microcode; runtime compilation is a
//     devkit-only convenience.
// The HLSL sources live in shaders\*.hlsl; regenerate via shaders\build_shaders.ps1.
// ---------------------------------------------------------------------------
static const DWORD g_textured_vs_bytecode[] = {
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
bool  Shaders::s_fogEnabled = false;

bool Shaders::init() {
    D3DVERTEXELEMENT9 decl[] = {
        { 0, 0,  D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
        { 0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,    0 },
        { 0, 16, D3DDECLTYPE_FLOAT2,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
        D3DDECL_END()
    };
    if (FAILED(g_pd3dDevice->CreateVertexDeclaration(decl, &s_vertexDecl)))
        s_vertexDecl = NULL;

    // Create the four shaders straight from the embedded 360 microcode. No
    // runtime compilation -> no D3DX compiler dependency, no fault path.
    if (FAILED(g_pd3dDevice->CreateVertexShader((const DWORD*)g_textured_vs_bytecode,   &s_texturedVS)))   s_texturedVS   = NULL;
    if (FAILED(g_pd3dDevice->CreatePixelShader ((const DWORD*)g_textured_ps_bytecode,   &s_texturedPS)))   s_texturedPS   = NULL;
    if (FAILED(g_pd3dDevice->CreateVertexShader((const DWORD*)g_untextured_vs_bytecode, &s_untexturedVS))) s_untexturedVS = NULL;
    if (FAILED(g_pd3dDevice->CreatePixelShader ((const DWORD*)g_untextured_ps_bytecode, &s_untexturedPS))) s_untexturedPS = NULL;

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
    float fogParams[4] = { s_fogDensity, s_fogEnabled ? 1.0f : 0.0f, 0, 0 };
    g_pd3dDevice->SetVertexShaderConstantF(8, fogParams, 1);
    float fogCol[4] = { s_fogColor[0], s_fogColor[1], s_fogColor[2], 1.0f };
    g_pd3dDevice->SetPixelShaderConstantF(0, fogCol, 1);
    g_pd3dDevice->SetPixelShaderConstantF(1, fogParams, 1);
}

void Shaders::setWorldMatrix(const D3DXMATRIX& m) { s_worldMatrix = m; }
void Shaders::setViewMatrix(const D3DXMATRIX& m)  { s_viewMatrix = m; }
void Shaders::setProjectionMatrix(const D3DXMATRIX& m) { s_projMatrix = m; }
D3DXMATRIX& Shaders::getWorldMatrix()  { return s_worldMatrix; }
D3DXMATRIX& Shaders::getViewMatrix()   { return s_viewMatrix; }
D3DXMATRIX& Shaders::getProjectionMatrix() { return s_projMatrix; }
void Shaders::setFogColor(float r, float g, float b) { s_fogColor[0]=r; s_fogColor[1]=g; s_fogColor[2]=b; }
void Shaders::setFogDensity(float d) { s_fogDensity = d; }
void Shaders::setFogEnabled(bool e)  { s_fogEnabled = e; }
