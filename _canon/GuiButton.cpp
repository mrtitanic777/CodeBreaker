#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from GuiButton.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiButton.h"
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
#include "Gui.h"
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

GuiButton::GuiButton(int var1, int var2, int var3, const JavaString& var4)
	: width(0)
	, height(0)
	, xPosition(0)
	, yPosition(0)
	, id(0)
	, enabled(0)
	, enabled2(0)
{
	// inlined this(var1, var2, var3, 200, 20, var4) -- C++03 lacks ctor delegation
	this->width = 200;
	this->height = 20;
	this->enabled = true;
	this->enabled2 = true;
	this->id = var1;
	this->xPosition = var2;
	this->yPosition = var3;
	this->width = 200;
	this->height = 20;
	this->displayString = var4;
}

GuiButton::GuiButton(int var1, int var2, int var3, int var4, int var5, const JavaString& var6)
	: width(0)
	, height(0)
	, xPosition(0)
	, yPosition(0)
	, id(0)
	, enabled(0)
	, enabled2(0)
{
	this->width = 200;
	this->height = 20;
	this->enabled = true;
	this->enabled2 = true;
	this->id = var1;
	this->xPosition = var2;
	this->yPosition = var3;
	this->width = var4;
	this->height = var5;
	this->displayString = var6;
}

GuiButton::~GuiButton()
{
}

int GuiButton::getHoverState(bool var1)
{
	unsigned char var2 = 1;
	if(!this->enabled) {
	var2 = 0;
	} else if(var1) {
	var2 = 2;
	}
	return var2;
}

void GuiButton::drawButton(Minecraft* var1, int var2, int var3)
{
	if(this->enabled2) {
	FontRenderer* var4 = var1->fontRenderer;
	GL11::glBindTexture(GL11::GL_TEXTURE_2D, var1->renderEngine->getTexture("/gui/gui.png"));
	GL11::glColor4f(1.0F, 1.0F, 1.0F, 1.0F);
	bool var5 = var2 >= this->xPosition && var3 >= this->yPosition && var2 < this->xPosition + this->width && var3 < this->yPosition + this->height;
	int var6 = this->getHoverState(var5);
	this->drawTexturedModalRect(this->xPosition, this->yPosition, 0, 46 + var6 * 20, this->width / 2, this->height);
	this->drawTexturedModalRect(this->xPosition + this->width / 2, this->yPosition, 200 - this->width / 2, 46 + var6 * 20, this->width / 2, this->height);
	this->mouseDragged(var1, var2, var3);
	if(!this->enabled) {
	this->drawCenteredString(var4, this->displayString, this->xPosition + this->width / 2, this->yPosition + (this->height - 8) / 2, -6250336);
	} else if(var5) {
	this->drawCenteredString(var4, this->displayString, this->xPosition + this->width / 2, this->yPosition + (this->height - 8) / 2, 16777120);
	} else {
	this->drawCenteredString(var4, this->displayString, this->xPosition + this->width / 2, this->yPosition + (this->height - 8) / 2, 14737632);
	}
	}
}

void GuiButton::mouseDragged(Minecraft* var1, int var2, int var3)
{
}

void GuiButton::mouseReleased(int var1, int var2)
{
}

bool GuiButton::mousePressed(Minecraft* var1, int var2, int var3)
{
	return this->enabled && var2 >= this->xPosition && var3 >= this->yPosition && var2 < this->xPosition + this->width && var3 < this->yPosition + this->height;
}

void GuiButton::initStatics()
{
}
