#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from TileEntityMobSpawnerRenderer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "TileEntityMobSpawnerRenderer.h"
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
#include "EntityList.h"
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
#include "TileEntityMobSpawner.h"
#include "TileEntityRenderer.h"
#include "TileEntitySpecialRenderer.h"
#include "Timer.h"
#include "URL.h"
#include "Vec3D.h"
#include "World.h"
#include "WorldChunkManager.h"
#include "WorldProvider.h"
#include "WorldRenderer.h"

TileEntityMobSpawnerRenderer::TileEntityMobSpawnerRenderer()
{
	this->cachedEntities = std::map<JObject, JObject>();
}

TileEntityMobSpawnerRenderer::~TileEntityMobSpawnerRenderer()
{
}

void TileEntityMobSpawnerRenderer::a(TileEntityMobSpawner* var1, double var2, double var4, double var6, float var8)
{
	GL11::glPushMatrix();
	GL11::glTranslatef((float)var2 + 0.5F, (float)var4, (float)var6 + 0.5F);
	Entity* var9 = (Entity*)this->cachedEntities[var1->entityID];
	if(var9 == NULL) {
	var9 = EntityList::createEntityInWorld(var1->entityID, (World*)NULL);
	this->cachedEntities[var1->entityID] = var9;
	}
	if(var9 != NULL) {
	var9->setWorld(var1->worldObj);
	float var10 = 7.0F / 16.0F;
	GL11::glTranslatef(0.0F, 0.4F, 0.0F);
	GL11::glRotatef((float)(var1->prevSpinAngle + (var1->spinAngle - var1->prevSpinAngle) * (double)var8) * 10.0F, 0.0F, 1.0F, 0.0F);
	GL11::glRotatef(-30.0F, 1.0F, 0.0F, 0.0F);
	GL11::glTranslatef(0.0F, -0.4F, 0.0F);
	GL11::glScalef(var10, var10, var10);
	var9->setLocationAndAngles(var2, var4, var6, 0.0F, 0.0F);
	RenderManager::s_instance->doRenderEntity(var9, 0.0, 0.0, 0.0, 0.0F, var8);
	}
	GL11::glPopMatrix();
}

void TileEntityMobSpawnerRenderer::renderTileEntityAt(TileEntity* var1, double var2, double var4, double var6, float var8)
{
	this->a((TileEntityMobSpawner*)var1, var2, var4, var6, var8);
}

void TileEntityMobSpawnerRenderer::initStatics()
{
}
