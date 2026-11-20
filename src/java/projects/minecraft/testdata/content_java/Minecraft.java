package net.minecraft.client;

public class Minecraft implements Runnable {
    public void startGame() {
        this.renderEngine = new RenderEngine(this.gameSettings);
        this.fontRenderer = new FontRenderer(this.gameSettings, "/font/default.png", this.renderEngine);
        this.sndManager.playStreaming("menu.title", 0.0F, 0.0F, 0.0F, 1.0F, 1.0F);
        this.loadScreen = this.renderEngine.getTexture("/title/mojang.png");
        String lang = "/lang/en_US.lang"; // not "/lang/ignored.lang"
        String text = "/font.txt";
        String hidden = "/misc/not an asset.png";
    }
}
