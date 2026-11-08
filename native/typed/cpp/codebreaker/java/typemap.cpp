// codebreaker/java/typemap.cpp -- typed C++ from codebreaker.java.typemap.

#include "codebreaker/java/typemap.hpp"

#include <initializer_list>
#include <utility>

namespace codebreaker::java::typemap {

namespace {

// {'a', 'b', ...}
py::set<py::str> strset(std::initializer_list<const char*> names) {
    py::set<py::str> out;
    for (const char* n : names) out.add(py::str(n));
    return out;
}

// {'a': 'b', ...} (insertion order kept)
py::dict<py::str, py::str> strdict(std::initializer_list<std::pair<const char*, const char*>> items) {
    py::dict<py::str, py::str> out;
    for (const auto& kv : items) out.set(py::str(kv.first), py::str(kv.second));
    return out;
}

}  // namespace

const py::set<py::str> PRIMITIVES = strset({
    "boolean", "byte", "short", "int", "long", "float", "double", "char", "void",
});

const py::dict<py::str, py::str> JAVA_TO_CPP = strdict({
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
});

py::set<std::tuple<py::str, py::str>> UNSIGNED_BYTE_SCALAR_FIELDS;   // (seeded from the generic profile: empty)

const py::dict<py::str, py::str> PORT_NOTE_TYPES = strdict({
    {"Object", "was Object (untyped); verify the concrete type"},
});

const py::set<py::str> CONTAINERS = strset({
    "ArrayList", "List", "LinkedList", "Collection", "Iterable", "HashMap", "HashSet", "TreeMap",
    "LinkedHashMap", "TreeSet", "Map", "Set",
});

const py::dict<py::str, py::str> RAW_CONTAINER_DEFAULT = strdict({
    {"JList", "<JObject>"}, {"JSet", "<JObject>"}, {"std::vector", "<void*>"}, {"std::set", "<void*>"},
    // raw Map -> JMap<JObject, JObject>: a Java raw map mixes key/value types
    // (String/Integer/Character keys; any object/boxed value). JObject boxes
    // all of them (and has operator< for the key), so put/get/containsKey/remove
    // all bind regardless of the concrete Java types. Keys/values convert in
    // implicitly; reads convert out via the cast the converter already emits.
    {"JMap", "<JObject, JObject>"}, {"std::map", "<JObject, JObject>"},
});

const py::set<py::str> VALUE_STDLIB = strset({"String", "ContextCapabilities"});

const py::set<py::str> BOXED_VALUE_TYPES = strset({
    "Integer", "Long", "Float", "Double", "Boolean", "Byte", "Short", "Character",
});

py::set<py::str> KNOWN_NULLABLE;           // (seeded from the generic profile: empty)
py::set<py::str> KNOWN_NULLABLE_FIELDS;    // (seeded from the generic profile: empty)
py::set<py::str> KNOWN_NULLABLE_GETTERS;   // (seeded from the generic profile: empty)
py::dict<py::str, py::str> NULLABLE_GETTER_RETURN_TYPE;   // (seeded from the generic profile: empty)

const py::set<py::str> RUNTIME_STATIC_HOLDERS = strset({
    "GL11", "GL12", "GL15", "GLU", "NVFogDistance", "Keyboard", "Mouse", "Display",
    "ARBOcclusionQuery", "ARBVertexBufferObject", "BufferUtils", "ImageIO",
    "Sys", "GLContext", "Controllers", "Collections",
    "Pattern",                // java.util.regex Pattern.compile(...) is static
    "Runtime", "ByteOrder",   // Runtime.getRuntime(), ByteOrder.BIG_ENDIAN
    "Charset",                // java.nio.charset Charset.forName("UTF-8") is static
    "MouseInfo",              // java.awt MouseInfo.getPointerInfo() is static (Indev MouseHelper)
    "SoundSystemConfig",
});

const py::set<py::str> RUNTIME_HEADERS = strset({
    "Random", "Runnable", "Thread", "Comparator", "File",
    "FilenameFilter", "Canvas", "Applet",
    "DisplayMode", "FloatBuffer", "IntBuffer", "ByteBuffer",
    "DataInput", "DataOutput", "SoundSystem", "BufferedImage", "BufferedReader",
    "FileReader", "FileWriter", "PrintWriter",
    "Graphics", "Component", "Cursor", "Robot", "URL", "FileDialog",
    "DecimalFormat", "ImageObserver", "Class",
    "InputStream", "OutputStream", "DataInputStream", "DataOutputStream",
    "BufferedInputStream", "BufferedOutputStream", "FileInputStream",
    "FileOutputStream", "ZipFile", "Socket", "DateFormat", "Dimension",
    "KeyListener", "MouseListener", "MouseMotionListener", "MouseWheelListener",
    "WindowListener", "ComponentListener", "FocusListener", "WindowAdapter",
    "KeyEvent", "MouseEvent", "WindowEvent", "ComponentEvent", "FocusEvent",
    "InputEvent", "Panel", "Comparable", "CodecJOrbis", "URLConnection",
    "BorderLayout", "GridLayout", "FlowLayout", "CardLayout", "GridBagLayout", "Color",
    "Frame", "Pattern", "Matcher",
    "FileFilter", "ByteArrayOutputStream", "InputStreamReader", "HttpURLConnection",
    "ZipEntry", "Node", "NodeList", "Element", "Document", "DocumentBuilder",
    "DocumentBuilderFactory", "Date", "SimpleDateFormat", "GZIPInputStream",
    "GZIPOutputStream", "Runtime", "ByteOrder",
    "RandomAccessFile", "Writer", "StringWriter", "PushbackReader", "Stack",
    "Properties", "Entry", "NumberFormat", "MessageDigest", "RenderingHints",
    "DataFlavor", "Reference", "Logger", "Calendar", "GregorianCalendar",
    "Toolkit", "TextArea", "Transferable", "Locale", "Rectangle",
    "Throwable", "Exception", "Error", "RuntimeException", "IOException",
    "InterruptedException", "IllegalStateException", "IllegalArgumentException",
    "MalformedURLException", "AWTException", "LWJGLException", "NoSuchFieldError",
    "FileNotFoundException", "EOFException", "SocketException", "UnknownHostException",
    "ConnectException", "BindException", "NumberFormatException", "NullPointerException",
    "ArrayIndexOutOfBoundsException", "IndexOutOfBoundsException", "ClassCastException",
    "NoSuchElementException", "UnsupportedOperationException", "ConcurrentModificationException",
    "OutOfMemoryError", "NoSuchMethodException", "CloneNotSupportedException",
    "ClassNotFoundException",
    // additional java.* stubs surfaced by the Beta corpus (runtime.py _GENERATED)
    "Inflater", "Deflater", "InflaterInputStream", "DeflaterOutputStream",
    "ByteArrayInputStream", "DataFormatException", "NoSuchAlgorithmException",
    "BigInteger", "SoftReference", "InetAddress", "StringReader", "Font", "FontMetrics",
    "AffineTransform", "DataBufferInt", "Graphics2D", "BufferStrategy", "LibraryLWJGLOpenAL",
    "CodecWav", "FontMetrics", "Iterator", "Reader", "DataBuffer", "Raster", "WritableRaster",
    "SocketAddress", "URI", "Clipboard", "JOptionPane",
    // java.awt mouse/point stubs surfaced by Indev's MouseHelper + level-save Dialog
    "Point", "PointerInfo", "Dialog",
}) | RUNTIME_STATIC_HOLDERS;

const py::set<py::str> JAVART_STATICS = strset({
    "System", "Integer", "Long", "Double", "Float", "Character", "Arrays",
    "Short", "Byte", "Boolean", "String",   // String.valueOf/format statics (type still -> JavaString)
});

const py::set<py::str> RUNTIME_ENUM_LIKE = (RUNTIME_STATIC_HOLDERS | JAVART_STATICS) | strset({"Color"});

const py::dict<py::str, py::str> MATH_API = strdict({
    // TRIG is DOUBLE, not float. Java's Math.sin/cos/tan are double, and the game calls them on LARGE
    // arguments -- e.g. sin(System.currentTimeMillis()/100.0) for the pulsing block-highlight alpha. `sinf`
    // cannot range-reduce an argument that large (it returns NaN on the 360 PPC CRT), which packed the
    // highlight's alpha to 0 -> invisible. `sin` (double, native on PPC -> no perf cost) reduces correctly.
    // J2X-BUGFIX trig-double-precision.
    // J2X-BUGFIX sqrt-double-precision: Math.sqrt returns DOUBLE in Java. Mapping it to sqrtf (float) made
    // `atan2(Math.sqrt(...), doubleExpr)` an ambiguous (float,double) overload (RenderGlobal). A bare `sqrt`
    // doesn't fix it either: MSVC's <math.h> has a float sqrt(float) overload, so a float argument still
    // yields float. The JavaRT.h shim javaMathSqrt(double) forces the promotion (double sqrt is native on
    // PPC -- no perf cost). MathHelper.sqrt_float is a separate key and stays sqrtf.
    {"abs", "abs"}, {"sqrt", "javaMathSqrt"}, {"sin", "sin"}, {"cos", "cos"}, {"tan", "tan"},
    {"atan", "atan"}, {"atan2", "atan2"}, {"asin", "asin"}, {"acos", "acos"},
    {"floor", "floorf"}, {"ceil", "ceilf"}, {"pow", "powf"}, {"exp", "expf"}, {"log", "logf"},
    {"min", "min"}, {"max", "max"},
    // Math.round(x) returns an integer (round-half-up); overloaded free-function shim in the prelude
    // (int for float, long long for double) -- there is no single C stdlib call with Java's semantics.
    {"round", "javaMathRound"},
    // constants (no call): Math.PI is a member, not a method -> inline the literal. A DOUBLE, as in Java.
    // J2X-BUGFIX math-pi-double: it was the float 3.1415927F, so `(float)Math.sin(i * Math.PI * 2.0D / 65536.0D)`
    // (MathHelper's SIN_TABLE) was computed with a single-precision PI and every MathHelper.sin/cos of a
    // non-zero angle came out a few ulps off Java's -- found by the java kind's behavioural judge. Where the
    // game casts it, `(float)Math.PI`, the result is the same float as before.
    {"PI", "3.141592653589793"},
    // no clean inline form -> free-function shims from the runtime prelude (JavaRT.h)
    {"random", "javaMathRandom"}, {"signum", "javaMathSignum"},
});

// {k: ('::' + v if (v == k and v not in ('min', 'max')) else v) for k, v in MATH_API.items()}
static py::dict<py::str, py::str> math_api_emit() {
    py::dict<py::str, py::str> out;
    for (const auto& kv : MATH_API.items()) {
        const py::str& k = std::get<0>(kv);
        const py::str& v = std::get<1>(kv);
        bool qualify = v == k && v != py::str("min") && v != py::str("max");
        out.set(k, qualify ? py::str("::") + v : v);
    }
    return out;
}
const py::dict<py::str, py::str> MATH_API_EMIT = math_api_emit();

py::set<py::str> DEFAULT_VALUE_TYPES;   // (seeded from the generic profile: empty)

void _reseed(const Profile& p) {
    UNSIGNED_BYTE_SCALAR_FIELDS = py::set<std::tuple<py::str, py::str>>::from(p.unsigned_byte_scalar_fields);
    KNOWN_NULLABLE = py::set<py::str>::from(p.nullable);
    KNOWN_NULLABLE_FIELDS = py::set<py::str>::from(p.nullable_fields);
    KNOWN_NULLABLE_GETTERS = py::set<py::str>::from(p.nullable_getters);
    NULLABLE_GETTER_RETURN_TYPE = p.nullable_getter_return_type.copy();
    DEFAULT_VALUE_TYPES = py::set<py::str>::from(p.default_value_types);
}

}  // namespace codebreaker::java::typemap
