#ifndef TESSELLATOR_H
#define TESSELLATOR_H

// Auto-generated from Tessellator.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include <vector>
#include "Shaders.h"   // TesselatorVertex (for extractTris -> threaded meshing)
class ByteBuffer;
class FloatBuffer;
class IntBuffer;

class Tessellator {
public:
	Tessellator(int var1);
	virtual ~Tessellator();

	ByteBuffer* byteBuffer;
	IntBuffer* intBuffer;
	FloatBuffer* floatBuffer;
	JArray<int > rawBuffer;
	int vertexCount;
	double textureU;
	double textureV;
	int color;
	bool hasColor;
	bool hasTexture;
	bool hasNormals;
	int rawBufferIndex;
	int addedVertices;
	bool isColorDisabled;
	int drawMode;
	double xOffset;
	double yOffset;
	double zOffset;
	int normal;
	bool isDrawing;
	bool useVBO;
	IntBuffer* vertexBuffers;
	int vboIndex;
	int vboCount;
	int bufferSize;
	static bool s_convertQuadsToTriangles;
	static bool s_tryVBO;
	static Tessellator* s_instance;
	// PORT(★ threaded chunk mesh): the Tessellator that block-mesh code (RenderBlocks' 24 sites + WorldRenderer)
	// builds into. Defaults to s_instance (the GUI/HUD/immediate singleton). Stage A: redirected via s_active to
	// a dedicated MESH Tessellator during a chunk build so it never disturbs the GUI singleton. Stage B: becomes
	// per-worker (TLS) so concurrent mesh jobs each have their own. GUI/HUD/immediate code keeps using s_instance.
	static Tessellator* s_active;
	static Tessellator* active();

	virtual void draw();
	virtual void reset();
	virtual void startDrawingQuads();
	virtual void startDrawing(int var1);
	virtual void setTextureUV(double var1, double var3);
	virtual void setColorOpaque_F(float var1, float var2, float var3);
	virtual void setColorRGBA_F(float var1, float var2, float var3, float var4);
	virtual void setColorOpaque(int var1, int var2, int var3);
	virtual void setColorRGBA(int var1, int var2, int var3, int var4);
	virtual void addVertexWithUV(double var1, double var3, double var5, double var7, double var9);
	virtual void addVertex(double var1, double var3, double var5);
	virtual void setColorOpaque_I(int var1);
	virtual void setColorRGBA_I(int var1, int var2);
	virtual void disableColor();
	virtual void setNormal(float var1, float var2, float var3);
	virtual void setTranslationD(double var1, double var3, double var5);
	virtual void setTranslationF(float var1, float var2, float var3);

	// THREADED MESHING: pull the accumulated geometry out as a finished triangle list (the same verts
	// glDrawArrays would record/draw) WITHOUT touching any GL/device global -- so a worker thread can
	// tessellate a chunk into its own Tessellator and hand the result to the main thread. Resets after.
	void extractTris(std::vector<TesselatorVertex>& out);
};

#endif // TESSELLATOR_H