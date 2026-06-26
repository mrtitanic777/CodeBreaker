#include "stdafx.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
// Auto-generated from ModelRenderer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ModelRenderer.h"
#include "Common.h"
#include "Shaders.h"
#include "Tesselator.h"
#include "Vec3.h"

ModelRenderer::ModelRenderer()
	: m_corners(NULL)
	, m_faces(NULL)
	, m_textureOffsetX(0)
	, m_textureOffsetY(0)
	, m_rotationPointX(0)
	, m_rotationPointY(0)
	, m_rotationPointZ(0)
	, m_rotateAngleX(0)
	, m_rotateAngleY(0)
	, m_rotateAngleZ(0)
	, m_compiled(false)
	, m_displayList(0)
	, m_mirror(false)
	, m_showModel(true)
	, m_isHidden(false)
{
}

ModelRenderer::ModelRenderer(int var1, int var2)
	: m_corners(NULL)
	, m_faces(NULL)
	, m_textureOffsetX(0)
	, m_textureOffsetY(0)
	, m_rotationPointX(0)
	, m_rotationPointY(0)
	, m_rotationPointZ(0)
	, m_rotateAngleX(0)
	, m_rotateAngleY(0)
	, m_rotateAngleZ(0)
	, m_compiled(false)
	, m_displayList(0)
	, m_mirror(false)
	, m_showModel(true)
	, m_isHidden(false)
{
		m_textureOffsetX = var1;
		m_textureOffsetY = var2;
}

ModelRenderer::~ModelRenderer() {
	delete[] m_corners;
	m_corners = NULL;
	delete[] m_faces;
	m_faces = NULL;
}

void ModelRenderer::addBox(float var1, float var2, float var3, int var4, int var5, int var6, float var7) {
		m_corners = new PositionTextureVertex[8];
		m_faces = new TexturedQuad[6];
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
			var7 = var8;
			var8 = var1;
			var1 = var7;
		}

		PositionTextureVertex var20 = PositionTextureVertex(var1, var2, var3, 0.0F, 0.0F);
		PositionTextureVertex var11 = PositionTextureVertex(var8, var2, var3, 0.0F, 8.0F);
		PositionTextureVertex var12 = PositionTextureVertex(var8, var9, var3, 8.0F, 8.0F);
		PositionTextureVertex var18 = PositionTextureVertex(var1, var9, var3, 8.0F, 0.0F);
		PositionTextureVertex var13 = PositionTextureVertex(var1, var2, var10, 0.0F, 0.0F);
		PositionTextureVertex var15 = PositionTextureVertex(var8, var2, var10, 0.0F, 8.0F);
		PositionTextureVertex var21 = PositionTextureVertex(var8, var9, var10, 8.0F, 8.0F);
		PositionTextureVertex var14 = PositionTextureVertex(var1, var9, var10, 8.0F, 0.0F);
		m_corners[0] = var20;
		m_corners[1] = var11;
		m_corners[2] = var12;
		m_corners[3] = var18;
		m_corners[4] = var13;
		m_corners[5] = var15;
		m_corners[6] = var21;
		m_corners[7] = var14;
  PositionTextureVertex _tmpVerts1[4] = {var15, var11, var12, var21};
		m_faces[0] = TexturedQuad(_tmpVerts1, m_textureOffsetX + var6 + var4, m_textureOffsetY + var6, m_textureOffsetX + var6 + var4 + var6, m_textureOffsetY + var6 + var5);
  PositionTextureVertex _tmpVerts2[4] = {var20, var13, var14, var18};
		m_faces[1] = TexturedQuad(_tmpVerts2, m_textureOffsetX, m_textureOffsetY + var6, m_textureOffsetX + var6, m_textureOffsetY + var6 + var5);
  PositionTextureVertex _tmpVerts3[4] = {var15, var13, var20, var11};
		m_faces[2] = TexturedQuad(_tmpVerts3, m_textureOffsetX + var6, m_textureOffsetY, m_textureOffsetX + var6 + var4, m_textureOffsetY + var6);
  PositionTextureVertex _tmpVerts4[4] = {var12, var18, var14, var21};
		m_faces[3] = TexturedQuad(_tmpVerts4, m_textureOffsetX + var6 + var4, m_textureOffsetY, m_textureOffsetX + var6 + var4 + var4, m_textureOffsetY + var6);
  PositionTextureVertex _tmpVerts5[4] = {var11, var20, var18, var12};
		m_faces[4] = TexturedQuad(_tmpVerts5, m_textureOffsetX + var6, m_textureOffsetY + var6, m_textureOffsetX + var6 + var4, m_textureOffsetY + var6 + var5);
  PositionTextureVertex _tmpVerts6[4] = {var13, var15, var21, var14};
		m_faces[5] = TexturedQuad(_tmpVerts6, m_textureOffsetX + var6 + var4 + var6, m_textureOffsetY + var6, m_textureOffsetX + var6 + var4 + var6 + var4, m_textureOffsetY + var6 + var5);
		if(m_mirror) {
			for(int var16 = 0; var16 < 6; ++var16) {
				TexturedQuad var17 = m_faces[var16];
				PositionTextureVertex* var19 = new PositionTextureVertex[var17.m_vertexPositions /* PORT: array length - use tracked size */];

				for(var4 = 0; var4 < var17.m_vertexPositions /* PORT: array length - use tracked size */; ++var4) {
					var19[var4] = var17.m_vertexPositions[var17.m_vertexPositions /* PORT: array length - use tracked size */ - var4 - 1];
				}

				var17.m_vertexPositions = var19;
			}
		}

}

