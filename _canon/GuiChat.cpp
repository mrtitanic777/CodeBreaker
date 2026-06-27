#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from GuiChat.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiChat.h"
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
#include "GameSettings.h"
#include "GuiButton.h"
#include "GuiIngame.h"
#include "GuiScreen.h"
#include "IBlockAccess.h"
#include "IChunkLoader.h"
#include "IChunkProvider.h"
#include "InputStream.h"
#include "IntBuffer.h"
#include "InventoryPlayer.h"
#include "Item.h"
#include "ItemRenderer.h"
#include "ItemStack.h"
#include "KeyBinding.h"
#include "Keyboard.h"
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

GuiChat::GuiChat()
	: updateCounter(0)
{
	this->message = "";
	this->updateCounter = 0;
}

GuiChat::~GuiChat()
{
}

void GuiChat::initGui()
{
	Keyboard::enableRepeatEvents(true);
}

void GuiChat::onGuiClosed()
{
	Keyboard::enableRepeatEvents(false);
}

void GuiChat::updateScreen()
{
	++this->updateCounter;
}

void GuiChat::keyTyped(char var1, int var2)
{
	if(var2 == 1) {
	this->mc->displayGuiScreen((GuiScreen*)NULL);
	} else if(var2 == 28) {
	JavaString var3 = this->message.trim();
	if(var3.length() > 0) {
	this->mc->thePlayer->sendChatMessage(this->message.trim());
	}
	this->mc->displayGuiScreen((GuiScreen*)NULL);
	} else {
	if(var2 == 14 && this->message.length() > 0) {
	this->message = this->message.substr(0, (this->message.length() - 1) - (0));
	}
	if(JavaString(" !\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_\'abcdefghijklmnopqrstuvwxyz{|}~\x7f\u00c7\u00fc\u00e9\u00e2\u00e4\u00e0\u00e5\u00e7\u00ea\u00eb\u00e8\u00ef\u00ee\u00ec\u00c4\u00c5\u00c9\u00e6\u00c6\u00f4\u00f6\u00f2\u00fb\u00f9\u00ff\u00d6\u00dc\u00f8\u00a3\u00d8\u00d7\u0192\u00e1\u00ed\u00f3\u00fa\u00f1\u00d1\u00aa\u00ba\u00bf\u00ae\u00ac\u00bd\u00bc\u00a1\u00ab\u00bb").indexOf(var1) >= 0 && this->message.length() < 100) {
	this->message = this->message + var1;
	}
	}
}

void GuiChat::drawScreen(int var1, int var2, float var3)
{
	this->drawRect(2, this->height - 14, this->width - 2, this->height - 2, Integer::MIN_VALUE);
	this->drawString(this->fontRenderer,(JCat() + "> " + this->message + (this->updateCounter / 6 % 2 == 0 ? "_" : "")),4,this->height - 12,14737632);
}

void GuiChat::mouseClicked(int var1, int var2, int var3)
{
	if(var3 == 0 && this->mc->ingameGUI->clickedChatText != NULL) {
	if(this->message.length() > 0 && !this->message.endsWith(" ")) {
	this->message = (JCat() + this->message + " ");
	}
	this->message = this->message + this->mc->ingameGUI->clickedChatText;
	unsigned char var4 = 100;
	if(this->message.length() > var4) {
	this->message = this->message.substr(0, (var4) - (0));
	}
	}
}

void GuiChat::initStatics()
{
}
