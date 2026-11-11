package net.minecraft.game.stats;

public class Extra {
    public static Achievement flyPig = new Achievement(6, "flyPig", 9, -3, Item.saddle, killEnemy).setSpecial().registerAchievement();
    public static Achievement openInventory = new Achievement(0, "openInventory", 0, 0, Item.book, null).registerAchievement();
}
