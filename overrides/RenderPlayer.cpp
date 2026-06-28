#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from RenderPlayer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderPlayer.h"
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
#include "ItemArmor.h"
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

static JavaString _RenderPlayer_s_armorFilenamePrefix_data[] = { "cloth", "chain", "iron", "diamond", "gold" };
JArray<JavaString > RenderPlayer::s_armorFilenamePrefix(_RenderPlayer_s_armorFilenamePrefix_data, 5);

RenderPlayer::RenderPlayer()
	: RenderLiving(new ModelBiped(0.0F), 0.5F)
	, modelBipedMain(NULL)
	, modelArmorChestplate(NULL)
	, modelArmor(NULL)
{
	this->modelBipedMain = (ModelBiped*)this->e;
	this->modelArmorChestplate = new ModelBiped(1.0F);
	this->modelArmor = new ModelBiped(0.5F);
}

RenderPlayer::~RenderPlayer()
{
}

bool RenderPlayer::a(EntityPlayer* var1, int var2)
{
	ItemStack* var3 = var1->inventory->armorItemInSlot(3 - var2);
	if(var3 != NULL) {
	Item* var4 = var3->getItem();
	if((dynamic_cast<ItemArmor*>(var4) != NULL)) {
	ItemArmor* var5 = (ItemArmor*)var4;
	this->loadTexture((JCat() + "/armor/" + s_armorFilenamePrefix[var5->renderIndex] + "_" + (var2 == 2 ? 2 : 1) + ".png"));
	ModelBiped* var6 = var2 == 2 ? this->modelArmor : this->modelArmorChestplate;
	var6->bipedHead->showModel = var2 == 0;
	var6->bipedHeadwear->showModel = var2 == 0;
	var6->bipedBody->showModel = var2 == 1 || var2 == 2;
	var6->bipedRightArm->showModel = var2 == 1;
	var6->bipedLeftArm->showModel = var2 == 1;
	var6->bipedRightLeg->showModel = var2 == 2 || var2 == 3;
	var6->bipedLeftLeg->showModel = var2 == 2 || var2 == 3;
	this->setRenderPassModel(var6);
	return true;
	}
	}
	return false;
}

