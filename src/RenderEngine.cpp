#include "stdafx.h"
// Auto-generated from RenderEngine.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderEngine.h"
#include "Common.h"
#include "GameSettings.h"
#include "ImageBufferDownload.h"
#include "Map.h"
#include "TextureFX.h"
#include "ThreadDownloadImageData.h"
#include <string>
#include "Textures.h"

RenderEngine::RenderEngine()
	: m_singleIntBuffer(new int[1])
	, imageData(new unsigned char[262144])
	, m_urlToImageDataMap(NULL)
	, m_options(NULL)
	, m_clampTexture(false)
{
}

RenderEngine::RenderEngine(GameSettings* var1)
	: m_singleIntBuffer(new int[1])
	, imageData(new unsigned char[262144])
	, m_urlToImageDataMap(NULL)
	, m_options(NULL)
	, m_clampTexture(false)
{
		m_options = var1;
}

RenderEngine::~RenderEngine() {
	m_urlToImageDataMap = NULL;
	m_options = NULL;
}

int RenderEngine::getTexture(const std::string& var1) {
		int var2 = (int)m_textureMap[var1];
		if(var2 != NULL) {
			return var2.intValue();
		} else {
   { // was try
    memset(m_singleIntBuffer, 0, sizeof(m_singleIntBuffer));
    // PORT: D3D9 CreateTexture
				int var4 = m_singleIntBuffer[0];
				if(var1.startsWith("##")) {
     // PORT: ImageIO -> D3D9 texture loading  // was: setupTexture(unwrapImageByColumns(ImageIO.read(RenderEngine.class.getResourceAsStream(var1.substring(2)))), var4);
				} else if(var1.startsWith("%%")) {
					m_clampTexture = true;
     // PORT: ImageIO -> D3D9 texture loading  // was: setupTexture(ImageIO.read(RenderEngine.class.getResourceAsStream(var1.substring(2))), var4);
					m_clampTexture = false;
				} else {
     // PORT: ImageIO -> D3D9 texture loading  // was: setupTexture(ImageIO.read(RenderEngine.class.getResourceAsStream(var1)), var4);
				}

    m_textureMap[0] = var1, var4;
				return var4;
			} { DbgPrintF("CATCH IOException\n"); // was catch(IOException var3)
				DbgPrintF("ERROR [RuntimeException]: %s\n", "!!");
			}
		}
}

void* /* PORT: was BufferedImage */ RenderEngine::unwrapImageByColumns(void* /* PORT: was BufferedImage */ var0) {
		int var1 = var0.getWidth() / 16;
  // PORT: BufferedImage -> D3D9 texture loading  // was: BufferedImage* var2 = new BufferedImage(16, var0.getHeight() * var1, 2);
		Graphics var3 = var2.getGraphics();

		for(int var4 = 0; var4 < var1; ++var4) {
			var3.drawImage(var0, -var4 << 4, var4 * var0.getHeight(), (ImageObserver)NULL);
		}

		var3.dispose();
		return var2;
}

void RenderEngine::setupTexture(void* /* PORT: was BufferedImage */ var1, int var2) {
  g_pd3dDevice->SetTexture(0, Textures::getById(var2));
  // Texture params handled by SetSamplerState in init
  // Texture params handled by SetSamplerState in init
		if(m_clampTexture) {
   // Texture params handled by SetSamplerState in init
   // Texture params handled by SetSamplerState in init
		} else {
   // Texture params handled by SetSamplerState in init
   // Texture params handled by SetSamplerState in init
		}

		var2 = var1->getWidth();
		int var3 = var1->getHeight();
		int* var4 = new int[var2 * var3];
		unsigned char[] var5 = new unsigned char[var2 * var3 << 2];
		var1->getRGB(0, 0, var2, var3, var4, 0, var2);

		for(int var11 = 0; var11 < var2 * var3; ++var11) {
			int var6 = var4[var11] >>> 24;
			int var7 = var4[var11] >> 16 & 255;
			int var8 = var4[var11] >> 8 & 255;
			int var9 = var4[var11] & 255;
			if(m_options != NULL && m_options->m_anaglyph) {
				int var10 = (var7 * 30 + var8 * 59 + var9 * 11) / 100;
				var8 = (var7 * 30 + var8 * 70) / 100;
				var9 = (var7 * 30 + var9 * 70) / 100;
				var7 = var10;
				var8 = var8;
				var9 = var9;
			}

			var5[var11 << 2] = (unsigned char)var7;
			var5[(var11 << 2) + 1] = (unsigned char)var8;
			var5[(var11 << 2) + 2] = (unsigned char)var9;
			var5[(var11 << 2) + 3] = (unsigned char)var6;
		}

  memset(imageData, 0, sizeof(imageData));
  imageData[0] = var5;
  // buffer imageData position = 0
  // PORT: D3D9 texture create/update via LockRect
}

