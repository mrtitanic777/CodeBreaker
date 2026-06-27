#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from GuiSlider.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiSlider.h"
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

GuiSlider::GuiSlider(int var1, int var2, int var3, int var4, const JavaString& var5, float var6)
	: GuiButton(var1, var2, var3, 150, 20, var5)
	, sliderValue(0)
	, dragging(0)
	, idFloat(0)
{
	this->sliderValue = 1.0F;
	this->dragging = false;
	this->idFloat = 0;
	this->idFloat = var4;
	this->sliderValue = var6;
}

GuiSlider::~GuiSlider()
{
}

int GuiSlider::getHoverState(bool var1)
{
	return 0;
}

void GuiSlider::mouseDragged(Minecraft* var1, int var2, int var3)
{
	if(this->enabled2) {
	if(this->dragging) {
	this->sliderValue = (float)(var2 - (this->xPosition + 4)) / (float)(this->width - 8);
	if(this->sliderValue < 0.0F) {
	this->sliderValue = 0.0F;
	}
	if(this->sliderValue > 1.0F) {
	this->sliderValue = 1.0F;
	}
	var1->gameSettings->setOptionFloatValue(this->idFloat, this->sliderValue);
	this->displayString = var1->gameSettings->getOptionDisplayString(this->idFloat);
	}
	GL11::glColor4f(1.0F, 1.0F, 1.0F, 1.0F);
	this->drawTexturedModalRect(this->xPosition + (int)(this->sliderValue * (float)(this->width - 8)), this->yPosition, 0, 66, 4, 20);
	this->drawTexturedModalRect(this->xPosition + (int)(this->sliderValue * (float)(this->width - 8)) + 4, this->yPosition, 196, 66, 4, 20);
	}
}

bool GuiSlider::mousePressed(Minecraft* var1, int var2, int var3)
{
	if(GuiButton::mousePressed(var1, var2, var3)) {
	this->sliderValue = (float)(var2 - (this->xPosition + 4)) / (float)(this->width - 8);
	if(this->sliderValue < 0.0F) {
	this->sliderValue = 0.0F;
	}
	if(this->sliderValue > 1.0F) {
	this->sliderValue = 1.0F;
	}
	var1->gameSettings->setOptionFloatValue(this->idFloat, this->sliderValue);
	this->displayString = var1->gameSettings->getOptionDisplayString(this->idFloat);
	this->dragging = true;
	return true;
	} else {
	return false;
	}
}

void GuiSlider::mouseReleased(int var1, int var2)
{
	this->dragging = false;
}

void GuiSlider::initStatics()
{
}
