#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from Tessellator.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Tessellator.h"
#include "ARBVertexBufferObject.h"
#include "ByteBuffer.h"
#include "FloatBuffer.h"
#include "GL11.h"
#include "GL15.h"
#include "GLAllocation.h"
#include "GLContext.h"
#include "IllegalStateException.h"
#include "IntBuffer.h"

/* ---- ORIGINAL JAVA (stubbed below) ----
 *  // GL11 enums/entry points used by draw(); glNormalPointer actually uploads the normal array.
 *  // VBO path:    GL11.glNormalPointer(GL11.GL_BYTE, 32, 24L);
 *  // non-VBO:     this.byteBuffer.position(24);
 *  //              GL11.glNormalPointer(32, (ByteBuffer)this.byteBuffer);
 *  //              GL11.glEnableClientState(GL11.GL_NORMAL_ARRAY);
 *  // and:         GL11.glDrawArrays(GL11.GL_TRIANGLES, GL11.GL_POINTS, this.vertexCount);
 *  ---- end original Java ---- */
// --- Local GL11 shims for symbols missing from the GL11 runtime stub ----------
// Standard OpenGL 1.1 normal-array entry points / enums the stub doesn't declare.
// _glNormalPointer is a NO-OP: normals are not uploaded (behavioural stub).
namespace {
	const int _GL_BYTE = 0x1400;          // GL_BYTE
	const int _GL_TRIANGLES = 0x0004;     // GL_TRIANGLES
	const int _GL_NORMAL_ARRAY = 0x8075;  // GL_NORMAL_ARRAY
	inline void _glNormalPointer(...) {}  // glNormalPointer (no-op; matches GL11 stubs)
}

bool Tessellator::s_convertQuadsToTriangles = true;
bool Tessellator::s_tryVBO = false;
Tessellator* Tessellator::s_instance = new Tessellator(2097152);

Tessellator::Tessellator(int var1)
	: byteBuffer(NULL)
	, intBuffer(NULL)
	, floatBuffer(NULL)
	, vertexCount(0)
	, textureU(0)
	, textureV(0)
	, color(0)
	, hasColor(0)
	, hasTexture(0)
	, hasNormals(0)
	, rawBufferIndex(0)
	, addedVertices(0)
	, isColorDisabled(0)
	, drawMode(0)
	, xOffset(0)
	, yOffset(0)
	, zOffset(0)
	, normal(0)
	, isDrawing(0)
	, useVBO(0)
	, vertexBuffers(NULL)
	, vboIndex(0)
	, vboCount(0)
	, bufferSize(0)
{
	this->vboCount = 10;

	this->bufferSize = var1;
	this->byteBuffer = GLAllocation::createDirectByteBuffer(var1 * 4);
	this->intBuffer = this->byteBuffer->asIntBuffer();
	this->floatBuffer = this->byteBuffer->asFloatBuffer();
	this->rawBuffer = JArray<int >(var1);
	this->useVBO = s_tryVBO && GLContext::getCapabilities().GL_ARB_vertex_buffer_object;
	if(this->useVBO) {
	this->vertexBuffers = GLAllocation::createDirectIntBuffer(this->vboCount);
	ARBVertexBufferObject::glGenBuffersARB(this->vertexBuffers);
	}
}

Tessellator::~Tessellator()
{
}