void ModelRenderer::setRotationPoint(float var1, float var2, float var3) {
		m_rotationPointX = var1;
		m_rotationPointY = var2;
		m_rotationPointZ = var3;
}

void ModelRenderer::render(float var1) {
		if(m_showModel) {
			if(!m_compiled) {
				float var3 = var1;
				ModelRenderer var2 = this;
    // PORT: allocate 1 vertex buffer slots
    // BEGIN display list -> vertex buffer recording
				Tesselator var4 = Tesselator::getInstance();

				for(int var5 = 0; var5 < var2.m_faces /* PORT: array length - use tracked size */; ++var5) {
					var4.init();
					TexturedQuad var10000 = var2.m_faces[var5];
					float var8 = var3;
					Tesselator var7 = var4;
					TexturedQuad var6 = var10000;
					Vec3D var9 = var6.m_vertexPositions[1].vector3.0f.subtract(var6.m_vertexPositions[0].vector3.0f).normalize();
					Vec3D var10 = var6.m_vertexPositions[1].vector3.0f.subtract(var6.m_vertexPositions[2].vector3.0f).normalize();
					var9 = (new Vec3D(var9.m_yCoord * var10.m_zCoord - var9.m_zCoord * var10.m_yCoord, var9.m_zCoord * var10.m_xCoord - var9.m_xCoord * var10.m_zCoord, var9.m_xCoord * var10.m_yCoord - var9.m_yCoord * var10.m_xCoord))->normalize();
					Tesselator::normal(-var9.m_xCoord, -var9.m_yCoord, -var9.m_zCoord);

					for(int var11 = 0; var11 < 4; ++var11) {
						PositionTextureVertex var12 = var6.m_vertexPositions[var11];
						var7.vertexUV(var12.vector3.0f.m_xCoord * var8, var12.vector3.0f.m_yCoord * var8, var12.vector3.0f.m_zCoord * var8, var12.m_texturePositionX, var12.m_texturePositionY);
					}

					var4.flush();
				}

    // END display list -> vertex buffer recording
				var2.m_compiled = true;
			}

			if(m_rotateAngleX == 0.0F && m_rotateAngleY == 0.0F && m_rotateAngleZ == 0.0F) {
				if(m_rotationPointX == 0.0F && m_rotationPointY == 0.0F && m_rotationPointZ == 0.0F) {
     // Draw cached vertex buffer for list this.displayList
				} else {
     { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, this->m_rotationPointX * var1, this->m_rotationPointY * var1, this->m_rotationPointZ * var1);
       D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
       Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
     // Draw cached vertex buffer for list this.displayList
     { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, -this->m_rotationPointX * var1, -this->m_rotationPointY * var1, -this->m_rotationPointZ * var1);
       D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
       Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
				}
			} else {
    D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
    { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, this->m_rotationPointX * var1, this->m_rotationPointY * var1, this->m_rotationPointZ * var1);
      D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
      Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
				if(m_rotateAngleZ != 0.0F) {
     { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,0.0F,1.0F), (this->m_rotateAngleZ * (180.0F / (float)M_PI))*DEG_TO_RAD);
       D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
       Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
				}

				if(m_rotateAngleY != 0.0F) {
     { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,1.0F,0.0F), (this->m_rotateAngleY * (180.0F / (float)M_PI))*DEG_TO_RAD);
       D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
       Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
				}

				if(m_rotateAngleX != 0.0F) {
     { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(1.0F,0.0F,0.0F), (this->m_rotateAngleX * (180.0F / (float)M_PI))*DEG_TO_RAD);
       D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
       Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
				}

    // Draw cached vertex buffer for list this.displayList
    Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
			}
		}
}
