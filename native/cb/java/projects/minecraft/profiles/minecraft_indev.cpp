#include "cb/java/projects/minecraft/profiles/minecraft_indev.hpp"

#include "cb/java/projects/minecraft/profiles/minecraft_base.hpp"

namespace cb::java::projects::minecraft::profiles::minecraft_indev {

const OrderedMap<std::string, std::string>& package_roles() {
    static const OrderedMap<std::string, std::string> roles = [] {
        OrderedMap<std::string, std::string> m = minecraft_base::shared_package_roles();
        const std::pair<const char*, const char*> era[] = {
            {"com.mojang.nbt", "NBT tag type system (NBTBase + tag classes) -- level/entity serialization."},
            {"util", "Misc helpers: MathHelper, IProgressUpdate (progress callback)."},
            {"net.minecraft.game", "Top-level game interfaces/containers (IInventory, InventoryLargeChest)."},
            {"net.minecraft.game.entity", "Entity base hierarchy: Entity, EntityLiving, EntityCreature."},
            {"net.minecraft.game.entity.animal", "Passive mobs (pig, sheep, ...)."},
            {"net.minecraft.game.entity.monster", "Hostile mobs (zombie, skeleton, creeper, spider, ...)."},
            {"net.minecraft.game.entity.misc", "Non-living entities (dropped item, primed TNT)."},
            {"net.minecraft.game.entity.projectile", "Projectiles (arrow)."},
            {"net.minecraft.game.entity.player", "Player entity + inventory (EntityPlayer, InventoryPlayer)."},
            {"net.minecraft.game.item", "Item registry + item classes (Item, ItemStack, ItemBlock, tools/food)."},
            {"net.minecraft.game.item.recipe", "Crafting system (CraftingManager, recipes, RecipeSorter)."},
            {"net.minecraft.game.level",
             "The finite world + level services: World, LevelLoader, MobSpawner, Light, EntityMap, pathfinding hooks."},
            {"net.minecraft.game.level.block", "Block registry + block subclasses, step sounds."},
            {"net.minecraft.game.level.block.tileentity", "Tile entities (TileEntity, TileEntityChest)."},
            {"net.minecraft.game.level.generator", "Whole-level terrain generation (LevelGenerator)."},
            {"net.minecraft.game.level.generator.noise", "Perlin/octave noise generators."},
            {"net.minecraft.game.level.material", "Block material types (Material + liquid/logic/transparent)."},
            {"net.minecraft.game.level.path", "Mob A* pathfinding (Path, PathEntity, PathPoint, Pathfinder)."},
            {"net.minecraft.game.physics", "Math/collision primitives (AxisAlignedBB, Vec3D, MovingObjectPosition)."},
            {"net.minecraft.client",
             "Client core/bootstrap: Minecraft, applets, GameSettings, Session, Timer, loading/threads, main menu."},
            {"net.minecraft.client.controller", "Player interaction modes (survival/creative controllers)."},
            {"net.minecraft.client.effect", "Particles (EffectRenderer, Entity*FX)."},
            {"net.minecraft.client.gui", "GUI screens/widgets (GuiScreen, FontRenderer, level dialogs, buttons)."},
            {"net.minecraft.client.gui.container", "Inventory/crafting/chest container GUIs + slots."},
            {"net.minecraft.client.model", "3D entity models (ModelBase, ModelBiped, mob models)."},
            {"net.minecraft.client.player", "Client-side player + input (EntityPlayerSP, MovementInput)."},
            {"net.minecraft.client.render",
             "World/entity rendering (RenderGlobal, WorldRenderer, Tessellator, RenderEngine, ItemRenderer)."},
            {"net.minecraft.client.render.camera", "Frustum/camera (Frustrum, ICamera, isometric camera)."},
            {"net.minecraft.client.render.entity", "Entity renderers (Render, RenderManager, RenderPlayer, ...)."},
            {"net.minecraft.client.render.texture", "Animated textures (TextureFX: lava/water/gears/flames)."},
            {"net.minecraft.client.sound", "Audio (SoundManager, SoundPool, SoundPoolEntry)."},
        };
        for (const auto& [pkg, role] : era) m.insert_or_assign(pkg, role);
        return m;
    }();
    return roles;
}

const std::vector<std::string>& static_init_order() {
    static const std::vector<std::string> v = {"Material", "Block", "Item", "CraftingManager"};
    return v;
}

const typemap::NameSet& nullable() {
    static const typemap::NameSet s = {"blocksList", "itemsList"};
    return s;
}

const typemap::NameSet& nullable_fields() {
    static const typemap::NameSet s = {
        "theWorld",     "thePlayer", "renderGlobal", "effectRenderer", "renderEngine", "fontRenderer", "currentScreen",
        "objectMouseOver", "sndManager", "mouseHelper", "playerEntity", "entityMap",    "worldObj",
    };
    return s;
}

const typemap::NameSet& nullable_getters() {
    static const typemap::NameSet s = {
        "getBlockTileEntity", "rayTraceBlocks",   "findSubclassOf",       "findMatchingRecipe",
        "collisionRayTrace",  "getStackInSlot",   "decrStackSize",        "findPlayerToAttack",
        "getCollisionBoundingBoxFromPool", "getIntermediateWithXValue", "getIntermediateWithYValue",
        "getIntermediateWithZValue",
    };
    return s;
}

const typemap::NameMap& nullable_getter_return_type() {
    static const typemap::NameMap m = {
        {"getBlockTileEntity", "TileEntity"},
        {"rayTraceBlocks", "MovingObjectPosition"},
        {"findSubclassOf", "Entity"},
        {"findMatchingRecipe", "ItemStack"},
        {"collisionRayTrace", "MovingObjectPosition"},
        {"getCollisionBoundingBoxFromPool", "AxisAlignedBB"},
        {"getStackInSlot", "ItemStack"},
        {"decrStackSize", "ItemStack"},
        {"findPlayerToAttack", "Entity"},
        {"getIntermediateWithXValue", "Vec3D"},
        {"getIntermediateWithYValue", "Vec3D"},
        {"getIntermediateWithZValue", "Vec3D"},
    };
    return m;
}

java::profiles::Profile profile() {
    java::profiles::Profile p;
    p.name = "minecraft_indev";
    p.static_init_order = static_init_order();
    p.package_roles = package_roles();
    p.heavy_types = minecraft_base::shared_heavy_types();
    p.vcxproj_filters = minecraft_base::build_filters();
    p.dead_types = std::string(minecraft_base::shared_dead_types());
    p.type.nullable = nullable();
    p.type.nullable_fields = nullable_fields();
    p.type.nullable_getters = nullable_getters();
    p.type.nullable_getter_return_type = nullable_getter_return_type();
    return p;
}

}  // namespace cb::java::projects::minecraft::profiles::minecraft_indev
