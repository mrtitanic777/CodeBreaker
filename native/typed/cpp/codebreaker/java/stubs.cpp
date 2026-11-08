// codebreaker/java/stubs.cpp -- C++17 port of codebreaker.java.stubs (see stubs.hpp).

#include "codebreaker/java/stubs.hpp"

namespace codebreaker::java::stubs {

namespace {

py::dict<py::str, Subsystem> build_subsystems() {
    py::dict<py::str, Subsystem> d;
    d.set(py::str("rendering"), Subsystem{
        py::str("Rendering / Display"),
        py::str("OpenGL/LWJGL stubs that need a D3D9 backend (vertex submission, "
                "texture loading, display management, framebuffer ops)."),
        py::str("D3D9 (d3d9.h, d3dx9.h), HLSL shaders"),
        py::set<py::str>{
            py::str("GL11"), py::str("GL12"), py::str("GL15"), py::str("GLU"), py::str("Display"), py::str("DisplayMode"),
            py::str("BufferedImage"), py::str("ImageIO"), py::str("Canvas"), py::str("Graphics"), py::str("Frame"),
            py::str("Panel"), py::str("Component"), py::str("Applet"), py::str("Color"), py::str("Font"), py::str("FontMetrics"),
            py::str("CardLayout"), py::str("BorderLayout"), py::str("FlowLayout"), py::str("GridLayout"),
            py::str("GridBagLayout"), py::str("Dimension"), py::str("Cursor"), py::str("Robot"),
            py::str("ComponentEvent"), py::str("ComponentListener"),
            py::str("FocusEvent"), py::str("FocusListener"),
            py::str("WindowAdapter"), py::str("WindowEvent"), py::str("WindowListener"),
            py::str("ARBOcclusionQuery"), py::str("ARBVertexBufferObject"),
            py::str("ImageObserver"), py::str("ScreenShotHelper"),
        }});
    d.set(py::str("audio"), Subsystem{
        py::str("Audio"),
        py::str("OpenAL/paulscode stubs that need a platform audio backend "
                "(sound effects, music, 3D positional audio)."),
        py::str("XACT3 (xact3.h, xact3wb.h) or XAudio2"),
        py::set<py::str>{
            py::str("SoundSystem"), py::str("SoundPool"), py::str("SoundManager"),
            py::str("CodecJOrbis"), py::str("CodecWav"), py::str("SoundSystemConfig"), py::str("SoundBuffer"),
        }});
    d.set(py::str("input"), Subsystem{
        py::str("Input / Controllers"),
        py::str("LWJGL Keyboard/Mouse stubs that need platform input "
                "(controller, touchscreen, keyboard emulation)."),
        py::str("XInput2 (xinput2.h) for Xbox 360 controller"),
        py::set<py::str>{
            py::str("Keyboard"), py::str("Mouse"), py::str("Controllers"),
            py::str("MouseEvent"), py::str("MouseListener"), py::str("MouseMotionListener"),
            py::str("MouseWheelListener"), py::str("KeyEvent"), py::str("KeyListener"),
            py::str("InputEvent"),
        }});
    d.set(py::str("io"), Subsystem{
        py::str("File I/O / Streams"),
        py::str("Java I/O stubs that need platform file system access "
                "(file read/write, data streams, buffered I/O)."),
        py::str("Platform file API (CreateFile/ReadFile on Xbox, or stdio)"),
        py::set<py::str>{
            py::str("File"), py::str("FileInputStream"), py::str("FileOutputStream"),
            py::str("InputStream"), py::str("OutputStream"),
            py::str("DataInput"), py::str("DataOutput"), py::str("DataInputStream"), py::str("DataOutputStream"),
            py::str("BufferedInputStream"), py::str("BufferedOutputStream"), py::str("BufferedReader"),
            py::str("PrintWriter"), py::str("ByteArrayOutputStream"),
            py::str("FileReader"), py::str("FileWriter"), py::str("FileDialog"),
            py::str("FileNotFoundException"),
        }});
    d.set(py::str("compression"), Subsystem{
        py::str("Compression"),
        py::str("GZIP/ZIP stubs for save data and resource loading."),
        py::str("LZX (xcompress.h) on Xbox 360, or zlib/miniz"),
        py::set<py::str>{
            py::str("GZIPInputStream"), py::str("GZIPOutputStream"), py::str("ZipFile"), py::str("ZipEntry"),
            py::str("Inflate"),
        }});
    d.set(py::str("networking"), Subsystem{
        py::str("Networking"),
        py::str("Java socket/URL stubs for multiplayer and web requests."),
        py::str("Winsock2 / XNet (xnetn.h) on Xbox 360"),
        py::set<py::str>{
            py::str("Socket"), py::str("URL"), py::str("URLConnection"), py::str("HttpURLConnection"),
            py::str("InetAddress"), py::str("BindException"), py::str("SocketException"),
            py::str("MalformedURLException"), py::str("ConnectException"), py::str("UnknownHostException"),
        }});
    d.set(py::str("nio"), Subsystem{
        py::str("NIO Buffers"),
        py::str("Java NIO buffer stubs (ByteBuffer, IntBuffer, FloatBuffer) "
                "used for vertex data and texture uploads."),
        py::str("Direct memory buffers (malloc/free or D3D vertex buffers)"),
        py::set<py::str>{
            py::str("ByteBuffer"), py::str("IntBuffer"), py::str("FloatBuffer"), py::str("BufferUtils"),
        }});
    d.set(py::str("threading"), Subsystem{
        py::str("Threading"),
        py::str("Java Thread stubs for background tasks (downloads, network, sleep)."),
        py::str("Platform threads (CreateThread / _beginthreadex)"),
        py::set<py::str>{
            py::str("Thread"), py::str("ThreadDownloadImage"), py::str("ThreadDownloadResources"),
            py::str("ThreadSleepForever"), py::str("ThreadConnectToServer"), py::str("ThreadStatistics"),
            py::str("ThreadShutdown"), py::str("ThreadCheckHasPaid"),
        }});
    d.set(py::str("text"), Subsystem{
        py::str("Text / Locale / Date"),
        py::str("Java text formatting stubs (SimpleDateFormat, Locale, Calendar)."),
        py::str("strftime / manual formatting (no ICU on 360)"),
        py::set<py::str>{
            py::str("SimpleDateFormat"), py::str("Locale"), py::str("Calendar"), py::str("DateFormat"), py::str("DecimalFormat"),
        }});
    d.set(py::str("reflection"), Subsystem{
        py::str("Reflection / Class"),
        py::str("Java reflection stubs (Class, typeid). Minecraft uses these for "
                "entity/tile-entity registry lookups."),
        py::str("RTTI (typeid) + manual factory registry"),
        py::set<py::str>{
            py::str("Class"),
        }});
    d.set(py::str("exceptions"), Subsystem{
        py::str("Exceptions"),
        py::str("Java exception type stubs. Mostly trivial (extend std::exception)."),
        py::str("C++ exceptions (std::exception subclasses)"),
        py::set<py::str>{
            py::str("Exception"), py::str("RuntimeException"), py::str("IOException"),
            py::str("AWTException"), py::str("ArrayIndexOutOfBoundsException"),
            py::str("ClassCastException"), py::str("ClassNotFoundException"),
            py::str("CloneNotSupportedException"), py::str("NumberFormatException"),
            py::str("IllegalArgumentException"), py::str("IllegalStateException"),
            py::str("IndexOutOfBoundsException"), py::str("InterruptedException"),
            py::str("LWJGLException"), py::str("NoSuchElementException"),
            py::str("NoSuchMethodException"), py::str("NullPointerException"),
            py::str("UnsupportedOperationException"), py::str("ConcurrentModificationException"),
            py::str("EOFException"), py::str("ConnectException"),
        }});
    d.set(py::str("system"), Subsystem{
        py::str("System / Runtime"),
        py::str("JVM system stubs (Sys timer, error handling)."),
        py::str("QueryPerformanceCounter / GetTickCount"),
        py::set<py::str>{
            py::str("Sys"), py::str("Throwable"), py::str("Error"), py::str("OutOfMemoryError"), py::str("NoSuchFieldError"),
            py::str("Comparable"),
        }});
    return d;
}

}  // namespace

const py::dict<py::str, Subsystem> SUBSYSTEMS = build_subsystems();

std::tuple<py::str, std::optional<Subsystem>> classify_stub(const py::str& class_name) {
    for (const auto& sid : SUBSYSTEMS) {
        const Subsystem& info = SUBSYSTEMS.at(sid);
        if (info.members.contains(class_name)) return {sid, info};
    }
    return {py::str("unknown"), std::nullopt};
}

}  // namespace codebreaker::java::stubs
