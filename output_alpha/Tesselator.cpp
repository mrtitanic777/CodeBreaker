#include "stdafx.h"
#include "Tesselator.h"
#include "Common.h"
#include "Shaders.h"

Tesselator* Tesselator::instance = NULL;

Tesselator::Tesselator()
    : m_vertexCount(0), m_u(0), m_v(0)
    , m_color(0xFFFFFFFF), m_hasTexture(false)
    , m_hasColor(false), m_colorDisabled(false), m_addedVertices(0)
{
}

Tesselator& Tesselator::getInstance() {
    static Tesselator s_instance;
    instance = &s_instance;
    return s_instance;
}

void Tesselator::init() {
    m_vertexCount = 0;
    m_addedVertices = 0;
    m_hasTexture = false;
    m_hasColor = false;
    m_colorDisabled = false;
}

// Java Tessellator.startDrawing(int mode) variant: GL primitive mode (GL_QUADS=7,
// GL_LINE_STRIP=3, GL_LINES=1, ...). The D3D9 Tesselator currently always emits
// quads via flush(); the mode is accepted for source compatibility (PORT: line/
// strip primitive modes draw as quads for now).
void Tesselator::init(int mode) {
    (void)mode;
    init();
}

void Tesselator::flush() {
    if (m_vertexCount == 0) return;

    // Bind a shader for this batch. The Xbox 360 D3D runtime REJECTS any draw call
    // with no vertex shader set ("ERR[D3D]: A vertex shader must be set"). Pick
    // textured vs untextured by whether texcoords were supplied; useTexturedShader/
    // useUntexturedShader also set the vertex declaration. The caller has already
    // bound the texture and the transform matrices; updateShaderConstants() uploads
    // the current WVP so the batch draws with the right transform.
    if (m_hasTexture) {
        Shaders::useTexturedShader();
    } else {
        Shaders::useUntexturedShader();
    }
    Shaders::updateShaderConstants();

    // Convert quads to triangles with reversed winding for D3D CW culling
    int quadCount = m_vertexCount / 4;
    int triVertCount = quadCount * 6;
    TesselatorVertex* triVerts = (TesselatorVertex*)alloca(triVertCount * sizeof(TesselatorVertex));

    int out = 0;
    for (int q = 0; q < quadCount; ++q) {
        int base = q * 4;
        // Triangle 1: 0, 2, 1 (reversed winding)
        triVerts[out++] = m_vertices[base + 0];
        triVerts[out++] = m_vertices[base + 2];
        triVerts[out++] = m_vertices[base + 1];
        // Triangle 2: 0, 3, 2
        triVerts[out++] = m_vertices[base + 0];
        triVerts[out++] = m_vertices[base + 3];
        triVerts[out++] = m_vertices[base + 2];
    }

    // Leftover triangles (if vertex count not multiple of 4)
    int leftover = m_vertexCount - (quadCount * 4);
    if (leftover == 3) {
        triVerts[out++] = m_vertices[m_vertexCount - 3];
        triVerts[out++] = m_vertices[m_vertexCount - 1];
        triVerts[out++] = m_vertices[m_vertexCount - 2];
    }

    if (out > 0) {
        g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, out / 3,
            triVerts, sizeof(TesselatorVertex));
    }

    m_vertexCount = 0;
    m_addedVertices = 0;
}

void Tesselator::tex(float u, float v) { m_u = u; m_v = v; m_hasTexture = true; }

void Tesselator::color3f(float r, float g, float b) {
    if (m_colorDisabled) return;
    int ri = (int)(r * 255.0f); if (ri > 255) ri = 255; if (ri < 0) ri = 0;
    int gi = (int)(g * 255.0f); if (gi > 255) gi = 255; if (gi < 0) gi = 0;
    int bi = (int)(b * 255.0f); if (bi > 255) bi = 255; if (bi < 0) bi = 0;
    m_color = D3DCOLOR_ARGB(255, ri, gi, bi);
    m_hasColor = true;
}

void Tesselator::color4f(float r, float g, float b, float a) {
    if (m_colorDisabled) return;
    int ri = (int)(r * 255.0f); if (ri > 255) ri = 255; if (ri < 0) ri = 0;
    int gi = (int)(g * 255.0f); if (gi > 255) gi = 255; if (gi < 0) gi = 0;
    int bi = (int)(b * 255.0f); if (bi > 255) bi = 255; if (bi < 0) bi = 0;
    int ai = (int)(a * 255.0f); if (ai > 255) ai = 255; if (ai < 0) ai = 0;
    m_color = D3DCOLOR_ARGB(ai, ri, gi, bi);
    m_hasColor = true;
}

void Tesselator::color3b(int r, int g, int b) { m_color = D3DCOLOR_ARGB(255, r&0xFF, g&0xFF, b&0xFF); m_hasColor = true; }
void Tesselator::color4b(int r, int g, int b, int a) { m_color = D3DCOLOR_ARGB(a&0xFF, r&0xFF, g&0xFF, b&0xFF); m_hasColor = true; }
void Tesselator::colorI(int rgb) { m_color = D3DCOLOR_ARGB(255, (rgb>>16)&0xFF, (rgb>>8)&0xFF, rgb&0xFF); m_hasColor = true; }
void Tesselator::normal(float x, float y, float z) { /* normals stored via shader lighting if needed */ }
void Tesselator::disableColor() { m_colorDisabled = true; }

void Tesselator::vertex(float x, float y, float z) {
    if (m_vertexCount >= MAX_VERTICES) { flush(); }
    TesselatorVertex& v = m_vertices[m_vertexCount++];
    v.x = x; v.y = y; v.z = z;
    v.color = m_color;
    v.u = m_u; v.v = m_v;
    ++m_addedVertices;
}

void Tesselator::vertexUV(float x, float y, float z, float u, float v) {
    m_u = u; m_v = v; m_hasTexture = true;
    vertex(x, y, z);
}
