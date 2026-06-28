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
    static void setFogStart(float s);
    static void setFogEnd(float e);
    static void setFogEnabled(bool enabled);
    static void setGamma(float g);   // PORT(brightness): PS c3 gamma exponent (1.0 = identity)
    // PORT(★ alpha cutout): Xenos ignores D3DRS_ALPHATESTENABLE; the textured PS does
    // clip(texel.a - g_FogParams.z). setAlphaRef sets that threshold. 0.5 = hard cutout
    // (discard transparent texels -> items/particles/leaves render with proper holes instead
    // of opaque black boxes). 0.0 = keep everything (for soft-blended passes: water, smoke,
    // overlays). Default is 0.0 to preserve the working world/water/vignette; cutout draws
    // bracket themselves with setAlphaRef(0.5)...setAlphaRef(0.0). Mirrors the indev reference.
    static void setAlphaRef(float a);

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
    static float s_fogStart;
    static float s_fogEnd;
    static bool  s_fogEnabled;
    static float s_gamma;   // PORT(brightness): PS c3 gamma exponent (1.0 = identity)
    static float s_alphaRef; // PORT(alpha cutout): clip threshold uploaded to PS g_FogParams.z
};

#endif // SHADERS_H
