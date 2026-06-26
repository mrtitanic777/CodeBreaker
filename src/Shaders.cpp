#include "stdafx.h"
#include "Shaders.h"
#include "Common.h"

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

static const char* s_texturedVS_src =
    "float4x4 g_WorldViewProj : register(c0);\n"
    "float4 g_FogParams : register(c8);\n"
    "struct VS_IN { float3 pos:POSITION; float4 col:COLOR0; float2 tex:TEXCOORD0; };\n"
    "struct VS_OUT { float4 pos:POSITION; float4 col:COLOR0; float2 tex:TEXCOORD0; float fog:TEXCOORD1; };\n"
    "VS_OUT main(VS_IN i) {\n"
    "  VS_OUT o;\n"
    "  o.pos = mul(float4(i.pos,1), g_WorldViewProj);\n"
    "  o.col = i.col;\n"
    "  o.tex = i.tex;\n"
    "  o.fog = saturate(exp(-g_FogParams.x * o.pos.z));\n"
    "  return o;\n"
    "}\n";

static const char* s_texturedPS_src =
    "sampler s0 : register(s0);\n"
    "float4 g_FogColor : register(c0);\n"
    "float4 g_FogParams : register(c1);\n"
    "struct PS_IN { float4 col:COLOR0; float2 tex:TEXCOORD0; float fog:TEXCOORD1; };\n"
    "float4 main(PS_IN i) : COLOR {\n"
    "  float4 texel = tex2D(s0, i.tex) * i.col;\n"
    "  if (texel.a < 0.5) discard;\n"
    "  float3 fogCol = g_FogColor.rgb;\n"
    "  texel.rgb = lerp(fogCol, texel.rgb, i.fog * g_FogParams.y);\n"
    "  return texel;\n"
    "}\n";

static const char* s_untexturedVS_src =
    "float4x4 g_WorldViewProj : register(c0);\n"
    "struct VS_IN { float3 pos:POSITION; float4 col:COLOR0; float2 tex:TEXCOORD0; };\n"
    "struct VS_OUT { float4 pos:POSITION; float4 col:COLOR0; };\n"
    "VS_OUT main(VS_IN i) {\n"
    "  VS_OUT o;\n"
    "  o.pos = mul(float4(i.pos,1), g_WorldViewProj);\n"
    "  o.col = i.col;\n"
    "  return o;\n"
    "}\n";

static const char* s_untexturedPS_src =
    "struct PS_IN { float4 col:COLOR0; };\n"
    "float4 main(PS_IN i) : COLOR {\n"
    "  return i.col;\n"
    "}\n";

static IDirect3DVertexShader9* CompileVS(const char* src) {
    ID3DXBuffer* code = NULL;
    ID3DXBuffer* errs = NULL;
    HRESULT hr = D3DXCompileShader(src, (UINT)strlen(src), NULL, NULL, "main", "vs_3_0", 0, &code, &errs, NULL);
    if (FAILED(hr)) {
        if (errs) { OutputDebugStringA((const char*)errs->GetBufferPointer()); errs->Release(); }
        return NULL;
    }
    IDirect3DVertexShader9* vs = NULL;
    g_pd3dDevice->CreateVertexShader((const DWORD*)code->GetBufferPointer(), &vs);
    code->Release();
    if (errs) errs->Release();
    return vs;
}

static IDirect3DPixelShader9* CompilePS(const char* src) {
    ID3DXBuffer* code = NULL;
    ID3DXBuffer* errs = NULL;
    HRESULT hr = D3DXCompileShader(src, (UINT)strlen(src), NULL, NULL, "main", "ps_3_0", 0, &code, &errs, NULL);
    if (FAILED(hr)) {
        if (errs) { OutputDebugStringA((const char*)errs->GetBufferPointer()); errs->Release(); }
        return NULL;
    }
    IDirect3DPixelShader9* ps = NULL;
    g_pd3dDevice->CreatePixelShader((const DWORD*)code->GetBufferPointer(), &ps);
    code->Release();
    if (errs) errs->Release();
    return ps;
}

bool Shaders::init() {
    D3DVERTEXELEMENT9 decl[] = {
        { 0, 0,  D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
        { 0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,    0 },
        { 0, 16, D3DDECLTYPE_FLOAT2,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
        D3DDECL_END()
    };
    g_pd3dDevice->CreateVertexDeclaration(decl, &s_vertexDecl);
    s_texturedVS   = CompileVS(s_texturedVS_src);
    s_texturedPS   = CompilePS(s_texturedPS_src);
    s_untexturedVS = CompileVS(s_untexturedVS_src);
    s_untexturedPS = CompilePS(s_untexturedPS_src);
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
