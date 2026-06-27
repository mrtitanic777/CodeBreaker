#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from ThreadDownloadResources.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ThreadDownloadResources.h"
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
#include "FileOutputStream.h"
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
#include "RuntimeException.h"
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
#include "TileEntity.h"
#include "Timer.h"
#include "URL.h"
#include "Vec3D.h"
#include "World.h"
#include "WorldChunkManager.h"
#include "WorldProvider.h"
#include "WorldRenderer.h"

/* ---- ORIGINAL JAVA (stubbed below) ----
 *  import javax.xml.parsers.DocumentBuilder;
 *  import javax.xml.parsers.DocumentBuilderFactory;
 *  import org.w3c.dom.Document;
 *  import org.w3c.dom.Element;
 *  import org.w3c.dom.Node;
 *  import org.w3c.dom.NodeList;
 *  // The real DOM parse drives run(); these types here are no-op shims so it parses nothing:
 *  DocumentBuilderFactory var2 = DocumentBuilderFactory.newInstance();
 *  DocumentBuilder var3 = var2.newDocumentBuilder();
 *  Document var4 = var3.parse(var1.openStream());
 *  NodeList var5 = var4.getElementsByTagName("Contents");
 *  // ... node.getNodeType(), element.getElementsByTagName("Key"/"Size"),
 *  //     node.getChildNodes().item(0).getNodeValue() all return empty/zero in the shim.
 *  ---- end original Java ---- */
// --- Local runtime shims for the org.w3c.dom / javax.xml.parsers XML API used by
//     run(). These are not provided as central runtime headers. They are minimal
//     stubs (the real parse is a no-op on this platform). See "runtime-stub gaps".
namespace {
class Node {
public:
	virtual ~Node() {}
	virtual int getNodeType() { return 0; }
	virtual JavaString getNodeValue() { return JavaString(); }
	virtual struct NodeList getChildNodes();
};
struct NodeList {
	Node* item(int) { return 0; }
	int getLength() { return 0; }
};
inline NodeList Node::getChildNodes() { return NodeList(); }
class Element : public Node {
public:
	NodeList getElementsByTagName(const JavaString&) { return NodeList(); }
};
class Document {
public:
	virtual ~Document() {}
	NodeList* getElementsByTagName(const JavaString&) { return new NodeList(); }
};
class DocumentBuilder {
public:
	virtual ~DocumentBuilder() {}
	Document* parse(void*) { return new Document(); }
};
struct DocumentBuilderFactory {
	DocumentBuilder* newDocumentBuilder() { return new DocumentBuilder(); }
	static DocumentBuilderFactory* newInstance() { return new DocumentBuilderFactory(); }
};
}

ThreadDownloadResources::ThreadDownloadResources(File* var1, Minecraft* var2)
	: resourcesFolder(NULL)
	, mc(NULL)
	, closing(0)
{
	this->mc = var2;
	this->setName("Resource download thread");
	this->setDaemon(true);
	this->resourcesFolder = new File(var1, "resources/");
	if(!this->resourcesFolder->exists() && !this->resourcesFolder->mkdirs()) {
	JavaString _msg = (JCat() + "The working directory could not be created: " + this->resourcesFolder);
	throw new RuntimeException(_msg);
	}
}

ThreadDownloadResources::~ThreadDownloadResources()
{
	delete resourcesFolder;
	resourcesFolder = NULL;
}

void ThreadDownloadResources::run()
{
	try {
	URL* var1 = new URL("http://s3.amazonaws.com/MinecraftResources/");
	DocumentBuilderFactory* var2 = DocumentBuilderFactory::newInstance();
	DocumentBuilder* var3 = var2->newDocumentBuilder();
	Document* var4 = var3->parse(var1->openStream());
	NodeList* var5 = var4->getElementsByTagName("Contents");
	for(int var6 = 0; var6 < 2; ++var6) {
	for(int var7 = 0; var7 < var5->getLength(); ++var7) {
	Node* var8 = var5->item(var7);
	if(var8->getNodeType() == 1) {
	Element* var9 = (Element*)var8;
	JavaString var10 = ((Element*)var9->getElementsByTagName("Key").item(0))->getChildNodes().item(0)->getNodeValue();
	long long var11 = Long::parseLong(((Element*)var9->getElementsByTagName("Size").item(0))->getChildNodes().item(0)->getNodeValue());
	if(var11 > 0L) {
	this->downloadAndInstallResource(var1, var10, var11, var6);
	if(this->closing) {
	return;
	}
	}
	}
	}
	}
	} catch (Exception var13) {
	this->loadResource(this->resourcesFolder, "");
	var13.printStackTrace();
	}
}

void ThreadDownloadResources::reloadResources()
{
	this->loadResource(this->resourcesFolder, "");
}

void ThreadDownloadResources::loadResource(File* var1, const JavaString& var2)
{
	JArray<File* > var3 = JArray<File* >((File**)var1->listFiles(), 0);
	for(int var4 = 0; var4 < var3.length; ++var4) {
	if(var3[var4]->isDirectory()) {
	this->loadResource(var3[var4],(JCat() + var2 + var3[var4]->getName() + "/"));
	} else {
	try {
	this->mc->installResource(var2 + var3[var4]->getName(), var3[var4]);
	} catch (Exception var6) {
	System::out.println((JCat() + "Failed to add " + var2 + var3[var4]->getName()));
	}
	}
	}
}

void ThreadDownloadResources::downloadAndInstallResource(URL* var1, const JavaString& var2, long long var3, int var5)
{
	try {
	int var6 = var2.indexOf("/");
	JavaString var7 = var2.substr(0, (var6) - (0));
	if(!(var7 == "sound") && !(var7 == "newsound")) {
	if(var5 != 1) {
	return;
	}
	} else if(var5 != 0) {
	return;
	}
	File* var8 = new File(this->resourcesFolder, var2);
	if(!var8->exists() || var8->length() != var3) {
	var8->getParentFile()->mkdirs();
	JavaString var9 = var2.replaceAll(" ", "%20");
	this->downloadResource(new URL(var1, var9), var8, var3);
	if(this->closing) {
	return;
	}
	}
	this->mc->installResource(var2, var8);
	} catch (Exception var10) {
	var10.printStackTrace();
	}
}

void ThreadDownloadResources::downloadResource(URL* var1, File* var2, long long var3)
{
	JArray<unsigned char > var5 = JArray<unsigned char >(4096);
	DataInputStream* var6 = new DataInputStream(var1->openStream());
	DataOutputStream* var7 = new DataOutputStream(new FileOutputStream(var2));
	bool var8 = false;
	do {
	int var9 = var6->read(var5);
	if(var9 < 0) {
	var6->close();
	var7->close();
	return;
	}
	var7->write(var5, 0, var9);
	} while(!this->closing);
}

void ThreadDownloadResources::closeMinecraft()
{
	this->closing = true;
}
