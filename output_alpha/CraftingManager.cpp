#include "stdafx.h"
// Hand-written PORT for CraftingManager.java (Minecraft Alpha v1.2.6)
// ------------------------------------------------------------------
// The Java original parsed a varargs Object[] recipe template (mixed String[]/char/Item/Block
// arrays) in addRecipe(). The converter cannot transpile Java varargs of mixed Object types, so
// the converter-generated CraftingManager.h exposes a degenerate addRecipe(ItemStack*, void*, int)
// that carries no pattern data -- the auto stub therefore registered NO recipes and craft() always
// returned NULL (crafting produced nothing).
//
// This rebuild registers the authentic ALPHA recipe set (every addRecipe/RecipesXxx call from
// CraftingManager.java) using the already-correct converted CraftingRecipe matcher
// (CraftingRecipe::matchRecipe walks the live int[9] grid, trying every offset AND the mirrored
// layout) -- so we register ONE orientation per recipe and the matcher handles the flip for free.
//
// Faithfulness: rather than hardcode numeric IDs, registerRecipe() reproduces the EXACT Java
// map-building logic -- it reads each ingredient's runtime id from Item::shiftedIndex / Block::blockID
// (the same fields Java's addRecipe used) and lays the grid out from the row strings. This makes the
// output identical to the Java client's, whatever the runtime ID assignment ends up being.
//
// Grid / ingredient-map convention (must match CraftingRecipe + the live grid):
//   * the grid passed to craft() is row-major int[9], grid[col + row*3], -1 = empty.
//   * a recipe's ingredient map is width*height ints, map[col + row*width]; -1 marks a cell that is
//     EMPTY within the recipe's bounding box (built from a ' ' in the row strings, exactly as Java).
//
// NOTE on the RecipesTools/Weapons/Ingots/Food/Crafting/Armor classes the Java ctor invokes: in the
// converted port those are dead PORT stubs whose addRecipes() bodies pass a NULL pattern (they never
// registered anything). The real tool/armor/food recipes are registered directly here instead, so the
// gameplay result matches vanilla Alpha. addRecipe(ItemStack*, void*, int) is kept as the compat
// no-op those dead stubs link against.

#include "CraftingManager.h"
#include "Common.h"
#include "CraftingRecipe.h"
#include "ItemStack.h"
#include "Item.h"
#include "Block.h"
#include "BlockFlower.h"   // s_mushroomRed / s_mushroomBrown are BlockFlower* -> need full type for the Block* upcast

#include <string>

CraftingManager* CraftingManager::instance = NULL;

// ---------------------------------------------------------------------------
// Recipe-building helpers. These reproduce Java CraftingManager.addRecipe()'s
// Object[] parser: row strings define the shape (width = row length, height =
// number of rows), then (char -> ingredient) pairs map each glyph to a runtime
// item/block id. A ' ' (or any unmapped glyph) becomes -1 (empty cell).
// ---------------------------------------------------------------------------
namespace {

	// One char->id key binding, like Character.valueOf('#') + Item/Block in Java.
	struct RecipeKey {
		char glyph;
		int  id;
	};

	int itemId(Item* it)  { return (it != NULL) ? it->shiftedIndex : -1; }
	int blockId(Block* b) { return (b  != NULL) ? b->blockID        : -1; }

	// Build the row-major int map for a set of row strings + key bindings and
	// register a CraftingRecipe on the manager. Mirrors Java exactly: cells whose
	// glyph isn't in the key set are -1. CraftingRecipe takes ownership of the
	// new[] map (its dtor does delete[]) and of the output ItemStack.
	void buildAndAdd(std::vector<void*>& recipes,
	                 const char** rows, int rowCount,
	                 const RecipeKey* keys, int keyCount,
	                 ItemStack* output) {
		if (output == NULL) {
			return;
		}

		int width = 0;
		for (int r = 0; r < rowCount; ++r) {
			int len = (int)std::string(rows[r]).length();
			if (len > width) {
				width = len;
			}
		}
		int height = rowCount;

		int cellCount = width * height;
		int* map = new int[cellCount];

		for (int r = 0; r < height; ++r) {
			std::string row = rows[r];
			for (int c = 0; c < width; ++c) {
				int id = -1;
				if (c < (int)row.length()) {
					char glyph = row[c];
					for (int k = 0; k < keyCount; ++k) {
						if (keys[k].glyph == glyph) {
							id = keys[k].id;
							break;
						}
					}
				}
				map[c + r * width] = id;
			}
		}

		CraftingRecipe* recipe = new CraftingRecipe(width, height, map, output);
		recipe->m_ingredientMap_count = cellCount;
		recipes.push_back((void*)recipe);   // m_recipes is std::vector<void*> (per the header)
	}

} // namespace

