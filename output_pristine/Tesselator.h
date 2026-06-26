#ifndef TESSELATOR_H
#define TESSELATOR_H

#include "stdafx.h"
#include "Shaders.h"

// Xbox 360 D3D9 Tesselator — replaces Java Tessellator (OpenGL immediate mode)
// Collects quads as triangle pairs into a dynamic vertex buffer and flushes with DrawPrimitiveUP.

class Tesselator {
public:
    static Tesselator& getInstance();

    void init();
    void init(int mode);   // GL primitive-mode variant (mode accepted, draws quads)
    void flush();

    void vertex(float x, float y, float z);
    void vertexUV(float x, float y, float z, float u, float v);
    void tex(float u, float v);
    void color3f(float r, float g, float b);
    void color4f(float r, float g, float b, float a);
    void color3b(int r, int g, int b);
    void color4b(int r, int g, int b, int a);
    void colorI(int rgb);
    void normal(float x, float y, float z);
    void disableColor();

    int getVertexCount() const { return m_vertexCount; }
    const TesselatorVertex* getVertices() const { return m_vertices; }

    static Tesselator* instance;

private:
    Tesselator();
    static const int MAX_VERTICES = 262144;
    TesselatorVertex m_vertices[MAX_VERTICES];
    int m_vertexCount;
    float m_u, m_v;
    DWORD m_color;
    bool m_hasTexture;
    bool m_hasColor;
    bool m_colorDisabled;
    int m_addedVertices;
};

#endif // TESSELATOR_H