int RenderEngine::getTextureForDownloadableImage(const std::string& var1, const std::string& var2) {
		ThreadDownloadImageData var6 = (ThreadDownloadImageData)m_urlToImageDataMap[var1];
		if(var6 != NULL && var6.image != NULL && !var6.textureSetupComplete) {
			if(var6.textureName < 0) {
    // PORT: BufferedImage -> D3D9 texture loading  // was: BufferedImage var4 = var6.image;
    memset(m_singleIntBuffer, 0, sizeof(m_singleIntBuffer));
    // PORT: D3D9 CreateTexture
				int var5 = m_singleIntBuffer[0];
				setupTexture(var4, var5);
    m_textureContentsMap[0] = var5, var4;
				var6.textureName = var5;
			} else {
				setupTexture(var6.image, var6.textureName);
			}

			var6.textureSetupComplete = true;
		}

		return var6 != NULL && var6.textureName >= 0 ? var6.textureName : getTexture(var2);
}

ThreadDownloadImageData* RenderEngine::obtainImageData(const std::string& var1, ImageBufferDownload* var2) {
		ThreadDownloadImageData var3 = (ThreadDownloadImageData)m_urlToImageDataMap[var1];
		if(var3 == NULL) {
			// PORT-TODO: m_urlToImageDataMap->put(var1, new ThreadDownloadImageData(var1, var2));
		} else {
			++var3.referenceCount;
		}

		return var3;
}

void RenderEngine::releaseImageData(const std::string& var1) {
		ThreadDownloadImageData var2 = (ThreadDownloadImageData)m_urlToImageDataMap[var1];
		if(var2 != NULL) {
			--var2.referenceCount;
			if(var2.referenceCount == 0) {
				if(var2.textureName >= 0) {
					int var3 = var2.textureName;
					{ auto _it = std::find(m_textureContentsMap.begin(), m_textureContentsMap.end(), var3); if (_it != m_textureContentsMap.end()) m_textureContentsMap.erase(_it); }
     memset(m_singleIntBuffer, 0, sizeof(m_singleIntBuffer));
     m_singleIntBuffer[0] = var3;
     // NIO flip() � no-op for C array
					GL11.glDeleteTextures(m_singleIntBuffer);
				}

				m_urlToImageDataMap->erase(.begin() + var1) /* PORT: verify remove semantics */;
			}
		}

}

void RenderEngine::registerTextureFX(TextureFX* var1) {
		m_textureList.push_back(var1);
		var1->onTick();
}

void RenderEngine::updateDynamicTextures() {
		size_t var1;
		TextureFX var2;
		for(var1 = 0; var1 < m_textureList.size(); ++var1) {
			var2 = (TextureFX*)m_textureList[var1];
			var2.anaglyphEnabled = m_options->m_anaglyph;
			var2.onTick();
   memset(imageData, 0, sizeof(imageData));
   imageData[0] = var2.imageData;
   // buffer imageData position = 0
   // PORT: D3D9 texture create/update via LockRect
		}

		for(var1 = 0; var1 < m_textureList.size(); ++var1) {
			var2 = (TextureFX*)m_textureList[var1];
			if(var2.textureId > 0) {
    memset(imageData, 0, sizeof(imageData));
    imageData[0] = var2.imageData;
    // buffer imageData position = 0
    g_pd3dDevice->SetTexture(0, Textures::getById(var2.textureId));
    // PORT: D3D9 texture create/update via LockRect
			}
		}

}

void RenderEngine::refreshTextures() {
		Iterator var1 = m_textureContentsMap.keySet().iterator();

		int var2;
  // PORT: BufferedImage -> D3D9 texture loading  // was: BufferedImage var3;
		while(var1->hasNext()) {
			var2 = ((int)var1->next()).intValue();
   // PORT: BufferedImage -> D3D9 texture loading  // was: var3 = (BufferedImage)m_textureContentsMap.get(var2);
			setupTexture(var3, var2);
		}

		ThreadDownloadImageData var5;
		for(var1 = m_urlToImageDataMap->values().iterator(); var1->hasNext(); var5.textureSetupComplete = false) {
			var5 = (ThreadDownloadImageData)var1->next();
		}

		var1 = m_textureMap.keySet().iterator();

		while(var1->hasNext()) {
			std::string var6 = (std::string)var1->next();

   { // was try
				if(var6.startsWith("##")) {
     // PORT: ImageIO -> D3D9 texture loading  // was: var3 = unwrapImageByColumns(ImageIO.read(RenderEngine.class.getResourceAsStream(var6.substring(2))));
				} else if(var6.startsWith("%%")) {
					m_clampTexture = true;
     // PORT: ImageIO -> D3D9 texture loading  // was: var3 = ImageIO.read(RenderEngine.class.getResourceAsStream(var6.substring(2)));
					m_clampTexture = false;
				} else {
     // PORT: ImageIO -> D3D9 texture loading  // was: var3 = ImageIO.read(RenderEngine.class.getResourceAsStream(var6));
				}

				var2 = m_textureMap[var6]->intValue();
				setupTexture(var3, var2);
			} { DbgPrintF("CATCH IOException\n"); // was catch(IOException var4)
    // Removed Java stream/exception call: var4.printStackTrace();
			}
		}

}

void RenderEngine::bindTexture(int var0) {
		if(var0 >= 0) {
   g_pd3dDevice->SetTexture(0, Textures::getById(var0));
		}
}
