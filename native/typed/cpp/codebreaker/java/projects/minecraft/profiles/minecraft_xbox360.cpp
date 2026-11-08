// codebreaker/java/projects/minecraft/profiles/minecraft_xbox360.cpp -- typed C++ port of
// codebreaker.java.projects.minecraft.profiles.minecraft_xbox360 (see the header).

#include "codebreaker/java/projects/minecraft/profiles/minecraft_xbox360.hpp"

namespace codebreaker::java::projects::minecraft::profiles::minecraft_xbox360 {

const py::list<py::str> STATIC_INIT_ORDER = {
    py::str("MathHelper"), py::str("Material"), py::str("Block"), py::str("Item"), py::str("CraftingManager"),
    py::str("EntityList"), py::str("TileEntity"), py::str("MobSpawnerBase"), py::str("OsMap"), py::str("Packet"),
};

const py::dict<py::str, py::str> PACKAGE_ROLES = {
    {py::str("net.minecraft.game"), py::str("Top-level game wiring and shared game-wide types.")},
    {py::str("net.minecraft.game.entity"), py::str("Entity base classes and the entity update/movement core.")},
    {py::str("net.minecraft.game.entity.animal"), py::str("Passive/animal mobs.")},
    {py::str("net.minecraft.game.entity.monster"), py::str("Hostile mobs.")},
    {py::str("net.minecraft.game.entity.misc"), py::str("Non-living entities (items, projectiles, TNT, particles).")},
    {py::str("net.minecraft.game.entity.player"), py::str("The player entity and player inventory.")},
    {py::str("net.minecraft.game.item"), py::str("Items and item stacks (the inventory-side counterpart of blocks).")},
    {py::str("net.minecraft.game.level"), py::str("The world/level model: world container, chunks, generation, save/load.")},
    {py::str("net.minecraft.game.level.block"), py::str("Block definitions and per-block behaviour.")},
    {py::str("net.minecraft.game.level.material"), py::str("Block material properties (solidity, liquid, replaceable).")},
    {py::str("net.minecraft.game.physics"), py::str("Math/collision primitives: vectors, bounding boxes, ray hits.")},
    {py::str("net.minecraft.game.tile"), py::str("Tile entities (blocks with extra per-instance state, e.g. chests).")},
    {py::str("net.minecraft.client"), py::str("Client-side game shell, input and main loop.")},
    {py::str("net.minecraft.client.render"), py::str("Rendering: world/entity renderers and render helpers.")},
    {py::str("net.minecraft.client.gui"), py::str("GUI screens and HUD.")},
    {py::str("net.minecraft.client.sound"), py::str("Sound engine and sound playback.")},
    {py::str("util"), py::str("Standalone utility helpers (math, noise, etc.).")},
};

const py::list<py::str> HEAVY_TYPES = {
    py::str("Tesselator"), py::str("Tessellator"), py::str("NBTTagCompound"), py::str("NBTTagList"),
};

const py::set<py::str> NULLABLE = {
    py::str("blocksList"), py::str("itemsList"),
};

const py::set<py::str> NULLABLE_FIELDS = {
    py::str("worldObj"), py::str("mc"), py::str("thePlayer"), py::str("theWorld"),
    py::str("currentScreen"), py::str("playerController"), py::str("riddenByEntity"), py::str("ridingEntity"),
    py::str("renderEngine"), py::str("fontRenderer"), py::str("ingameGUI"), py::str("loadingScreen"),
    py::str("sndManager"), py::str("effectRenderer"), py::str("renderGlobal"), py::str("entityRenderer"),
    py::str("craftingInventory"), py::str("inventorySlots"),
};

const py::set<py::str> NULLABLE_GETTERS = {
    py::str("getChunkFromChunkCoords"), py::str("getChunkFromBlockCoords"), py::str("getItem"),
    py::str("getSmeltingResultItem"), py::str("getCompoundTag"), py::str("getNetClientHandler"),
    py::str("getDocumentBase"), py::str("getParentFile"), py::str("getReadThread"),
    py::str("getWriteThread"), py::str("getWorldFromRenderManager"),
};

const py::dict<py::str, py::str> NULLABLE_GETTER_RETURN_TYPE = {
    {py::str("getChunkFromChunkCoords"), py::str("Chunk")},
    {py::str("getChunkFromBlockCoords"), py::str("Chunk")},
    {py::str("getItem"), py::str("Item")},
    {py::str("getCompoundTag"), py::str("NBTTagCompound")},
    {py::str("getNetClientHandler"), py::str("NetClientHandler")},
    {py::str("getWorldFromRenderManager"), py::str("World")},
    {py::str("getDocumentBase"), py::str("URL")},
    {py::str("getParentFile"), py::str("File")},
    {py::str("getReadThread"), py::str("Thread")},
    {py::str("getWriteThread"), py::str("Thread")},
};

// (the Python builds this regex from adjacent string literals; so does this)
const py::str DEAD_TYPES = py::str(
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
    R"rx()$)rx"
);

// NOTE: unused platform SUBSYSTEMS (audio, networking, unused java.* libraries, ...) are NOT curated
// here -- pipeline._prune_unreferenced cuts any bundle the game never #includes, automatically, by
// full-product reachability from the game corpus. DEAD_TYPES above is kept only for FILTER grouping of
// the few external stubs that survive (used by the texture path), not for pruning.

const py::list<std::tuple<py::str, py::str>> VCXPROJ_FILTERS = {
    // ---- CodeBreaker runtime & build glue -------------------------------------------------------------
    {py::str(R"rx(^(Debug|HeapTrack|ThreadPool|Stack)$)rx"), py::str(R"rx(Debug)rx")}, // debug apparatus: trace log, heap watchdog, backtrace stack, worker pool
    {py::str(R"rx(^(JavaRT|JavaString|JArray|JList|JObject|JMap|JSet|JCat|Runnable|Iterator|Class|Runtime|j2x_\w*|stdafx|Common|ReflectionFactory)$)rx"), py::str(R"rx(Runtime)rx")},

    // ---- Platform layer: hand-written 360 backends + Java-stdlib bridges -----------------------
    {py::str(R"rx(^(Kernel|KernelConfig|ControllerConfig|SDKConfig)$)rx"), py::str(R"rx(Platform\Config)rx")},
    {py::str(R"rx(^(Save360|ContentStorage|STFS\w*)$)rx"), py::str(R"rx(Platform\Storage (STFS))rx")}, // XContent/STFS durable-save mount
    {py::str(R"rx(^(GL\d*|GLU|GLContext|GLAllocation|ARB\w+|NV\w+|BufferUtils|Display|DisplayMode|Sys|Project|Frustrum)$)rx"), py::str(R"rx(Platform\Video (GL->D3D9))rx")},
    {py::str(R"rx(^(Keyboard|Mouse|Controllers?|Input|InputConfig|OSK|MouseHelper)$)rx"), py::str(R"rx(Platform\Input)rx")},
    {py::str(R"rx(^(Socket\w*|InetAddress|SocketAddress|UdpSocket|URL|URI|URLConnection|HttpURLConnection|BindException|ConnectException|UnknownHostException|MalformedURLException)$)rx"), py::str(R"rx(Platform\Networking (XNet))rx")},
    {py::str(R"rx(^(Inflate|Inflater|InflaterInputStream|Deflater|DeflaterOutputStream|GZIP\w*|Zip\w*|DataFormatException)$)rx"), py::str(R"rx(Platform\Compression (zlib))rx")},
    {py::str(R"rx(^(File\w*|Buffered(Input|Output|Reader|Writer)\w*|Data(Input|Output)\w*|(Input|Output)Stream\w*|ByteArray\w*Stream|Print\w*|StringReader|StringWriter|PushbackReader|InputStreamReader|Reader|Writer|RandomAccessFile|FileIO|EOFException|IOException|FileNotFoundException)$)rx"), py::str(R"rx(Platform\Java IO)rx")},
    {py::str(R"rx(^(Pattern|Matcher|Charset|Locale|Calendar|Date|GregorianCalendar|SimpleDateFormat|DateFormat|DecimalFormat|NumberFormat|Properties|Logger|BigInteger|MessageDigest|MD5\w*|ByteOrder|Reference|SoftReference)$)rx"), py::str(R"rx(Platform\Java util)rx")},
    // External java.awt/swing/applet + DOM stubs. The DEAD unreferenced ones are PRUNED before the project
    // is written (see pipeline._prune_dead_unreferenced), so this folder only ever holds the few that are
    // actually USED (e.g. BufferedImage/Raster/Graphics/ImageIO on the texture-decode path).
    {DEAD_TYPES, py::str(R"rx(Platform\Java AWT/DOM (stubs))rx")},
    {py::str(R"rx(^(\w*Exception|\w*Error|Throwable)$)rx"), py::str(R"rx(Platform\Java exceptions)rx")},

    // ---- Game networking ----------------------------------------------------------------------
    {py::str(R"rx(^Packet\w*)rx"), py::str(R"rx(Network\Packets)rx")},
    {py::str(R"rx(^(NetClientHandler|NetServerHandler|NetHandler|NetLoginHandler)$)rx"), py::str(R"rx(Network\Handlers)rx")},
    {py::str(R"rx(^(NetworkManager|Network\w*Thread|ThreadConnectToServer|ThreadCloseConnection|TcpConnection|MemoryConnection)$)rx"), py::str(R"rx(Network\Connection)rx")},

    // ---- Audio (game engine + platform bridge) ------------------------------------------------
    {py::str(R"rx(^(SoundManager|SoundPool\w*|MusicTicker)$)rx"), py::str(R"rx(Audio\Engine)rx")},
    {py::str(R"rx(^(Sound\w*|Audio\w*|XactAudio|Codec\w*|Library\w*OpenAL)$)rx"), py::str(R"rx(Audio\Platform)rx")},

    // ---- GUI ----------------------------------------------------------------------------------
    {py::str(R"rx(^(GuiIngame\w*|GuiParticle|GuiAchievement)$)rx"), py::str(R"rx(GUI\In-game HUD)rx")},
    {py::str(R"rx(^(GuiButton|GuiSlot\w*|GuiSlider|GuiTextField|GuiSmall\w*|GuiScreenButtonList)$)rx"), py::str(R"rx(GUI\Widgets)rx")},
    {py::str(R"rx(^Gui\w*)rx"), py::str(R"rx(GUI\Screens)rx")},
    {py::str(R"rx(^(FontRenderer|ScaledResolution|LoadingScreen\w*)$)rx"), py::str(R"rx(GUI\Text & Layout)rx")},

    // ---- Rendering (before Entities/Items/Blocks so RenderX/ModelX win over the *X* fallbacks) -
    {py::str(R"rx(^(Render\w*|IRender\w*)$)rx"), py::str(R"rx(Rendering\Renderers)rx")},
    {py::str(R"rx(^Model\w*)rx"), py::str(R"rx(Rendering\Models)rx")},
    {py::str(R"rx(^(Texture\w*|TexturePack\w*|\w+FX|IconCoord|Colorizer\w*|\w*Colorizer)$)rx"), py::str(R"rx(Rendering\Textures)rx")},
    {py::str(R"rx(^(Tessellator|EffectRenderer|\w*Particle\w*)$)rx"), py::str(R"rx(Rendering\Tessellator & FX)rx")},
    // RubyDung/pre-classic render pipeline: the (one-'l') Tesselator, block renderer, shader + mipmap helpers.
    {py::str(R"rx(^(Tile|Tesselator|Shaders|Mipmaps)$)rx"), py::str(R"rx(Rendering\Renderers)rx")},

    // ---- TileEntities (before Entities: 'TileEntity*' also matches the Entity fallback) --------
    {py::str(R"rx(^TileEntity\w*)rx"), py::str(R"rx(TileEntities)rx")},

    // ---- Entities -----------------------------------------------------------------------------
    {py::str(R"rx(^(EntityPlayer\w*|EntityClientPlayerMP|InventoryPlayer)$)rx"), py::str(R"rx(Entities\Player)rx")},
    {py::str(R"rx(^(Path\w*|EntityAI\w*)$)rx"), py::str(R"rx(Entities\AI & Pathfinding)rx")},
    {py::str(R"rx(^(Entity|EntityLiving|EntityCreature|DataWatcher|WatchableObject)$)rx"), py::str(R"rx(Entities\Base)rx")},
    {py::str(R"rx(^(Entity\w*|\w+Entity|MobSpawner\w*|SpawnListEntry)$)rx"), py::str(R"rx(Entities\Types)rx")},

    // ---- Blocks -------------------------------------------------------------------------------
    {py::str(R"rx(^(Block|BlockContainer|Material|StepSound)$)rx"), py::str(R"rx(Blocks\Base)rx")},
    {py::str(R"rx(^Block.+)rx"), py::str(R"rx(Blocks\Types)rx")},

    // ---- Items & Inventory --------------------------------------------------------------------
    {py::str(R"rx(^(CraftingManager|\w*Recipe\w*|RecipesIngredients|FurnaceRecipes)$)rx"), py::str(R"rx(Items & Inventory\Recipes)rx")},
    {py::str(R"rx(^(Container\w*|Slot\w*|Inventory\w*|IInventory)$)rx"), py::str(R"rx(Items & Inventory\Containers)rx")},
    {py::str(R"rx(^(Item|Item.+|ItemStack)$)rx"), py::str(R"rx(Items & Inventory\Items)rx")},

    // ---- World & Generation -------------------------------------------------------------------
    {py::str(R"rx(^(Chunk\w*|IChunk\w*)$)rx"), py::str(R"rx(World & Generation\Chunk)rx")},
    {py::str(R"rx(^BiomeGen\w*)rx"), py::str(R"rx(World & Generation\Biomes)rx")},
    {py::str(R"rx(^(MapGen\w*|WorldGen\w*|NoiseGen\w*|\w*Generator|ChunkProvider\w*|StructureBoundingBox|Component\w*Structure)$)rx"), py::str(R"rx(World & Generation\Generation)rx")},
    {py::str(R"rx(^(Spawner\w*|SpawnerAnimals)$)rx"), py::str(R"rx(World & Generation\Spawning)rx")},
    {py::str(R"rx(^(World\w*|IWorld\w*|IBlockAccess|NextTickListEntry|WorldInfo|WorldProvider\w*)$)rx"), py::str(R"rx(World & Generation\World)rx")},

    // ---- IO & NBT (game-level save/load; the platform stream/zlib bridges are under Platform) --
    {py::str(R"rx(^NBT\w*)rx"), py::str(R"rx(IO & NBT\NBT)rx")},
    {py::str(R"rx(^RegionFile\w*)rx"), py::str(R"rx(IO & NBT\Region)rx")},
    {py::str(R"rx(^(Save\w*|\w*ChunkLoader|ChunkLoader|IChunkLoader|AnvilSave\w*|\w*Storage|MapStorage|ISaveFormat|ISaveHandler|CompressedStreamTools)$)rx"), py::str(R"rx(IO & NBT\Save)rx")},
    {py::str(R"rx(^(Compressed\w*|\w*Cache|MapData\w*|MapColor)$)rx"), py::str(R"rx(IO & NBT\Misc)rx")},

    // ---- Java library glue / dead code that would otherwise fall to 'Game (misc)' --------------
    {py::str(R"rx(^J_\w*)rx"), py::str(R"rx(Platform\JSON (argo))rx")}, // argo JSON parser (stats/achievements payloads)
    {py::str(R"rx(^(ByteBuffer|ShortBuffer|IntBuffer|FloatBuffer|DoubleBuffer|LongBuffer|CharBuffer|NIOBuffers)$)rx"), py::str(R"rx(Platform\Java NIO)rx")},
    {py::str(R"rx(^(Collection|Collections|Comparable|Comparator|Entry|Enumeration|JIterator|HashMap\w*|HashSet\w*|TreeMap\w*|TreeSet\w*|ArrayList\w*|LinkedList\w*|LinkedHashMap\w*|StringBuilder|StringBuffer)$)rx"), py::str(R"rx(Runtime)rx")},
    {py::str(R"rx(^Chat\w*)rx"), py::str(R"rx(GUI\Screens)rx")}, // ChatLine, ChatAllowedCharacters
    {py::str(R"rx(^(ClippingHelper\w*|Frustrum|ICamera)$)rx"), py::str(R"rx(Rendering\Renderers)rx")}, // frustum culling
    {py::str(R"rx(^Explosion$)rx"), py::str(R"rx(World & Generation\World)rx")},

    // ---- Math & Utilities ---------------------------------------------------------------------
    // RubyDung/pre-classic geometry primitives (AABB, Cube, Frustum, Polygon, Vec3, Vertex) join the
    // Beta-era math types (Vec3D/AxisAlignedBB/MathHelper/...).
    {py::str(R"rx(^(MathHelper|Vec3D\w*|Vec3|Vertex|AABB|Cube|Frustum|Polygon|AxisAlignedBB|MovingObjectPosition|Direction|Facing|ChunkCoordinates\w*|ChunkPosition|WeightedRandom\w*|Tuple|LongHashMap\w*|MCHash\w*|Random)$)rx"), py::str(R"rx(Math & Utilities)rx")},

    // ---- Enums --------------------------------------------------------------------------------
    {py::str(R"rx(^Enum\w*)rx"), py::str(R"rx(Enums)rx")},

    // ---- Core ---------------------------------------------------------------------------------
    {py::str(R"rx(^(StatBase|StatList|Stat\w*|IStat\w*|Achievement\w*|StatFileWriter|StatsSyncher|StatCollector|StatStringFormat\w*)$)rx"), py::str(R"rx(Core\Stats & Achievements)rx")},
    {py::str(R"rx(^(Minecraft|MinecraftApplet|RubyDung|Session|Timer|Main|GameSettings|EnumOptions\w*|Profiler|MovementInput\w*)$)rx"), py::str(R"rx(Core)rx")},
};

const Profile PROFILE = Profile{
    py::str("minecraft_xbox360"),
    STATIC_INIT_ORDER,
    PACKAGE_ROLES,
    HEAVY_TYPES,
    VCXPROJ_FILTERS,
    DEAD_TYPES,
    std::nullopt,                                  // optional_types
    NULLABLE,
    NULLABLE_FIELDS,
    NULLABLE_GETTERS,
    NULLABLE_GETTER_RETURN_TYPE,
    py::set<py::str>(),                            // unsigned_byte_scalar_fields=set()
    py::set<py::str>(),                            // default_value_types=set()
    py::list<py::str>(),                           // heavy_types_high = ()
};

}  // namespace codebreaker::java::projects::minecraft::profiles::minecraft_xbox360
