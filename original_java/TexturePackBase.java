package net.minecraft.src;

import java.io.IOException;
import java.io.InputStream;
import net.minecraft.client.Minecraft;

public abstract class TexturePackBase {
	public String texturePackFileName;
	public String firstDescriptionLine;
	public String secondDescriptionLine;
	public String texturePackID;

	public void deleteTexturePack() {
	}

	public void closeTexturePackFile() {
	}

	public void deleteTexturePackFile(Minecraft var1) throws IOException {
	}

	public void bindTexturePack(Minecraft var1) {
	}

	public void unbindTexturePack(Minecraft var1) {
	}

	public InputStream getResourceAsStream(String var1) {
		return TexturePackBase.class.getResourceAsStream(var1);
	}
}
