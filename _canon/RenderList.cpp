#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from RenderList.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderList.h"
#include "ByteBuffer.h"
#include "FloatBuffer.h"
#include "GL11.h"
#include "GLAllocation.h"
#include "IntBuffer.h"

RenderList::RenderList()
	: posX(0)
	, posY(0)
	, posZ(0)
	, cameraX(0)
	, cameraY(0)
	, cameraZ(0)
	, intBuffer(NULL)
	, isInitialized(0)
	, isBufferFlipped(0)
{
	this->intBuffer = GLAllocation::createDirectIntBuffer(65536);
	this->isInitialized = false;
	this->isBufferFlipped = false;
}

RenderList::~RenderList()
{
}

void RenderList::setupRenderList(int var1, int var2, int var3, double var4, double var6, double var8)
{
	this->isInitialized = true;
	this->intBuffer->clear();
	this->posX = var1;
	this->posY = var2;
	this->posZ = var3;
	this->cameraX = (float)var4;
	this->cameraY = (float)var6;
	this->cameraZ = (float)var8;
}

bool RenderList::isPositionMatching(int var1, int var2, int var3)
{
	return !this->isInitialized ? false : var1 == this->posX && var2 == this->posY && var3 == this->posZ;
}

void RenderList::addGLRenderList(int var1)
{
	this->intBuffer->put(var1);
	if(this->intBuffer->remaining() == 0) {
	this->callLists();
	}
}

void RenderList::callLists()
{
	if(this->isInitialized) {
	if(!this->isBufferFlipped) {
	this->intBuffer->flip();
	this->isBufferFlipped = true;
	}
	if(this->intBuffer->remaining() > 0) {
	GL11::glPushMatrix();
	GL11::glTranslatef((float)this->posX - this->cameraX, (float)this->posY - this->cameraY, (float)this->posZ - this->cameraZ);
	GL11::glCallLists(this->intBuffer);
	GL11::glPopMatrix();
	}
	}
}

void RenderList::resetList()
{
	this->isInitialized = false;
	this->isBufferFlipped = false;
}

void RenderList::initStatics()
{
}
