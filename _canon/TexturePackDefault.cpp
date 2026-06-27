#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from TexturePackDefault.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "TexturePackDefault.h"
#include "AxisAlignedBB.h"
#include "Block.h"
#include "BlockFire.h"
#include "BlockFlower.h"
#include "BlockGrass.h"
#include "BlockLeaves.h"
#include "BlockPortal.h"
#include "BufferedImage.h"
#include "ByteBuffer.h"
#include "Canvas.h"
#include "Chunk.h"
#include "Class.h"
#include "Component.h"
#include "Cursor.h"
#include "EffectRenderer.h"
#include "Entity.h"
#include "EntityFish.h"
#include "EntityItem.h"
#include "EntityPlayer.h"
#include "EntityPlayerSP.h"
#include "EntityRenderer.h"
#include "EnumOS2.h"
#include "Explosion.h"
#include "File.h"
#include "FloatBuffer.h"
#include "FontRenderer.h"
#include "GL11.h"
#include "GameSettings.h"
#include "GuiButton.h"
#include "GuiIngame.h"
#include "GuiScreen.h"
#include "IBlockAccess.h"
#include "IChunkLoader.h"
#include "IChunkProvider.h"
#include "IOException.h"
#include "ImageIO.h"
#include "InputStream.h"
#include "IntBuffer.h"
#include "InventoryPlayer.h"
#include "Item.h"
#include "ItemRenderer.h"
#include "ItemStack.h"
#include "KeyBinding.h"
#include "LoadingScreenRenderer.h"
#include "Material.h"
#include "Minecraft.h"
#include "MinecraftApplet.h"
#include "MobSpawnerBase.h"
#include "ModelBase.h"
#include "ModelBiped.h"
#include "ModelRenderer.h"
#include "MouseHelper.h"
#include "MovementInput.h"
#include "MovingObjectPosition.h"
#include "NBTBase.h"
#include "NBTTagCompound.h"
#include "NBTTagList.h"
#include "NibbleArray.h"
#include "NoiseGenerator2.h"
#include "NoiseGeneratorOctaves2.h"
#include "OpenGlCapsChecker.h"
#include "PathEntity.h"
#include "PathPoint.h"
#include "PlayerController.h"
#include "PositionTexureVertex.h"
#include "Random.h"
#include "Render.h"
#include "RenderBlocks.h"
#include "RenderEngine.h"
#include "RenderGlobal.h"
#include "RenderItem.h"
#include "RenderList.h"
#include "RenderManager.h"
#include "Session.h"
#include "SoundManager.h"
#include "SoundPool.h"
#include "SoundPoolEntry.h"
#include "SoundSystem.h"
#include "StepSound.h"
#include "Tessellator.h"
#include "TextureLavaFX.h"
#include "TexturePackBase.h"
#include "TexturePackList.h"
#include "TextureWaterFX.h"
#include "TexturedQuad.h"
#include "Thread.h"
#include "ThreadDownloadImageData.h"
#include "ThreadDownloadResources.h"
#include "TileEntity.h"
#include "Timer.h"
#include "URL.h"
#include "Vec3D.h"
#include "World.h"
#include "WorldChunkManager.h"
#include "WorldProvider.h"
#include "WorldRenderer.h"

TexturePackDefault::TexturePackDefault()
	: texturePackName(0)
	, packImage(NULL)
{
	this->texturePackName = -1;
	this->texturePackFileName = "Default";
	this->firstDescriptionLine = "The default look of Minecraft";
	try {
	this->packImage = ImageIO::read(Class(typeid(TexturePackDefault)).getResource("/pack.png"));
	} catch (IOException* var2) {
	var2->printStackTrace();
	}
}

TexturePackDefault::~TexturePackDefault()
{
}

void TexturePackDefault::bindTexturePack(Minecraft* var1)
{
	if(this->packImage != NULL) {
	var1->renderEngine->deleteTexture(this->texturePackName);
	}
}

void TexturePackDefault::unbindTexturePack(Minecraft* var1)
{
	if(this->packImage != NULL && this->texturePackName < 0) {
	this->texturePackName = var1->renderEngine->allocateAndSetupTexture(this->packImage);
	}
	if(this->packImage != NULL) {
	var1->renderEngine->bindTexture(this->texturePackName);
	} else {
	GL11::glBindTexture(GL11::GL_TEXTURE_2D, var1->renderEngine->getTexture("/gui/unknown_pack.png"));
	}
}

void TexturePackDefault::initStatics()
{
}
