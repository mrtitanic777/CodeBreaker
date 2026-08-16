// ============================ PLATFORM REFERENCE ============================
// Hand-written Xbox 360 platform layer, emitted by j2x from
//   docs/platform/reference/FileIO.reference.h
// This is the REAL implementation -- it REPLACES the converter no-op stub for FileIO.
// (NOT converter-generated game code; edit the .reference.h source, then reconvert.)
// ==========================================================================

// FileIO.h — java.io bridge (File / streams / DataInput-Output / getResourceAsStream) for the
// Xbox 360 j2x port, backed by Win32/XDK CreateFile/ReadFile/WriteFile + a mounted STFS save drive.
// [Step 5]
//
// REFERENCE IMPLEMENTATION for the Xbox-project integration. It REPLACES the converter's no-op
// java.io stubs (j2x/runtime.py: File.h, InputStream.h, OutputStream.h, FileInputStream.h,
// FileOutputStream.h, DataInput.h, DataOutput.h, DataInputStream.h, DataOutputStream.h,
// BufferedReader.h -- all return 0/false/self today) AND the Class::getResourceAsStream member
// (Class.h, currently `return 0`). It is the same bridge as the OLD port's JavaIO.h /
// DataInput.h / DataOutput.h (../MinecraftAlpha/), modernized for the NEW converter's call shapes:
//   * a REAL 3-arg InputStream::read(unsigned char* buf, int off, int len)  -- ImageIO.reference.cpp
//     calls in->read(buf, 0, len); the old port only had read(b, len).
//   * DataInput.readShort/Int/Long/Float/Double/UTF are BYTE-AT-A-TIME BIG-ENDIAN (NBT-faithful).
//   * Class::getResourceAsStream(name) resolves "/terrain.png" / "/gui/gui.png" -> game:\Content\...
//     and returns a real FileInputStream (the dependency Step 3 flagged for ImageIO::read).
//
// NOT compiled by the j2x build (which only syntax-checks the no-op stubs). Lives in the Xbox project
// against the XDK. Grounded in JavaIO.h, DataInput.h, DataOutput.h, Save360.cpp and
// docs/platform/04-textures-resources-fileio.md.
//
// !!! BIG-ENDIAN HAZARD (Xbox 360 is big-endian PowerPC) !!!
// DataInput/DataOutput assemble multi-byte values BYTE-AT-A-TIME so the wire format is identical on
// PPC and on a little-endian dev host -- this is the whole reason NBT round-trips correctly. NEVER
// "optimize" a reader to `memcpy(&i32, ptr, 4)` or `*(int*)ptr` over a serialized buffer: that is
// byte-swapped on the 360 vs the Java/PC NBT format. Floats/doubles are safe ONLY because the
// int/long is assembled big-endian FIRST and then bit-cast. (This is the opposite of NIOBuffers.h,
// whose RAW vertex scratch IS native-order on purpose -- different data, different rule.)

#ifndef FILEIO_H
#define FILEIO_H

#include "stdafx.h"          // xtl.h (CreateFile/ReadFile/WriteFile/GetFileAttributes/...)
#include "JArray.h"
#include <string>
#include <vector>

// === java.io.InputStream / OutputStream =======================================
// FIXED BUG / J2X-BUGFIX 2026-08-13 inputstream-odr: InputStream is the ONE canonical definition in InputStream.h --
// defining it here too (under the same guard) caused a per-TU ODR/vtable mismatch that made
// FileInputStream::read()/available() dispatch to the base stub (0), so every resource load came back
// empty. Include the canonical one; FileInputStream (below) overrides its typed virtuals.
#include "InputStream.h"

// FIXED BUG outputstream-odr (C4743): OutputStream is now the ONE canonical definition in OutputStream.h (like
// InputStream/File/FileInputStream above) -- defining it here too gave it two vtable sizes across TUs and folded
// SocketOutputStream::write. BufferedOutputStream (the netcode wrapper) is canonical too.
#include "OutputStream.h"
#include "BufferedOutputStream.h"

