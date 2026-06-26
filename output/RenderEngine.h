#ifndef RENDERENGINE_H
#define RENDERENGINE_H

// Auto-generated from RenderEngine.java
// Target: Xbox 360 / C++03 / DirectX 9

#include <map>
#include <string>
#include <vector>

class GameSettings;
class ImageBufferDownload;
class Map;
class TextureFX;
class ThreadDownloadImageData;

class RenderEngine {
public:
	RenderEngine();
	RenderEngine(GameSettings* var1);
	virtual ~RenderEngine();

	virtual int getTexture(const std::string& var1);
	virtual int getTextureForDownloadableImage(const std::string& var1, const std::string& var2);
	virtual ThreadDownloadImageData* obtainImageData(const std::string& var1, ImageBufferDownload* var2);
	virtual void releaseImageData(const std::string& var1);
	virtual void registerTextureFX(TextureFX* var1);
	virtual void updateDynamicTextures();
	virtual void refreshTextures();
	static void bindTexture(int var0);

public: // was private
	std::map<std::string, int> m_textureMap;
	std::map<std::string, int> m_textureContentsMap;
	int* /* PORT: was IntBuffer */ m_singleIntBuffer;
	unsigned char* /* PORT: was ByteBuffer */ imageData;
	std::vector<void*> /* PORT: determine element type */ m_textureList;
	std::map<std::string, void*>* m_urlToImageDataMap;
	GameSettings* m_options;
	bool m_clampTexture;

	void* /* PORT: was BufferedImage */ unwrapImageByColumns(void* /* PORT: was BufferedImage */ var0);
	void setupTexture(void* /* PORT: was BufferedImage */ var1, int var2);

};

#endif // RENDERENGINE_H