#include "stdafx.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
// Hand-written PORT of ModelRenderer.java (Minecraft Alpha v1.2.6)
// ------------------------------------------------------------------------
// Target: Xbox 360 / C++03 / DirectX 9
//
// ModelRenderer builds one textured box (8 corners + 6 TexturedQuad faces) from
// addBox(), positions it via setPosition(), and draws it with render()/func_926_b().
//
// This replaces the converter-generated ModelRenderer.cpp (~33 errors). The
// converter emitted `new PositionTexureVertex(...)` (pointers) into the value
// array `m_corners`/`m_faces` declared in ModelRenderer.h, and called
// func_808_a(Tesselator,float) by value -- both type-mismatches. It is rebuilt
// here to compile against the EXACT Alpha ModelRenderer.h interface, modeled on
// the working indev ModelRenderer override and adapted to Alpha's field/method
// names (m_corners / m_faces / func_921_a / func_926_b / func_808_a).
//
// Key Alpha-specific facts honored:
//  * PositionTexureVertex(float,float,float,float,float) is a BROKEN converter
//    stub (leaves m_vector3D == NULL), so corners are built through the
//    Vec3D* ctor: PositionTexureVertex(Vec3D::createVectorHelper(x,y,z), u, v) --
//    exactly what the original Java this(...) delegation did.
//  * Alpha's TexturedQuad(PositionTexureVertex* var1, ...) TAKES OWNERSHIP of the
//    4-vertex array (its dtor does delete[] m_field_1195_a). So every face gets
//    its OWN heap-allocated PositionTexureVertex[4] -- sharing m_corners across
//    faces would double-free.
//  * TexturedQuad::func_808_a takes a Tesselator* (pointer), not a value.
//
// PORT (render): the original Java compiled geometry ONCE into a GL display list
// (this.compiled gate) and replayed glCallList each frame. There is no persistent
// per-part vertex buffer here, so -- like the indev override -- we draw
// IMMEDIATE-MODE every frame: apply this part's translate + Z/Y/X rotations to the
// world matrix, then tessellate+flush all faces via func_808_a at that transform.

// NOTE on include order: Alpha's TexturedQuad.h declares
//   func_808_a(Tesselator* var1, float var2)  but forward-declares the type as
//   `class Tessellator;` (double-l typo) -- so `Tesselator` is an unknown
//   identifier when TexturedQuad.h is parsed alone, and the 2-arg overload won't
//   match. We must make `Tesselator` a known type BEFORE ModelRenderer.h pulls in
//   TexturedQuad.h. So Tesselator.h / PositionTexureVertex.h come first.
#include "Common.h"
#include "Tesselator.h"
#include "PositionTexureVertex.h"
#include "TexturedQuad.h"
#include "Vec3D.h"
#include "Shaders.h"
#include "ModelRenderer.h"

ModelRenderer::ModelRenderer()
	: m_offsetX(0)
	, m_offsetY(0)
	, m_offsetZ(0)
	, m_rotateAngleX(0)
	, m_rotateAngleY(0)
	, m_rotateAngleZ(0)
	, m_mirror(false)
	, m_field_1403_h(true)
	, m_field_1402_i(false)
	, m_corners(NULL)
	, m_faces(NULL)
	, m_textureOffsetX(0)
	, m_textureOffsetY(0)
	, m_compiled(false)
	, m_displayList(0)
	, m_corners_count(0)
	, m_faces_count(0)
{
}

ModelRenderer::ModelRenderer(int var1, int var2)
	: m_offsetX(0)
	, m_offsetY(0)
	, m_offsetZ(0)
	, m_rotateAngleX(0)
	, m_rotateAngleY(0)
	, m_rotateAngleZ(0)
	, m_mirror(false)
	, m_field_1403_h(true)
	, m_field_1402_i(false)
	, m_corners(NULL)
	, m_faces(NULL)
	, m_textureOffsetX(0)
	, m_textureOffsetY(0)
	, m_compiled(false)
	, m_displayList(0)
	, m_corners_count(0)
	, m_faces_count(0)
{
	m_textureOffsetX = var1;
	m_textureOffsetY = var2;
}

ModelRenderer::~ModelRenderer() {
	// m_corners is the box's own 8-vertex array. The 6 faces each OWN a separate
	// 4-vertex copy (delete[]'d by ~TexturedQuad), so deleting m_faces frees those.
	delete[] m_corners;
	m_corners = NULL;
	delete[] m_faces;
	m_faces = NULL;
}

void ModelRenderer::func_921_a(float var1, float var2, float var3, int var4, int var5, int var6) {
	addBox(var1, var2, var3, var4, var5, var6, 0.0F);
}