// === java.io.File =============================================================
// Win32-backed. Adds (vs the OLD JavaIO.h) length()/renameTo()/getName()/getParent()/listFiles-lite
// the NEW faithful chunk saver expects. mkdirs() is made RECURSIVE here (the old single-level
// CreateDirectory was a documented pitfall -- ChunkLoader needs <x36>\<z36> nesting).
// FIXED BUG / J2X-BUGFIX 2026-08-13 file-odr: File is the ONE canonical definition in File.h (game TUs include
// that header). Defining it here too folded the vtable -> listFiles()/exists() dispatched wrong.
#include "File.h"

// === FileInputStream / FileOutputStream (CreateFile on the mounted drive) ======
// Eager slurp on open (chunk-sized data; same as the old port). Works on game:\ (read-only assets)
// and Save:\ (STFS save mount) once Save360_Init has mounted the container.
// FIXED BUG / J2X-BUGFIX 2026-08-13 filestream-odr: FileInputStream is ONE canonical definition in FileInputStream.h
// (game TUs include that stub-header). Defining it here too made the linker fold two vtables and pick the
// stub's -> read()/available() dispatched to InputStream base (0) -> every resource load returned empty.
#include "FileInputStream.h"

// FIXED BUG save-writes-noop: FileOutputStream is now the ONE canonical definition in FileOutputStream.h (real
// CreateFile(CREATE_ALWAYS)+WriteFile on close), like OutputStream/File. Game code #includes FileOutputStream.h
// and reaches the real WriteFile instead of the no-op stub. FileIO.h just aggregates it.
#include "FileOutputStream.h"

// === DataInput / DataOutput / DataInputStream / DataOutputStream ===============
// FIXED BUG stream-odr (C4743 family): these are now the ONE canonical definition each, in their own headers
// (DataInput.h / DataOutput.h / DataInputStream.h / DataOutputStream.h). The game's per-type #includes get the
// REAL classes instead of the no-op stubs that were shadowing them -- the whole packet path (NetworkManager
// TX/RX) was silent otherwise. FileIO.h just aggregates them (same pattern as InputStream/File above).
#include "DataInput.h"
#include "DataOutput.h"
#include "DataInputStream.h"
#include "DataOutputStream.h"

// GZIP wrappers live in CompressedStreamTools.cpp (its own local defs) -- kept OUT of FileIO.h to
// avoid a double definition (CompressedStreamTools is the only GZIP user; saves are raw/uncompressed).

// === BufferedReader (options.txt) =============================================
// Real readLine() over a wrapped InputStream (drained eagerly). \n / \r\n terminated, EOF -> empty
// std::string AND ready()==false. (GameSettings can also keep the old shortcut of splitting itself.)
class BufferedReader {
	std::vector<unsigned char> _buf;
	size_t _pos;
public:
	BufferedReader(InputStream* in) : _pos(0) {
		if (in) { unsigned char t[4096]; int n;
			while ((n = in->read(t, 0, (int)sizeof(t))) > 0) _buf.insert(_buf.end(), t, t + n); }
	}
	template<class R> BufferedReader(R*) : _pos(0) {}   // Reader types (FileReader/InputStreamReader): stub
	bool ready() { return _pos < _buf.size(); }
	int  read() { return (_pos < _buf.size()) ? (int)_buf[_pos++] : -1; }
	std::string readLine() {
		if (_pos >= _buf.size()) return std::string();      // EOF (caller treats empty+!ready as null)
		std::string line;
		while (_pos < _buf.size()) {
			char c = (char)_buf[_pos++];
			if (c == '\n') break;
			if (c == '\r') { if (_pos < _buf.size() && _buf[_pos] == '\n') ++_pos; break; }
			line.push_back(c);
		}
		return line;
	}
	void close() {}
};

// === Class::getResourceAsStream — game:\Content resolution ======================
// The NEW code reaches assets via TexturePackBase::getResourceAsStream -> Class::getResourceAsStream
// (Class.h member, currently `return 0`). Wire that member to call this. `name` is a JAR-style
// resource path ("/terrain.png", "/gui/gui.png"); resolve to game:\Content\<name> (leading '/'
// dropped, '/' -> '\'). Returns a real FileInputStream (NULL only on bad name).
InputStream* J2X_getResourceAsStream(const char* name);

#endif // FILEIO_H