void Tessellator::draw()
{
	if(!this->isDrawing) {
	throw new IllegalStateException("Not tesselating!");
	} else {
	this->isDrawing = false;
	if(this->vertexCount > 0) {
	this->intBuffer->clear();
	this->intBuffer->put(this->rawBuffer, 0, this->rawBufferIndex);
	this->byteBuffer->position(0);
	this->byteBuffer->limit(this->rawBufferIndex * 4);
	if(this->useVBO) {
	this->vboIndex = (this->vboIndex + 1) % this->vboCount;
	ARBVertexBufferObject::glBindBufferARB(GL15::GL_ARRAY_BUFFER, this->vertexBuffers->get(this->vboIndex));
	ARBVertexBufferObject::glBufferDataARB(GL15::GL_ARRAY_BUFFER, this->byteBuffer, GL15::GL_STREAM_DRAW);
	}
	if(this->hasTexture) {
	if(this->useVBO) {
	GL11::glTexCoordPointer(2, GL11::GL_FLOAT, 32, 12LL);
	} else {
	this->floatBuffer->position(3);
	GL11::glTexCoordPointer(2, 32, (FloatBuffer*)this->floatBuffer);
	}
	GL11::glEnableClientState(GL11::GL_TEXTURE_COORD_ARRAY);
	}
	if(this->hasColor) {
	if(this->useVBO) {
	GL11::glColorPointer(4, GL11::GL_UNSIGNED_BYTE, 32, 20LL);
	} else {
	this->byteBuffer->position(20);
	GL11::glColorPointer(4, true, 32, this->byteBuffer);
	}
	GL11::glEnableClientState(GL11::GL_COLOR_ARRAY);
	}
	if(this->hasNormals) {
	if(this->useVBO) {
	/* ---- ORIGINAL JAVA (stubbed below) ----
	 *  GL11.glNormalPointer(GL11.GL_BYTE, 32, 24L);
	 *  ---- end original Java ---- */
	_glNormalPointer(_GL_BYTE, 32, 24LL);
	} else {
	this->byteBuffer->position(24);
	/* ---- ORIGINAL JAVA (stubbed below) ----
	 *  GL11.glNormalPointer(32, (ByteBuffer)this.byteBuffer);
	 *  ---- end original Java ---- */
	_glNormalPointer(32, (ByteBuffer*)this->byteBuffer);
	}
	GL11::glEnableClientState(_GL_NORMAL_ARRAY);
	}
	if(this->useVBO) {
	GL11::glVertexPointer(3, GL11::GL_FLOAT, 32, 0LL);
	} else {
	this->floatBuffer->position(0);
	GL11::glVertexPointer(3, 32, (FloatBuffer*)this->floatBuffer);
	}
	GL11::glEnableClientState(GL11::GL_VERTEX_ARRAY);
	if(this->drawMode == 7 && s_convertQuadsToTriangles) {
	GL11::glDrawArrays(_GL_TRIANGLES, GL11::GL_POINTS, this->vertexCount);
	} else {
	GL11::glDrawArrays(this->drawMode, GL11::GL_POINTS, this->vertexCount);
	}
	GL11::glDisableClientState(GL11::GL_VERTEX_ARRAY);
	if(this->hasTexture) {
	GL11::glDisableClientState(GL11::GL_TEXTURE_COORD_ARRAY);
	}
	if(this->hasColor) {
	GL11::glDisableClientState(GL11::GL_COLOR_ARRAY);
	}
	if(this->hasNormals) {
	GL11::glDisableClientState(_GL_NORMAL_ARRAY);
	}
	}
	this->reset();
	}
}

void Tessellator::reset()
{
	this->vertexCount = 0;
	this->byteBuffer->clear();
	this->rawBufferIndex = 0;
	this->addedVertices = 0;
}

void Tessellator::startDrawingQuads()
{
	this->startDrawing(7);
}

void Tessellator::startDrawing(int var1)
{
	if(this->isDrawing) {
	throw new IllegalStateException("Already tesselating!");
	} else {
	this->isDrawing = true;
	this->reset();
	this->drawMode = var1;
	this->hasNormals = false;
	this->hasColor = false;
	this->hasTexture = false;
	this->isColorDisabled = false;
	}
}

void Tessellator::setTextureUV(double var1, double var3)
{
	this->hasTexture = true;
	this->textureU = var1;
	this->textureV = var3;
}

void Tessellator::setColorOpaque_F(float var1, float var2, float var3)
{
	this->setColorOpaque((int)(var1 * 255.0F), (int)(var2 * 255.0F), (int)(var3 * 255.0F));
}

void Tessellator::setColorRGBA_F(float var1, float var2, float var3, float var4)
{
	this->setColorRGBA((int)(var1 * 255.0F), (int)(var2 * 255.0F), (int)(var3 * 255.0F), (int)(var4 * 255.0F));
}

void Tessellator::setColorOpaque(int var1, int var2, int var3)
{
	this->setColorRGBA(var1, var2, var3, 255);
}

void Tessellator::setColorRGBA(int var1, int var2, int var3, int var4)
{
	if(!this->isColorDisabled) {
	if(var1 > 255) {
	var1 = 255;
	}
	if(var2 > 255) {
	var2 = 255;
	}
	if(var3 > 255) {
	var3 = 255;
	}
	if(var4 > 255) {
	var4 = 255;
	}
	if(var1 < 0) {
	var1 = 0;
	}
	if(var2 < 0) {
	var2 = 0;
	}
	if(var3 < 0) {
	var3 = 0;
	}
	if(var4 < 0) {
	var4 = 0;
	}
	this->hasColor = true;
	this->color = (var4 << 24) | (var3 << 16) | (var2 << 8) | var1;
	}
}

