#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from ClippingHelperImplementation.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ClippingHelperImplementation.h"
#include "ClippingHelper.h"
#include "FloatBuffer.h"
#include "GL11.h"
#include "GLAllocation.h"   // createDirectFloatBuffer (used by the synthesized ctor)
#include "MathHelper.h"

// Local shims for GL symbols missing from the central GL11 stub (see report):
//  - GL_PROJECTION_MATRIX / GL_MODELVIEW_MATRIX enum query tokens (standard GL values)
//  - glGetFloat(token, FloatBuffer*) to read a matrix into a buffer (no-op until backed)
/* ---- ORIGINAL JAVA (stubbed below) ----
 *  // From init(): the projection and modelview matrices are read out of OpenGL into
 *  // the float buffers via GL11.glGetFloat. The glGetFloat shim below is a NO-OP, so
 *  // projectionBuffer / modelviewBuffer are never populated and the frustum planes
 *  // derived from them are not the real camera frustum.
 *  GL11.glGetFloat(GL11.GL_PROJECTION_MATRIX, this.projectionBuffer);
 *  GL11.glGetFloat(GL11.GL_MODELVIEW_MATRIX, this.modelviewBuffer);
 *  ---- end original Java ---- */
namespace _GLShim {
	const int GL_PROJECTION_MATRIX = 0x0BA7;
	const int GL_MODELVIEW_MATRIX  = 0x0BA6;
	inline void glGetFloat(int, FloatBuffer*) {}
}

ClippingHelperImplementation* ClippingHelperImplementation::s_instance = new ClippingHelperImplementation();

ClippingHelperImplementation::ClippingHelperImplementation()
	: projectionBuffer(NULL)
	, modelviewBuffer(NULL)
	, modelviewProjectionBuffer(NULL)
{
	this->projectionBuffer = GLAllocation::createDirectFloatBuffer(16);
	this->modelviewBuffer = GLAllocation::createDirectFloatBuffer(16);
	this->modelviewProjectionBuffer = GLAllocation::createDirectFloatBuffer(16);
}

ClippingHelperImplementation::~ClippingHelperImplementation()
{
}

ClippingHelper* ClippingHelperImplementation::getInstance()
{
	s_instance->init();
	return s_instance;
}

void ClippingHelperImplementation::normalize(JArray<JArray<float > > var1, int var2)
{
	float var3 = MathHelper::sqrt_float(var1[var2][0] * var1[var2][0] + var1[var2][1] * var1[var2][1] + var1[var2][2] * var1[var2][2]);
	var1[var2][0] /= var3;
	var1[var2][1] /= var3;
	var1[var2][2] /= var3;
	var1[var2][3] /= var3;
}