void ModelRenderer::addBox(float var1, float var2, float var3, int var4, int var5, int var6, float var7) {
	// Free any prior box (addBox may be re-invoked) to avoid leaks.
	delete[] m_corners;
	m_corners = NULL;
	delete[] m_faces;
	m_faces = NULL;

	m_corners = new PositionTexureVertex[8];
	m_corners_count = 8;
	m_faces = new TexturedQuad[6];
	m_faces_count = 6;

	float var8 = var1 + (float)var4;
	float var9 = var2 + (float)var5;
	float var10 = var3 + (float)var6;
	var1 -= var7;
	var2 -= var7;
	var3 -= var7;
	var8 += var7;
	var9 += var7;
	var10 += var7;
	if(m_mirror) {
		float var11 = var8;
		var8 = var1;
		var1 = var11;
	}

	// Build the 8 corner vertices. PORT: Alpha's 5-float PositionTexureVertex ctor
	// is a stub that never sets m_vector3D -- construct through the Vec3D* ctor so
	// each corner has a valid position (matches the Java this(createVectorHelper..)).
	PositionTexureVertex var20(Vec3D::createVectorHelper((double)var1, (double)var2, (double)var3),  0.0F, 0.0F);
	PositionTexureVertex var12(Vec3D::createVectorHelper((double)var8, (double)var2, (double)var3),  0.0F, 8.0F);
	PositionTexureVertex var13(Vec3D::createVectorHelper((double)var8, (double)var9, (double)var3),  8.0F, 8.0F);
	PositionTexureVertex var14(Vec3D::createVectorHelper((double)var1, (double)var9, (double)var3),  8.0F, 0.0F);
	PositionTexureVertex var15(Vec3D::createVectorHelper((double)var1, (double)var2, (double)var10), 0.0F, 0.0F);
	PositionTexureVertex var16(Vec3D::createVectorHelper((double)var8, (double)var2, (double)var10), 0.0F, 8.0F);
	PositionTexureVertex var17(Vec3D::createVectorHelper((double)var8, (double)var9, (double)var10), 8.0F, 8.0F);
	PositionTexureVertex var18(Vec3D::createVectorHelper((double)var1, (double)var9, (double)var10), 8.0F, 0.0F);

	m_corners[0] = var20;
	m_corners[1] = var12;
	m_corners[2] = var13;
	m_corners[3] = var14;
	m_corners[4] = var15;
	m_corners[5] = var16;
	m_corners[6] = var17;
	m_corners[7] = var18;

	// Each face owns its own heap 4-vertex array (TexturedQuad ctor takes ownership
	// and its dtor delete[]'s it). Build a fresh new[4] per face -- never share
	// m_corners, which would double-free.
	{
		PositionTexureVertex* f = new PositionTexureVertex[4];
		f[0] = var16; f[1] = var12; f[2] = var13; f[3] = var17;
		m_faces[0] = TexturedQuad(f, m_textureOffsetX + var6 + var4, m_textureOffsetY + var6, m_textureOffsetX + var6 + var4 + var6, m_textureOffsetY + var6 + var5);
	}
	{
		PositionTexureVertex* f = new PositionTexureVertex[4];
		f[0] = var20; f[1] = var15; f[2] = var18; f[3] = var14;
		m_faces[1] = TexturedQuad(f, m_textureOffsetX + 0, m_textureOffsetY + var6, m_textureOffsetX + var6, m_textureOffsetY + var6 + var5);
	}
	{
		PositionTexureVertex* f = new PositionTexureVertex[4];
		f[0] = var16; f[1] = var15; f[2] = var20; f[3] = var12;
		m_faces[2] = TexturedQuad(f, m_textureOffsetX + var6, m_textureOffsetY + 0, m_textureOffsetX + var6 + var4, m_textureOffsetY + var6);
	}
	{
		PositionTexureVertex* f = new PositionTexureVertex[4];
		f[0] = var13; f[1] = var14; f[2] = var18; f[3] = var17;
		m_faces[3] = TexturedQuad(f, m_textureOffsetX + var6 + var4, m_textureOffsetY + 0, m_textureOffsetX + var6 + var4 + var4, m_textureOffsetY + var6);
	}
	{
		PositionTexureVertex* f = new PositionTexureVertex[4];
		f[0] = var12; f[1] = var20; f[2] = var14; f[3] = var13;
		m_faces[4] = TexturedQuad(f, m_textureOffsetX + var6, m_textureOffsetY + var6, m_textureOffsetX + var6 + var4, m_textureOffsetY + var6 + var5);
	}
	{
		PositionTexureVertex* f = new PositionTexureVertex[4];
		f[0] = var15; f[1] = var16; f[2] = var17; f[3] = var18;
		m_faces[5] = TexturedQuad(f, m_textureOffsetX + var6 + var4 + var6, m_textureOffsetY + var6, m_textureOffsetX + var6 + var4 + var6 + var4, m_textureOffsetY + var6 + var5);
	}

	if(m_mirror) {
		for(int var19 = 0; var19 < 6; ++var19) {
			m_faces[var19].func_809_a();
		}
	}
}

