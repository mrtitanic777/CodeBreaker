// FileIO.cpp — out-of-line CreateFile bodies + game:\Content resource resolution + Save:\ mapping.
// [Step 5]  REFERENCE (Xbox-side; not built by j2x). See FileIO.reference.h for the design + the
// BIG-ENDIAN hazard banner. Grounded in JavaIO.h (FileInputStream/FileOutputStream) and Save360.cpp.

#include "stdafx.h"
#include "FileIO.h"
// #include "Save360.h"   // Save360_GetMountName()/_GetBaseDir() if you route via the live mount name

// ------------------------------------------------------------------------------
// FileInputStream::_load — slurp the whole file via CreateFile/GetFileSize/ReadFile.
// Works for game:\Content\*.png (read-only DVD) and Save:\* (STFS mount, after Save360_Init).
// ------------------------------------------------------------------------------
// Raw buffer (not std::vector) -- see fileinputstream-odr note in FileInputStream.h.
void FileInputStream::_load(const char* p) {
	HANDLE h = CreateFile(p, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (h == INVALID_HANDLE_VALUE) return;                  // _data stays NULL -> read() == -1 (EOF)
	DWORD sz = GetFileSize(h, NULL);
	if (sz != 0 && sz != ((DWORD)0xFFFFFFFF)) {
		_data = new unsigned char[sz];
		DWORD r = 0; ReadFile(h, _data, sz, &r, NULL); _size = (int)r;
	}
	CloseHandle(h);
}

// File* ctor is out-of-line: FileInputStream.h only forward-declares File (f->path needs the full type).
FileInputStream::FileInputStream(File* f) : _data(0), _size(0), _pos(0) { _load(f ? f->path.c_str() : ""); }

// ------------------------------------------------------------------------------
// FileOutputStream::close — write-on-close. CREATE_ALWAYS overwrites (no atomic temp+rename; the
// old port did the same -- a crash mid-write corrupts that one chunk file, pitfall #5 in doc 04).
// REMEMBER: the STFS container is not durable until Save360_Flush()/XContentFlush(). Call it after a
// save batch (e.g. world save / on pause) -- see 05b note.
// ------------------------------------------------------------------------------
void FileOutputStream::close() {
	if (_path.empty()) return;
	HANDLE h = CreateFile(_path.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (h == INVALID_HANDLE_VALUE) return;
	DWORD w = 0; if (!_buf.empty()) WriteFile(h, &_buf[0], (DWORD)_buf.size(), &w, NULL);
	CloseHandle(h);
	_buf.clear();
	// HARDWARE-VERIFY: after a save, Save360_Flush() must run or the bytes never persist to the device.
}

// ------------------------------------------------------------------------------
// Class::getResourceAsStream resolution: "/terrain.png" -> "game:\Content\terrain.png".
// Strip a single leading '/', convert '/' -> '\', prefix the read-only Content root.
// (Matches the old port's RenderEngine::toContentPath base behavior. The %blur%/%clamp%/## texture-
//  name PREFIXES are already stripped by the new RenderEngine via var1.substr(...) BEFORE this is
//  called -- see RenderEngine.cpp getTexture -- so we only handle the leading-slash + separators.)
// ------------------------------------------------------------------------------
static const char* kContentRoot = "game:\\Content";

static std::string toContentPath(const char* name) {
	std::string n = name ? name : "";
	std::string::size_type i = 0;
	while (i < n.size() && (n[i] == '/' || n[i] == '\\')) ++i;   // drop leading separator(s)
	std::string rel = n.substr(i);
	for (std::string::size_type k = 0; k < rel.size(); ++k) if (rel[k] == '/') rel[k] = '\\';
	std::string out = kContentRoot;
	out += '\\';
	out += rel;
	return out;
}

InputStream* J2X_getResourceAsStream(const char* name) {
	if (!name || !name[0]) return 0;
	std::string path = toContentPath(name);
	FileInputStream* in = new FileInputStream(path);
	if (in->available() <= 0) {
		// Basename fallback: an asset flattened out of a subfolder (old port pitfall #7 guard).
		// e.g. "/gui/gui.png" missing under gui\ -> retry game:\Content\gui.png.
		std::string::size_type s = path.find_last_of('\\');
		if (s != std::string::npos) {
			std::string basePath = std::string(kContentRoot) + "\\" + path.substr(s + 1);
			delete in;
			in = new FileInputStream(basePath);
		}
	}
	return in;   // NEVER NULL on a valid name: an empty stream is fine for the caller; a missing
	             // texture must fall back to default.png at the RenderEngine/Textures layer (pitfall #7).
}

// ------------------------------------------------------------------------------
// Save path mapping (documentation helper; the converted ChunkLoader builds these strings itself).
// The Java saveDir is the Save: mount root. Alpha chunk layout (base-36, two-level):
//     <saveDir>\<x&63 base36>\<z&63 base36>\c.<x base36>.<z base36>.dat
// With saveDir == "Save:", a chunk at (x,z) lands at e.g.  Save:\<a>\<b>\c.<x36>.<z36>.dat
// level.dat lives at Save:\level.dat. After Save360_Init mounts the container, plain
// CreateFile("Save:\...") (what FileInputStream/FileOutputStream use) just works -- no special-casing
// is needed here; File/FIS/FOS treat "Save:\..." like any other path. This function only documents
// the join + base36 so the mapping is greppable in one place.
// ------------------------------------------------------------------------------
static std::string intToBase36(int v) {
	if (v == 0) return "0";
	bool neg = v < 0; unsigned int u = neg ? (unsigned int)(-(long long)v) : (unsigned int)v;
	std::string s;
	const char* d = "0123456789abcdefghijklmnopqrstuvwxyz";
	while (u) { s.push_back(d[u % 36]); u /= 36; }
	if (neg) s.push_back('-');
	for (size_t a = 0, b = s.size() ? s.size() - 1 : 0; a < b; ++a, --b) { char t = s[a]; s[a] = s[b]; s[b] = t; }
	return s;
}

std::string J2X_chunkPath(const std::string& saveDirMount, int x, int z) {
	std::string p = saveDirMount;                            // e.g. "Save:"
	if (!p.empty() && p[p.size() - 1] != ':' && p[p.size() - 1] != '\\') p += '\\';
	p += intToBase36(x & 63); p += '\\';
	p += intToBase36(z & 63); p += '\\';
	p += "c." + intToBase36(x) + "." + intToBase36(z) + ".dat";
	return p;
	// NOTE: create the two dir levels first (File(dir).mkdirs()) -- mkdirs() here is recursive.
}
