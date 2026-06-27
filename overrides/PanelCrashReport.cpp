#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from PanelCrashReport.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "PanelCrashReport.h"
#include "AxisAlignedBB.h"
#include "Block.h"
#include "BlockFire.h"
#include "BlockFlower.h"
#include "BlockGrass.h"
#include "BlockLeaves.h"
#include "BlockPortal.h"
#include "BorderLayout.h"
#include "BufferedImage.h"
#include "ByteBuffer.h"
#include "Canvas.h"
#include "CanvasCrashReport.h"
#include "CanvasMajongLogo.h"
#include "Chunk.h"
#include "Class.h"
#include "Color.h"
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
#include "Panel.h"
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
#include "Sys.h"
#include "Tessellator.h"
#include "TextureLavaFX.h"
#include "TexturePackBase.h"
#include "TexturePackList.h"
#include "TextureWaterFX.h"
#include "TexturedQuad.h"
#include "Thread.h"
#include "ThreadDownloadImageData.h"
#include "ThreadDownloadResources.h"
#include "Throwable.h"
#include "TileEntity.h"
#include "Timer.h"
#include "URL.h"
#include "UnexpectedThrowable.h"
#include "Vec3D.h"
#include "World.h"
#include "WorldChunkManager.h"
#include "WorldProvider.h"
#include "WorldRenderer.h"

/* ---- ORIGINAL JAVA (stubbed below) ----
 *  // These java.io/java.text/java.awt types are stubbed by the local shims below.
 *  // Behaviour lost vs. the originals: PrintWriter(StringWriter) does NOT capture
 *  // the stack trace (the shim ignores writes from printStackTrace), and
 *  // SimpleDateFormat.format(Date) returns "" instead of a real timestamp.
 *  import java.awt.BorderLayout;
 *  import java.awt.Color;
 *  import java.awt.Font;
 *  import java.awt.Panel;
 *  import java.awt.TextArea;
 *  import java.io.PrintWriter;
 *  import java.io.StringWriter;
 *  import java.text.SimpleDateFormat;
 *  import java.util.Date;
 *  ---- end original Java ---- */
// --- local shims for java.io / java.text / java.awt classes that have no -----
// runtime header. Minimal faithful behavior so the crash-report panel compiles.
// See report's runtime-stub gaps for the central additions that would remove them.
class StringWriter {
public:
	JavaString _buf;
	StringWriter() {}
	void write(const JavaString& s) { _buf += s; }
	JavaString toString() { return _buf; }
};
class PrintWriter {
public:
	StringWriter* _sw;
	PrintWriter(StringWriter* sw) : _sw(sw) {}
};
class Date {
public:
	Date() {}
};
class SimpleDateFormat {
public:
	SimpleDateFormat() {}
	SimpleDateFormat(const JavaString&) {}
	JavaString format(Date*) { return JavaString(""); }
};
class Font {
public:
	Font(const JavaString&, int, int) {}
};
class TextArea {
public:
	JavaString _text;
	TextArea(const JavaString& text, int, int, int) : _text(text) {}
	void setFont(Font*) {}
};