void Tessellator::addVertexWithUV(double var1, double var3, double var5, double var7, double var9)
{
	this->setTextureUV(var7, var9);
	this->addVertex(var1, var3, var5);
}

void Tessellator::addVertex(double var1, double var3, double var5)
{
	++this->addedVertices;
	if(this->drawMode == 7 && s_convertQuadsToTriangles && this->addedVertices % 4 == 0) {
	for(int var7 = 0; var7 < 2; ++var7) {
	int var8 = 8 * (3 - var7);
	if(this->hasTexture) {
	this->rawBuffer[this->rawBufferIndex + 3] = this->rawBuffer[this->rawBufferIndex - var8 + 3];
	this->rawBuffer[this->rawBufferIndex + 4] = this->rawBuffer[this->rawBufferIndex - var8 + 4];
	}
	if(this->hasColor) {
	this->rawBuffer[this->rawBufferIndex + 5] = this->rawBuffer[this->rawBufferIndex - var8 + 5];
	}
	this->rawBuffer[this->rawBufferIndex + 0] = this->rawBuffer[this->rawBufferIndex - var8 + 0];
	this->rawBuffer[this->rawBufferIndex + 1] = this->rawBuffer[this->rawBufferIndex - var8 + 1];
	this->rawBuffer[this->rawBufferIndex + 2] = this->rawBuffer[this->rawBufferIndex - var8 + 2];
	++this->vertexCount;
	this->rawBufferIndex += 8;
	}
	}
	if(this->hasTexture) {
	this->rawBuffer[this->rawBufferIndex + 3] = Float::floatToRawIntBits((float)this->textureU);
	this->rawBuffer[this->rawBufferIndex + 4] = Float::floatToRawIntBits((float)this->textureV);
	}
	if(this->hasColor) {
	this->rawBuffer[this->rawBufferIndex + 5] = this->color;
	}
	if(this->hasNormals) {
	this->rawBuffer[this->rawBufferIndex + 6] = this->normal;
	}
	this->rawBuffer[this->rawBufferIndex + 0] = Float::floatToRawIntBits((float)(var1 + this->xOffset));
	this->rawBuffer[this->rawBufferIndex + 1] = Float::floatToRawIntBits((float)(var3 + this->yOffset));
	this->rawBuffer[this->rawBufferIndex + 2] = Float::floatToRawIntBits((float)(var5 + this->zOffset));
	this->rawBufferIndex += 8;
	++this->vertexCount;
	if(this->vertexCount % 4 == 0 && this->rawBufferIndex >= this->bufferSize - 32) {
	this->draw();
	this->isDrawing = true;
	}
}

void Tessellator::setColorOpaque_I(int var1)
{
	int var2 = (var1 >> 16) & 255;
	int var3 = (var1 >> 8) & 255;
	int var4 = var1 & 255;
	this->setColorOpaque(var2, var3, var4);
}

void Tessellator::setColorRGBA_I(int var1, int var2)
{
	int var3 = (var1 >> 16) & 255;
	int var4 = (var1 >> 8) & 255;
	int var5 = var1 & 255;
	this->setColorRGBA(var3, var4, var5, var2);
}

void Tessellator::disableColor()
{
	this->isColorDisabled = true;
}

void Tessellator::setNormal(float var1, float var2, float var3)
{
	if(!this->isDrawing) {
	System::out.println("But..");
	}
	this->hasNormals = true;
	unsigned char var4 = (unsigned char)((int)(var1 * 128.0F));
	unsigned char var5 = (unsigned char)((int)(var2 * 127.0F));
	unsigned char var6 = (unsigned char)((int)(var3 * 127.0F));
	this->normal = var4 | (var5 << 8) | (var6 << 16);
}

void Tessellator::setTranslationD(double var1, double var3, double var5)
{
	this->xOffset = var1;
	this->yOffset = var3;
	this->zOffset = var5;
}

void Tessellator::setTranslationF(float var1, float var2, float var3)
{
	this->xOffset += (double)var1;
	this->yOffset += (double)var2;
	this->zOffset += (double)var3;
}
