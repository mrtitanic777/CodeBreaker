package net.minecraft.game.level.block;

public class Block {
    public static final StepSound soundPowderFootstep = new StepSound("stone", 1.0F, 1.0F);
    public static final StepSound soundGrassFootstep = new StepSound( "grass", 1.0F, 1.0F);
    public static final StepSound soundWoodFootstep = new  StepSound("wood", 1.0F, 1.0F);
    public static final StepSound soundOdd = new StepSound("Stone", 1.0F, 1.0F);
    public static final Block grass = (new BlockGrass(2)).setTextureFile("/terrain.png");
    public void onBlockPlaced(World world) {
        world.playSoundEffect(0.5D, 0.5D, 0.5D, "random.click", 1.0F, 1.0F);
        world.playSoundAtEntity(entity, "step.stone", 1.0F, 1.0F);
        world.playSoundFX("random.pop", 0.5F, 0.5F);
    }
}
