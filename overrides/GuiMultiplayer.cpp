#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from GuiMultiplayer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiMultiplayer.h"
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
#include "ChunkProviderClient.h"
#include "Class.h"
#include "Component.h"
#include "Cursor.h"
#include "DataInputStream.h"
#include "DataOutputStream.h"
#include "EffectRenderer.h"
#include "Entity.h"
#include "EntityFish.h"
#include "EntityItem.h"
#include "EntityPlayer.h"
#include "EntityPlayerSP.h"
#include "EntityRenderer.h"
#include "EnumOS2.h"
#include "Exception.h"
#include "Explosion.h"
#include "File.h"
#include "FloatBuffer.h"
#include "FontRenderer.h"
#include "GameSettings.h"
#include "GuiButton.h"
#include "GuiConnecting.h"
#include "OSK.h"            // CONSOLE: system on-screen keyboard for the server-address field
#include "GuiIngame.h"
#include "GuiScreen.h"
#include "HashEntry.h"
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
#include "MCHashTable.h"
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
#include "NetClientHandler.h"
#include "NetHandler.h"
#include "NetworkManager.h"
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
#include "Socket.h"
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
#include "WorldClient.h"
#include "WorldProvider.h"
#include "WorldRenderer.h"

GuiMultiplayer::GuiMultiplayer(GuiScreen* var1)
	: updateCounter(NULL)
	, parentScreen(0)
{
	this->parentScreen = 0;
	this->serverAddress = "";
	this->updateCounter = var1;
}

GuiMultiplayer::~GuiMultiplayer()
{
}

void GuiMultiplayer::updateScreen()
{
	++this->parentScreen;
	// CONSOLE: drive the async on-screen keyboard; when the user finishes, take the typed address.
	std::string _ip;
	if(OSK::poll(_ip)) {
	if(!_ip.empty()) { this->serverAddress = _ip; }
	((GuiButton*)this->controlList[0])->enabled = this->serverAddress.length() > 0;
	}
}

void GuiMultiplayer::initGui()
{
	this->controlList.clear();
	this->controlList.push_back(new GuiButton(0,this->width / 2 - 100,this->height / 4 + 96 + 12,"Connect"));
	this->controlList.push_back(new GuiButton(1,this->width / 2 - 100,this->height / 4 + 120 + 12,"Cancel"));
	// CONSOLE: no PC keyboard -> a button + the system OSK to enter the server address with the controller.
	this->controlList.push_back(new GuiButton(2,this->width / 2 - 100,this->height / 4 + 72 + 12,"Edit Address"));
	this->serverAddress = this->mc->gameSettings->lastServer.replaceAll("_", ":");
	((GuiButton*)this->controlList[0])->enabled = this->serverAddress.length() > 0;
	OSK::show(this->serverAddress, L"Server Address", L"Enter server IP (e.g. 192.168.1.5 or host:port)", 64);
}

void GuiMultiplayer::actionPerformed(GuiButton* var1)
{
	if(var1->id == 2) {   // CONSOLE: re-open the on-screen keyboard to edit the address (allowed even if Connect disabled)
	OSK::show(this->serverAddress, L"Server Address", L"Enter server IP (e.g. 192.168.1.5 or host:port)", 64);
	return;
	}
	if(var1->enabled) {
	if(var1->id == 1) {
	this->mc->displayGuiScreen(this->updateCounter);
	} else if(var1->id == 0) {
	this->mc->gameSettings->lastServer = this->serverAddress.replaceAll(":", "_");
	this->mc->gameSettings->saveOptions();
	JArray<JavaString > var2 = this->serverAddress.split(":");
	this->mc->displayGuiScreen(new GuiConnecting(this->mc, var2[0], var2.length > 1 ? this->getBiomeAt(var2[1], 25565) : 25565));
	}
	}
}

int GuiMultiplayer::getBiomeAt(const JavaString& var1, int var2)
{
	try {
	return Integer::parseInt(var1.trim());
	} catch (Exception*) {
	return var2;
	}
}

void GuiMultiplayer::keyTyped(char var1, int var2)
{
	if(var1 == 22) {
	JavaString var3 = GuiScreen::getClipboardString();
	if(var3 == NULL) {
	var3 = "";
	}
	int var4 = 32 - this->serverAddress.length();
	if(var4 > var3.length()) {
	var4 = var3.length();
	}
	if(var4 > 0) {
	this->serverAddress = this->serverAddress + var3.substr(0, (var4) - (0));
	}
	}
	if(var1 == 13) {
	this->actionPerformed((GuiButton*)this->controlList[0]);
	}
	if(var2 == 14 && this->serverAddress.length() > 0) {
	this->serverAddress = this->serverAddress.substr(0, (this->serverAddress.length() - 1) - (0));
	}
	if(JavaString(" !\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_\'abcdefghijklmnopqrstuvwxyz{|}~\x7f\u00c7\u00fc\u00e9\u00e2\u00e4\u00e0\u00e5\u00e7\u00ea\u00eb\u00e8\u00ef\u00ee\u00ec\u00c4\u00c5\u00c9\u00e6\u00c6\u00f4\u00f6\u00f2\u00fb\u00f9\u00ff\u00d6\u00dc\u00f8\u00a3\u00d8\u00d7\u0192\u00e1\u00ed\u00f3\u00fa\u00f1\u00d1\u00aa\u00ba\u00bf\u00ae\u00ac\u00bd\u00bc\u00a1\u00ab\u00bb").indexOf(var1) >= 0 && this->serverAddress.length() < 32) {
	this->serverAddress = this->serverAddress + var1;
	}
	((GuiButton*)this->controlList[0])->enabled = this->serverAddress.length() > 0;
}

void GuiMultiplayer::drawScreen(int var1, int var2, float var3)
{
	this->drawDefaultBackground();
	this->drawCenteredString(this->fontRenderer,"Play Multiplayer",this->width / 2,this->height / 4 - 60 + 20,16777215);
	this->drawString(this->fontRenderer,"Minecraft Multiplayer is currently not finished, but there",this->width / 2 - 140,this->height / 4 - 60 + 60 + 0,10526880);
	this->drawString(this->fontRenderer,"is some buggy early testing going on.",this->width / 2 - 140,this->height / 4 - 60 + 60 + 9,10526880);
	this->drawString(this->fontRenderer,"Enter the IP of a server to connect to it:",this->width / 2 - 140,this->height / 4 - 60 + 60 + 36,10526880);
	int var4 = this->width / 2 - 100;
	int var5 = this->height / 4 - 10 + 50 + 18;
	short var6 = 200;
	unsigned char var7 = 20;
	this->drawRect(var4 - 1, var5 - 1, var4 + var6 + 1, var5 + var7 + 1, -6250336);
	this->drawRect(var4, var5, var4 + var6, var5 + var7, -16777216);
	this->drawString(this->fontRenderer,this->serverAddress + (this->parentScreen / 6 % 2 == 0 ? "_" : ""),var4 + 4,var5 + (var7 - 8) / 2,14737632);
	GuiScreen::drawScreen(var1, var2, var3);
}

void GuiMultiplayer::initStatics()
{
}
