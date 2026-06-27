#ifndef FILE_H
#define FILE_H

// ============================ EXTERNAL-LIBRARY STUB ============================
// Stands in for java.io.File.
// This is NOT part of the decompiled Minecraft source -- it is an external Java
// library. Symbol-only: methods are no-ops / return defaults so the converted game
// code compiles and links; the real behaviour is the Xbox 360 platform layer
// (Direct3D 9 / XInput / file IO / sound).
// ==============================================================================
#include <string>
class URL;

class File {
public:
	File(...) {}
	virtual ~File() {}
	virtual std::string getName(...) { return std::string(); }
	virtual std::string getPath(...) { return std::string(); }
	virtual std::string getAbsolutePath(...) { return std::string(); }
	virtual std::string toString(...) { return std::string(); }
	virtual bool exists(...) { return false; }
	virtual bool isDirectory(...) { return false; }
	virtual bool isFile(...) { return false; }
	virtual bool mkdir(...) { return false; }
	virtual bool mkdirs(...) { return false; }
	virtual bool renameTo(...) { return false; }
	virtual long long length(...) { return 0LL; }
	virtual File* getParentFile(...) { return 0; }
	virtual File* getAbsoluteFile(...) { return 0; }
	virtual void* toURI(...) { return 0; }
	virtual void* listFiles(...) { return 0; }
	virtual long long lastModified(...) { return 0LL; }
	virtual void* list(...) { return 0; }
	virtual std::string getParent(...) { return std::string(); }
	virtual bool canRead(...) { return false; }
	virtual bool canWrite(...) { return false; }
};
#endif // FILE_H
