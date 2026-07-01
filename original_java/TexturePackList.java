package net.minecraft.src;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import net.minecraft.client.Minecraft;

public class TexturePackList {
	private List availableTexturePacks = new ArrayList();
	private TexturePackBase defaultTexturePack = new TexturePackDefault();
	public TexturePackBase selectedTexturePack;
	private Map texturePackCache = new HashMap();
	private Minecraft mc;
	private File texturePackDir;
	private String selectedTexturePackName;

	public TexturePackList(Minecraft var1, File var2) {
		this.mc = var1;
		this.texturePackDir = new File(var2, "texturepacks");
		if(!this.texturePackDir.exists()) {
			this.texturePackDir.mkdirs();
		}

		this.selectedTexturePackName = var1.gameSettings.skin;
		this.updateAvailableTexturePacks();
		this.selectedTexturePack.deleteTexturePack();
	}

	public boolean setTexturePack(TexturePackBase var1) {
		if(var1 == this.selectedTexturePack) {
			return false;
		} else {
			this.selectedTexturePack.closeTexturePackFile();
			this.selectedTexturePackName = var1.texturePackFileName;
			this.selectedTexturePack = var1;
			this.mc.gameSettings.skin = this.selectedTexturePackName;
			this.mc.gameSettings.saveOptions();
			this.selectedTexturePack.deleteTexturePack();
			return true;
		}
	}

	public void updateAvailableTexturePacks() {
		ArrayList var1 = new ArrayList();
		this.selectedTexturePack = null;
		var1.add(this.defaultTexturePack);
		if(this.texturePackDir.exists() && this.texturePackDir.isDirectory()) {
			File[] var2 = this.texturePackDir.listFiles();
			File[] var3 = var2;
			int var4 = var2.length;

			for(int var5 = 0; var5 < var4; ++var5) {
				File var6 = var3[var5];
				if(var6.isFile() && var6.getName().toLowerCase().endsWith(".zip")) {
					String var7 = var6.getName() + ":" + var6.length() + ":" + var6.lastModified();

					try {
						if(!this.texturePackCache.containsKey(var7)) {
							TexturePackCustom var8 = new TexturePackCustom(var6);
							var8.texturePackID = var7;
							this.texturePackCache.put(var7, var8);
							var8.deleteTexturePackFile(this.mc);
						}

						TexturePackBase var12 = (TexturePackBase)this.texturePackCache.get(var7);
						if(var12.texturePackFileName.equals(this.selectedTexturePackName)) {
							this.selectedTexturePack = var12;
						}

						var1.add(var12);
					} catch (IOException var9) {
						var9.printStackTrace();
					}
				}
			}
		}

		if(this.selectedTexturePack == null) {
			this.selectedTexturePack = this.defaultTexturePack;
		}

		this.availableTexturePacks.removeAll(var1);
		Iterator var10 = this.availableTexturePacks.iterator();

		while(var10.hasNext()) {
			TexturePackBase var11 = (TexturePackBase)var10.next();
			var11.bindTexturePack(this.mc);
			this.texturePackCache.remove(var11.texturePackID);
		}

		this.availableTexturePacks = var1;
	}

	public List availableTexturePacks() {
		return new ArrayList(this.availableTexturePacks);
	}
}
