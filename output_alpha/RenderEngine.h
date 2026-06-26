#ifndef RENDERENGINE_H
#define RENDERENGINE_H

// Auto-generated from RenderEngine.java
// Target: Xbox 360 / C++03 / DirectX 9

#include <map>
#include <string>
#include <vector>

class GameSettings;
class ImageBuffer;
class InputStream;
class Map;
class TextureFX;
class TexturePackList;
class ThreadDownloadImageData;

class RenderEngine {
public:
	RenderEngine();
	RenderEngine(TexturePackList* var1, GameSettings* var2);
	virtual ~RenderEngine();

	static bool s_useMipmaps;

	virtual int getTexture(const std::string& var1);
	virtual int allocateAndSetupTexture(void* /* PORT: was BufferedImage */ var1);
	virtual void setupTexture(void* /* PORT: was BufferedImage */ var1, int var2);
	virtual void deleteTexture(int var1);
	virtual int getTextureForDownloadableImage(const std::string& var1, const std::string& var2);
	virtual ThreadDownloadImageData* obtainImageData(const std::string& var1, ImageBuffer* var2);
	virtual void releaseImageData(const std::string& var1);
	virtual void registerTextureFX(TextureFX* var1);
	virtual void func_1067_a();
	virtual void refreshTextures();
	virtual void bindTexture(int var1);

public: // was private
	std::map<std::string, int> m_textureMap;
	std::map<std::string, int> m_textureNameToImageMap;
	int* /* PORT: was IntBuffer */ m_singleIntBuffer;
	unsigned char* /* PORT: was ByteBuffer */ imageData;
	std::vector<void*> /* PORT: determine element type */ m_field_1604_f;
	std::map<std::string, void*>* m_urlToImageDataMap;
	GameSettings* m_options;
	bool m_clampTexture;
	bool m_blurTexture;
	TexturePackList* m_field_6527_k;

	void* /* PORT: was BufferedImage */ unwrapImageByColumns(void* /* PORT: was BufferedImage */ var1);
	int averageColor(int var1, int var2);
	int weightedAverageColor(int var1, int var2);
	void* /* PORT: was BufferedImage */ readTextureImage(InputStream* var1);

};

#endif // RENDERENGINE_H