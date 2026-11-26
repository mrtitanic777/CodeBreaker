// cb-unity: alone
// java/runtime_data.cpp -- the tables the Java runtime is made from: the stub specifications (one per emitted
// header), the member types of the stub classes, and the platform timer replacements. Declarative: the code that
// turns them into text is runtime.cpp.
#include "java/runtime.hpp"
#include "java/runtime_data.hpp"
#include "java/runtime_texts.hpp"

namespace cb::java::runtime {

namespace {

using StrList = std::vector<std::string>;

StrList plus(StrList a, const StrList& b) {
    a.insert(a.end(), b.begin(), b.end());
    return a;
}

const StrList kAwtVoid = {"setFocusable", "setSize", "repaint", "requestFocus", "setVisible", "addNotify",
    "removeNotify", "setPreferredSize", "setMinimumSize", "setMaximumSize", "enableInputMethods",
    "setBackground", "setForeground", "validate", "setLayout", "add", "remove", "removeAll", "addKeyListener",
    "addMouseListener", "addMouseMotionListener", "addMouseWheelListener", "addFocusListener",
    "addComponentListener", "addWindowListener", "setFocusTraversalKeysEnabled", "setCursor", "transferFocus",
    "paint", "update", "doLayout", "invalidate", "revalidate", "pack"};

// The methods every exception stub answers.
const std::vector<Function> kExceptionMethods = {{"getMessage", "std::string", "std::string()"},
    {"getLocalizedMessage", "std::string", "std::string()"}, {"toString", "std::string", "std::string()"}};

}  // namespace

// Best-effort Java package of the external types stubbed, so the banner names the real library.
const std::map<std::string, std::string, std::less<>>& external_packages() {
    static const std::map<std::string, std::string, std::less<>> table = {
        {"GL11", "org.lwjgl.opengl"},
        {"GL12", "org.lwjgl.opengl"},
        {"GL13", "org.lwjgl.opengl"},
        {"ARBMultitexture", "org.lwjgl.opengl"},
        {"GL15", "org.lwjgl.opengl"},
        {"NVFogDistance", "org.lwjgl.opengl"},
        {"GLU", "org.lwjgl.util.glu"},
        {"GLContext", "org.lwjgl.opengl"},
        {"ARBOcclusionQuery", "org.lwjgl.opengl"},
        {"ARBVertexBufferObject", "org.lwjgl.opengl"},
        {"Keyboard", "org.lwjgl.input"},
        {"Mouse", "org.lwjgl.input"},
        {"Controllers", "org.lwjgl.input"},
        {"Display", "org.lwjgl.opengl"},
        {"DisplayMode", "org.lwjgl.opengl"},
        {"BufferUtils", "org.lwjgl"},
        {"Sys", "org.lwjgl"},
        {"File", "java.io"},
        {"InputStream", "java.io"},
        {"OutputStream", "java.io"},
        {"DataInput", "java.io"},
        {"DataOutput", "java.io"},
        {"DataInputStream", "java.io"},
        {"DataOutputStream", "java.io"},
        {"BufferedInputStream", "java.io"},
        {"BufferedOutputStream", "java.io"},
        {"FileInputStream", "java.io"},
        {"FileOutputStream", "java.io"},
        {"BufferedReader", "java.io"},
        {"FileReader", "java.io"},
        {"FileWriter", "java.io"},
        {"PrintWriter", "java.io"},
        {"Socket", "java.net"},
        {"InetSocketAddress", "java.net"},   // (Beta 1.8's server-list ping: socket.connect(new InetSocketAddress(host, port), 3000))
        {"URL", "java.net"},
        {"URLConnection", "java.net"},
        {"SoundSystem", "paulscode.sound"},
        {"CodecJOrbis", "paulscode.sound.codecs"},
        {"ImageIO", "javax.imageio"},
        {"BufferedImage", "java.awt.image"},
        {"ImageObserver", "java.awt.image"},
        {"ZipFile", "java.util.zip"},
        {"DecimalFormat", "java.text"},
        {"DateFormat", "java.text"},
        {"Class", "java.lang"},
        {"Comparable", "java.lang"},
        {"Comparator", "java.util"},
        {"Canvas", "java.awt"},
        {"Component", "java.awt"},
        {"Frame", "java.awt"},
        {"Panel", "java.awt"},
        {"Graphics", "java.awt"},
        {"Color", "java.awt"},
        {"Cursor", "java.awt"},
        {"Robot", "java.awt"},
        {"FileDialog", "java.awt"},
        {"JFileChooser", "javax.swing"},
        {"JDialog", "javax.swing"},
        {"Window", "java.awt"},
        {"Cipher", "javax.crypto"}, {"CipherInputStream", "javax.crypto"}, {"SecretKey", "javax.crypto"}, {"SecretKeyFactory", "javax.crypto"},
        {"PBEKeySpec", "javax.crypto.spec"}, {"PBEParameterSpec", "javax.crypto.spec"}, {"GeneralSecurityException", "java.security"},                         // (early Indev's level load/save dialog)
        {"FileNameExtensionFilter", "javax.swing.filechooser"},
        {"PixelFormat", "org.lwjgl.opengl"},                     // (Indev's Display.create(new PixelFormat(), new ContextAttribs(1, 2)))
        {"ContextAttribs", "org.lwjgl.opengl"},
        {"Dimension", "java.awt"},
        {"Applet", "java.applet"},
        {"BorderLayout", "java.awt"},
        {"GridLayout", "java.awt"},
        {"FlowLayout", "java.awt"},
        {"CardLayout", "java.awt"},
        {"GridBagLayout", "java.awt"},
        {"Point", "java.awt"},
        {"MouseInfo", "java.awt"},
        {"PointerInfo", "java.awt"},
        {"Dialog", "java.awt"},
        {"FloatBuffer", "java.nio"},
        {"IntBuffer", "java.nio"},
        {"ByteBuffer", "java.nio"},
        {"ShortBuffer", "java.nio"},
    };
    return table;
}

// java.awt.Color constants: (name, ARGB).
const std::vector<std::pair<std::string_view, std::uint32_t>>& color_constants() {
    static const std::vector<std::pair<std::string_view, std::uint32_t>> table = {
        {"BLACK", 0xFF000000},
        {"WHITE", 0xFFFFFFFF},
        {"GRAY", 0xFF808080},
        {"LIGHT_GRAY", 0xFFC0C0C0},
        {"DARK_GRAY", 0xFF404040},
        {"RED", 0xFFFF0000},
        {"GREEN", 0xFF00FF00},
        {"BLUE", 0xFF0000FF},
        {"YELLOW", 0xFFFFFF00},
        {"CYAN", 0xFF00FFFF},
        {"MAGENTA", 0xFFFF00FF},
        {"ORANGE", 0xFFFFC800},
        {"PINK", 0xFFFFAFAF},
        {"black", 0xFF000000},
        {"white", 0xFFFFFFFF},
        {"gray", 0xFF808080},
        {"lightGray", 0xFFC0C0C0},
        {"darkGray", 0xFF404040},
        {"red", 0xFFFF0000},
        {"green", 0xFF00FF00},
        {"blue", 0xFF0000FF},
        {"yellow", 0xFFFFFF00},
        {"cyan", 0xFF00FFFF},
        {"magenta", 0xFFFF00FF},
        {"orange", 0xFFFFC800},
        {"pink", 0xFFFFAFAF},
    };
    return table;
}

// LWJGL 2 Keyboard.getKeyName table: key code -> constant name without KEY_ (sorted by code).
const std::vector<std::pair<int, std::string_view>>& lwjgl_key_names() {
    static const std::vector<std::pair<int, std::string_view>> table = {
        {0, "NONE"},
        {1, "ESCAPE"},
        {2, "1"},
        {3, "2"},
        {4, "3"},
        {5, "4"},
        {6, "5"},
        {7, "6"},
        {8, "7"},
        {9, "8"},
        {10, "9"},
        {11, "0"},
        {12, "MINUS"},
        {13, "EQUALS"},
        {14, "BACK"},
        {15, "TAB"},
        {16, "Q"},
        {17, "W"},
        {18, "E"},
        {19, "R"},
        {20, "T"},
        {21, "Y"},
        {22, "U"},
        {23, "I"},
        {24, "O"},
        {25, "P"},
        {26, "LBRACKET"},
        {27, "RBRACKET"},
        {28, "RETURN"},
        {29, "LCONTROL"},
        {30, "A"},
        {31, "S"},
        {32, "D"},
        {33, "F"},
        {34, "G"},
        {35, "H"},
        {36, "J"},
        {37, "K"},
        {38, "L"},
        {39, "SEMICOLON"},
        {40, "APOSTROPHE"},
        {41, "GRAVE"},
        {42, "LSHIFT"},
        {43, "BACKSLASH"},
        {44, "Z"},
        {45, "X"},
        {46, "C"},
        {47, "V"},
        {48, "B"},
        {49, "N"},
        {50, "M"},
        {51, "COMMA"},
        {52, "PERIOD"},
        {53, "SLASH"},
        {54, "RSHIFT"},
        {55, "MULTIPLY"},
        {56, "LMENU"},
        {57, "SPACE"},
        {58, "CAPITAL"},
        {59, "F1"},
        {60, "F2"},
        {61, "F3"},
        {62, "F4"},
        {63, "F5"},
        {64, "F6"},
        {65, "F7"},
        {66, "F8"},
        {67, "F9"},
        {68, "F10"},
        {69, "NUMLOCK"},
        {70, "SCROLL"},
        {71, "NUMPAD7"},
        {72, "NUMPAD8"},
        {73, "NUMPAD9"},
        {74, "SUBTRACT"},
        {75, "NUMPAD4"},
        {76, "NUMPAD5"},
        {77, "NUMPAD6"},
        {78, "ADD"},
        {79, "NUMPAD1"},
        {80, "NUMPAD2"},
        {81, "NUMPAD3"},
        {82, "NUMPAD0"},
        {83, "DECIMAL"},
        {87, "F11"},
        {88, "F12"},
        {100, "F13"},
        {101, "F14"},
        {102, "F15"},
        {103, "F16"},
        {104, "F17"},
        {105, "F18"},
        {112, "KANA"},
        {113, "F19"},
        {121, "CONVERT"},
        {123, "NOCONVERT"},
        {125, "YEN"},
        {141, "NUMPADEQUALS"},
        {144, "CIRCUMFLEX"},
        {145, "AT"},
        {146, "COLON"},
        {147, "UNDERLINE"},
        {148, "KANJI"},
        {149, "STOP"},
        {150, "AX"},
        {151, "UNLABELED"},
        {156, "NUMPADENTER"},
        {157, "RCONTROL"},
        {167, "SECTION"},
        {179, "NUMPADCOMMA"},
        {181, "DIVIDE"},
        {183, "SYSRQ"},
        {184, "RMENU"},
        {196, "FUNCTION"},
        {197, "PAUSE"},
        {199, "HOME"},
        {200, "UP"},
        {201, "PRIOR"},
        {203, "LEFT"},
        {205, "RIGHT"},
        {207, "END"},
        {208, "DOWN"},
        {209, "NEXT"},
        {210, "INSERT"},
        {211, "DELETE"},
        {218, "CLEAR"},
        {219, "LMETA"},
        {220, "RMETA"},
        {221, "APPS"},
        {222, "POWER"},
        {223, "SLEEP"},
    };
    return table;
}

// Per platform: what replaces the Windows timer calls of JavaRT.h / Random.h.
const std::map<std::string, TimerPatch, std::less<>>& timer_patches() {
    static const std::map<std::string, TimerPatch, std::less<>> table = {
        {"generic", TimerPatch{
            .get_tick_count = "((unsigned long)(clock() * 1000.0 / CLOCKS_PER_SEC))",
            .current_time_millis = "inline long long currentTimeMillis() {\n\t\treturn (long long)time(0) * 1000LL;\n\t}",
            .nano_time = "inline long long nanoTime() {\n\t\treturn (long long)(clock() * (1000000000.0 / CLOCKS_PER_SEC));\n\t}"}},
        {"ps3", TimerPatch{
            .get_tick_count = "((unsigned long)(sys_time_get_system_time() / 1000))",
            .current_time_millis = "inline long long currentTimeMillis() {\n\t\treturn (long long)(sys_time_get_system_time() / 1000);\n\t}",
            .nano_time = "inline long long nanoTime() {\n\t\treturn (long long)sys_time_get_system_time() * 1000LL;\n\t}"}},
        {"wii", TimerPatch{
            .get_tick_count = "((unsigned long)(ticks_to_millisecs(gettime())))",
            .current_time_millis = "inline long long currentTimeMillis() {\n\t\treturn (long long)ticks_to_millisecs(gettime());\n\t}",
            .nano_time = "inline long long nanoTime() {\n\t\treturn (long long)ticks_to_nanosecs(gettime());\n\t}"}},
        {"psp", TimerPatch{
            .get_tick_count = "((unsigned long)(sceKernelGetSystemTimeLow() / 1000))",
            .current_time_millis = "inline long long currentTimeMillis() {\n\t\tSceRtcTick tick; sceRtcGetCurrentTick(&tick);\n\t\treturn (long long)(tick.tick / 1000);\n\t}",
            .nano_time = "inline long long nanoTime() {\n\t\tSceRtcTick tick; sceRtcGetCurrentTick(&tick);\n\t\treturn (long long)(tick.tick * 1000LL);\n\t}"}},
        {"nds", TimerPatch{
            .get_tick_count = "((unsigned long)(cpuGetTiming() / BUS_CLOCK * 1000))",
            .current_time_millis = "inline long long currentTimeMillis() {\n\t\treturn 0LL; // TODO: implement DS timer\n\t}",
            .nano_time = "inline long long nanoTime() {\n\t\treturn 0LL; // TODO: implement DS timer\n\t}"}},
    };
    return table;
}

// Build the stub half of the runtime: the headers that are generated or taken from the canonical folder, and the member
// types of the stub classes the chain resolver reads. Order matters only where a later entry replaces an earlier one.
void Runtime::build_stubs() {
    // (getKeyName and getKeyIndex come from the LWJGL key table: keyboard_name_lines())
    add_ns_stub("Keyboard.h", {
        .name = "Keyboard",
        .constants = {{"KEY_ESCAPE", 1}, {"KEY_1", 2}, {"KEY_2", 3}, {"KEY_3", 4}, {"KEY_4", 5}, {"KEY_5", 6}, {"KEY_6", 7},
            {"KEY_7", 8}, {"KEY_8", 9}, {"KEY_9", 10}, {"KEY_0", 11}, {"KEY_MINUS", 12}, {"KEY_EQUALS", 13},
            {"KEY_BACK", 14}, {"KEY_TAB", 15}, {"KEY_Q", 16}, {"KEY_W", 17}, {"KEY_E", 18}, {"KEY_R", 19},
            {"KEY_T", 20}, {"KEY_Y", 21}, {"KEY_U", 22}, {"KEY_I", 23}, {"KEY_O", 24}, {"KEY_P", 25},
            {"KEY_LBRACKET", 26}, {"KEY_RBRACKET", 27}, {"KEY_RETURN", 28}, {"KEY_LCONTROL", 29},
            {"KEY_A", 30}, {"KEY_S", 31}, {"KEY_D", 32}, {"KEY_F", 33}, {"KEY_G", 34}, {"KEY_H", 35},
            {"KEY_J", 36}, {"KEY_K", 37}, {"KEY_L", 38}, {"KEY_SEMICOLON", 39}, {"KEY_APOSTROPHE", 40},
            {"KEY_GRAVE", 41}, {"KEY_LSHIFT", 42}, {"KEY_BACKSLASH", 43}, {"KEY_Z", 44}, {"KEY_X", 45},
            {"KEY_C", 46}, {"KEY_V", 47}, {"KEY_B", 48}, {"KEY_N", 49}, {"KEY_M", 50}, {"KEY_COMMA", 51},
            {"KEY_PERIOD", 52}, {"KEY_SLASH", 53}, {"KEY_RSHIFT", 54}, {"KEY_MULTIPLY", 55}, {"KEY_LMENU", 56},
            {"KEY_SPACE", 57}, {"KEY_CAPITAL", 58}, {"KEY_F1", 59}, {"KEY_F2", 60}, {"KEY_F3", 61},
            {"KEY_F4", 62}, {"KEY_F5", 63}, {"KEY_F6", 64}, {"KEY_F7", 65}, {"KEY_F8", 66}, {"KEY_F9", 67},
            {"KEY_F10", 68}, {"KEY_NUMLOCK", 69}, {"KEY_SCROLL", 70}, {"KEY_F11", 87}, {"KEY_F12", 88},
            {"KEY_NUMPAD0", 82}, {"KEY_NUMPAD1", 79}, {"KEY_NUMPAD2", 80}, {"KEY_NUMPAD3", 81},
            {"KEY_NUMPAD4", 75}, {"KEY_NUMPAD5", 76}, {"KEY_NUMPAD6", 77}, {"KEY_NUMPAD7", 71},
            {"KEY_NUMPAD8", 72}, {"KEY_NUMPAD9", 73}, {"KEY_ADD", 78}, {"KEY_SUBTRACT", 74},
            {"KEY_DECIMAL", 83}, {"KEY_RCONTROL", 157}, {"KEY_RMENU", 184}, {"KEY_HOME", 199}, {"KEY_UP", 200},
            {"KEY_PRIOR", 201}, {"KEY_LEFT", 203}, {"KEY_RIGHT", 205}, {"KEY_END", 207}, {"KEY_DOWN", 208},
            {"KEY_NEXT", 209}, {"KEY_INSERT", 210}, {"KEY_DELETE", 211}, {"CHAR_NONE", 0}, {"KEY_NONE", 0},
            {"KEY_LMETA", 219}, {"KEY_RMETA", 221}, {"KEY_LWIN", 219}, {"KEY_RWIN", 221}, {"KEY_APPS", 221}},
        .void_fns = {"create", "destroy", "enableRepeatEvents"},
        .ret_fns = {{"getEventKey", "int", "0"}, {"getEventKeyState", "bool", "false"}, {"getEventCharacter", "char", "0"},
            {"isKeyDown", "bool", "false"}, {"next", "bool", "false"}},
        .prelude = {"#include \"JavaString.h\""},
        .extra_lines = keyboard_name_lines()});
    add_ns_stub("Mouse.h", {
        .name = "Mouse",
        .void_fns = {"create", "destroy", "setNativeCursor", "setGrabbed", "setCursorPosition"},
        .ret_fns = {{"getDX", "int", "0"}, {"getDY", "int", "0"}, {"getX", "int", "0"}, {"getY", "int", "0"},
            {"getEventButton", "int", "0"}, {"getEventButtonState", "bool", "false"},
            {"getEventDWheel", "int", "0"}, {"getEventX", "int", "0"}, {"getEventY", "int", "0"},
            {"isButtonDown", "bool", "false"}, {"next", "bool", "false"}}});
    add_class_stub("DisplayMode.h", {
        .name = "DisplayMode",
        .ret_methods = {{"getWidth", "int", "0"}, {"getHeight", "int", "0"}, {"getFrequency", "int", "0"},
            {"getBitsPerPixel", "int", "0"}}});
    add_ns_stub("Display.h", {
        .name = "Display",
        .void_fns = {"create", "destroy", "setDisplayMode", "setFullscreen", "setParent", "setTitle", "update", "swapBuffers", "setVSyncEnabled", "setIcon", "sync", "setResizable", "setLocation", "releaseContext", "makeCurrent"},
        .ret_fns = {{"isActive", "bool", "false"}, {"isCloseRequested", "bool", "false"}, {"isCreated", "bool", "false"},
            {"isDirty", "bool", "false"}, {"getWidth", "int", "0"}, {"getHeight", "int", "0"},
            {"getDisplayMode", "DisplayMode", "DisplayMode()"},
            {"getDesktopDisplayMode", "DisplayMode", "DisplayMode()"},
            {"getAvailableDisplayModes", "void*", "0"}},
        .includes = {"DisplayMode"}});
    add_ns_stub("GL15.h", {
        .name = "GL15",
        .constants = {{"GL_ARRAY_BUFFER", 34962}, {"GL_STREAM_DRAW", 35040}, {"GL_SAMPLES_PASSED", 35092},
            {"GL_QUERY_RESULT", 34918}, {"GL_QUERY_RESULT_AVAILABLE", 34919}, {"GL_QUERY_COUNTER_BITS", 34916}}});
    add_ns_stub("GL12.h", {
        .name = "GL12",
        .constants = {{"GL_RESCALE_NORMAL", 32826}, {"GL_CLAMP_TO_EDGE", 33071}, {"GL_SMOOTH_POINT_SIZE_RANGE", 2834},
            {"GL_BGR", 32992}, {"GL_BGRA", 32993}}});   // (Beta 1.2_02's screenshot: glReadPixels in BGR)
    // multitexture (Beta 1.8's lightmap on the second unit): the PC's GL13.reference makes the calls; elsewhere no-ops
    add_ns_stub("GL13.h", {
        .name = "GL13",
        .constants = {{"GL_TEXTURE0", 33984}, {"GL_TEXTURE1", 33985}, {"GL_TEXTURE2", 33986}, {"GL_TEXTURE3", 33987}},
        .void_fns = {"glActiveTexture", "glClientActiveTexture", "glMultiTexCoord2f"}});
    add_ns_stub("ARBMultitexture.h", {
        .name = "ARBMultitexture",
        .constants = {{"GL_TEXTURE0_ARB", 33984}, {"GL_TEXTURE1_ARB", 33985}, {"GL_TEXTURE2_ARB", 33986}, {"GL_TEXTURE3_ARB", 33987}},
        .void_fns = {"glActiveTextureARB", "glClientActiveTextureARB", "glMultiTexCoord2fARB"}});
    add_ns_stub("NVFogDistance.h", {
        .name = "NVFogDistance",
        .constants = {{"GL_FOG_DISTANCE_MODE_NV", 34138}, {"GL_EYE_RADIAL_NV", 34139}, {"GL_EYE_PLANE_ABSOLUTE_NV", 34140}}});
    add_ns_stub("GLU.h", {
        .name = "GLU",
        .void_fns = {"gluPerspective", "gluLookAt", "gluProject", "gluUnProject"},
        .ret_fns = {{"gluErrorString", "std::string", "std::string()"}, {"gluBuild2DMipmaps", "int", "0"}}});
    add_ns_stub("ARBOcclusionQuery.h", {
        .name = "ARBOcclusionQuery",
        .void_fns = {"glBeginQueryARB", "glEndQueryARB", "glGetQueryObjectuARB", "glGetQueryARB"},
        .ret_fns = {{"glGenQueriesARB", "int", "0"}}});
    add_ns_stub("ARBVertexBufferObject.h", {
        .name = "ARBVertexBufferObject",
        .void_fns = {"glBindBufferARB", "glBufferDataARB", "glDeleteBuffersARB"},
        .ret_fns = {{"glGenBuffersARB", "int", "0"}}});
    put_file("BufferUtils.h", verbatim::kBufferUtilsHeader);
    add_ns_stub("ImageIO.h", {
        .name = "ImageIO",
        .void_fns = {"write"},
        .ret_fns = {{"read", "BufferedImage*", "0"}},
        .forward_decls = {"BufferedImage"}});
    add_ns_stub("JOptionPane.h", {
        .name = "JOptionPane",
        .constants = {{"YES_OPTION", 0}, {"NO_OPTION", 1}, {"CANCEL_OPTION", 2}, {"OK_OPTION", 0}, {"ERROR_MESSAGE", 0},
            {"INFORMATION_MESSAGE", 1}},
        .void_fns = {"showMessageDialog"},
        .ret_fns = {{"showConfirmDialog", "int", "2"}, {"showInputDialog", "void*", "0"}}});
    add_canonical("DataInput.h");
    add_canonical("DataOutput.h");
    add_canonical("Regex.h");
    add_canonical("Matcher.h");
    add_canonical("Pattern.h");
    add_class_stub("FileFilter.h", {.name = "FileFilter", .ret_methods = {{"accept", "bool", "false"}}, .forward_decls = {"File"}});
    add_canonical("ByteArrayOutputStream.h");
    add_class_stub("Reader.h", {.name = "Reader", .void_methods = {"close"}, .ret_methods = {{"read", "int", "-1"}}});
    add_canonical("InputStreamReader.h");
    add_class_stub("HttpURLConnection.h", {
        .name = "HttpURLConnection",
        .void_methods = {"setDoInput", "setDoOutput", "connect", "disconnect", "setRequestMethod", "setRequestProperty"},
        .ret_methods = {{"getResponseCode", "int", "-1"}, {"getInputStream", "InputStream*", "0"}},
        .bases = {"URLConnection"},
        .includes = {"URLConnection"},
        .forward_decls = {"InputStream"}});
    add_class_stub("ZipEntry.h", {
        .name = "ZipEntry",
        .ret_methods = {{"getName", "std::string", "std::string()"}, {"getSize", "long long", "0"}, {"isDirectory", "bool", "false"}}});
    add_class_stub("NodeList.h", {
        .name = "NodeList",
        .ret_methods = {{"getLength", "int", "0"}, {"item", "Node*", "0"}},
        .forward_decls = {"Node"}});
    add_class_stub("Node.h", {
        .name = "Node",
        .ret_methods = {{"getNodeType", "int", "0"}, {"getNodeName", "std::string", "std::string()"},
            {"getNodeValue", "std::string", "std::string()"}, {"getChildNodes", "NodeList*", "0"}},
        .forward_decls = {"NodeList"}});
    add_class_stub("Element.h", {
        .name = "Element",
        .ret_methods = {{"getAttribute", "std::string", "std::string()"}, {"getElementsByTagName", "NodeList*", "0"}},
        .bases = {"Node"},
        .includes = {"Node"},
        .forward_decls = {"NodeList"}});
    add_class_stub("Document.h", {
        .name = "Document",
        .ret_methods = {{"getElementsByTagName", "NodeList*", "0"}, {"getDocumentElement", "Element*", "0"}},
        .forward_decls = {"NodeList", "Element"}});
    add_class_stub("DocumentBuilder.h", {
        .name = "DocumentBuilder",
        .ret_methods = {{"parse", "Document*", "0"}},
        .forward_decls = {"Document"}});
    add_class_stub("DocumentBuilderFactory.h", {
        .name = "DocumentBuilderFactory",
        .ret_methods = {{"newDocumentBuilder", "DocumentBuilder*", "0"}},
        .forward_decls = {"DocumentBuilder"},
        .extra = {"static DocumentBuilderFactory* newInstance(...) { return new DocumentBuilderFactory(); }"}});
    add_canonical("Date.h");
    put_file("SimpleDateFormat.h", verbatim::kSimpleDateFormatHeader);
    add_canonical("GZIPInputStream.h");
    add_canonical("GZIPOutputStream.h");
    put_file("Runtime.h", verbatim::kRuntimeHeader);
    add_ns_stub("ByteOrder.h", {
        .name = "ByteOrder",
        .constants = {{"BIG_ENDIAN", 1}, {"LITTLE_ENDIAN", 0}},
        // the machine's own order, asked of the machine: big-endian on the Xbox 360's PowerPC, little-endian on a PC. A fixed big
        // answer packed every vertex colour of Beta's Tessellator byte-reversed on PC (red grass, leaves and water).
        .ret_fns = {{"nativeOrder", "int", "j2xNativeOrder()"}},
        .prelude = {"inline int j2xNativeOrder() { const unsigned int one = 1; return *(const unsigned char*)&one ? 0 : 1; }   // LITTLE_ENDIAN 0, BIG_ENDIAN 1"}});
    put_file("RandomAccessFile.h", verbatim::kRandomAccessFileHeader);
    add_class_stub("Writer.h", {
        .name = "Writer",
        .void_methods = {"write", "flush", "close"},
        .extra = {"virtual void write(const JavaString&) {}", "virtual void write(int) {}", "Writer* append(...) { return this; }"}});
    add_class_stub("StringWriter.h", {
        .name = "StringWriter",
        .ret_methods = {{"toString", "std::string", "std::string()"}, {"getBuffer", "void*", "0"}},
        .bases = {"Writer"},
        .includes = {"Writer"}});
    add_class_stub("PushbackReader.h", {
        .name = "PushbackReader",
        .void_methods = {"unread", "close"},
        .ret_methods = {{"read", "int", "-1"}}});
    put_file("Stack.h", verbatim::kStackHeader);
    add_canonical("Properties.h");
    put_file("Entry.h", verbatim::kEntryHeader);
    add_canonical("NumberFormat.h");
    add_canonical("MessageDigest.h");
    add_class_stub("Reference.h", {.name = "Reference", .ret_methods = {{"get", "void*", "0"}}});
    add_class_stub("Logger.h", {
        .name = "Logger",
        .void_methods = {"log", "info", "warning", "severe", "fine"},
        .extra = {"static Logger* getLogger(...) { return new Logger(); }"}});
    // java.util.Calendar over the computer's clock (local time): get() answers the field, so Beta's date splashes ("Happy new year!",
    // "Merry X-mas!", Notch's birthday) and the save dates are right. FIXED BUG calendar-zero: every field was 0.
    add_class_stub("Calendar.h", {
        .name = "Calendar",
        .void_methods = {"set", "setTime", "add"},
        .ret_methods = {{"getTime", "void*", "0"}},
        .ctors = false,
        .extra = {"long long _j2xms;",
                  "Calendar() : _j2xms((long long)time(0) * 1000LL) {}",
                  "static Calendar* getInstance() { return new Calendar(); }",
                  "void setTimeInMillis(long long ms) { _j2xms = ms; }",
                  "long long getTimeInMillis() { return _j2xms; }",
                  "int get(int field) { time_t t = (time_t)(_j2xms / 1000LL); struct tm* lt = localtime(&t); if (!lt) return 0; "
                  "switch (field) { case 1: return lt->tm_year + 1900; case 2: return lt->tm_mon; case 5: return lt->tm_mday; "
                  "case 6: return lt->tm_yday + 1; case 7: return lt->tm_wday + 1; case 10: return lt->tm_hour % 12; case 11: return lt->tm_hour; "
                  "case 12: return lt->tm_min; case 13: return lt->tm_sec; } return 0; }",
                  "static const int JANUARY = 0, FEBRUARY = 1, DECEMBER = 11, YEAR = 1, MONTH = 2, DATE = 5, DAY_OF_MONTH = 5, DAY_OF_YEAR = 6, "
                  "DAY_OF_WEEK = 7, HOUR = 10, HOUR_OF_DAY = 11, MINUTE = 12, SECOND = 13;"},
        .prelude = {"#include <ctime>"}});
    add_class_stub("GregorianCalendar.h", {.name = "GregorianCalendar", .bases = {"Calendar"}, .includes = {"Calendar"}});
    add_class_stub("Clipboard.h", {
        .name = "Clipboard",
        .ret_methods = {{"getContents", "Transferable*", "0"}},
        .forward_decls = {"Transferable"}});
    add_class_stub("Toolkit.h", {
        .name = "Toolkit",
        .includes = {"Clipboard"},
        .extra = {"static Toolkit* getDefaultToolkit() { return new Toolkit(); }", "Clipboard getSystemClipboard(...) { return Clipboard(); }"}});
    add_ns_stub("SoundSystemConfig.h", {
        .name = "SoundSystemConfig",
        .void_fns = {"setCodec", "setNumberStreamingChannels", "addLibrary", "removeLibrary", "setSoundFilesPackage"}});
    add_class_stub("Collection.h", {
        .name = "Collection",
        .void_methods = {"add", "clear"},
        .ret_methods = {{"size", "int", "0"}, {"isEmpty", "bool", "true"}, {"iterator", "void*", "0"}, {"contains", "bool", "false"}}});
    add_class_stub("Locale.h", {.name = "Locale", .extra = {"static const int US = 0, ENGLISH = 0, ROOT = 0;"}});
    add_class_stub("Charset.h", {.name = "Charset", .extra = {"static Charset* forName(...) { return new Charset(); }"}});
    add_class_stub("Rectangle.h", {.name = "Rectangle", .extra = {"int x, y, width, height;"}});
    add_class_stub("RenderingHints.h", {
        .name = "RenderingHints",
        .void_methods = {"put"},
        .extra = {"static const int s_KEY_INTERPOLATION = 0;", "static const int s_VALUE_INTERPOLATION_NEAREST_NEIGHBOR = 0;", "static const int s_KEY_ANTIALIASING = 0;", "static const int s_VALUE_ANTIALIAS_OFF = 0;"}});
    add_class_stub("CodecWav.h", {.name = "CodecWav"});
    add_class_stub("DataFlavor.h", {.name = "DataFlavor", .extra = {"static const int s_stringFlavor = 0;"}});
    add_class_stub("Transferable.h", {
        .name = "Transferable",
        .ret_methods = {{"isDataFlavorSupported", "bool", "false"}, {"getTransferData", "JObject", "JObject()"}}});
    add_class_stub("TextArea.h", {
        .name = "TextArea",
        .void_methods = {"setFont", "setText"},
        .ret_methods = {{"getText", "std::string", "std::string()"}}});
    put_file("InputStream.h", verbatim::kInputStreamHeader);
    add_canonical("OutputStream.h");
    add_canonical("DataInputStream.h");
    add_canonical("DataOutputStream.h");
    add_canonical("BufferedOutputStream.h");
    add_class_stub("BufferedInputStream.h", {.name = "BufferedInputStream", .bases = {"InputStream"}, .includes = {"InputStream"}});
    put_file("FileInputStream.h", verbatim::kFileInputStreamHeader);
    add_canonical("FileOutputStream.h");
    add_class_stub("ZipFile.h", {
        .name = "ZipFile",
        .void_methods = {"close"},
        .ret_methods = {{"getEntry", "ZipEntry*", "0"}, {"getInputStream", "InputStream*", "0"}, {"size", "int", "0"}},
        .forward_decls = {"InputStream", "ZipEntry"}});
    add_class_stub("SocketAddress.h", {.name = "SocketAddress"});
    add_class_stub("InetSocketAddress.h", {.name = "InetSocketAddress", .bases = {"SocketAddress"}, .includes = {"SocketAddress"}});
    add_class_stub("Socket.h", {
        .name = "Socket",
        .void_methods = {"close", "setSoTimeout", "setTrafficClass", "setTcpNoDelay", "setKeepAlive", "connect"},
        .ret_methods = {{"getInputStream", "InputStream*", "0"}, {"getOutputStream", "OutputStream*", "0"},
            {"getRemoteSocketAddress", "SocketAddress*", "0"}, {"getInetAddress", "InetAddress*", "0"},
            {"getPort", "int", "0"}, {"isConnected", "bool", "false"}, {"isClosed", "bool", "false"}},
        .forward_decls = {"InputStream", "OutputStream", "SocketAddress", "InetAddress"}});
    add_canonical("DateFormat.h");
    add_class_stub("Dimension.h", {.name = "Dimension", .ret_methods = {{"getWidth", "int", "0"}, {"getHeight", "int", "0"}}});
    add_class_stub("WindowAdapter.h", {
        .name = "WindowAdapter",
        .void_methods = {"windowClosing", "windowOpened", "windowClosed", "windowActivated", "windowDeactivated", "windowIconified", "windowDeiconified"}});
    add_class_stub("Panel.h", {
        .name = "Panel",
        .void_methods = kAwtVoid,
        .ret_methods = {{"getWidth", "int", "J2X_COMPONENT_WIDTH()"}, {"getHeight", "int", "J2X_COMPONENT_HEIGHT()"},
            {"getGraphics", "Graphics*", "0"}},
        .bases = {"Component"},
        .includes = {"Component"},
        .forward_decls = {"Graphics"}});
    add_class_stub("Comparable.h", {.name = "Comparable", .ret_methods = {{"compareTo", "int", "0"}}});
    add_class_stub("Frame.h", {
        .name = "Frame",
        .void_methods = plus(kAwtVoid, {"setTitle", "setResizable", "dispose", "show", "hide", "toFront", "setIconImage", "setMenuBar", "setLocationRelativeTo", "setLocation", "setUndecorated", "setState", "setExtendedState"}),
        .ret_methods = {{"getWidth", "int", "J2X_COMPONENT_WIDTH()"}, {"getHeight", "int", "J2X_COMPONENT_HEIGHT()"},
            {"getGraphics", "Graphics*", "0"}, {"getInsets", "void*", "0"},
            {"getTitle", "std::string", "std::string()"}},
        .bases = {"Component"},
        .includes = {"Component"},
        .forward_decls = {"Graphics"}});
    put_file("Color.h", color_header());
    add_ns_stub("Controllers.h", {
        .name = "Controllers",
        .void_fns = {"create", "destroy", "poll", "clearEvents"},
        .ret_fns = {{"isCreated", "bool", "false"}, {"getControllerCount", "int", "0"}, {"getController", "void*", "0"},
            {"next", "bool", "false"}, {"getEventSource", "void*", "0"}, {"getEventControlIndex", "int", "0"},
            {"getEventButtonState", "bool", "false"}}});
    add_ns_stub("Sys.h", {
        .name = "Sys",
        .void_fns = {"openURL", "alert"},
        .ret_fns = {{"getVersion", "std::string", "std::string()"}, {"getTime", "long long", "0LL"},
            {"getTimerResolution", "long long", "1000LL"}}});
    put_file("GLContext.h", verbatim::kGLContextHeader);
    add_class_stub("URLConnection.h", {
        .name = "URLConnection",
        .void_methods = {"connect", "setDoInput", "setDoOutput", "setUseCaches", "setRequestProperty", "setConnectTimeout", "setReadTimeout", "disconnect"},
        .ret_methods = {{"getInputStream", "InputStream*", "0"}, {"getOutputStream", "OutputStream*", "0"},
            {"getContentLength", "int", "0"}, {"getResponseCode", "int", "0"},
            {"getHeaderField", "std::string", "std::string()"}},
        .forward_decls = {"InputStream", "OutputStream"}});
    add_class_stub("CodecJOrbis.h", {
        .name = "CodecJOrbis",
        .void_methods = {"initialize", "cleanup", "setAudioFormat", "reverseByteOrder"},
        .ret_methods = {{"read", "void*", "0"}, {"readAll", "void*", "0"}, {"getAudioFormat", "void*", "0"},
            {"initialized", "bool", "false"}, {"endOfStream", "bool", "false"}},
        .includes = {"URL", "URLConnection"},
        .extra = {"URL* url;", "URLConnection urlConnection;"}});
    add_class_stub("SoundSystem.h", {
        .name = "SoundSystem",
        .void_methods = {"cleanup", "newSource", "newStreamingSource", "play", "setListenerOrientation", "setListenerPosition", "setPitch", "setVolume", "stop", "pause", "removeSource", "backgroundMusic", "quickPlay", "quickStream", "setPosition", "fadeOut", "fadeOutIn"},
        .ret_methods = {{"playing", "bool", "false"}}});
    add_canonical("BufferedReader.h");
    add_canonical("FileReader.h");
    add_canonical("FileWriter.h");
    add_canonical("PrintWriter.h");
    add_class_stub("Graphics.h", {
        .name = "Graphics",
        .void_methods = {"drawImage", "dispose", "setColor", "fillRect", "drawString"},
        .trailer = {"inline Graphics* j2xSharedGraphics() { static Graphics _g; return &_g; }"}});
    add_class_stub("Cursor.h", {.name = "Cursor"});
    put_file("Class.h", verbatim::kClassHeader);
    add_class_stub("Robot.h", {
        .name = "Robot",
        .extra = {"void mouseMove(int x, int y) { J2X_ROBOT_MOUSE_MOVE(x, y); }"},
        .prelude = {"#ifndef J2X_ROBOT_MOUSE_MOVE", "#define J2X_ROBOT_MOUSE_MOVE(x, y) ((void)(x), (void)(y))", "#endif"}});
    add_class_stub("URL.h", {
        .name = "URL",
        .ret_methods = {{"openConnection", "URLConnection*", "J2X_URL_OPEN_CONNECTION(this)"},
            {"openStream", "InputStream*", "J2X_URL_OPEN_STREAM(this)"},
            {"toString", "std::string", "std::string()"}, {"getPath", "std::string", "std::string()"},
            {"getFile", "std::string", "std::string()"}, {"getProtocol", "std::string", "std::string()"},
            {"getHost", "std::string", "std::string()"}, {"getQuery", "std::string", "std::string()"},
            {"getPort", "int", "-1"}, {"getDefaultPort", "int", "-1"}, {"toURI", "void*", "0"}},
        .ctors = false,
        .forward_decls = {"InputStream", "URLConnection"},
        .extra = {"std::string _j2xpath;", "explicit URL(...) {}   // explicit: a string reaches URL only through the path ctor below", "URL(const std::string& p) : _j2xpath(p) {}",
                  // new URL(context, spec): a spec with its own scheme stands alone, else it replaces what follows the context's last '/'
                  "URL(URL* c, const std::string& s) : _j2xpath(s.find(\"://\") != std::string::npos || !c ? s : c->_j2xpath.substr(0, c->_j2xpath.rfind('/') + 1) + s) {}",
                  "const std::string& j2xResourcePath() const { return _j2xpath; }"},
        .prelude = {"#ifndef J2X_URL_OPEN_CONNECTION", "#define J2X_URL_OPEN_CONNECTION(u) 0", "#define J2X_URL_OPEN_STREAM(u) 0", "#endif"}});
    add_canonical("DecimalFormat.h");
    add_class_stub("ImageObserver.h", {.name = "ImageObserver"});
    put_file("Inflater.h", verbatim::kInflaterHeader);
    add_class_stub("Deflater.h", {
        .name = "Deflater",
        .void_methods = {"setInput", "finish", "end", "setLevel", "reset"},
        .ret_methods = {{"deflate", "int", "0"}, {"finished", "bool", "false"}, {"getTotalOut", "int", "0"}}});
    add_canonical("InflaterInputStream.h");
    add_canonical("DeflaterOutputStream.h");
    add_canonical("ByteArrayInputStream.h");
    add_class_stub("DataFormatException.h", {.name = "DataFormatException", .bases = {"Exception"}, .includes = {"Exception"}});
    add_class_stub("NoSuchAlgorithmException.h", {.name = "NoSuchAlgorithmException", .bases = {"Exception"}, .includes = {"Exception"}});
    // javax.crypto, as early Alpha's multiplayer invite code uses it (a PBE cipher over the server address): no cipher -- the stream
    // reads nothing, so the code is "not valid", which the game handles (it catches and answers false)
    add_class_stub("GeneralSecurityException.h", {.name = "GeneralSecurityException", .bases = {"Exception"}, .includes = {"Exception"}});
    add_class_stub("Cipher.h", {.name = "Cipher", .void_methods = {"init"}, .extra = {"static Cipher* getInstance(...) { return new Cipher(); }"}});
    add_class_stub("SecretKey.h", {.name = "SecretKey"});
    add_class_stub("SecretKeyFactory.h", {
        .name = "SecretKeyFactory",
        .ret_methods = {{"generateSecret", "SecretKey*", "0"}},
        .forward_decls = {"SecretKey"},
        .extra = {"static SecretKeyFactory* getInstance(...) { return new SecretKeyFactory(); }"}});
    add_class_stub("PBEKeySpec.h", {.name = "PBEKeySpec"});
    add_class_stub("PBEParameterSpec.h", {.name = "PBEParameterSpec"});
    add_class_stub("CipherInputStream.h", {.name = "CipherInputStream", .bases = {"InputStream"}, .includes = {"InputStream"},
                                            .extra = {"int read() { return -1; }"}});
    add_canonical("BigInteger.h");
    put_file("SoftReference.h", verbatim::kSoftReferenceHeader);
    put_file("InetAddress.h", verbatim::kInetAddressHeader);
    add_class_stub("StringReader.h", {.name = "StringReader", .bases = {"InputStreamReader"}, .includes = {"InputStreamReader"}});
    add_class_stub("Font.h", {.name = "Font", .ret_methods = {{"getSize", "int", "0"}, {"deriveFont", "Font*", "0"}}});
    add_class_stub("FontMetrics.h", {
        .name = "FontMetrics",
        .ret_methods = {{"stringWidth", "int", "0"}, {"getHeight", "int", "0"}, {"charWidth", "int", "0"}}});
    add_class_stub("AffineTransform.h", {.name = "AffineTransform"});
    add_class_stub("Graphics2D.h", {
        .name = "Graphics2D",
        .void_methods = {"setClip", "setRenderingHint", "translate", "scale", "setColor", "fillRect", "drawImage", "setTransform", "drawString", "dispose", "rotate", "shear"},
        .ret_methods = {{"getTransform", "AffineTransform*", "0"}, {"getClipBounds", "Rectangle*", "0"},
            {"getFontMetrics", "FontMetrics*", "0"}},
        .bases = {"Graphics"},
        .includes = {"Graphics", "FontMetrics"},
        .forward_decls = {"AffineTransform", "Rectangle"}});
    add_class_stub("BufferStrategy.h", {
        .name = "BufferStrategy",
        .void_methods = {"show"},
        .ret_methods = {{"getDrawGraphics", "Graphics*", "0"}},
        .includes = {"Graphics"},
        .forward_decls = {"Graphics"}});
    add_class_stub("LibraryLWJGLOpenAL.h", {.name = "LibraryLWJGLOpenAL"});
    add_class_stub("LibraryJavaSound.h", {.name = "LibraryJavaSound"});   // (early Indev registers it with SoundSystemConfig.addLibrary)
    add_class_stub("Throwable.h", {.name = "Throwable", .void_methods = {"printStackTrace"}, .ret_methods = kExceptionMethods});
    add_class_stub("URI.h", {
        .name = "URI",
        .ctors = false,
        .includes = {"URL"},
        .extra = {"std::string _j2xpath;", "URI(...) {}", "URI(const std::string& p) : _j2xpath(p) {}", "URL* toURL(...) { return new URL(_j2xpath); }", "JavaString getPath(...) { return JavaString(_j2xpath); }", "JavaString toString(...) { return JavaString(_j2xpath); }"}});
    add_canonical("File.h");
    add_class_stub("Applet.h", {
        .name = "Applet",
        .void_methods = {"validate", "setLayout", "add", "repaint", "requestFocus", "setVisible", "resize", "init", "start", "stop", "remove", "removeAll", "setSize", "setBackground", "setForeground", "addKeyListener", "addMouseListener", "addMouseMotionListener", "addFocusListener", "addComponentListener", "setFocusable", "setFocusTraversalKeysEnabled", "enableInputMethods"},
        .ret_methods = {{"getParameter", "std::string", "std::string()"}, {"getDocumentBase", "URL*", "0"},
            {"getCodeBase", "URL*", "0"}, {"getWidth", "int", "J2X_COMPONENT_WIDTH()"},
            {"getHeight", "int", "J2X_COMPONENT_HEIGHT()"}, {"getGraphics", "Graphics*", "0"},
            {"isActive", "bool", "false"}, {"isDisplayable", "bool", "false"}, {"getParent", "void*", "0"}},
        .bases = {"Component"},
        .includes = {"Component"},
        .forward_decls = {"URL", "Graphics"}});
    add_class_stub("Canvas.h", {
        .name = "Canvas",
        .void_methods = kAwtVoid,
        .ret_methods = {{"getWidth", "int", "J2X_COMPONENT_WIDTH()"}, {"getHeight", "int", "J2X_COMPONENT_HEIGHT()"},
            {"getGraphics", "Graphics*", "0"}, {"getBufferStrategy", "BufferStrategy*", "0"}},
        .bases = {"Component"},
        .includes = {"Component"},
        .forward_decls = {"Graphics", "BufferStrategy"},
        .extra = {"void createBufferStrategy(...) {}"}});
    add_class_stub("Component.h", {
        .name = "Component",
        .void_methods = kAwtVoid,
        .ret_methods = {{"getWidth", "int", "J2X_COMPONENT_WIDTH()"}, {"getHeight", "int", "J2X_COMPONENT_HEIGHT()"},
            {"getGraphics", "Graphics*", "0"}},
        .includes = {"Point"},
        .forward_decls = {"Graphics"},
        .extra = {"Point* getLocationOnScreen() { static Point _p; _p.x = J2X_COMPONENT_SCREEN_X(); _p.y = J2X_COMPONENT_SCREEN_Y(); return &_p; }"},
        .prelude = {"#ifndef J2X_COMPONENT_WIDTH", "#define J2X_COMPONENT_WIDTH() 0", "#define J2X_COMPONENT_HEIGHT() 0", "#endif",
                    "#ifndef J2X_COMPONENT_SCREEN_X", "#define J2X_COMPONENT_SCREEN_X() 0", "#define J2X_COMPONENT_SCREEN_Y() 0", "#endif"}});
    add_class_stub("FileDialog.h", {
        .name = "FileDialog",
        .void_methods = {"setVisible", "setMode", "setDirectory", "setFile", "setFilenameFilter", "setLocationRelativeTo"},
        .ret_methods = {{"getFile", "std::string", "std::string()"}, {"getDirectory", "std::string", "std::string()"}}});
    // Swing's file chooser: no dialog -- showOpenDialog / showSaveDialog answer CANCEL_OPTION (1), so a level load or save is cancelled
    add_class_stub("JFileChooser.h", {
        .name = "JFileChooser",
        .void_methods = {"setFileFilter", "setLocation", "setDialogTitle", "setCurrentDirectory", "setFileSelectionMode", "setAcceptAllFileFilterUsed"},
        .ret_methods = {{"showOpenDialog", "int", "1"}, {"showSaveDialog", "int", "1"}, {"getSelectedFile", "File*", "0"},
            {"createDialog", "JDialog*", "new JDialog()"}},   // (Indev 20100130's chooser overrides it and dresses the dialog)
        .includes = {"Point", "JDialog"},
        .forward_decls = {"File"},
        .extra = {"Point* getLocation() { static Point _p; return &_p; }", "static const int APPROVE_OPTION = 0;", "static const int CANCEL_OPTION = 1;"}});
    // the chooser's dialog: never shown (the chooser answers CANCEL), so its window calls do nothing
    add_class_stub("Window.h", {.name = "Window", .void_methods = {"toFront", "setVisible", "dispose"}});
    add_class_stub("JDialog.h", {
        .name = "JDialog",
        .void_methods = {"setLocationRelativeTo", "setAlwaysOnTop", "addWindowFocusListener", "setVisible", "dispose", "toFront",
            "setTitle", "setModal", "pack"},
        .bases = {"Window"},
        .includes = {"Window"}});
    add_class_stub("FileNameExtensionFilter.h", {.name = "FileNameExtensionFilter"});
    add_class_stub("PixelFormat.h", {   // (Beta 1.8: new PixelFormat().withDepthBits(24); the SDL window chooses its own)
        .name = "PixelFormat",
        .ret_methods = {{"withDepthBits", "PixelFormat*", "this"}, {"withStencilBits", "PixelFormat*", "this"},
            {"withAlphaBits", "PixelFormat*", "this"}, {"withSamples", "PixelFormat*", "this"}, {"withBitsPerPixel", "PixelFormat*", "this"}}});
    add_class_stub("ContextAttribs.h", {.name = "ContextAttribs"});
    add_class_stub("Point.h", {.name = "Point", .extra = {"int x;", "int y;"}});
    add_class_stub("PointerInfo.h", {
        .name = "PointerInfo",
        .includes = {"Point"},
        .extra = {"Point* getLocation() { static Point _p; _p.x = J2X_POINTER_X(); _p.y = J2X_POINTER_Y(); return &_p; }"},
        .prelude = {"#ifndef J2X_POINTER_X", "#define J2X_POINTER_X() 0", "#define J2X_POINTER_Y() 0", "#endif"}});
    add_ns_stub("MouseInfo.h", {
        .name = "MouseInfo",
        .ret_fns = {{"getPointerInfo", "PointerInfo", "PointerInfo()"}},
        .includes = {"PointerInfo"}});
    add_class_stub("Dialog.h", {.name = "Dialog"});
    add_class_stub("DataBuffer.h", {.name = "DataBuffer"});
    add_class_stub("DataBufferInt.h", {
        .name = "DataBufferInt",
        .ret_methods = {{"getData", "JArray<int >", "JArray<int >()"}},
        .bases = {"DataBuffer"},
        .includes = {"DataBuffer", "JArray"}});
    add_class_stub("Raster.h", {
        .name = "Raster",
        .ret_methods = {{"getDataBuffer", "DataBuffer*", "0"}},
        .forward_decls = {"DataBuffer"}});
    add_class_stub("WritableRaster.h", {.name = "WritableRaster", .bases = {"Raster"}, .includes = {"Raster"}});
    add_class_stub("BufferedImage.h", {
        .name = "BufferedImage",
        .void_methods = {"setRGB", "flush"},
        .ret_methods = {{"getWidth", "int", "0"}, {"getHeight", "int", "0"}, {"getRGB", "int", "0"}, {"getData", "void*", "0"},
            {"getRaster", "Raster", "Raster()"}, {"getGraphics", "Graphics*", "j2xSharedGraphics()"},
            {"createGraphics", "Graphics*", "j2xSharedGraphics()"}},
        .includes = {"Raster", "Graphics"}});

    members_.insert_or_assign("Matcher", StubMembers{.parent = std::nullopt, .methods = {{"matches", "bool"}, {"find", "bool"}, {"lookingAt", "bool"}, {"group", "JavaString"}, {"groupCount", "int"},
            {"start", "int"}, {"end", "int"}, {"reset", "Matcher*"}}});
    members_.insert_or_assign("Pattern", StubMembers{.parent = std::nullopt, .methods = {{"compile", "Pattern*"}, {"matcher", "Matcher*"}, {"matches", "bool"}, {"pattern", "JavaString"},
            {"split", "JArray<JavaString>"}}});
    members_.insert_or_assign("InputStreamReader", StubMembers{.parent = std::optional<std::string>("Reader"), .methods = {}});
    members_.insert_or_assign("Properties", StubMembers{.parent = std::nullopt, .methods = {{"getProperty", "std::string"}}});
    members_.insert_or_assign("NumberFormat", StubMembers{.parent = std::nullopt, .methods = {{"format", "std::string"}, {"getInstance", "NumberFormat*"}, {"getIntegerInstance", "NumberFormat*"},
            {"getNumberInstance", "NumberFormat*"}}});
    members_.insert_or_assign("MessageDigest", StubMembers{.parent = std::nullopt, .methods = {{"getInstance", "MessageDigest*"}, {"digest", "JArray<unsigned char >"}}});
    members_.insert_or_assign("BufferedReader", StubMembers{.parent = std::nullopt, .methods = {{"readLine", "std::string"}}});
    members_.insert_or_assign("FileReader", StubMembers{.parent = std::optional<std::string>("Reader"), .methods = {}});
    members_.insert_or_assign("FileWriter", StubMembers{.parent = std::optional<std::string>("Writer"), .methods = {}});
    members_.insert_or_assign("DecimalFormat", StubMembers{.parent = std::optional<std::string>("NumberFormat"), .methods = {{"format", "std::string"}}});
    members_.insert_or_assign("BigInteger", StubMembers{.parent = std::nullopt, .methods = {{"toString", "std::string"}, {"abs", "BigInteger*"}, {"signum", "int"}}});
    members_.insert_or_assign("File", StubMembers{.parent = std::nullopt, .methods = {{"getName", "std::string"}, {"getPath", "std::string"}, {"getAbsolutePath", "std::string"},
            {"toString", "std::string"}, {"getParent", "std::string"}, {"exists", "bool"},
            {"isDirectory", "bool"}, {"isFile", "bool"}, {"canRead", "bool"}, {"canWrite", "bool"},
            {"mkdir", "bool"}, {"mkdirs", "bool"}, {"renameTo", "bool"}, {"delete_", "bool"},
            {"createNewFile", "bool"}, {"length", "long long"}, {"lastModified", "long long"},
            {"getParentFile", "File*"}, {"getAbsoluteFile", "File*"}, {"toURI", "URI"},
            {"listFiles", "JArray<File* >"}, {"list", "void*"}}});
    members_.insert_or_assign("JList", StubMembers{.parent = std::nullopt, .methods = {{"iterator", "Iterator*"}}});
    members_.insert_or_assign("JSet", StubMembers{.parent = std::nullopt, .methods = {{"iterator", "Iterator*"}}});
    members_.insert_or_assign("JMap", StubMembers{.parent = std::nullopt, .methods = {{"entrySet", "JList"}, {"values", "JList"}, {"keySet", "JSet"}}});
    members_.insert_or_assign("ByteBuffer", StubMembers{.parent = std::nullopt, .methods = {{"allocateDirect", "ByteBuffer*"}, {"allocate", "ByteBuffer*"}, {"order", "ByteBuffer*"},
            {"asIntBuffer", "IntBuffer*"}, {"asFloatBuffer", "FloatBuffer*"}}});
}

}  // namespace cb::java::runtime