void ModelRenderer::setPosition(float var1, float var2, float var3) {
	m_offsetX = var1;
	m_offsetY = var2;
	m_offsetZ = var3;
}

// Apply this part's local transform (translate to offset, then Z/Y/X rotations) to
// the world matrix, draw all faces immediate-mode, then restore. PRE-multiply for
// the row-vector / transposed-WVP pipeline (operand order flipped vs the converter's
// post-multiply -- see project memory "post-vs-pre-multiply-bug").
void ModelRenderer::render(float var1) {
	if(m_field_1402_i) {
		return;
	}
	if(!m_field_1403_h) {
		return;
	}
	if(!m_compiled) {
		compileDisplayList(var1);
	}

	D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();

	if(m_offsetX != 0.0F || m_offsetY != 0.0F || m_offsetZ != 0.0F) {
		D3DXMATRIX _t; D3DXMatrixTranslation(&_t, this->m_offsetX * var1, this->m_offsetY * var1, this->m_offsetZ * var1);
		D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_t, &Shaders::getWorldMatrix());
		Shaders::setWorldMatrix(_w);
	}
	// rotateAngle* are radians; pass directly to D3DXMatrixRotationAxis.
	if(m_rotateAngleZ != 0.0F) {
		D3DXVECTOR3 _axis(0.0F, 0.0F, 1.0F);
		D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &_axis, this->m_rotateAngleZ);
		D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_r, &Shaders::getWorldMatrix());
		Shaders::setWorldMatrix(_w);
	}
	if(m_rotateAngleY != 0.0F) {
		D3DXVECTOR3 _axis(0.0F, 1.0F, 0.0F);
		D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &_axis, this->m_rotateAngleY);
		D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_r, &Shaders::getWorldMatrix());
		Shaders::setWorldMatrix(_w);
	}
	if(m_rotateAngleX != 0.0F) {
		D3DXVECTOR3 _axis(1.0F, 0.0F, 0.0F);
		D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &_axis, this->m_rotateAngleX);
		D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_r, &Shaders::getWorldMatrix());
		Shaders::setWorldMatrix(_w);
	}
	Shaders::updateShaderConstants();

	Tesselator& t = Tesselator::getInstance();
	for(int var3 = 0; var3 < m_faces_count; ++var3) {
		m_faces[var3].func_808_a(&t, var1);
	}

	Shaders::setWorldMatrix(_savedMatrix);
	Shaders::updateShaderConstants();
}

// func_926_b: like render() but the caller has already pushed a parent matrix; this
// variant only adds the offset translate when non-zero (matches the Java branch that
// skipped glTranslate at the origin) before applying rotations. Same immediate-mode
// draw + restore.
void ModelRenderer::func_926_b(float var1) {
	if(m_field_1402_i) {
		return;
	}
	if(!m_field_1403_h) {
		return;
	}
	if(!m_compiled) {
		compileDisplayList(var1);
	}

	D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();

	if(m_offsetX != 0.0F || m_offsetY != 0.0F || m_offsetZ != 0.0F) {
		D3DXMATRIX _t; D3DXMatrixTranslation(&_t, this->m_offsetX * var1, this->m_offsetY * var1, this->m_offsetZ * var1);
		D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_t, &Shaders::getWorldMatrix());
		Shaders::setWorldMatrix(_w);
	}
	if(m_rotateAngleZ != 0.0F) {
		D3DXVECTOR3 _axis(0.0F, 0.0F, 1.0F);
		D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &_axis, this->m_rotateAngleZ);
		D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_r, &Shaders::getWorldMatrix());
		Shaders::setWorldMatrix(_w);
	}
	if(m_rotateAngleY != 0.0F) {
		D3DXVECTOR3 _axis(0.0F, 1.0F, 0.0F);
		D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &_axis, this->m_rotateAngleY);
		D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_r, &Shaders::getWorldMatrix());
		Shaders::setWorldMatrix(_w);
	}
	if(m_rotateAngleX != 0.0F) {
		D3DXVECTOR3 _axis(1.0F, 0.0F, 0.0F);
		D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &_axis, this->m_rotateAngleX);
		D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_r, &Shaders::getWorldMatrix());
		Shaders::setWorldMatrix(_w);
	}
	Shaders::updateShaderConstants();

	Tesselator& t = Tesselator::getInstance();
	for(int var3 = 0; var3 < m_faces_count; ++var3) {
		m_faces[var3].func_808_a(&t, var1);
	}

	Shaders::setWorldMatrix(_savedMatrix);
	Shaders::updateShaderConstants();
}

// PORT: the Java compiled a GL display list here. We have no persistent per-part
// vertex buffer (faces are tessellated immediate-mode every frame in render/
// func_926_b), so this just flips the compiled flag so the gate fires once.
void ModelRenderer::compileDisplayList(float var1) {
	(void)var1;
	m_compiled = true;
}
