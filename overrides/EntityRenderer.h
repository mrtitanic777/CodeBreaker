#ifndef ENTITYRENDERER_H
#define ENTITYRENDERER_H

// Auto-generated from EntityRenderer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class Entity;
class FloatBuffer;
class ItemRenderer;
class Minecraft;
class Random;

class EntityRenderer {
public:
	EntityRenderer(Minecraft* var1);
	virtual ~EntityRenderer();

	Minecraft* mc;
	float farPlaneDistance;
	ItemRenderer* itemRenderer;
	int rendererUpdateCount;
	Entity* pointedEntity;
	long long prevFrameTime;
	Random* random;
	int field_1394_b;
	int field_1393_c;
	FloatBuffer* fogColorBuffer;
	float fogColorRed;
	float fogColorGreen;
	float fogColorBlue;
	float prevFogLightBrightness;
	float fogLightBrightness;

	virtual void updateRenderer();
	virtual void getMouseOver(float var1);
	virtual float getFOVModifier(float var1);
	virtual void hurtCameraEffect(float var1);
	virtual void setupViewBobbing(float var1);
	virtual void orientCamera(float var1);
	virtual void setupCameraTransform(float var1, int var2);
	virtual void renderHand(float var1, int var2);
	virtual void updateCameraAndRender(float var1);
	virtual void renderWorld(float var1);
	virtual void addRainParticles();
	virtual void setupOverlayRendering();
	virtual void updateFogColor(float var1);
	virtual void setupFog(int var1);
	virtual FloatBuffer* setFogColorBuffer(float var1, float var2, float var3, float var4);

private:
	// owns heap; copying disabled so a copy cannot double-free (Rule of Three)
	EntityRenderer(const EntityRenderer&);
	EntityRenderer& operator=(const EntityRenderer&);
};

#endif // ENTITYRENDERER_H