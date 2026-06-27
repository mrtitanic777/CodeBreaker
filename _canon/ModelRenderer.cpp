#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from ModelRenderer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ModelRenderer.h"
#include "ByteBuffer.h"
#include "FloatBuffer.h"
#include "GL11.h"
#include "GLAllocation.h"
#include "IntBuffer.h"
#include "PositionTexureVertex.h"
#include "Tessellator.h"
#include "TexturedQuad.h"
#include "Vec3D.h"

ModelRenderer::ModelRenderer(int var1, int var2)
	: textureOffsetX(0)
	, textureOffsetY(0)
	, offsetX(0)
	, offsetY(0)
	, offsetZ(0)
	, rotateAngleX(0)
	, rotateAngleY(0)
	, rotateAngleZ(0)
	, compiled(0)
	, displayList(0)
	, mirror(0)
	, showModel(0)
	, isHidden(0)
{
	this->compiled = false;
	this->displayList = 0;
	this->mirror = false;
	this->showModel = true;
	this->isHidden = false;
	this->textureOffsetX = var1;
	this->textureOffsetY = var2;
}

ModelRenderer::~ModelRenderer()
{
}

void ModelRenderer::addBox(float var1, float var2, float var3, int var4, int var5, int var6)
{
	this->addBox(var1, var2, var3, var4, var5, var6, 0.0F);
}

void ModelRenderer::addBox(float var1, float var2, float var3, int var4, int var5, int var6, float var7)
{
	this->corners = JArray<PositionTexureVertex* >(8);
	this->faces = JArray<TexturedQuad* >(6);
	float var8 = var1 + (float)var4;
	float var9 = var2 + (float)var5;
	float var10 = var3 + (float)var6;
	var1 -= var7;
	var2 -= var7;
	var3 -= var7;
	var8 += var7;
	var9 += var7;
	var10 += var7;
	if(this->mirror) {
	float var11 = var8;
	var8 = var1;
	var1 = var11;
	}
	PositionTexureVertex* var20 = new PositionTexureVertex(var1, var2, var3, 0.0F, 0.0F);
	PositionTexureVertex* var12 = new PositionTexureVertex(var8, var2, var3, 0.0F, 8.0F);
	PositionTexureVertex* var13 = new PositionTexureVertex(var8, var9, var3, 8.0F, 8.0F);
	PositionTexureVertex* var14 = new PositionTexureVertex(var1, var9, var3, 8.0F, 0.0F);
	PositionTexureVertex* var15 = new PositionTexureVertex(var1, var2, var10, 0.0F, 0.0F);
	PositionTexureVertex* var16 = new PositionTexureVertex(var8, var2, var10, 0.0F, 8.0F);
	PositionTexureVertex* var17 = new PositionTexureVertex(var8, var9, var10, 8.0F, 8.0F);
	PositionTexureVertex* var18 = new PositionTexureVertex(var1, var9, var10, 8.0F, 0.0F);
	this->corners[0] = var20;
	this->corners[1] = var12;
	this->corners[2] = var13;
	this->corners[3] = var14;
	this->corners[4] = var15;
	this->corners[5] = var16;
	this->corners[6] = var17;
	this->corners[7] = var18;
	this->faces[0] = new TexturedQuad((JArrayInit<PositionTexureVertex* >(), var16, var12, var13, var17), this->textureOffsetX + var6 + var4, this->textureOffsetY + var6, this->textureOffsetX + var6 + var4 + var6, this->textureOffsetY + var6 + var5);
	this->faces[1] = new TexturedQuad((JArrayInit<PositionTexureVertex* >(), var20, var15, var18, var14), this->textureOffsetX + 0, this->textureOffsetY + var6, this->textureOffsetX + var6, this->textureOffsetY + var6 + var5);
	this->faces[2] = new TexturedQuad((JArrayInit<PositionTexureVertex* >(), var16, var15, var20, var12), this->textureOffsetX + var6, this->textureOffsetY + 0, this->textureOffsetX + var6 + var4, this->textureOffsetY + var6);
	this->faces[3] = new TexturedQuad((JArrayInit<PositionTexureVertex* >(), var13, var14, var18, var17), this->textureOffsetX + var6 + var4, this->textureOffsetY + 0, this->textureOffsetX + var6 + var4 + var4, this->textureOffsetY + var6);
	this->faces[4] = new TexturedQuad((JArrayInit<PositionTexureVertex* >(), var12, var20, var14, var13), this->textureOffsetX + var6, this->textureOffsetY + var6, this->textureOffsetX + var6 + var4, this->textureOffsetY + var6 + var5);
	this->faces[5] = new TexturedQuad((JArrayInit<PositionTexureVertex* >(), var15, var16, var17, var18), this->textureOffsetX + var6 + var4 + var6, this->textureOffsetY + var6, this->textureOffsetX + var6 + var4 + var6 + var4, this->textureOffsetY + var6 + var5);
	if(this->mirror) {
	for(int var19 = 0; var19 < this->faces.length; ++var19) {
	if (this->faces[var19] != NULL) { this->faces[var19]->flipFace(); }
	}
	}
}

