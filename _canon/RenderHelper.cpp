#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from RenderHelper.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderHelper.h"
#include "ByteBuffer.h"
#include "FloatBuffer.h"
#include "GL11.h"
#include "GLAllocation.h"
#include "IntBuffer.h"
#include "Vec3D.h"

FloatBuffer* RenderHelper::s_colorBuffer;

RenderHelper::~RenderHelper()
{
}

void RenderHelper::disableStandardItemLighting()
{
	GL11::glDisable(GL11::GL_LIGHTING);
	GL11::glDisable(GL11::GL_LIGHT0);
	GL11::glDisable(GL11::GL_LIGHT1);
	GL11::glDisable(GL11::GL_COLOR_MATERIAL);
}

void RenderHelper::enableStandardItemLighting()
{
	GL11::glEnable(GL11::GL_LIGHTING);
	GL11::glEnable(GL11::GL_LIGHT0);
	GL11::glEnable(GL11::GL_LIGHT1);
	GL11::glEnable(GL11::GL_COLOR_MATERIAL);
	GL11::glColorMaterial(GL11::GL_FRONT_AND_BACK, GL11::GL_AMBIENT_AND_DIFFUSE);
	float var0 = 0.4F;
	float var1 = 0.6F;
	float var2 = 0.0F;
	Vec3D* var3 = Vec3D::createVector((double)0.2F, 1.0, (double)-0.7F)->normalize();
	GL11::glLight(GL11::GL_LIGHT0, GL11::GL_POSITION, setColorBufferDouble(var3->xCoord, var3->yCoord, var3->zCoord, 0.0));
	GL11::glLight(GL11::GL_LIGHT0, GL11::GL_DIFFUSE, setColorBuffer(var1, var1, var1, 1.0F));
	GL11::glLight(GL11::GL_LIGHT0, GL11::GL_AMBIENT, setColorBuffer(0.0F, 0.0F, 0.0F, 1.0F));
	GL11::glLight(GL11::GL_LIGHT0, GL11::GL_SPECULAR, setColorBuffer(var2, var2, var2, 1.0F));
	var3 = Vec3D::createVector((double)-0.2F, 1.0, (double)0.7F)->normalize();
	GL11::glLight(GL11::GL_LIGHT1, GL11::GL_POSITION, setColorBufferDouble(var3->xCoord, var3->yCoord, var3->zCoord, 0.0));
	GL11::glLight(GL11::GL_LIGHT1, GL11::GL_DIFFUSE, setColorBuffer(var1, var1, var1, 1.0F));
	GL11::glLight(GL11::GL_LIGHT1, GL11::GL_AMBIENT, setColorBuffer(0.0F, 0.0F, 0.0F, 1.0F));
	GL11::glLight(GL11::GL_LIGHT1, GL11::GL_SPECULAR, setColorBuffer(var2, var2, var2, 1.0F));
	GL11::glShadeModel(GL11::GL_FLAT);
	GL11::glLightModel(GL11::GL_LIGHT_MODEL_AMBIENT, setColorBuffer(var0, var0, var0, 1.0F));
}

FloatBuffer* RenderHelper::setColorBufferDouble(double var0, double var2, double var4, double var6)
{
	return setColorBuffer((float)var0, (float)var2, (float)var4, (float)var6);
}

FloatBuffer* RenderHelper::setColorBuffer(float var0, float var1, float var2, float var3)
{
	s_colorBuffer->clear();
	s_colorBuffer->put(var0).put(var1).put(var2).put(var3);
	s_colorBuffer->flip();
	return s_colorBuffer;
}

void RenderHelper::initStatics()
{
	RenderHelper::s_colorBuffer = GLAllocation::createDirectFloatBuffer(16);
}
