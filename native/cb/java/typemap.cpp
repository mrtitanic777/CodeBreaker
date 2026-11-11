#include "cb/java/typemap.hpp"

namespace cb::java::typemap {

const NameSet& primitives() {
    static const NameSet kSet = {
        "boolean", "byte", "char", "double", "float", "int", "long", "short", "void"};
    return kSet;
}

const NameMap& java_to_cpp() {
    static const NameMap kMap = {
        {"boolean", "bool"}, {"byte", "signed char"}, {"short", "short"}, {"int", "int"},
        {"long", "long long"}, {"float", "float"}, {"double", "double"}, {"char", "unsigned char"},
        {"void", "void"},
        {"String", "JavaString"},
        {"ArrayList", "JList"}, {"List", "JList"}, {"LinkedList", "JList"},
        {"Collection", "JList"}, {"Iterable", "JList"},   // java.util.Collection/Iterable -> a by-value list
        {"HashMap", "JMap"}, {"HashSet", "JSet"}, {"TreeMap", "JMap"}, {"LinkedHashMap", "JMap"},
        {"TreeSet", "JSet"},                              // sorted set -> JSet (std::set subclass w/ Java API)
        {"Map", "JMap"}, {"Set", "JSet"},                 // Map/HashMap/... -> JMap (std::map subclass w/ Java API)
        {"Random", "Random"},
        // Java root / boxed types. Object -> JObject (a boxed-value type: holds null/int/char/String/
        // String[]/typed-pointer with RTTI), so Object[] autoboxing + instanceof + reflective recipe
        // decode work. JObject is a value (NOT a pointer); it converts to T*/JavaString via operators.
        {"Object", "JObject"},
        {"Integer", "int"}, {"Long", "long long"}, {"Float", "float"}, {"Double", "double"},
        {"Boolean", "bool"}, {"Byte", "signed char"}, {"Short", "short"}, {"Character", "unsigned char"},
    };
    return kMap;
}

const NameMap& port_note_types() {
    static const NameMap kMap = {{"Object", "was Object (untyped); verify the concrete type"}};
    return kMap;
}


const NameSet& containers() {
    static const NameSet kSet = {
        "ArrayList", "Collection", "HashMap", "HashSet", "Iterable", "LinkedHashMap", "LinkedList", "List",
        "Map", "Set", "TreeMap", "TreeSet"};
    return kSet;
}

const NameMap& raw_container_default() {
    static const NameMap kMap = {
        {"JList", "<JObject>"},
        {"JSet", "<JObject>"},
        {"std::vector", "<void*>"},
        {"std::set", "<void*>"},
        {"JMap", "<JObject, JObject>"},
        {"std::map", "<JObject, JObject>"},
    };
    return kMap;
}






const NameSet& value_stdlib() {
    static const NameSet kSet = {
        "ContextCapabilities", "String"};
    return kSet;
}

const NameSet& boxed_value_types() {
    static const NameSet kSet = {
        "Boolean", "Byte", "Character", "Double", "Float", "Integer", "Long", "Short"};
    return kSet;
}

const NameSet& runtime_static_holders() {
    static const NameSet kSet = {
        "ARBOcclusionQuery", "ARBVertexBufferObject", "BufferUtils", "ByteOrder", "Charset", "Collections",
        "Controllers", "Display", "GL11", "GL12", "GL15", "GLContext", "GLU", "ImageIO", "Keyboard",
        "Mouse", "MouseInfo", "NVFogDistance", "Pattern", "Runtime", "SoundSystemConfig", "Sys"};
    return kSet;
}

const NameSet& runtime_headers() {
    static const NameSet kSet = {
        "ARBOcclusionQuery", "ARBVertexBufferObject", "AWTException", "AffineTransform", "Applet",
        "ArrayIndexOutOfBoundsException", "BigInteger", "BindException", "BorderLayout", "BufferStrategy",
        "BufferUtils", "BufferedImage", "BufferedInputStream", "BufferedOutputStream", "BufferedReader",
        "ByteArrayInputStream", "ByteArrayOutputStream", "ByteBuffer", "ByteOrder", "Calendar", "Canvas",
        "CardLayout", "Charset", "Class", "ClassCastException", "ClassNotFoundException", "Clipboard",
        "CloneNotSupportedException", "CodecJOrbis", "CodecWav", "Collections", "Color", "Comparable",
        "Comparator", "Component", "ComponentEvent", "ComponentListener",
        "ConcurrentModificationException", "ConnectException", "Controllers", "Cursor", "DataBuffer",
        "DataBufferInt", "DataFlavor", "DataFormatException", "DataInput", "DataInputStream", "DataOutput",
        "DataOutputStream", "Date", "DateFormat", "DecimalFormat", "Deflater", "DeflaterOutputStream",
        "Dialog", "Dimension", "Display", "DisplayMode", "Document", "DocumentBuilder",
        "DocumentBuilderFactory", "EOFException", "Element", "Entry", "Error", "Exception", "File",
        "FileDialog", "FileFilter", "FileInputStream", "FileNotFoundException", "FileOutputStream",
        "FileReader", "FileWriter", "FilenameFilter", "FloatBuffer", "FlowLayout", "FocusEvent",
        "FocusListener", "Font", "FontMetrics", "Frame", "GL11", "GL12", "GL15", "GLContext", "GLU",
        "GZIPInputStream", "GZIPOutputStream", "Graphics", "Graphics2D", "GregorianCalendar",
        "GridBagLayout", "GridLayout", "HttpURLConnection", "IOException", "IllegalArgumentException",
        "IllegalStateException", "ImageIO", "ImageObserver", "IndexOutOfBoundsException", "InetAddress",
        "Inflater", "InflaterInputStream", "InputEvent", "InputStream", "InputStreamReader", "IntBuffer",
        "InterruptedException", "Iterator", "JOptionPane", "KeyEvent", "KeyListener", "Keyboard",
        "LWJGLException", "LibraryLWJGLOpenAL", "Locale", "Logger", "MalformedURLException", "Matcher",
        "MessageDigest", "Mouse", "MouseEvent", "MouseInfo", "MouseListener", "MouseMotionListener",
        "MouseWheelListener", "NVFogDistance", "NoSuchAlgorithmException", "NoSuchElementException",
        "NoSuchFieldError", "NoSuchMethodException", "Node", "NodeList", "NullPointerException",
        "NumberFormat", "NumberFormatException", "OutOfMemoryError", "OutputStream", "Panel", "Pattern",
        "Point", "PointerInfo", "PrintWriter", "Properties", "PushbackReader", "Random",
        "RandomAccessFile", "Raster", "Reader", "Rectangle", "Reference", "RenderingHints", "Robot",
        "Runnable", "Runtime", "RuntimeException", "SimpleDateFormat", "Socket", "SocketAddress",
        "SocketException", "SoftReference", "SoundSystem", "SoundSystemConfig", "Stack", "StringReader",
        "StringWriter", "Sys", "TextArea", "Thread", "Throwable", "Toolkit", "Transferable", "URI", "URL",
        "URLConnection", "UnknownHostException", "UnsupportedOperationException", "WindowAdapter",
        "WindowEvent", "WindowListener", "WritableRaster", "Writer", "ZipEntry", "ZipFile"};
    return kSet;
}

const NameSet& javart_statics() {
    static const NameSet kSet = {
        "Arrays", "Boolean", "Byte", "Character", "Double", "Float", "Integer", "Long", "Short", "String",
        "System"};
    return kSet;
}

const NameSet& runtime_enum_like() {
    static const NameSet kSet = [] {
        NameSet s = runtime_static_holders();
        s.insert(javart_statics().begin(), javart_statics().end());
        s.insert("Color");
        return s;
    }();
    return kSet;
}

const NameMap& math_api() {
    static const NameMap kMap = {
        {"abs", "abs"}, {"sqrt", "javaMathSqrt"}, {"sin", "sin"}, {"cos", "cos"}, {"tan", "tan"},
        {"atan", "atan"}, {"atan2", "atan2"}, {"asin", "asin"}, {"acos", "acos"},
        {"floor", "floorf"}, {"ceil", "ceilf"}, {"pow", "powf"}, {"exp", "expf"}, {"log", "logf"},
        {"min", "min"}, {"max", "max"},
        {"round", "javaMathRound"},
        // constants (no call): Math.PI is a member, not a method -> inline the literal
        {"PI", "3.141592653589793"},
        // no clean inline form -> free-function shims from the runtime prelude (JavaRT.h)
        {"random", "javaMathRandom"}, {"signum", "javaMathSignum"},
    };
    return kMap;
}

const NameMap& math_api_emit() {
    static const NameMap kMap = [] {
        NameMap m;
        for (const auto& [java_name, c_name] : math_api()) {
            const bool unchanged = c_name == java_name && c_name != "min" && c_name != "max";
            m.emplace(java_name, unchanged ? "::" + c_name : c_name);
        }
        return m;
    }();
    return kMap;
}

}  // namespace cb::java::typemap