void RenderPlayer::a(EntityPlayer* var1, double var2, double var4, double var6, float var8, float var9)
{
	// render (RenderLiving::a); a "post-super" with no "done" = crash in the nametag render below.
	ItemStack* var10 = var1->inventory->getCurrentItem();
	this->modelArmorChestplate->heldItemRight = this->modelArmor->heldItemRight = this->modelBipedMain->heldItemRight = var10 != NULL;
	this->modelArmorChestplate->isSneak = this->modelArmor->isSneak = this->modelBipedMain->isSneak = var1->isSneaking();
	double var11 = var4 - (double)var1->yOffset;
	if(var1->isSneakingRender) {
	var11 -= 0.125;
	}
	RenderLiving::a(var1, var2, var11, var6, var8, var9);
	this->modelArmorChestplate->isSneak = this->modelArmor->isSneak = this->modelBipedMain->isSneak = false;
	this->modelArmorChestplate->heldItemRight = this->modelArmor->heldItemRight = this->modelBipedMain->heldItemRight = false;
	float var13 = 1.6F;
	float var14 = (float)(1.0 / 60.0) * var13;
	float var15 = var1->getDistanceToEntity(this->renderManager->livingPlayer);
	float var16 = var1->isSneaking() ? 32.0F : 64.0F;
	if(var15 < var16) {
	var14 = (float)((double)var14 * (sqrtf((double)var15) / 2.0));
	FontRenderer* var17 = this->getFontRendererFromRenderManager();
	GL11::glPushMatrix();
	GL11::glTranslatef((float)var2 + 0.0F, (float)var4 + 2.3F, (float)var6);
	GL11::glNormal3f(0.0F, 1.0F, 0.0F);
	GL11::glRotatef(-this->renderManager->playerViewYaw, 0.0F, 1.0F, 0.0F);
	GL11::glRotatef(this->renderManager->playerViewPitch, 1.0F, 0.0F, 0.0F);
	GL11::glScalef(-var14, -var14, var14);
	JavaString var18 = var1->field_771_i;
	GL11::glDisable(GL11::GL_LIGHTING);
	Tessellator* var19;
	int var20;
	if(!var1->isSneaking()) {
	GL11::glDepthMask(false);
	GL11::glDisable(GL11::GL_DEPTH_TEST);
	GL11::glEnable(GL11::GL_BLEND);
	GL11::glBlendFunc(GL11::GL_SRC_ALPHA, GL11::GL_ONE_MINUS_SRC_ALPHA);
	var19 = Tessellator::s_instance;
	GL11::glDisable(GL11::GL_TEXTURE_2D);
	var19->startDrawingQuads();
	var20 = var17->getStringWidth(var18) / 2;
	var19->setColorRGBA_F(0.0F, 0.0F, 0.0F, 0.25F);
	var19->addVertex((double)(-var20 - 1), -1.0, 0.0);
	var19->addVertex((double)(-var20 - 1), 8.0, 0.0);
	var19->addVertex((double)(var20 + 1), 8.0, 0.0);
	var19->addVertex((double)(var20 + 1), -1.0, 0.0);
	var19->draw();
	GL11::glEnable(GL11::GL_TEXTURE_2D);
	var17->drawString(var18, -var17->getStringWidth(var18) / 2, 0, 553648127);
	GL11::glEnable(GL11::GL_DEPTH_TEST);
	GL11::glDepthMask(true);
	var17->drawString(var18, -var17->getStringWidth(var18) / 2, 0, -1);
	GL11::glEnable(GL11::GL_LIGHTING);
	GL11::glDisable(GL11::GL_BLEND);
	GL11::glColor4f(1.0F, 1.0F, 1.0F, 1.0F);
	GL11::glPopMatrix();
	} else {
	GL11::glTranslatef(0.0F, 0.25F / var14, 0.0F);
	GL11::glDepthMask(false);
	GL11::glEnable(GL11::GL_BLEND);
	GL11::glBlendFunc(GL11::GL_SRC_ALPHA, GL11::GL_ONE_MINUS_SRC_ALPHA);
	var19 = Tessellator::s_instance;
	GL11::glDisable(GL11::GL_TEXTURE_2D);
	var19->startDrawingQuads();
	var20 = var17->getStringWidth(var18) / 2;
	var19->setColorRGBA_F(0.0F, 0.0F, 0.0F, 0.25F);
	var19->addVertex((double)(-var20 - 1), -1.0, 0.0);
	var19->addVertex((double)(-var20 - 1), 8.0, 0.0);
	var19->addVertex((double)(var20 + 1), 8.0, 0.0);
	var19->addVertex((double)(var20 + 1), -1.0, 0.0);
	var19->draw();
	GL11::glEnable(GL11::GL_TEXTURE_2D);
	GL11::glDepthMask(true);
	var17->drawString(var18, -var17->getStringWidth(var18) / 2, 0, 553648127);
	GL11::glEnable(GL11::GL_LIGHTING);
	GL11::glDisable(GL11::GL_BLEND);
	GL11::glColor4f(1.0F, 1.0F, 1.0F, 1.0F);
	GL11::glPopMatrix();
	}
	}
}