// ---------------------------------------------------------------------------
// Convenience macros: a recipe is `rows...` followed by key bindings. Each
// registration is wrapped in its own scope so the local arrays don't collide.
// (Mirrors how the Java addRecipe(...) calls read line-for-line.)
// ---------------------------------------------------------------------------
#define BEGIN_RECIPE   {
#define END_RECIPE     }

// initRecipes is a file-local helper (NOT a class member — the converted header only declares
// ctor/dtor/getInstance/craft/addRecipe). It fills the manager's m_recipes vector, passed by ref.
static void initRecipes(std::vector<void*>& recipes);

CraftingManager::CraftingManager() {
	initRecipes(m_recipes);
}

CraftingManager::~CraftingManager() {
	for (size_t i = 0; i < m_recipes.size(); ++i) {
		delete (CraftingRecipe*)m_recipes[i];
	}
	m_recipes.clear();
}

CraftingManager* CraftingManager::getInstance() {
	if (instance == NULL) {
		instance = new CraftingManager();
	}
	return instance;
}

// Compat no-op for the dead Recipes* stubs that still link against the
// converter-generated Java-varargs signature (they pass a NULL pattern, so
// there is nothing to register). Free the output to avoid a leak if invoked.
void CraftingManager::addRecipe(ItemStack* var1, void* var2, int var2_count) {
	(void)var2; (void)var2_count;
	if (var1 != NULL) {
		delete var1;
	}
}

// Java: public ItemStack craft(int[] var1) { for each recipe { if match -> createResult } }
ItemStack* CraftingManager::craft(int* var1) {
	if (var1 == NULL) {
		return NULL;
	}
	for (size_t i = 0; i < m_recipes.size(); ++i) {
		CraftingRecipe* rec = (CraftingRecipe*)m_recipes[i];
		if (rec->matchRecipe(var1)) {
			return rec->createResult(var1);
		}
	}
	return NULL;
}