void ModelRenderer::setPosition(float var1, float var2, float var3)
{
	this->offsetX = var1;
	this->offsetY = var2;
	this->offsetZ = var3;
}

void ModelRenderer::render(float var1)
{
	if(!this->isHidden) {
	if(this->showModel) {
	if(!this->compiled) {
	this->compileDisplayList(var1);
	}
	if(this->rotateAngleX == 0.0F && this->rotateAngleY == 0.0F && this->rotateAngleZ == 0.0F) {
	if(this->offsetX == 0.0F && this->offsetY == 0.0F && this->offsetZ == 0.0F) {
	GL11::glCallList(this->displayList);
	} else {
	GL11::glTranslatef(this->offsetX * var1, this->offsetY * var1, this->offsetZ * var1);
	GL11::glCallList(this->displayList);
	GL11::glTranslatef(-this->offsetX * var1, -this->offsetY * var1, -this->offsetZ * var1);
	}
	} else {
	GL11::glPushMatrix();
	GL11::glTranslatef(this->offsetX * var1, this->offsetY * var1, this->offsetZ * var1);
	if(this->rotateAngleZ != 0.0F) {
	GL11::glRotatef(this->rotateAngleZ * (180.0F / (float)3.1415927F), 0.0F, 0.0F, 1.0F);
	}
	if(this->rotateAngleY != 0.0F) {
	GL11::glRotatef(this->rotateAngleY * (180.0F / (float)3.1415927F), 0.0F, 1.0F, 0.0F);
	}
	if(this->rotateAngleX != 0.0F) {
	GL11::glRotatef(this->rotateAngleX * (180.0F / (float)3.1415927F), 1.0F, 0.0F, 0.0F);
	}
	GL11::glCallList(this->displayList);
	GL11::glPopMatrix();
	}
	}
	}
}

void ModelRenderer::postRender(float var1)
{
	if(!this->isHidden) {
	if(this->showModel) {
	if(!this->compiled) {
	this->compileDisplayList(var1);
	}
	if(this->rotateAngleX == 0.0F && this->rotateAngleY == 0.0F && this->rotateAngleZ == 0.0F) {
	if(this->offsetX != 0.0F || this->offsetY != 0.0F || this->offsetZ != 0.0F) {
	GL11::glTranslatef(this->offsetX * var1, this->offsetY * var1, this->offsetZ * var1);
	}
	} else {
	GL11::glTranslatef(this->offsetX * var1, this->offsetY * var1, this->offsetZ * var1);
	if(this->rotateAngleZ != 0.0F) {
	GL11::glRotatef(this->rotateAngleZ * (180.0F / (float)3.1415927F), 0.0F, 0.0F, 1.0F);
	}
	if(this->rotateAngleY != 0.0F) {
	GL11::glRotatef(this->rotateAngleY * (180.0F / (float)3.1415927F), 0.0F, 1.0F, 0.0F);
	}
	if(this->rotateAngleX != 0.0F) {
	GL11::glRotatef(this->rotateAngleX * (180.0F / (float)3.1415927F), 1.0F, 0.0F, 0.0F);
	}
	}
	}
	}
}

void ModelRenderer::compileDisplayList(float var1)
{
	this->displayList = GLAllocation::generateDisplayLists(1);
	GL11::glNewList(this->displayList, GL11::GL_COMPILE);
	Tessellator* var2 = Tessellator::s_instance;
	for(int var3 = 0; var3 < this->faces.length; ++var3) {
	if (this->faces[var3] != NULL) { this->faces[var3]->draw(var2, var1); }
	}
	GL11::glEndList();
	this->compiled = true;
}

void ModelRenderer::initStatics()
{
}
