#include "cb/java/stubs.hpp"

#include <algorithm>
#include <string>
#include <unordered_map>

namespace cb::java::stubs {

namespace {

const std::vector<SubsystemInfo>& table() {
    static const std::vector<SubsystemInfo> kTable = {
        {Subsystem::Rendering, "rendering", "Rendering / Display",
         "OpenGL/LWJGL stubs that need a D3D9 backend (vertex submission, texture loading, display management, framebuffer ops).",
         "D3D9 (d3d9.h, d3dx9.h), HLSL shaders",
         {"ARBOcclusionQuery", "ARBVertexBufferObject", "Applet", "BorderLayout", "BufferedImage", "Canvas",
          "CardLayout", "Color", "Component", "ComponentEvent", "ComponentListener", "Cursor", "Dimension",
          "Display", "DisplayMode", "FlowLayout", "FocusEvent", "FocusListener", "Font", "FontMetrics", "Frame",
          "GL11", "GL12", "GL15", "GLU", "Graphics", "GridBagLayout", "GridLayout", "ImageIO", "ImageObserver",
          "Panel", "Robot", "ScreenShotHelper", "WindowAdapter", "WindowEvent", "WindowListener"}},
        {Subsystem::Audio, "audio", "Audio",
         "OpenAL/paulscode stubs that need a platform audio backend (sound effects, music, 3D positional audio).",
         "XACT3 (xact3.h, xact3wb.h) or XAudio2",
         {"CodecJOrbis", "CodecWav", "SoundBuffer", "SoundManager", "SoundPool", "SoundSystem",
          "SoundSystemConfig"}},
        {Subsystem::Input, "input", "Input / Controllers",
         "LWJGL Keyboard/Mouse stubs that need platform input (controller, touchscreen, keyboard emulation).",
         "XInput2 (xinput2.h) for Xbox 360 controller",
         {"Controllers", "InputEvent", "KeyEvent", "KeyListener", "Keyboard", "Mouse", "MouseEvent",
          "MouseListener", "MouseMotionListener", "MouseWheelListener"}},
        {Subsystem::Io, "io", "File I/O / Streams",
         "Java I/O stubs that need platform file system access (file read/write, data streams, buffered I/O).",
         "Platform file API (CreateFile/ReadFile on Xbox, or stdio)",
         {"BufferedInputStream", "BufferedOutputStream", "BufferedReader", "ByteArrayOutputStream", "DataInput",
          "DataInputStream", "DataOutput", "DataOutputStream", "File", "FileDialog", "FileInputStream",
          "FileNotFoundException", "FileOutputStream", "FileReader", "FileWriter", "InputStream", "OutputStream",
          "PrintWriter"}},
        {Subsystem::Compression, "compression", "Compression",
         "GZIP/ZIP stubs for save data and resource loading.",
         "LZX (xcompress.h) on Xbox 360, or zlib/miniz",
         {"GZIPInputStream", "GZIPOutputStream", "Inflate", "ZipEntry", "ZipFile"}},
        {Subsystem::Networking, "networking", "Networking",
         "Java socket/URL stubs for multiplayer and web requests.",
         "Winsock2 / XNet (xnetn.h) on Xbox 360",
         {"BindException", "ConnectException", "HttpURLConnection", "InetAddress", "MalformedURLException",
          "Socket", "SocketException", "URL", "URLConnection", "UnknownHostException"}},
        {Subsystem::Nio, "nio", "NIO Buffers",
         "Java NIO buffer stubs (ByteBuffer, IntBuffer, FloatBuffer) used for vertex data and texture uploads.",
         "Direct memory buffers (malloc/free or D3D vertex buffers)",
         {"BufferUtils", "ByteBuffer", "FloatBuffer", "IntBuffer"}},
        {Subsystem::Threading, "threading", "Threading",
         "Java Thread stubs for background tasks (downloads, network, sleep).",
         "Platform threads (CreateThread / _beginthreadex)",
         {"Thread", "ThreadCheckHasPaid", "ThreadConnectToServer", "ThreadDownloadImage",
          "ThreadDownloadResources", "ThreadShutdown", "ThreadSleepForever", "ThreadStatistics"}},
        {Subsystem::Text, "text", "Text / Locale / Date",
         "Java text formatting stubs (SimpleDateFormat, Locale, Calendar).",
         "strftime / manual formatting (no ICU on 360)",
         {"Calendar", "DateFormat", "DecimalFormat", "Locale", "SimpleDateFormat"}},
        {Subsystem::Reflection, "reflection", "Reflection / Class",
         "Java reflection stubs (Class, typeid). Minecraft uses these for entity/tile-entity registry lookups.",
         "RTTI (typeid) + manual factory registry",
         {"Class"}},
        {Subsystem::Exceptions, "exceptions", "Exceptions",
         "Java exception type stubs. Mostly trivial (extend std::exception).",
         "C++ exceptions (std::exception subclasses)",
         {"AWTException", "ArrayIndexOutOfBoundsException", "ClassCastException", "ClassNotFoundException",
          "CloneNotSupportedException", "ConcurrentModificationException", "ConnectException", "EOFException",
          "Exception", "IOException", "IllegalArgumentException", "IllegalStateException",
          "IndexOutOfBoundsException", "InterruptedException", "LWJGLException", "NoSuchElementException",
          "NoSuchMethodException", "NullPointerException", "NumberFormatException", "RuntimeException",
          "UnsupportedOperationException"}},
        {Subsystem::System, "system", "System / Runtime",
         "JVM system stubs (Sys timer, error handling).",
         "QueryPerformanceCounter / GetTickCount",
         {"Comparable", "Error", "NoSuchFieldError", "OutOfMemoryError", "Sys", "Throwable"}},
    };
    return kTable;
}

}  // namespace

std::span<const SubsystemInfo> subsystems() { return table(); }

const SubsystemInfo* classify_stub(std::string_view class_name) {
    static const std::unordered_map<std::string_view, const SubsystemInfo*> kOwner = [] {
        std::unordered_map<std::string_view, const SubsystemInfo*> owner;   // lookup only, never iterated
        for (const SubsystemInfo& info : table()) {
            for (std::string_view member : info.members) owner.emplace(member, &info);   // first claim wins
        }
        return owner;
    }();
    auto it = kOwner.find(class_name);
    return it == kOwner.end() ? nullptr : it->second;
}

}  // namespace cb::java::stubs