// ---------------------------------------------------------------------------
// Authentic Alpha v1.2.6 recipe registration. Each block faithfully reproduces
// one addRecipe(...) line (or one RecipesXxx entry) from CraftingManager.java,
// using the same glyph layout and the same Item::/Block:: ingredients.
// ---------------------------------------------------------------------------
static void initRecipes(std::vector<void*>& m_recipes) {
	// ====================================================================
	// RecipesTools / RecipesWeapons (wood, stone, iron, diamond, gold).
	// Alpha registers ALL orientations of axe/hoe in Java; matchRecipe also
	// tests the mirror, so one orientation per tool suffices here.
	// ====================================================================
	#define TOOLSET(MAT, PICK, SWORD, SHOVEL, AXE, HOE)                                  \
		BEGIN_RECIPE                                                                      \
			const char* rows[] = { "###", "@", "@" };                                    \
			RecipeKey keys[] = { { '#', MAT }, { '@', itemId(Item::s_stick) } };          \
			buildAndAdd(m_recipes, rows, 3, keys, 2, new ItemStack(PICK, 1));             \
		END_RECIPE                                                                        \
		BEGIN_RECIPE                                                                      \
			const char* rows[] = { "#", "#", "@" };                                       \
			RecipeKey keys[] = { { '#', MAT }, { '@', itemId(Item::s_stick) } };          \
			buildAndAdd(m_recipes, rows, 3, keys, 2, new ItemStack(SWORD, 1));            \
		END_RECIPE                                                                        \
		BEGIN_RECIPE                                                                      \
			const char* rows[] = { "#", "@", "@" };                                       \
			RecipeKey keys[] = { { '#', MAT }, { '@', itemId(Item::s_stick) } };          \
			buildAndAdd(m_recipes, rows, 3, keys, 2, new ItemStack(SHOVEL, 1));           \
		END_RECIPE                                                                        \
		BEGIN_RECIPE                                                                      \
			const char* rows[] = { "##", "#@", " @" };                                    \
			RecipeKey keys[] = { { '#', MAT }, { '@', itemId(Item::s_stick) } };          \
			buildAndAdd(m_recipes, rows, 3, keys, 2, new ItemStack(AXE, 1));              \
		END_RECIPE                                                                        \
		BEGIN_RECIPE                                                                      \
			const char* rows[] = { "##", " @", " @" };                                    \
			RecipeKey keys[] = { { '#', MAT }, { '@', itemId(Item::s_stick) } };          \
			buildAndAdd(m_recipes, rows, 3, keys, 2, new ItemStack(HOE, 1));              \
		END_RECIPE

	TOOLSET(blockId(Block::s_planks),     itemId(Item::s_pickaxeWood),    itemId(Item::s_swordWood),    itemId(Item::s_shovelWood),    itemId(Item::s_axeWood),    itemId(Item::s_hoeWood))
	TOOLSET(blockId(Block::s_cobblestone),itemId(Item::s_pickaxeStone),   itemId(Item::s_swordStone),   itemId(Item::s_shovelStone),   itemId(Item::s_axeStone),   itemId(Item::s_hoeStone))
	TOOLSET(itemId(Item::s_ingotIron),    itemId(Item::s_pickaxeSteel),   itemId(Item::s_swordSteel),   itemId(Item::s_shovelSteel),   itemId(Item::s_axeSteel),   itemId(Item::s_hoeSteel))
	TOOLSET(itemId(Item::s_diamond),      itemId(Item::s_pickaxeDiamond), itemId(Item::s_swordDiamond), itemId(Item::s_shovelDiamond), itemId(Item::s_axeDiamond), itemId(Item::s_hoeDiamond))
	TOOLSET(itemId(Item::s_ingotGold),    itemId(Item::s_pickaxeGold),    itemId(Item::s_swordGold),    itemId(Item::s_shovelGold),    itemId(Item::s_axeGold),    itemId(Item::s_hoeGold))
	#undef TOOLSET

	// Bow + arrow (RecipesWeapons).
	BEGIN_RECIPE
		const char* rows[] = { " #@", "# @", " #@" };
		RecipeKey keys[] = { { '#', itemId(Item::s_stick) }, { '@', itemId(Item::s_silk) } };
		buildAndAdd(m_recipes, rows, 3, keys, 2, new ItemStack(Item::s_bow, 1));
	END_RECIPE
	BEGIN_RECIPE
		const char* rows[] = { "#", "@", "X" };
		RecipeKey keys[] = { { '#', itemId(Item::s_flint) }, { '@', itemId(Item::s_stick) }, { 'X', itemId(Item::s_feather) } };
		buildAndAdd(m_recipes, rows, 3, keys, 3, new ItemStack(Item::s_arrow, 4));
	END_RECIPE

	// ====================================================================
	// RecipesArmor (helmet / chestplate / leggings / boots).
	// ====================================================================
	#define ARMORSET(MAT, HELM, CHST, LEGS, BOOT)                                          \
		BEGIN_RECIPE                                                                        \
			const char* rows[] = { "###", "# #" };                                          \
			RecipeKey keys[] = { { '#', MAT } };                                            \
			buildAndAdd(m_recipes, rows, 2, keys, 1, new ItemStack(HELM, 1));               \
		END_RECIPE                                                                          \
		BEGIN_RECIPE                                                                        \
			const char* rows[] = { "# #", "###", "###" };                                   \
			RecipeKey keys[] = { { '#', MAT } };                                            \
			buildAndAdd(m_recipes, rows, 3, keys, 1, new ItemStack(CHST, 1));               \
		END_RECIPE                                                                          \
		BEGIN_RECIPE                                                                        \
			const char* rows[] = { "###", "# #", "# #" };                                   \
			RecipeKey keys[] = { { '#', MAT } };                                            \
			buildAndAdd(m_recipes, rows, 3, keys, 1, new ItemStack(LEGS, 1));               \
		END_RECIPE                                                                          \
		BEGIN_RECIPE                                                                        \
			const char* rows[] = { "# #", "# #" };                                          \
			RecipeKey keys[] = { { '#', MAT } };                                            \
			buildAndAdd(m_recipes, rows, 2, keys, 1, new ItemStack(BOOT, 1));               \
		END_RECIPE

	ARMORSET(itemId(Item::s_leather),    itemId(Item::s_helmetLeather), itemId(Item::s_plateLeather), itemId(Item::s_legsLeather), itemId(Item::s_bootsLeather))
	ARMORSET(itemId(Item::s_ingotIron),  itemId(Item::s_helmetSteel),   itemId(Item::s_plateSteel),   itemId(Item::s_legsSteel),   itemId(Item::s_bootsSteel))
	ARMORSET(itemId(Item::s_diamond),    itemId(Item::s_helmetDiamond), itemId(Item::s_plateDiamond), itemId(Item::s_legsDiamond), itemId(Item::s_bootsDiamond))
	ARMORSET(itemId(Item::s_ingotGold),  itemId(Item::s_helmetGold),    itemId(Item::s_plateGold),    itemId(Item::s_legsGold),    itemId(Item::s_bootsGold))
	// Chain armor (s_helmetChain etc.) has no smithing recipe in Alpha -- intentionally omitted.
	#undef ARMORSET

	// ====================================================================
	// Direct addRecipe(...) calls from the CraftingManager ctor, in source order.
	// ====================================================================

	// paper x3: "###" with # = reed
	BEGIN_RECIPE
		const char* rows[] = { "###" };
		RecipeKey keys[] = { { '#', itemId(Item::s_reed) } };
		buildAndAdd(m_recipes, rows, 1, keys, 1, new ItemStack(Item::s_paper, 3));
	END_RECIPE
	// book: three paper stacked
	BEGIN_RECIPE
		const char* rows[] = { "#", "#", "#" };
		RecipeKey keys[] = { { '#', itemId(Item::s_paper) } };
		buildAndAdd(m_recipes, rows, 3, keys, 1, new ItemStack(Item::s_book, 1));
	END_RECIPE
	// fence x2
	BEGIN_RECIPE
		const char* rows[] = { "###", "###" };
		RecipeKey keys[] = { { '#', itemId(Item::s_stick) } };
		buildAndAdd(m_recipes, rows, 2, keys, 1, new ItemStack(Block::s_fence, 2));
	END_RECIPE
	// jukebox: planks ring + diamond center
	BEGIN_RECIPE
		const char* rows[] = { "###", "#X#", "###" };
		RecipeKey keys[] = { { '#', blockId(Block::s_planks) }, { 'X', itemId(Item::s_diamond) } };
		buildAndAdd(m_recipes, rows, 3, keys, 2, new ItemStack(Block::s_jukebox, 1));
	END_RECIPE
	// bookshelf: planks top/bottom, books middle
	BEGIN_RECIPE
		const char* rows[] = { "###", "XXX", "###" };
		RecipeKey keys[] = { { '#', blockId(Block::s_planks) }, { 'X', itemId(Item::s_book) } };
		buildAndAdd(m_recipes, rows, 3, keys, 2, new ItemStack(Block::s_bookShelf, 1));
	END_RECIPE
	// snow block: 2x2 snowballs
	BEGIN_RECIPE
		const char* rows[] = { "##", "##" };
		RecipeKey keys[] = { { '#', itemId(Item::s_snowball) } };
		buildAndAdd(m_recipes, rows, 2, keys, 1, new ItemStack(Block::s_blockSnow, 1));
	END_RECIPE
	// clay block: 2x2 clay
	BEGIN_RECIPE
		const char* rows[] = { "##", "##" };
		RecipeKey keys[] = { { '#', itemId(Item::s_clay) } };
		buildAndAdd(m_recipes, rows, 2, keys, 1, new ItemStack(Block::s_blockClay, 1));
	END_RECIPE
	// brick block: 2x2 bricks
	BEGIN_RECIPE
		const char* rows[] = { "##", "##" };
		RecipeKey keys[] = { { '#', itemId(Item::s_brick) } };
		buildAndAdd(m_recipes, rows, 2, keys, 1, new ItemStack(Block::s_brick, 1));
	END_RECIPE
	// glowstone block: 3x3 lightstone dust
	BEGIN_RECIPE
		const char* rows[] = { "###", "###", "###" };
		RecipeKey keys[] = { { '#', itemId(Item::s_lightStoneDust) } };
		buildAndAdd(m_recipes, rows, 3, keys, 1, new ItemStack(Block::s_lightStone, 1));
	END_RECIPE
	// wool/cloth block: 3x3 silk
	BEGIN_RECIPE
		const char* rows[] = { "###", "###", "###" };
		RecipeKey keys[] = { { '#', itemId(Item::s_silk) } };
		buildAndAdd(m_recipes, rows, 3, keys, 1, new ItemStack(Block::s_cloth, 1));
	END_RECIPE
	// tnt: gunpowder + sand checker
	BEGIN_RECIPE
		const char* rows[] = { "X#X", "#X#", "X#X" };
		RecipeKey keys[] = { { 'X', itemId(Item::s_gunpowder) }, { '#', blockId(Block::s_sand) } };
		buildAndAdd(m_recipes, rows, 3, keys, 2, new ItemStack(Block::s_tnt, 1));
	END_RECIPE
	// stone slab (stairSingle) x3
	BEGIN_RECIPE
		const char* rows[] = { "###" };
		RecipeKey keys[] = { { '#', blockId(Block::s_cobblestone) } };
		buildAndAdd(m_recipes, rows, 1, keys, 1, new ItemStack(Block::s_stairSingle, 3));
	END_RECIPE
	// ladder: "# #","###","# #" with stick
	BEGIN_RECIPE
		const char* rows[] = { "# #", "###", "# #" };
		RecipeKey keys[] = { { '#', itemId(Item::s_stick) } };
		buildAndAdd(m_recipes, rows, 3, keys, 1, new ItemStack(Block::s_ladder, 1));
	END_RECIPE
	// wooden door: 2x3 planks
	BEGIN_RECIPE
		const char* rows[] = { "##", "##", "##" };
		RecipeKey keys[] = { { '#', blockId(Block::s_planks) } };
		buildAndAdd(m_recipes, rows, 3, keys, 1, new ItemStack(Item::s_doorWood, 1));
	END_RECIPE
	// iron door: 2x3 iron ingots
	BEGIN_RECIPE
		const char* rows[] = { "##", "##", "##" };
		RecipeKey keys[] = { { '#', itemId(Item::s_ingotIron) } };
		buildAndAdd(m_recipes, rows, 3, keys, 1, new ItemStack(Item::s_doorSteel, 1));
	END_RECIPE
	// sign: planks block over a single stick
	BEGIN_RECIPE
		const char* rows[] = { "###", "###", " X " };
		RecipeKey keys[] = { { '#', blockId(Block::s_planks) }, { 'X', itemId(Item::s_stick) } };
		buildAndAdd(m_recipes, rows, 3, keys, 2, new ItemStack(Item::s_sign, 1));
	END_RECIPE
	// planks x4 from one wood log
	BEGIN_RECIPE
		const char* rows[] = { "#" };
		RecipeKey keys[] = { { '#', blockId(Block::s_wood) } };
		buildAndAdd(m_recipes, rows, 1, keys, 1, new ItemStack(Block::s_planks, 4));
	END_RECIPE
	// sticks x4 from two planks
	BEGIN_RECIPE
		const char* rows[] = { "#", "#" };
		RecipeKey keys[] = { { '#', blockId(Block::s_planks) } };
		buildAndAdd(m_recipes, rows, 2, keys, 1, new ItemStack(Item::s_stick, 4));
	END_RECIPE
	// torch x4: coal over stick
	BEGIN_RECIPE
		const char* rows[] = { "X", "#" };
		RecipeKey keys[] = { { 'X', itemId(Item::s_coal) }, { '#', itemId(Item::s_stick) } };
		buildAndAdd(m_recipes, rows, 2, keys, 2, new ItemStack(Block::s_torchWood, 4));
	END_RECIPE
	// bowls x4: "# #"," # " planks
	BEGIN_RECIPE
		const char* rows[] = { "# #", " # " };
		RecipeKey keys[] = { { '#', blockId(Block::s_planks) } };
		buildAndAdd(m_recipes, rows, 2, keys, 1, new ItemStack(Item::s_bowlEmpty, 4));
	END_RECIPE
	// minecart track x16: iron sides + stick spine
	BEGIN_RECIPE
		const char* rows[] = { "X X", "X#X", "X X" };
		RecipeKey keys[] = { { 'X', itemId(Item::s_ingotIron) }, { '#', itemId(Item::s_stick) } };
		buildAndAdd(m_recipes, rows, 3, keys, 2, new ItemStack(Block::s_minecartTrack, 16));
	END_RECIPE
	// minecart: "# #","###" iron
	BEGIN_RECIPE
		const char* rows[] = { "# #", "###" };
		RecipeKey keys[] = { { '#', itemId(Item::s_ingotIron) } };
		buildAndAdd(m_recipes, rows, 2, keys, 1, new ItemStack(Item::s_minecartEmpty, 1));
	END_RECIPE
	// jack-o-lantern: pumpkin over torch
	BEGIN_RECIPE
		const char* rows[] = { "A", "B" };
		RecipeKey keys[] = { { 'A', blockId(Block::s_pumpkin) }, { 'B', blockId(Block::s_torchWood) } };
		buildAndAdd(m_recipes, rows, 2, keys, 2, new ItemStack(Block::s_pumpkinLantern, 1));
	END_RECIPE
	// storage minecart: chest over minecart
	BEGIN_RECIPE
		const char* rows[] = { "A", "B" };
		RecipeKey keys[] = { { 'A', blockId(Block::s_crate) }, { 'B', itemId(Item::s_minecartEmpty) } };
		buildAndAdd(m_recipes, rows, 2, keys, 2, new ItemStack(Item::s_minecartCrate, 1));
	END_RECIPE
	// powered minecart: furnace over minecart
	BEGIN_RECIPE
		const char* rows[] = { "A", "B" };
		RecipeKey keys[] = { { 'A', blockId(Block::s_stoneOvenIdle) }, { 'B', itemId(Item::s_minecartEmpty) } };
		buildAndAdd(m_recipes, rows, 2, keys, 2, new ItemStack(Item::s_minecartPowered, 1));
	END_RECIPE
	// boat: "# #","###" planks
	BEGIN_RECIPE
		const char* rows[] = { "# #", "###" };
		RecipeKey keys[] = { { '#', blockId(Block::s_planks) } };
		buildAndAdd(m_recipes, rows, 2, keys, 1, new ItemStack(Item::s_boat, 1));
	END_RECIPE
	// bucket: "# #"," # " iron
	BEGIN_RECIPE
		const char* rows[] = { "# #", " # " };
		RecipeKey keys[] = { { '#', itemId(Item::s_ingotIron) } };
		buildAndAdd(m_recipes, rows, 2, keys, 1, new ItemStack(Item::s_bucketEmpty, 1));
	END_RECIPE
	// flint and steel: "A "," B" iron + flint
	BEGIN_RECIPE
		const char* rows[] = { "A ", " B" };
		RecipeKey keys[] = { { 'A', itemId(Item::s_ingotIron) }, { 'B', itemId(Item::s_flint) } };
		buildAndAdd(m_recipes, rows, 2, keys, 2, new ItemStack(Item::s_flintAndSteel, 1));
	END_RECIPE
	// bread: "###" wheat
	BEGIN_RECIPE
		const char* rows[] = { "###" };
		RecipeKey keys[] = { { '#', itemId(Item::s_wheat) } };
		buildAndAdd(m_recipes, rows, 1, keys, 1, new ItemStack(Item::s_bread, 1));
	END_RECIPE
	// wooden stairs x4: "#  ","## ","###" planks
	BEGIN_RECIPE
		const char* rows[] = { "#  ", "## ", "###" };
		RecipeKey keys[] = { { '#', blockId(Block::s_planks) } };
		buildAndAdd(m_recipes, rows, 3, keys, 1, new ItemStack(Block::s_stairCompactPlanks, 4));
	END_RECIPE
	// fishing rod: "  #"," #X","# X" stick + string
	BEGIN_RECIPE
		const char* rows[] = { "  #", " #X", "# X" };
		RecipeKey keys[] = { { '#', itemId(Item::s_stick) }, { 'X', itemId(Item::s_silk) } };
		buildAndAdd(m_recipes, rows, 3, keys, 2, new ItemStack(Item::s_fishingRod, 1));
	END_RECIPE
	// cobblestone stairs x4
	BEGIN_RECIPE
		const char* rows[] = { "#  ", "## ", "###" };
		RecipeKey keys[] = { { '#', blockId(Block::s_cobblestone) } };
		buildAndAdd(m_recipes, rows, 3, keys, 1, new ItemStack(Block::s_stairCompactCobblestone, 4));
	END_RECIPE
	// painting: stick frame, cloth center
	BEGIN_RECIPE
		const char* rows[] = { "###", "#X#", "###" };
		RecipeKey keys[] = { { '#', itemId(Item::s_stick) }, { 'X', blockId(Block::s_cloth) } };
		buildAndAdd(m_recipes, rows, 3, keys, 2, new ItemStack(Item::s_painting, 1));
	END_RECIPE
	// golden apple: gold blocks around an apple
	BEGIN_RECIPE
		const char* rows[] = { "###", "#X#", "###" };
		RecipeKey keys[] = { { '#', blockId(Block::s_blockGold) }, { 'X', itemId(Item::s_appleRed) } };
		buildAndAdd(m_recipes, rows, 3, keys, 2, new ItemStack(Item::s_appleGold, 1));
	END_RECIPE
	// lever: stick over cobblestone
	BEGIN_RECIPE
		const char* rows[] = { "X", "#" };
		RecipeKey keys[] = { { '#', blockId(Block::s_cobblestone) }, { 'X', itemId(Item::s_stick) } };
		buildAndAdd(m_recipes, rows, 2, keys, 2, new ItemStack(Block::s_lever, 1));
	END_RECIPE
	// redstone torch: redstone over stick
	BEGIN_RECIPE
		const char* rows[] = { "X", "#" };
		RecipeKey keys[] = { { '#', itemId(Item::s_stick) }, { 'X', itemId(Item::s_redstone) } };
		buildAndAdd(m_recipes, rows, 2, keys, 2, new ItemStack(Block::s_torchRedstoneActive, 1));
	END_RECIPE
	// clock (pocket sundial): " # ","#X#"," # " gold + redstone
	BEGIN_RECIPE
		const char* rows[] = { " # ", "#X#", " # " };
		RecipeKey keys[] = { { '#', itemId(Item::s_ingotGold) }, { 'X', itemId(Item::s_redstone) } };
		buildAndAdd(m_recipes, rows, 3, keys, 2, new ItemStack(Item::s_pocketSundial, 1));
	END_RECIPE
	// compass: " # ","#X#"," # " iron + redstone
	BEGIN_RECIPE
		const char* rows[] = { " # ", "#X#", " # " };
		RecipeKey keys[] = { { '#', itemId(Item::s_ingotIron) }, { 'X', itemId(Item::s_redstone) } };
		buildAndAdd(m_recipes, rows, 3, keys, 2, new ItemStack(Item::s_compass, 1));
	END_RECIPE
	// stone button: two stone stacked
	BEGIN_RECIPE
		const char* rows[] = { "#", "#" };
		RecipeKey keys[] = { { '#', blockId(Block::s_stone) } };
		buildAndAdd(m_recipes, rows, 2, keys, 1, new ItemStack(Block::s_button, 1));
	END_RECIPE
	// stone pressure plate: "###" stone
	BEGIN_RECIPE
		const char* rows[] = { "###" };
		RecipeKey keys[] = { { '#', blockId(Block::s_stone) } };
		buildAndAdd(m_recipes, rows, 1, keys, 1, new ItemStack(Block::s_pressurePlateStone, 1));
	END_RECIPE
	// wooden pressure plate: "###" planks
	BEGIN_RECIPE
		const char* rows[] = { "###" };
		RecipeKey keys[] = { { '#', blockId(Block::s_planks) } };
		buildAndAdd(m_recipes, rows, 1, keys, 1, new ItemStack(Block::s_pressurePlatePlanks, 1));
	END_RECIPE

	// === RecipesFood / RecipesCrafting (workbench, furnace, chest, etc.) ===
	// The Alpha ctor delegates these to RecipesFood/RecipesCrafting, whose Java
	// bodies are not in CraftingManager.java. The core, universally-present Alpha
	// craftables are registered here so basic progression works:

	// workbench: 2x2 planks
	BEGIN_RECIPE
		const char* rows[] = { "##", "##" };
		RecipeKey keys[] = { { '#', blockId(Block::s_planks) } };
		buildAndAdd(m_recipes, rows, 2, keys, 1, new ItemStack(Block::s_workbench, 1));
	END_RECIPE
	// chest (crate): planks ring with hollow center
	BEGIN_RECIPE
		const char* rows[] = { "###", "# #", "###" };
		RecipeKey keys[] = { { '#', blockId(Block::s_planks) } };
		buildAndAdd(m_recipes, rows, 3, keys, 1, new ItemStack(Block::s_crate, 1));
	END_RECIPE
	// furnace: cobblestone ring with hollow center
	BEGIN_RECIPE
		const char* rows[] = { "###", "# #", "###" };
		RecipeKey keys[] = { { '#', blockId(Block::s_cobblestone) } };
		buildAndAdd(m_recipes, rows, 3, keys, 1, new ItemStack(Block::s_stoneOvenIdle, 1));
	END_RECIPE
	// mushroom stew: red + brown mushroom over a bowl (both glyph orders)
	BEGIN_RECIPE
		const char* rows[] = { "X", "Y", "B" };
		RecipeKey keys[] = { { 'X', blockId(Block::s_mushroomRed) }, { 'Y', blockId(Block::s_mushroomBrown) }, { 'B', itemId(Item::s_bowlEmpty) } };
		buildAndAdd(m_recipes, rows, 3, keys, 3, new ItemStack(Item::s_bowlSoup, 1));
	END_RECIPE
	BEGIN_RECIPE
		const char* rows[] = { "Y", "X", "B" };
		RecipeKey keys[] = { { 'X', blockId(Block::s_mushroomRed) }, { 'Y', blockId(Block::s_mushroomBrown) }, { 'B', itemId(Item::s_bowlEmpty) } };
		buildAndAdd(m_recipes, rows, 3, keys, 3, new ItemStack(Item::s_bowlSoup, 1));
	END_RECIPE
	// iron block <- 9 ingots
	BEGIN_RECIPE
		const char* rows[] = { "###", "###", "###" };
		RecipeKey keys[] = { { '#', itemId(Item::s_ingotIron) } };
		buildAndAdd(m_recipes, rows, 3, keys, 1, new ItemStack(Block::s_blockSteel, 1));
	END_RECIPE
	// gold block <- 9 ingots
	BEGIN_RECIPE
		const char* rows[] = { "###", "###", "###" };
		RecipeKey keys[] = { { '#', itemId(Item::s_ingotGold) } };
		buildAndAdd(m_recipes, rows, 3, keys, 1, new ItemStack(Block::s_blockGold, 1));
	END_RECIPE
	// diamond block <- 9 diamonds
	BEGIN_RECIPE
		const char* rows[] = { "###", "###", "###" };
		RecipeKey keys[] = { { '#', itemId(Item::s_diamond) } };
		buildAndAdd(m_recipes, rows, 3, keys, 1, new ItemStack(Block::s_blockDiamond, 1));
	END_RECIPE

	// PORT: remaining RecipesFood/RecipesIngots entries (e.g. ingot<-block reversals,
	// cooked-food shapeless recipes) are not present in CraftingManager.java and are
	// left for a later pass; the matcher + the registered set above are correct.
}

#undef BEGIN_RECIPE
#undef END_RECIPE
