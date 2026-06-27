#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from RenderBiped.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderBiped.h"
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
#include "EntityLiving.h"
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
#include "RenderLiving.h"
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

RenderBiped::RenderBiped(ModelBiped* var1, float var2)
	: RenderLiving(var1, var2)
	, modelBipedMain(NULL)
{
	this->modelBipedMain = var1;
}

RenderBiped::~RenderBiped()
{
}

void RenderBiped::renderEquippedItems(EntityLiving* var1, float var2)
{
	ItemStack* var3 = var1->getHeldItem();
	if(var3 != NULL) {
	GL11::glPushMatrix();
	this->modelBipedMain->bipedRightArm->postRender(1.0F / 16.0F);
	GL11::glTranslatef(-(1.0F / 16.0F), 7.0F / 16.0F, 1.0F / 16.0F);
	float var4;
	if(var3->itemID < 256 && RenderBlocks::canRenderBlockAsItem3D(Block::s_blocksList[var3->itemID]->getRenderType())) {
	var4 = 0.5F;
	GL11::glTranslatef(0.0F, 3.0F / 16.0F, -(5.0F / 16.0F));
	var4 *= 12.0F / 16.0F;
	GL11::glRotatef(20.0F, 1.0F, 0.0F, 0.0F);
	GL11::glRotatef(45.0F, 0.0F, 1.0F, 0.0F);
	GL11::glScalef(var4, -var4, var4);
	} else if(Item::s_itemsList[var3->itemID]->isFull3D()) {
	var4 = 10.0F / 16.0F;
	GL11::glTranslatef(0.0F, 3.0F / 16.0F, 0.0F);
	GL11::glScalef(var4, -var4, var4);
	GL11::glRotatef(-100.0F, 1.0F, 0.0F, 0.0F);
	GL11::glRotatef(45.0F, 0.0F, 1.0F, 0.0F);
	} else {
	var4 = 6.0F / 16.0F;
	GL11::glTranslatef(0.25F, 3.0F / 16.0F, -(3.0F / 16.0F));
	GL11::glScalef(var4, var4, var4);
	GL11::glRotatef(60.0F, 0.0F, 0.0F, 1.0F);
	GL11::glRotatef(-90.0F, 1.0F, 0.0F, 0.0F);
	GL11::glRotatef(20.0F, 0.0F, 0.0F, 1.0F);
	}
	this->renderManager->itemRenderer->renderItem(var3);
	GL11::glPopMatrix();
	}
}

void RenderBiped::initStatics()
{
}
