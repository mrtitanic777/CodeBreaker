package net.minecraft.src;

import java.awt.Canvas;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Rectangle;
import java.awt.RenderingHints;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.awt.geom.AffineTransform;
import java.awt.image.BufferStrategy;
import java.awt.image.ImageObserver;
import java.io.File;
import java.util.Collections;
import java.util.LinkedList;
import java.util.List;

public class CanvasIsomPreview extends Canvas implements KeyListener, MouseListener, MouseMotionListener, Runnable {
	private int frameCount = 0;
	private int zoom = 2;
	private boolean showInfo = true;
	private World worldObj;
	private File dataFolder = this.getDataFolder();
	private boolean running = true;
	private List loadQueue = Collections.synchronizedList(new LinkedList());
	private IsoImageBuffer[][] imageBuffers = new IsoImageBuffer[64][64];
	private int offsetX;
	private int offsetY;
	private int lastMouseX;
	private int lastMouseY;

	public File getDataFolder() {
		if(this.dataFolder == null) {
			this.dataFolder = this.getMinecraftDir("minecraft");
		}

		return this.dataFolder;
	}

	public File getMinecraftDir(String var1) {
		String var2 = System.getProperty("user.home", ".");
		File var3;
		switch(OsMap.osTypeOrdinals[getOSType().ordinal()]) {
		case 1:
		case 2:
			var3 = new File(var2, '.' + var1 + '/');
			break;
		case 3:
			String var4 = System.getenv("APPDATA");
			if(var4 != null) {
				var3 = new File(var4, "." + var1 + '/');
			} else {
				var3 = new File(var2, '.' + var1 + '/');
			}
			break;
		case 4:
			var3 = new File(var2, "Library/Application Support/" + var1);
			break;
		default:
			var3 = new File(var2, var1 + '/');
		}

		if(!var3.exists() && !var3.mkdirs()) {
			throw new RuntimeException("The working directory could not be created: " + var3);
		} else {
			return var3;
		}
	}

	private static EnumOS1 getOSType() {
		String var0 = System.getProperty("os.name").toLowerCase();
		return var0.contains("win") ? EnumOS1.windows : (var0.contains("mac") ? EnumOS1.macos : (var0.contains("solaris") ? EnumOS1.solaris : (var0.contains("sunos") ? EnumOS1.solaris : (var0.contains("linux") ? EnumOS1.linux : (var0.contains("unix") ? EnumOS1.linux : EnumOS1.unknown)))));
	}

	public CanvasIsomPreview() {
		for(int var1 = 0; var1 < 64; ++var1) {
			for(int var2 = 0; var2 < 64; ++var2) {
				this.imageBuffers[var1][var2] = new IsoImageBuffer((World)null, var1, var2);
			}
		}

		this.addMouseListener(this);
		this.addMouseMotionListener(this);
		this.addKeyListener(this);
		this.setFocusable(true);
		this.requestFocus();
		this.setBackground(Color.red);
	}

	public void loadWorld(String var1) {
		this.offsetX = this.offsetY = 0;
		this.worldObj = new WorldIso(this, new File(this.dataFolder, "saves"), var1);
		this.worldObj.skylightSubtracted = 0;
		List var2 = this.loadQueue;
		synchronized(var2) {
			this.loadQueue.clear();

			for(int var3 = 0; var3 < 64; ++var3) {
				for(int var4 = 0; var4 < 64; ++var4) {
					this.imageBuffers[var3][var4].setup(this.worldObj, var3, var4);
				}
			}

		}
	}

	private void setSkylightSubtracted(int var1) {
		List var2 = this.loadQueue;
		synchronized(var2) {
			this.worldObj.skylightSubtracted = var1;
			this.loadQueue.clear();

			for(int var3 = 0; var3 < 64; ++var3) {
				for(int var4 = 0; var4 < 64; ++var4) {
					this.imageBuffers[var3][var4].setup(this.worldObj, var3, var4);
				}
			}

		}
	}

	public void startThreads() {
		(new ThreadRunIsoClient(this)).start();

		for(int var1 = 0; var1 < 8; ++var1) {
			(new Thread(this)).start();
		}

	}

	public void stopThreads() {
		this.running = false;
	}

	private IsoImageBuffer getImageBuffer(int var1, int var2) {
		int var3 = var1 & 63;
		int var4 = var2 & 63;
		IsoImageBuffer var5 = this.imageBuffers[var3][var4];
		if(var5.anotherInt == var1 && var5.anInt == var2) {
			return var5;
		} else {
			List var6 = this.loadQueue;
			synchronized(var6) {
				this.loadQueue.remove(var5);
			}

			var5.resetData(var1, var2);
			return var5;
		}
	}

	public void run() {
		TerrainTextureManager var1 = new TerrainTextureManager();

		while(this.running) {
			IsoImageBuffer var2 = null;
			List var3 = this.loadQueue;
			synchronized(var3) {
				if(this.loadQueue.size() > 0) {
					var2 = (IsoImageBuffer)this.loadQueue.remove(0);
				}
			}

			if(var2 != null) {
				if(this.frameCount - var2.counter < 2) {
					var1.renderIsoTile(var2);
					this.repaint();
				} else {
					var2.isLoaded = false;
				}
			}

			try {
				Thread.sleep(2L);
			} catch (InterruptedException var5) {
				var5.printStackTrace();
			}
		}

	}

	public void update(Graphics var1) {
	}

	public void paint(Graphics var1) {
	}

	public void render() {
		BufferStrategy var1 = this.getBufferStrategy();
		if(var1 == null) {
			this.createBufferStrategy(2);
		} else {
			this.drawWorld((Graphics2D)var1.getDrawGraphics());
			var1.show();
		}
	}