void RenderPlayer::a(EntityPlayer* var1, float var2)
{
	ItemStack* var3 = var1->inventory->armorItemInSlot(3);
	// CONSOLE guard: getItem() can be NULL (unregistered id) and s_blocksList[itemID] can be NULL;
	// the virtual calls below would otherwise read a vtable at 0x0 (the IP-0 crash class).
	if(var3 != NULL && var3->getItem() != NULL && var3->getItem()->shiftedIndex < 256 && Block::s_blocksList[var3->itemID] != NULL) {
	GL11::glPushMatrix();
	this->modelBipedMain->bipedHead->postRender(1.0F / 16.0F);
	if(RenderBlocks::canRenderBlockAsItem3D(Block::s_blocksList[var3->itemID]->getRenderType())) {
	float var4 = 10.0F / 16.0F;
	GL11::glTranslatef(0.0F, -0.25F, 0.0F);
	GL11::glRotatef(180.0F, 0.0F, 1.0F, 0.0F);
	GL11::glScalef(var4, -var4, var4);
	}
	this->renderManager->itemRenderer->renderItem(var3);
	GL11::glPopMatrix();
	}
	ItemStack* var6 = var1->inventory->getCurrentItem();
	// CONSOLE guard: an id mapping to an unregistered (NULL) block/item slot would make the
	// getRenderType()/isFull3D() virtual calls below read a vtable at 0x0 (IP 0x0 crash). Skip it.
	if(var6 != NULL && (var6->itemID < 256 ? Block::s_blocksList[var6->itemID] != NULL : Item::s_itemsList[var6->itemID] != NULL)) {
	GL11::glPushMatrix();
	this->modelBipedMain->bipedRightArm->postRender(1.0F / 16.0F);
	GL11::glTranslatef(-(1.0F / 16.0F), 7.0F / 16.0F, 1.0F / 16.0F);
	if(var1->fishEntity != NULL) {
	var6 = new ItemStack(Item::s_stick->shiftedIndex);
	}
	float var5;
	if(var6->itemID < 256 && RenderBlocks::canRenderBlockAsItem3D(Block::s_blocksList[var6->itemID]->getRenderType())) {
	var5 = 0.5F;
	GL11::glTranslatef(0.0F, 3.0F / 16.0F, -(5.0F / 16.0F));
	var5 *= 12.0F / 16.0F;
	GL11::glRotatef(20.0F, 1.0F, 0.0F, 0.0F);
	GL11::glRotatef(45.0F, 0.0F, 1.0F, 0.0F);
	GL11::glScalef(var5, -var5, var5);
	} else if(Item::s_itemsList[var6->itemID]->isFull3D()) {
	var5 = 10.0F / 16.0F;
	if(Item::s_itemsList[var6->itemID]->shouldRotateAroundWhenRendering()) {
	GL11::glRotatef(180.0F, 0.0F, 0.0F, 1.0F);
	GL11::glTranslatef(0.0F, -(2.0F / 16.0F), 0.0F);
	}
	GL11::glTranslatef(0.0F, 3.0F / 16.0F, 0.0F);
	GL11::glScalef(var5, -var5, var5);
	GL11::glRotatef(-100.0F, 1.0F, 0.0F, 0.0F);
	GL11::glRotatef(45.0F, 0.0F, 1.0F, 0.0F);
	} else {
	var5 = 6.0F / 16.0F;
	GL11::glTranslatef(0.25F, 3.0F / 16.0F, -(3.0F / 16.0F));
	GL11::glScalef(var5, var5, var5);
	GL11::glRotatef(60.0F, 0.0F, 0.0F, 1.0F);
	GL11::glRotatef(-90.0F, 1.0F, 0.0F, 0.0F);
	GL11::glRotatef(20.0F, 0.0F, 0.0F, 1.0F);
	}
	this->renderManager->itemRenderer->renderItem(var6);
	GL11::glPopMatrix();
	}
}

void RenderPlayer::b(EntityPlayer* var1, float var2)
{
	float var3 = 15.0F / 16.0F;
	GL11::glScalef(var3, var3, var3);
}

void RenderPlayer::drawFirstPersonHand()
{
	this->modelBipedMain->onGround = 0.0F;
	this->modelBipedMain->setRotationAngles(0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 1.0F / 16.0F);
	this->modelBipedMain->bipedRightArm->render(1.0F / 16.0F);
}

void RenderPlayer::preRenderCallback(EntityLiving* var1, float var2)
{
	this->b((EntityPlayer*)var1, var2);
}

bool RenderPlayer::shouldRenderPass(EntityLiving* var1, int var2)
{
	return this->a((EntityPlayer*)var1, var2);
}

void RenderPlayer::renderEquippedItems(EntityLiving* var1, float var2)
{
	this->a((EntityPlayer*)var1, var2);
}

void RenderPlayer::a(EntityLiving* var1, double var2, double var4, double var6, float var8, float var9)
{
	this->a((EntityPlayer*)var1, var2, var4, var6, var8, var9);
}

void RenderPlayer::doRender(Entity* var1, double var2, double var4, double var6, float var8, float var9)
{
	this->a((EntityPlayer*)var1, var2, var4, var6, var8, var9);
}

void RenderPlayer::initStatics()
{
}
