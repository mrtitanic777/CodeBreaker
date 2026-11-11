package net.minecraft.client.render;

public class RenderGlobal {
    void load(RenderEngine var1) {
        int a = var1.getTexture("/environment/clouds.png");
        int b = var1.getTexture("/terrain/sun.PNG");
        int c = var1.getTexture("/item/arrows.png");
        int d = var1.getTexture("/gui/gui.png");
        int e = var1.getTexture("/art/kz.png");
        this.world.playSound("random.click", 1.0F, 1.0F);
        this.world.playNoteBlock("note.harp", 1.0F, 1.0F);
        this.world.playSound(someCall(), 1.0F); // no event literal here
        this.snd.playSound("random.pop", 1.0F);
        this.snd.playSound("click", 1.0F); // an event has a dot
        String other = "/data/custom.properties";
        String shaders = "/shaders/post.fsh";
    }
}