	public void drawWorld(Graphics2D var1) {
		++this.frameCount;
		AffineTransform var2 = var1.getTransform();
		var1.setClip(0, 0, this.getWidth(), this.getHeight());
		var1.setRenderingHint(RenderingHints.KEY_INTERPOLATION, RenderingHints.VALUE_INTERPOLATION_NEAREST_NEIGHBOR);
		var1.translate(this.getWidth() / 2, this.getHeight() / 2);
		var1.scale((double)this.zoom, (double)this.zoom);
		var1.translate(this.offsetX, this.offsetY);
		if(this.worldObj != null) {
			var1.translate(-(this.worldObj.spawnX + this.worldObj.spawnZ), -(-this.worldObj.spawnX + this.worldObj.spawnZ) + 64);
		}

		Rectangle var3 = var1.getClipBounds();
		var1.setColor(new Color(-15724512));
		var1.fillRect(var3.x, var3.y, var3.width, var3.height);
		byte var4 = 16;
		byte var5 = 3;
		int var6 = var3.x / var4 / 2 - 2 - var5;
		int var7 = (var3.x + var3.width) / var4 / 2 + 1 + var5;
		int var8 = var3.y / var4 - 1 - var5 * 2;
		int var9 = (var3.y + var3.height + 16 + 128) / var4 + 1 + var5 * 2;

		int var10;
		for(var10 = var8; var10 <= var9; ++var10) {
			for(int var11 = var6; var11 <= var7; ++var11) {
				int var12 = var11 - (var10 >> 1);
				int var13 = var11 + (var10 + 1 >> 1);
				IsoImageBuffer var14 = this.getImageBuffer(var12, var13);
				var14.counter = this.frameCount;
				if(!var14.isDirty) {
					if(!var14.isLoaded) {
						var14.isLoaded = true;
						this.loadQueue.add(var14);
					}
				} else {
					var14.isLoaded = false;
					if(!var14.isUpdating) {
						int var15 = var11 * var4 * 2 + (var10 & 1) * var4;
						int var16 = var10 * var4 - 128 - 16;
						var1.drawImage(var14.image, var15, var16, (ImageObserver)null);
					}
				}
			}
		}

		if(this.showInfo) {
			var1.setTransform(var2);
			var10 = this.getHeight() - 32 - 4;
			var1.setColor(new Color(Integer.MIN_VALUE, true));
			var1.fillRect(4, this.getHeight() - 32 - 4, this.getWidth() - 8, 32);
			var1.setColor(Color.WHITE);
			String var17 = "F1 - F5: load levels   |   0-9: Set time of day   |   Space: return to spawn   |   Double click: zoom   |   Escape: hide this text";
			var1.drawString(var17, this.getWidth() / 2 - var1.getFontMetrics().stringWidth(var17) / 2, var10 + 20);
		}

		var1.dispose();
	}

	public void mouseDragged(MouseEvent var1) {
		int var2 = var1.getX() / this.zoom;
		int var3 = var1.getY() / this.zoom;
		this.offsetX += var2 - this.lastMouseX;
		this.offsetY += var3 - this.lastMouseY;
		this.lastMouseX = var2;
		this.lastMouseY = var3;
		this.repaint();
	}

	public void mouseMoved(MouseEvent var1) {
	}

	public void mouseClicked(MouseEvent var1) {
		if(var1.getClickCount() == 2) {
			this.zoom = 3 - this.zoom;
			this.repaint();
		}

	}

	public void mouseEntered(MouseEvent var1) {
	}

	public void mouseExited(MouseEvent var1) {
	}

	public void mousePressed(MouseEvent var1) {
		int var2 = var1.getX() / this.zoom;
		int var3 = var1.getY() / this.zoom;
		this.lastMouseX = var2;
		this.lastMouseY = var3;
	}

	public void mouseReleased(MouseEvent var1) {
	}

	public void keyPressed(KeyEvent var1) {
		if(var1.getKeyCode() == 48) {
			this.setSkylightSubtracted(11);
		}

		if(var1.getKeyCode() == 49) {
			this.setSkylightSubtracted(10);
		}

		if(var1.getKeyCode() == 50) {
			this.setSkylightSubtracted(9);
		}

		if(var1.getKeyCode() == 51) {
			this.setSkylightSubtracted(7);
		}

		if(var1.getKeyCode() == 52) {
			this.setSkylightSubtracted(6);
		}

		if(var1.getKeyCode() == 53) {
			this.setSkylightSubtracted(5);
		}

		if(var1.getKeyCode() == 54) {
			this.setSkylightSubtracted(3);
		}

		if(var1.getKeyCode() == 55) {
			this.setSkylightSubtracted(2);
		}

		if(var1.getKeyCode() == 56) {
			this.setSkylightSubtracted(1);
		}

		if(var1.getKeyCode() == 57) {
			this.setSkylightSubtracted(0);
		}

		if(var1.getKeyCode() == 112) {
			this.loadWorld("World1");
		}

		if(var1.getKeyCode() == 113) {
			this.loadWorld("World2");
		}

		if(var1.getKeyCode() == 114) {
			this.loadWorld("World3");
		}

		if(var1.getKeyCode() == 115) {
			this.loadWorld("World4");
		}

		if(var1.getKeyCode() == 116) {
			this.loadWorld("World5");
		}

		if(var1.getKeyCode() == 32) {
			this.offsetX = this.offsetY = 0;
		}

		if(var1.getKeyCode() == 27) {
			this.showInfo = !this.showInfo;
		}

		this.repaint();
	}

	public void keyReleased(KeyEvent var1) {
	}

	public void keyTyped(KeyEvent var1) {
	}

	static boolean getRunning(CanvasIsomPreview var0) {
		return var0.running;
	}
}
