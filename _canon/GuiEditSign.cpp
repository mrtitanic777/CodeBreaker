#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from GuiEditSign.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiEditSign.h"
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
#include "TileEntityRenderer.h"
#include "TileEntitySign.h"
#include "TileEntitySpecialRenderer.h"
#include "Timer.h"
#include "URL.h"
#include "Vec3D.h"
#include "World.h"
#include "WorldChunkManager.h"
#include "WorldProvider.h"
#include "WorldRenderer.h"

GuiEditSign::GuiEditSign(TileEntitySign* var1)
	: entitySign(NULL)
	, updateCounter(0)
	, editLine(0)
{
	this->screenTitle = "Edit sign message:";
	this->editLine = 0;
	this->entitySign = var1;
}

GuiEditSign::~GuiEditSign()
{
}

void GuiEditSign::initGui()
{
	this->controlList.clear();
	Keyboard::enableRepeatEvents(true);
	this->controlList.push_back(new GuiButton(0,this->width / 2 - 100,this->height / 4 + 120,"Done"));
}

void GuiEditSign::onGuiClosed()
{
	Keyboard::enableRepeatEvents(false);
}

void GuiEditSign::updateScreen()
{
	++this->updateCounter;
}

void GuiEditSign::actionPerformed(GuiButton* var1)
{
	if(var1->enabled) {
	if(var1->id == 0) {
	this->entitySign->onInventoryChanged();
	this->mc->displayGuiScreen((GuiScreen*)NULL);
	}
	}
}

void GuiEditSign::keyTyped(char var1, int var2)
{
	if(var2 == 200) {
	this->editLine = (this->editLine - 1) & 3;
	}
	if(var2 == 208 || var2 == 28) {
	this->editLine = (this->editLine + 1) & 3;
	}
	if(var2 == 14 && this->entitySign->signText[this->editLine].length() > 0) {
	this->entitySign->signText[this->editLine] = this->entitySign->signText[this->editLine].substring(0, this->entitySign->signText[this->editLine].length() - 1);
	}
	if(JavaString(" !\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_\'abcdefghijklmnopqrstuvwxyz{|}~\x7f\u00c7\u00fc\u00e9\u00e2\u00e4\u00e0\u00e5\u00e7\u00ea\u00eb\u00e8\u00ef\u00ee\u00ec\u00c4\u00c5\u00c9\u00e6\u00c6\u00f4\u00f6\u00f2\u00fb\u00f9\u00ff\u00d6\u00dc\u00f8\u00a3\u00d8\u00d7\u0192\u00e1\u00ed\u00f3\u00fa\u00f1\u00d1\u00aa\u00ba\u00bf\u00ae\u00ac\u00bd\u00bc\u00a1\u00ab\u00bb").indexOf(var1) >= 0 && this->entitySign->signText[this->editLine].length() < 15) {
	this->entitySign->signText[this->editLine] = this->entitySign->signText[this->editLine] + var1;
	}
}

void GuiEditSign::drawScreen(int var1, int var2, float var3)
{
	this->drawDefaultBackground();
	this->drawCenteredString(this->fontRenderer, this->screenTitle, this->width / 2, 40, 16777215);
	GL11::glPushMatrix();
	GL11::glTranslatef((float)(this->width / 2), (float)(this->height / 2), 50.0F);
	float var4 = 93.75F;
	GL11::glScalef(-var4, -var4, -var4);
	GL11::glRotatef(180.0F, 0.0F, 1.0F, 0.0F);
	Block* var5 = this->entitySign->getBlockType();
	if(var5 == Block::s_signPost) {
	float var6 = (float)(this->entitySign->getBlockMetadata() * 360) / 16.0F;
	GL11::glRotatef(var6, 0.0F, 1.0F, 0.0F);
	GL11::glTranslatef(0.0F, 5.0F / 16.0F, 0.0F);
	} else {
	int var8 = this->entitySign->getBlockMetadata();
	float var7 = 0.0F;
	if(var8 == 2) {
	var7 = 180.0F;
	}
	if(var8 == 4) {
	var7 = 90.0F;
	}
	if(var8 == 5) {
	var7 = -90.0F;
	}
	GL11::glRotatef(var7, 0.0F, 1.0F, 0.0F);
	GL11::glTranslatef(0.0F, 5.0F / 16.0F, 0.0F);
	}
	if(this->updateCounter / 6 % 2 == 0) {
	this->entitySign->lineBeingEdited = this->editLine;
	}
	TileEntityRenderer::s_instance->renderTileEntityAt(this->entitySign, -0.5, -0.75, -0.5, 0.0F);
	this->entitySign->lineBeingEdited = -1;
	GL11::glPopMatrix();
	GuiScreen::drawScreen(var1, var2, var3);
}

void GuiEditSign::initStatics()
{
}
