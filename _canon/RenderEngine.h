#ifndef RENDERENGINE_H
#define RENDERENGINE_H

// Auto-generated from RenderEngine.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include <map>

class BufferedImage;
class ByteBuffer;
class GameSettings;
class ImageBuffer;
class InputStream;
class IntBuffer;
class TextureFX;
class TexturePackList;
class ThreadDownloadImageData;

class RenderEngine {
public:
	RenderEngine(TexturePackList* var1, GameSettings* var2);
	virtual ~RenderEngine();

	std::map<std::string, int> textureMap;
	std::map<int, void*> textureNameToImageMap;
	IntBuffer* singleIntBuffer;
	ByteBuffer* imageData;
	JList<void*> textureList;
	std::map<std::string, void*> urlToImageDataMap;
	GameSettings* options;
	bool clampTexture;
	bool blurTexture;
	TexturePackList* texturePackList;
	static bool s_useMipmaps;

	virtual int getTexture(const JavaString& var1);
	virtual BufferedImage* unwrapImageByColumns(BufferedImage* var1);
	virtual int allocateAndSetupTexture(BufferedImage* var1);
	virtual void setupTexture(BufferedImage* var1, int var2);
	virtual void deleteTexture(int var1);
	virtual int getTextureForDownloadableImage(const JavaString& var1, const JavaString& var2);
	virtual ThreadDownloadImageData* obtainImageData(const JavaString& var1, ImageBuffer* var2);
	virtual void releaseImageData(const JavaString& var1);
	virtual void registerTextureFX(TextureFX* var1);
	virtual void updateDynamicTextures();
	virtual int averageColor(int var1, int var2);
	virtual int weightedAverageColor(int var1, int var2);
	virtual void refreshTextures();
	virtual BufferedImage* readTextureImage(InputStream* var1);
	virtual void bindTexture(int var1);
};

#endif // RENDERENGINE_H