PanelCrashReport::PanelCrashReport(UnexpectedThrowable* var1)
{
	this->setBackground(new Color(3028036));
	this->setLayout(new BorderLayout());
	/* ---- ORIGINAL JAVA (stubbed below) ----
	 *  // printStackTrace(new PrintWriter(var2)) writes the full stack trace into
	 *  // var2; var3 then holds that text. With the PrintWriter/StringWriter shims
	 *  // nothing is captured, so var3 is empty in the port.
	 *  StringWriter var2 = new StringWriter();
	 *  var1.exception.printStackTrace(new PrintWriter(var2));
	 *  String var3 = var2.toString();
	 *  ---- end original Java ---- */
	StringWriter* var2 = new StringWriter();
	var1->exception->printStackTrace(new PrintWriter(var2));
	JavaString var3 = var2->toString();
	JavaString var4 = "";
	JavaString var5 = "";
	try {
	/* ---- ORIGINAL JAVA (stubbed below) ----
	 *  // (new SimpleDateFormat()).format(new Date()) yields the current date/time
	 *  // string. The SimpleDateFormat shim's format() returns "", so the port
	 *  // prints "Generated \n" with no timestamp.
	 *  var5 = var5 + "Generated " + (new SimpleDateFormat()).format(new Date()) + "\n";
	 *  ---- end original Java ---- */
	var5 = (JCat() + var5 + "Generated " + (new SimpleDateFormat())->format(new Date()) + "\n");
	var5 = (JCat() + var5 + "\n");
	var5 = (JCat() + var5 + "Minecraft: Minecraft Alpha v1.2.6\n");
	var5 = (JCat() + var5 + "OS: " + System::getProperty("os.name") + " (" + System::getProperty("os.arch") + ") version " + System::getProperty("os.version") + "\n");
	var5 = (JCat() + var5 + "Java: " + System::getProperty("java.version") + ", " + System::getProperty("java.vendor") + "\n");
	var5 = (JCat() + var5 + "VM: " + System::getProperty("java.vm.name") + " (" + System::getProperty("java.vm.info") + "), " + System::getProperty("java.vm.vendor") + "\n");
	var5 = (JCat() + var5 + "LWJGL: " + Sys::getVersion() + "\n");
	var4 = GL11::glGetString(GL11::GL_VENDOR);
	var5 = (JCat() + var5 + "OpenGL: " + GL11::glGetString(GL11::GL_RENDERER) + " version " + GL11::glGetString(GL11::GL_VERSION) + ", " + GL11::glGetString(GL11::GL_VENDOR) + "\n");
	} catch (Throwable var8) {
	var5 = (JCat() + var5 + "[failed to get system properties (" + var8.toString() + ")]\n");
	}
	var5 = (JCat() + var5 + "\n");
	var5 = var5 + var3;
	JavaString var6 = "";
	var6 = (JCat() + var6 + "\n");
	var6 = (JCat() + var6 + "\n");
	if(var3.contains("Pixel format not accelerated")) {
	var6 = (JCat() + var6 + "      Bad video card drivers!      \n");
	var6 = (JCat() + var6 + "      -----------------------      \n");
	var6 = (JCat() + var6 + "\n");
	var6 = (JCat() + var6 + "Minecraft was unable to start because it failed to find an accelerated OpenGL mode.\n");
	var6 = (JCat() + var6 + "This can usually be fixed by updating the video card drivers.\n");
	if(var4.toLowerCase().contains("nvidia")) {
	var6 = (JCat() + var6 + "\n");
	var6 = (JCat() + var6 + "You might be able to find drivers for your video card here:\n");
	var6 = (JCat() + var6 + "  http://www.nvidia.com/\n");
	} else if(var4.toLowerCase().contains("ati")) {
	var6 = (JCat() + var6 + "\n");
	var6 = (JCat() + var6 + "You might be able to find drivers for your video card here:\n");
	var6 = (JCat() + var6 + "  http://www.amd.com/\n");
	}
	} else {
	var6 = (JCat() + var6 + "      Minecraft has crashed!      \n");
	var6 = (JCat() + var6 + "      ----------------------      \n");
	var6 = (JCat() + var6 + "\n");
	var6 = (JCat() + var6 + "Minecraft has stopped running because it encountered a problem.\n");
	var6 = (JCat() + var6 + "\n");
	var6 = (JCat() + var6 + "If you wish to report this, please copy this entire text and email it to support@mojang.com.\n");
	var6 = (JCat() + var6 + "Please include a description of what you did when the error occured.\n");
	}
	var6 = (JCat() + var6 + "\n");
	var6 = (JCat() + var6 + "\n");
	var6 = (JCat() + var6 + "\n");
	var6 = (JCat() + var6 + "--- BEGIN ERROR REPORT " + Integer::toHexString(var6.hashCode()) + " --------\n");
	var6 = var6 + var5;
	var6 = (JCat() + var6 + "--- END ERROR REPORT " + Integer::toHexString(var6.hashCode()) + " ----------\n");
	var6 = (JCat() + var6 + "\n");
	var6 = (JCat() + var6 + "\n");
	TextArea* var7 = new TextArea(var6, 0, 0, 1);
	var7->setFont(new Font("Monospaced", 0, 12));
	this->add(new CanvasMajongLogo(), "North");
	this->add(new CanvasCrashReport(80), "East");
	this->add(new CanvasCrashReport(80), "West");
	this->add(new CanvasCrashReport(100), "South");
	this->add(var7, "Center");
}

PanelCrashReport::~PanelCrashReport()
{
}
