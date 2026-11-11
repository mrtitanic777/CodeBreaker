#include "cb/java/projects/minecraft/profiles/minecraft_xbox360.hpp"

namespace cb::java::projects::minecraft::profiles::minecraft_xbox360 {

namespace {

// DEAD external desktop/GUI types: java.awt / javax.swing / java.applet + org.w3c.dom, plus the crash-report
// isometric-preview canvases. These cannot function on the console (no window, no AWT, no DOM). The pipeline
// DROPS any of these that is also unreferenced (reachability is the safety net), so no-op stubs for them are
// never brought into the port. PRECISE ON PURPOSE -- explicit class names, never a `\w*Listener`/`\w*Event`
// wildcard, so a real game type (LevelListener, a game FooEvent) is never swept in. Reachable ones
// (BufferedImage/Raster/Graphics/ImageIO on the texture-decode path) are kept and grouped by the matching filter
// rule below. Shared as the prune predicate (profile.dead_types).
constexpr const char kDeadTypes[] =

    R"rx(^()rx"
    R"rx(Frame|Panel|Canvas|Component|Applet|MinecraftApplet|Cursor|Robot|Toolkit|Clipboard)rx"
    R"rx(|Transferable|DataFlavor|BorderLayout|GridLayout|FlowLayout|CardLayout|GridBagLayout)rx"
    R"rx(|Window|Dialog|WindowAdapter|KeyAdapter|MouseAdapter|FocusAdapter|ComponentAdapter)rx"
    R"rx(|KeyListener|MouseListener|MouseMotionListener|MouseWheelListener|WindowListener)rx"
    R"rx(|ComponentListener|FocusListener|ActionListener|ContainerListener)rx"
    R"rx(|KeyEvent|MouseEvent|MouseWheelEvent|WindowEvent|ComponentEvent|FocusEvent|InputEvent|ActionEvent)rx"
    R"rx(|Graphics|Graphics2D|Font|FontMetrics|Rectangle|Dimension|Color|BufferStrategy)rx"
    R"rx(|BufferedImage|Raster|WritableRaster|DataBuffer|DataBufferInt|ImageObserver|ImageIO)rx"
    R"rx(|Document|DocumentBuilder|DocumentBuilderFactory|Node|NodeList|Element)rx"
    R"rx(|Iso\w*|IsomPreview\w*)rx"
    R"rx()$)rx";

}  // namespace

const std::vector<std::string>& static_init_order() {
    static const std::vector<std::string> v = {
    "MathHelper", "Material", "Block", "Item", "CraftingManager",
    "EntityList", "TileEntity", "MobSpawnerBase", "OsMap", "Packet",
    };
    return v;
}

const OrderedMap<std::string, std::string>& package_roles() {
    static const OrderedMap<std::string, std::string> m = {
    {"net.minecraft.game", "Top-level game wiring and shared game-wide types."},
    {"net.minecraft.game.entity", "Entity base classes and the entity update/movement core."},
    {"net.minecraft.game.entity.animal", "Passive/animal mobs."},
    {"net.minecraft.game.entity.monster", "Hostile mobs."},
    {"net.minecraft.game.entity.misc", "Non-living entities (items, projectiles, TNT, particles)."},
    {"net.minecraft.game.entity.player", "The player entity and player inventory."},
    {"net.minecraft.game.item", "Items and item stacks (the inventory-side counterpart of blocks)."},
    {"net.minecraft.game.level", "The world/level model: world container, chunks, generation, save/load."},
    {"net.minecraft.game.level.block", "Block definitions and per-block behaviour."},
    {"net.minecraft.game.level.material", "Block material properties (solidity, liquid, replaceable)."},
    {"net.minecraft.game.physics", "Math/collision primitives: vectors, bounding boxes, ray hits."},
    {"net.minecraft.game.tile", "Tile entities (blocks with extra per-instance state, e.g. chests)."},
    {"net.minecraft.client", "Client-side game shell, input and main loop."},
    {"net.minecraft.client.render", "Rendering: world/entity renderers and render helpers."},
    {"net.minecraft.client.gui", "GUI screens and HUD."},
    {"net.minecraft.client.sound", "Sound engine and sound playback."},
    {"util", "Standalone utility helpers (math, noise, etc.)."},
    };
    return m;
}

const std::vector<std::string>& heavy_types() {
    static const std::vector<std::string> v = {
    "Tesselator", "Tessellator", "NBTTagCompound", "NBTTagList",
    };
    return v;
}

const typemap::NameSet& nullable() {
    static const typemap::NameSet s = {
    "blocksList", "itemsList",
    };
    return s;
}

const typemap::NameSet& nullable_fields() {
    static const typemap::NameSet s = {
    "worldObj", "mc", "thePlayer", "theWorld",
    "currentScreen", "playerController", "riddenByEntity", "ridingEntity",
    "renderEngine", "fontRenderer", "ingameGUI", "loadingScreen",
    "sndManager", "effectRenderer", "renderGlobal", "entityRenderer",
    "craftingInventory", "inventorySlots",
    };
    return s;
}

const typemap::NameSet& nullable_getters() {
    static const typemap::NameSet s = {
    "getChunkFromChunkCoords", "getChunkFromBlockCoords", "getItem",
    "getSmeltingResultItem", "getCompoundTag", "getNetClientHandler",
    "getDocumentBase", "getParentFile", "getReadThread",
    "getWriteThread", "getWorldFromRenderManager",
    };
    return s;
}

const typemap::NameMap& nullable_getter_return_type() {
    static const typemap::NameMap m = {
    {"getChunkFromChunkCoords", "Chunk"},
    {"getChunkFromBlockCoords", "Chunk"},
    {"getItem", "Item"},
    {"getCompoundTag", "NBTTagCompound"},
    {"getNetClientHandler", "NetClientHandler"},
    {"getWorldFromRenderManager", "World"},
    {"getDocumentBase", "URL"},
    {"getParentFile", "File"},
    {"getReadThread", "Thread"},
    {"getWriteThread", "Thread"},
    };
    return m;
}

std::string_view dead_types() { return kDeadTypes; }

// NOTE: unused platform SUBSYSTEMS (audio, networking, unused java.* libraries, ...) are NOT curated here --
// pipeline._prune_unreferenced cuts any bundle the game never #includes, automatically, by full-product
// reachability from the game corpus. dead_types() above is kept only for FILTER grouping of the few external
// stubs that survive (used by the texture path), not for pruning.
const std::vector<java::profiles::FilterRule>& vcxproj_filters() {
    static const std::vector<java::profiles::FilterRule> v = {
    // ---- CodeBreaker runtime & build glue -------------------------------------------------------------
    {R"rx(^(Debug|HeapTrack|ThreadPool|Stack)$)rx", R"rx(Debug)rx"}, // debug apparatus: trace log, heap watchdog, backtrace stack, worker pool
    {R"rx(^(JavaRT|JavaString|JArray|JList|JObject|JMap|JSet|JCat|Runnable|Iterator|Class|Runtime|j2x_\w*|stdafx|Common|ReflectionFactory)$)rx", R"rx(Runtime)rx"},

    // ---- Platform layer: hand-written 360 backends + Java-stdlib bridges -----------------------
    {R"rx(^(Kernel|KernelConfig|ControllerConfig|SDKConfig)$)rx", R"rx(Platform\Config)rx"},
    {R"rx(^(Save360|ContentStorage|STFS\w*)$)rx", R"rx(Platform\Storage (STFS))rx"}, // XContent/STFS durable-save mount
    {R"rx(^(GL\d*|GLU|GLContext|GLAllocation|ARB\w+|NV\w+|BufferUtils|Display|DisplayMode|Sys|Project|Frustrum)$)rx", R"rx(Platform\Video (GL->D3D9))rx"},
    {R"rx(^(Keyboard|Mouse|Controllers?|Input|InputConfig|OSK|MouseHelper)$)rx", R"rx(Platform\Input)rx"},
    {R"rx(^(Socket\w*|InetAddress|SocketAddress|UdpSocket|URL|URI|URLConnection|HttpURLConnection|BindException|ConnectException|UnknownHostException|MalformedURLException)$)rx", R"rx(Platform\Networking (XNet))rx"},
    {R"rx(^(Inflate|Inflater|InflaterInputStream|Deflater|DeflaterOutputStream|GZIP\w*|Zip\w*|DataFormatException)$)rx", R"rx(Platform\Compression (zlib))rx"},
    {R"rx(^(File\w*|Buffered(Input|Output|Reader|Writer)\w*|Data(Input|Output)\w*|(Input|Output)Stream\w*|ByteArray\w*Stream|Print\w*|StringReader|StringWriter|PushbackReader|InputStreamReader|Reader|Writer|RandomAccessFile|FileIO|EOFException|IOException|FileNotFoundException)$)rx", R"rx(Platform\Java IO)rx"},
    {R"rx(^(Pattern|Matcher|Charset|Locale|Calendar|Date|GregorianCalendar|SimpleDateFormat|DateFormat|DecimalFormat|NumberFormat|Properties|Logger|BigInteger|MessageDigest|MD5\w*|ByteOrder|Reference|SoftReference)$)rx", R"rx(Platform\Java util)rx"},
    // External java.awt/swing/applet + DOM stubs. The DEAD unreferenced ones are PRUNED before the project
    // is written (see pipeline._prune_dead_unreferenced), so this folder only ever holds the few that are
    // actually USED (e.g. BufferedImage/Raster/Graphics/ImageIO on the texture-decode path).
    {std::string(kDeadTypes), R"rx(Platform\Java AWT/DOM (stubs))rx"},
    {R"rx(^(\w*Exception|\w*Error|Throwable)$)rx", R"rx(Platform\Java exceptions)rx"},

    // ---- Game networking ----------------------------------------------------------------------
    {R"rx(^Packet\w*)rx", R"rx(Network\Packets)rx"},
    {R"rx(^(NetClientHandler|NetServerHandler|NetHandler|NetLoginHandler)$)rx", R"rx(Network\Handlers)rx"},
    {R"rx(^(NetworkManager|Network\w*Thread|ThreadConnectToServer|ThreadCloseConnection|TcpConnection|MemoryConnection)$)rx", R"rx(Network\Connection)rx"},

    // ---- Audio (game engine + platform bridge) ------------------------------------------------
    {R"rx(^(SoundManager|SoundPool\w*|MusicTicker)$)rx", R"rx(Audio\Engine)rx"},
    {R"rx(^(Sound\w*|Audio\w*|XactAudio|Codec\w*|Library\w*OpenAL)$)rx", R"rx(Audio\Platform)rx"},

    // ---- GUI ----------------------------------------------------------------------------------
    {R"rx(^(GuiIngame\w*|GuiParticle|GuiAchievement)$)rx", R"rx(GUI\In-game HUD)rx"},
    {R"rx(^(GuiButton|GuiSlot\w*|GuiSlider|GuiTextField|GuiSmall\w*|GuiScreenButtonList)$)rx", R"rx(GUI\Widgets)rx"},
    {R"rx(^Gui\w*)rx", R"rx(GUI\Screens)rx"},
    {R"rx(^(FontRenderer|ScaledResolution|LoadingScreen\w*)$)rx", R"rx(GUI\Text & Layout)rx"},

    // ---- Rendering (before Entities/Items/Blocks so RenderX/ModelX win over the *X* fallbacks) -
    {R"rx(^(Render\w*|IRender\w*)$)rx", R"rx(Rendering\Renderers)rx"},
    {R"rx(^Model\w*)rx", R"rx(Rendering\Models)rx"},
    {R"rx(^(Texture\w*|TexturePack\w*|\w+FX|IconCoord|Colorizer\w*|\w*Colorizer)$)rx", R"rx(Rendering\Textures)rx"},
    {R"rx(^(Tessellator|EffectRenderer|\w*Particle\w*)$)rx", R"rx(Rendering\Tessellator & FX)rx"},
    // RubyDung/pre-classic render pipeline: the (one-'l') Tesselator, block renderer, shader + mipmap helpers.
    {R"rx(^(Tile|Tesselator|Shaders|Mipmaps)$)rx", R"rx(Rendering\Renderers)rx"},

    // ---- TileEntities (before Entities: 'TileEntity*' also matches the Entity fallback) --------
    {R"rx(^TileEntity\w*)rx", R"rx(TileEntities)rx"},

    // ---- Entities -----------------------------------------------------------------------------
    {R"rx(^(EntityPlayer\w*|EntityClientPlayerMP|InventoryPlayer)$)rx", R"rx(Entities\Player)rx"},
    {R"rx(^(Path\w*|EntityAI\w*)$)rx", R"rx(Entities\AI & Pathfinding)rx"},
    {R"rx(^(Entity|EntityLiving|EntityCreature|DataWatcher|WatchableObject)$)rx", R"rx(Entities\Base)rx"},
    {R"rx(^(Entity\w*|\w+Entity|MobSpawner\w*|SpawnListEntry)$)rx", R"rx(Entities\Types)rx"},

    // ---- Blocks -------------------------------------------------------------------------------
    {R"rx(^(Block|BlockContainer|Material|StepSound)$)rx", R"rx(Blocks\Base)rx"},
    {R"rx(^Block.+)rx", R"rx(Blocks\Types)rx"},

    // ---- Items & Inventory --------------------------------------------------------------------
    {R"rx(^(CraftingManager|\w*Recipe\w*|RecipesIngredients|FurnaceRecipes)$)rx", R"rx(Items & Inventory\Recipes)rx"},
    {R"rx(^(Container\w*|Slot\w*|Inventory\w*|IInventory)$)rx", R"rx(Items & Inventory\Containers)rx"},
    {R"rx(^(Item|Item.+|ItemStack)$)rx", R"rx(Items & Inventory\Items)rx"},

    // ---- World & Generation -------------------------------------------------------------------
    {R"rx(^(Chunk\w*|IChunk\w*)$)rx", R"rx(World & Generation\Chunk)rx"},
    {R"rx(^BiomeGen\w*)rx", R"rx(World & Generation\Biomes)rx"},
    {R"rx(^(MapGen\w*|WorldGen\w*|NoiseGen\w*|\w*Generator|ChunkProvider\w*|StructureBoundingBox|Component\w*Structure)$)rx", R"rx(World & Generation\Generation)rx"},
    {R"rx(^(Spawner\w*|SpawnerAnimals)$)rx", R"rx(World & Generation\Spawning)rx"},
    {R"rx(^(World\w*|IWorld\w*|IBlockAccess|NextTickListEntry|WorldInfo|WorldProvider\w*)$)rx", R"rx(World & Generation\World)rx"},

    // ---- IO & NBT (game-level save/load; the platform stream/zlib bridges are under Platform) --
    {R"rx(^NBT\w*)rx", R"rx(IO & NBT\NBT)rx"},
    {R"rx(^RegionFile\w*)rx", R"rx(IO & NBT\Region)rx"},
    {R"rx(^(Save\w*|\w*ChunkLoader|ChunkLoader|IChunkLoader|AnvilSave\w*|\w*Storage|MapStorage|ISaveFormat|ISaveHandler|CompressedStreamTools)$)rx", R"rx(IO & NBT\Save)rx"},
    {R"rx(^(Compressed\w*|\w*Cache|MapData\w*|MapColor)$)rx", R"rx(IO & NBT\Misc)rx"},

    // ---- Java library glue / dead code that would otherwise fall to 'Game (misc)' --------------
    {R"rx(^J_\w*)rx", R"rx(Platform\JSON (argo))rx"}, // argo JSON parser (stats/achievements payloads)
    {R"rx(^(ByteBuffer|ShortBuffer|IntBuffer|FloatBuffer|DoubleBuffer|LongBuffer|CharBuffer|NIOBuffers)$)rx", R"rx(Platform\Java NIO)rx"},
    {R"rx(^(Collection|Collections|Comparable|Comparator|Entry|Enumeration|JIterator|HashMap\w*|HashSet\w*|TreeMap\w*|TreeSet\w*|ArrayList\w*|LinkedList\w*|LinkedHashMap\w*|StringBuilder|StringBuffer)$)rx", R"rx(Runtime)rx"},
    {R"rx(^Chat\w*)rx", R"rx(GUI\Screens)rx"}, // ChatLine, ChatAllowedCharacters
    {R"rx(^(ClippingHelper\w*|Frustrum|ICamera)$)rx", R"rx(Rendering\Renderers)rx"}, // frustum culling
    {R"rx(^Explosion$)rx", R"rx(World & Generation\World)rx"},

    // ---- Math & Utilities ---------------------------------------------------------------------
    // RubyDung/pre-classic geometry primitives (AABB, Cube, Frustum, Polygon, Vec3, Vertex) join the
    // Beta-era math types (Vec3D/AxisAlignedBB/MathHelper/...).
    {R"rx(^(MathHelper|Vec3D\w*|Vec3|Vertex|AABB|Cube|Frustum|Polygon|AxisAlignedBB|MovingObjectPosition|Direction|Facing|ChunkCoordinates\w*|ChunkPosition|WeightedRandom\w*|Tuple|LongHashMap\w*|MCHash\w*|Random)$)rx", R"rx(Math & Utilities)rx"},

    // ---- Enums --------------------------------------------------------------------------------
    {R"rx(^Enum\w*)rx", R"rx(Enums)rx"},

    // ---- Core ---------------------------------------------------------------------------------
    {R"rx(^(StatBase|StatList|Stat\w*|IStat\w*|Achievement\w*|StatFileWriter|StatsSyncher|StatCollector|StatStringFormat\w*)$)rx", R"rx(Core\Stats & Achievements)rx"},
    {R"rx(^(Minecraft|MinecraftApplet|RubyDung|Session|Timer|Main|GameSettings|EnumOptions\w*|Profiler|MovementInput\w*)$)rx", R"rx(Core)rx"},
    };
    return v;
}

java::profiles::Profile profile() {
    java::profiles::Profile p;
    p.name = "minecraft_xbox360";
    p.static_init_order = static_init_order();
    p.package_roles = package_roles();
    p.heavy_types = heavy_types();
    p.vcxproj_filters = vcxproj_filters();
    p.dead_types = std::string(kDeadTypes);
    p.type.nullable = nullable();
    p.type.nullable_fields = nullable_fields();
    p.type.nullable_getters = nullable_getters();
    p.type.nullable_getter_return_type = nullable_getter_return_type();
    return p;
}

}  // namespace cb::java::projects::minecraft::profiles::minecraft_xbox360