void ClippingHelperImplementation::init()
{
	this->projectionBuffer->clear();
	this->modelviewBuffer->clear();
	this->modelviewProjectionBuffer->clear();
	_GLShim::glGetFloat(_GLShim::GL_PROJECTION_MATRIX, this->projectionBuffer);
	_GLShim::glGetFloat(_GLShim::GL_MODELVIEW_MATRIX, this->modelviewBuffer);
	this->projectionBuffer->flip().limit(16);
	this->projectionBuffer->get(this->projectionMatrix);
	this->modelviewBuffer->flip().limit(16);
	this->modelviewBuffer->get(this->modelviewMatrix);
	this->clippingMatrix[0] = this->modelviewMatrix[0] * this->projectionMatrix[0] + this->modelviewMatrix[1] * this->projectionMatrix[4] + this->modelviewMatrix[2] * this->projectionMatrix[8] + this->modelviewMatrix[3] * this->projectionMatrix[12];
	this->clippingMatrix[1] = this->modelviewMatrix[0] * this->projectionMatrix[1] + this->modelviewMatrix[1] * this->projectionMatrix[5] + this->modelviewMatrix[2] * this->projectionMatrix[9] + this->modelviewMatrix[3] * this->projectionMatrix[13];
	this->clippingMatrix[2] = this->modelviewMatrix[0] * this->projectionMatrix[2] + this->modelviewMatrix[1] * this->projectionMatrix[6] + this->modelviewMatrix[2] * this->projectionMatrix[10] + this->modelviewMatrix[3] * this->projectionMatrix[14];
	this->clippingMatrix[3] = this->modelviewMatrix[0] * this->projectionMatrix[3] + this->modelviewMatrix[1] * this->projectionMatrix[7] + this->modelviewMatrix[2] * this->projectionMatrix[11] + this->modelviewMatrix[3] * this->projectionMatrix[15];
	this->clippingMatrix[4] = this->modelviewMatrix[4] * this->projectionMatrix[0] + this->modelviewMatrix[5] * this->projectionMatrix[4] + this->modelviewMatrix[6] * this->projectionMatrix[8] + this->modelviewMatrix[7] * this->projectionMatrix[12];
	this->clippingMatrix[5] = this->modelviewMatrix[4] * this->projectionMatrix[1] + this->modelviewMatrix[5] * this->projectionMatrix[5] + this->modelviewMatrix[6] * this->projectionMatrix[9] + this->modelviewMatrix[7] * this->projectionMatrix[13];
	this->clippingMatrix[6] = this->modelviewMatrix[4] * this->projectionMatrix[2] + this->modelviewMatrix[5] * this->projectionMatrix[6] + this->modelviewMatrix[6] * this->projectionMatrix[10] + this->modelviewMatrix[7] * this->projectionMatrix[14];
	this->clippingMatrix[7] = this->modelviewMatrix[4] * this->projectionMatrix[3] + this->modelviewMatrix[5] * this->projectionMatrix[7] + this->modelviewMatrix[6] * this->projectionMatrix[11] + this->modelviewMatrix[7] * this->projectionMatrix[15];
	this->clippingMatrix[8] = this->modelviewMatrix[8] * this->projectionMatrix[0] + this->modelviewMatrix[9] * this->projectionMatrix[4] + this->modelviewMatrix[10] * this->projectionMatrix[8] + this->modelviewMatrix[11] * this->projectionMatrix[12];
	this->clippingMatrix[9] = this->modelviewMatrix[8] * this->projectionMatrix[1] + this->modelviewMatrix[9] * this->projectionMatrix[5] + this->modelviewMatrix[10] * this->projectionMatrix[9] + this->modelviewMatrix[11] * this->projectionMatrix[13];
	this->clippingMatrix[10] = this->modelviewMatrix[8] * this->projectionMatrix[2] + this->modelviewMatrix[9] * this->projectionMatrix[6] + this->modelviewMatrix[10] * this->projectionMatrix[10] + this->modelviewMatrix[11] * this->projectionMatrix[14];
	this->clippingMatrix[11] = this->modelviewMatrix[8] * this->projectionMatrix[3] + this->modelviewMatrix[9] * this->projectionMatrix[7] + this->modelviewMatrix[10] * this->projectionMatrix[11] + this->modelviewMatrix[11] * this->projectionMatrix[15];
	this->clippingMatrix[12] = this->modelviewMatrix[12] * this->projectionMatrix[0] + this->modelviewMatrix[13] * this->projectionMatrix[4] + this->modelviewMatrix[14] * this->projectionMatrix[8] + this->modelviewMatrix[15] * this->projectionMatrix[12];
	this->clippingMatrix[13] = this->modelviewMatrix[12] * this->projectionMatrix[1] + this->modelviewMatrix[13] * this->projectionMatrix[5] + this->modelviewMatrix[14] * this->projectionMatrix[9] + this->modelviewMatrix[15] * this->projectionMatrix[13];
	this->clippingMatrix[14] = this->modelviewMatrix[12] * this->projectionMatrix[2] + this->modelviewMatrix[13] * this->projectionMatrix[6] + this->modelviewMatrix[14] * this->projectionMatrix[10] + this->modelviewMatrix[15] * this->projectionMatrix[14];
	this->clippingMatrix[15] = this->modelviewMatrix[12] * this->projectionMatrix[3] + this->modelviewMatrix[13] * this->projectionMatrix[7] + this->modelviewMatrix[14] * this->projectionMatrix[11] + this->modelviewMatrix[15] * this->projectionMatrix[15];
	this->frustum[0][0] = this->clippingMatrix[3] - this->clippingMatrix[0];
	this->frustum[0][1] = this->clippingMatrix[7] - this->clippingMatrix[4];
	this->frustum[0][2] = this->clippingMatrix[11] - this->clippingMatrix[8];
	this->frustum[0][3] = this->clippingMatrix[15] - this->clippingMatrix[12];
	this->normalize(this->frustum, 0);
	this->frustum[1][0] = this->clippingMatrix[3] + this->clippingMatrix[0];
	this->frustum[1][1] = this->clippingMatrix[7] + this->clippingMatrix[4];
	this->frustum[1][2] = this->clippingMatrix[11] + this->clippingMatrix[8];
	this->frustum[1][3] = this->clippingMatrix[15] + this->clippingMatrix[12];
	this->normalize(this->frustum, 1);
	this->frustum[2][0] = this->clippingMatrix[3] + this->clippingMatrix[1];
	this->frustum[2][1] = this->clippingMatrix[7] + this->clippingMatrix[5];
	this->frustum[2][2] = this->clippingMatrix[11] + this->clippingMatrix[9];
	this->frustum[2][3] = this->clippingMatrix[15] + this->clippingMatrix[13];
	this->normalize(this->frustum, 2);
	this->frustum[3][0] = this->clippingMatrix[3] - this->clippingMatrix[1];
	this->frustum[3][1] = this->clippingMatrix[7] - this->clippingMatrix[5];
	this->frustum[3][2] = this->clippingMatrix[11] - this->clippingMatrix[9];
	this->frustum[3][3] = this->clippingMatrix[15] - this->clippingMatrix[13];
	this->normalize(this->frustum, 3);
	this->frustum[4][0] = this->clippingMatrix[3] - this->clippingMatrix[2];
	this->frustum[4][1] = this->clippingMatrix[7] - this->clippingMatrix[6];
	this->frustum[4][2] = this->clippingMatrix[11] - this->clippingMatrix[10];
	this->frustum[4][3] = this->clippingMatrix[15] - this->clippingMatrix[14];
	this->normalize(this->frustum, 4);
	this->frustum[5][0] = this->clippingMatrix[3] + this->clippingMatrix[2];
	this->frustum[5][1] = this->clippingMatrix[7] + this->clippingMatrix[6];
	this->frustum[5][2] = this->clippingMatrix[11] + this->clippingMatrix[10];
	this->frustum[5][3] = this->clippingMatrix[15] + this->clippingMatrix[14];
	this->normalize(this->frustum, 5);
}
