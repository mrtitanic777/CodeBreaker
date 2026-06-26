#ifndef SHADERS_H
#define SHADERS_H

#include "stdafx.h"

struct TesselatorVertex {
    float x, y, z;
    DWORD color;
    float u, v;
};

class Shaders {
public:
    static bool init();
    static void shutdown();

    static void useTexturedShader();
    static void useUntexturedShader();
    static void updateShaderConstants();

    static void setWorldMatrix(const D3DXMATRIX& m);
    static void setViewMatrix(const D3DXMATRIX& m);
    static void setProjectionMatrix(const D3DXMATRIX& m);
    static void setFogColor(float r, float g, float b);
    static void setFogDensity(float d);
    static void setFogEnabled(bool enabled);

    static D3DXMATRIX& getWorldMatrix();
    static D3DXMATRIX& getViewMatrix();
    static D3DXMATRIX& getProjectionMatrix();

    static IDirect3DVertexDeclaration9* s_vertexDecl;

public: // was private
    static IDirect3DVertexShader9*  s_texturedVS;
    static IDirect3DPixelShader9*   s_texturedPS;
    static IDirect3DVertexShader9*  s_untexturedVS;
    static IDirect3DPixelShader9*   s_untexturedPS;
    static D3DXMATRIX s_worldMatrix;
    static D3DXMATRIX s_viewMatrix;
    static D3DXMATRIX s_projMatrix;
    static float s_fogColor[3];
    static float s_fogDensity;
    static bool  s_fogEnabled;
};

#endif // SHADERS_H
