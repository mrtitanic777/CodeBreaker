#ifndef FILE_H
#define FILE_H
// ============================ Canonical java.io.File ==========================================
// REAL path-backed File -- NOT a symbol-only stub. The whole IO layer depends on File::path:
// FileInputStream/FileOutputStream/RandomAccessFile and the canonical stream headers CreateFile/fopen
// `f->path`, and resource loading recurses listFiles()/isDirectory(). A no-op stub (empty path, empty
// listFiles) silently breaks ALL file IO -- saves never write and the sound/resource pools never fill
// (that is the "path is not a member of File" + silent no-audio class of failure).
//
// Win32/360 file APIs (GetFileAttributesA / FindFirstFileA / CreateDirectoryA / MoveFileA / DeleteFileA)
// come from <xtl.h>, pulled in by stdafx.h (the PCH) -- the same convention RandomAccessFile.h relies on.
// [360] equivalent: GetFileAttributes / FindFirstFile / CreateDirectory / MoveFile / DeleteFile (xboxkrnl).
// =============================================================================================
#include <string>
#include <vector>
#include "JavaString.h"
#include "JArray.h"
#include "URI.h"
class URL;

class File {
public:
	std::string path;                 // the real filesystem path (game:\... or Save:\...); read by streams/RAF

	// Join parent+child into a single 360 path: '/' -> '\', collapse the separator, drop a trailing one
	// ("resources/" -> "resources") so FindFirstFile / CreateFile see a clean path.
	void _join(const std::string& parent, const std::string& child) {
		path = parent;
		if (!path.empty()) { char c = path[path.size() - 1]; if (c != '\\' && c != '/') path += "\\"; }
		path += child;
		_norm();
	}
	void _norm() {
		for (std::string::size_type i = 0; i < path.size(); ++i) if (path[i] == '/') path[i] = '\\';
		if (!path.empty() && path[path.size() - 1] == '\\') path.erase(path.size() - 1);
	}

	// String params are const std::string&. std::string / const char* / JavaString (is-a std::string) all
	// bind here EXACTLY/via one clean conversion, so the variadic File(...) is never reached for them (a JCat
	// concat arg also converts -- both its operator std::string()/operator JavaString() yield the same string
	// content). Do NOT use const JavaString& here: `new File(_p)` where _p is a std::string then becomes
	// AMBIGUOUS between File(const JavaString&) and File(...), and MSVC resolves it to the variadic -> EMPTY
	// path (mcDataDir blank -> options.txt/saves collapse to a relative "options.txt", device not found).
	File(...) {}
	File(const std::string& p) : path(p) { _norm(); }
	File(const std::string& parent, const std::string& child) { _join(parent, child); }
	File(File* parent, const std::string& child) { _join(parent ? parent->path : std::string(), child); }
	virtual ~File() {}

	std::string _base() const {
		std::string::size_type s = path.find_last_of("\\/");
		return (s == std::string::npos) ? path : path.substr(s + 1);
	}
	std::string _parent() const {
		std::string::size_type s = path.find_last_of("\\/");
		return (s == std::string::npos) ? std::string() : path.substr(0, s);
	}

	virtual JavaString getName(...)         { return JavaString(_base()); }
	virtual JavaString getPath(...)         { return JavaString(path); }
	virtual JavaString getAbsolutePath(...) { return JavaString(path); }
	virtual JavaString toString(...)        { return JavaString(path); }
	virtual JavaString getParent(...)       { return JavaString(_parent()); }

	virtual bool exists(...)      { return GetFileAttributesA(path.c_str()) != ((DWORD)-1); }
	virtual bool isDirectory(...) { DWORD a = GetFileAttributesA(path.c_str());
	                                return a != ((DWORD)-1) && (a & FILE_ATTRIBUTE_DIRECTORY) != 0; }
	virtual bool isFile(...)      { DWORD a = GetFileAttributesA(path.c_str());
	                                return a != ((DWORD)-1) && (a & FILE_ATTRIBUTE_DIRECTORY) == 0; }
	virtual bool canRead(...)     { return exists(); }
	virtual bool canWrite(...)    { return exists(); }
	// mkdir/mkdirs return true OPTIMISTICALLY even when the create fails. On the read-only game:\ device
	// CreateDirectory fails, but Java callers treat mkdirs()==false as FATAL -- ThreadDownloadResources
	// throws "The working directory could not be created", which on this emulator (throw = no-op that
	// doesn't unwind) freezes the boot on the Mojang logo. We still ATTEMPT the create so a writable device
	// (emulator host / Save:) really gets the dir. Documented in beta-boot-staticinit-crash -- do NOT make
	// these return the real isDirectory() result.
	virtual bool mkdir(...) { CreateDirectoryA(path.c_str(), NULL); return true; }
	virtual bool mkdirs(...) {                       // attempt each missing parent segment, then the leaf
		if (path.empty()) return true;
		for (std::string::size_type i = 1; i < path.size(); ++i)
			if (path[i] == '\\') { std::string seg = path.substr(0, i); CreateDirectoryA(seg.c_str(), NULL); }
		CreateDirectoryA(path.c_str(), NULL);
		return true;
	}
	virtual bool createNewFile(...) {               // create iff absent (real STFS/DVD write via the streams)
		HANDLE h = CreateFile(path.c_str(), GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
		if (h == INVALID_HANDLE_VALUE) return false;
		CloseHandle(h); return true;
	}
	virtual bool renameTo(File* dst) { return dst && MoveFileA(path.c_str(), dst->path.c_str()) != 0; }
	virtual bool delete_(...)        { return DeleteFileA(path.c_str()) != 0 ||
	                                          RemoveDirectoryA(path.c_str()) != 0; }
	virtual long long length(...) {
		WIN32_FIND_DATAA fd; HANDLE h = FindFirstFileA(path.c_str(), &fd);
		if (h == INVALID_HANDLE_VALUE) return 0LL;
		FindClose(h);
		return ((long long)fd.nFileSizeHigh << 32) | (long long)fd.nFileSizeLow;
	}
	virtual long long lastModified(...) { return 0LL; }
	virtual File* getParentFile(...) { std::string p = _parent(); return p.empty() ? 0 : new File(p); }
	virtual File* getAbsoluteFile(...) { return new File(path); }
	virtual URI toURI(...) { return URI(); }
	virtual void* list(...) { return 0; }

	// Enumerate children (files AND directories -- resource loading recurses via isDirectory()).
	virtual JArray<File*> listFiles(...) {
		std::vector<File*> out;
		std::string pat = path;
		if (!pat.empty()) { char c = pat[pat.size() - 1]; if (c != '\\' && c != '/') pat += "\\"; }
		pat += "*";
		WIN32_FIND_DATAA fd;
		HANDLE h = FindFirstFileA(pat.c_str(), &fd);
		if (h != INVALID_HANDLE_VALUE) {
			do {
				const char* n = fd.cFileName;
				if (n[0] == '.' && (n[1] == 0 || (n[1] == '.' && n[2] == 0))) continue;   // skip . and ..
				out.push_back(new File(this, std::string(n)));
			} while (FindNextFileA(h, &fd));
			FindClose(h);
		}
		JArray<File*> r((int)out.size());
		for (std::vector<File*>::size_type i = 0; i < out.size(); ++i) r[(int)i] = out[i];
		return r;
	}
};
#endif // FILE_H
