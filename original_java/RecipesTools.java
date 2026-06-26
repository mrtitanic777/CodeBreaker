package net.minecraft.game.item.recipe;

import net.minecraft.game.item.Item;
import net.minecraft.game.item.ItemStack;
import net.minecraft.game.level.block.Block;

public final class RecipesTools {
	private String[][] recipePatterns = new String[][]{{"XXX", " # ", " # "}, {"X", "#", "#"}, {"XX", "X#", " #"}, {"XX", " #", " #"}};
	private Object[][] recipeItems = new Object[][]{{Block.planks, Block.cobblestone, Item.ingotIron, Item.diamond, Item.ingotGold}, {Item.axeWood, Item.axeStone, Item.pickaxeSteel, Item.axeDiamond, Item.axeGold}, {Item.pickaxeWood, Item.pickaxeStone, Item.shovel, Item.pickaxeDiamond, Item.pickaxeGold}, {Item.swordStone, Item.swordDiamond, Item.axeSteel, Item.stick, Item.silk}, {Item.hoeStone, Item.hoeSteel, Item.hoeDiamond, Item.hoeGold, Item.seeds}};

	public final void addRecipes(CraftingManager var1) {
		for(int var2 = 0; var2 < this.recipeItems[0].length; ++var2) {
			Object var3 = this.recipeItems[0][var2];

			for(int var4 = 0; var4 < this.recipeItems.length - 1; ++var4) {
				Item var5 = (Item)this.recipeItems[var4 + 1][var2];
				var1.addRecipe(new ItemStack(var5), new Object[]{this.recipePatterns[var4], Character.valueOf('#'), Item.bowlEmpty, Character.valueOf('X'), var3});
			}
		}

	}
}
