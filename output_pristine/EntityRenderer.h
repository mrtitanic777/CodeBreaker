#ifndef ENTITYRENDERER_H
#define ENTITYRENDERER_H

// Auto-generated from EntityRenderer.java
// Target: Xbox 360 / C++03 / DirectX 9

class DecimalFormat;
class Entity;
class ItemRenderer;
class Minecraft;
class Random;
class Vec3D;

class EntityRenderer {
public:
	EntityRenderer();
	EntityRenderer(Minecraft* var1);
	virtual ~EntityRenderer();

	ItemRenderer* m_itemRenderer;

	virtual void updateRenderer();
	virtual void updateCameraAndRender(float var1);
	virtual void grabLargeScreenshot();
	virtual void setupOverlayRendering();

private:
	Minecraft* mc;
	bool m_anaglyphEnable;
	float m_farPlaneDistance;
	int m_rendererUpdateCount;
	Entity* m_pointedEntity;
	int m_entityRendererInt1;
	int m_entityRendererInt2;
	char* /* PORT: was DecimalFormat */ m_entityDecimalFormat;
	unsigned char* /* PORT: was ByteBuffer */ m_entityByteBuffer;
	float* /* PORT: was FloatBuffer */ m_entityFloatBuffer;
	Random* m_random;
	float* /* PORT: was FloatBuffer */ m_fogColorBuffer;
	float m_fogColorRed;
	float m_fogColorGreen;
	float m_fogColorBlue;
	float m_prevFogColor;
	float m_fogColor;

	Vec3D* orientCamera(float var1);
	void hurtCameraEffect(float var1);
	void setupViewBobbing(float var1);
	void* /* PORT: was BufferedImage */ screenshotBuffer(unsigned char* /* PORT: was ByteBuffer */ var0, int var1, int var2);
	void getMouseOver(float var1);
	void updateFogColor(float var1);
	void setupFog();

};

#endif // ENTITYRENDERER_H