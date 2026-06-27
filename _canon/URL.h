#ifndef URL_H
#define URL_H

// ============================ EXTERNAL-LIBRARY STUB ============================
// Stands in for java.net.URL.
// This is NOT part of the decompiled Minecraft source -- it is an external Java
// library. Symbol-only: methods are no-ops / return defaults so the converted game
// code compiles and links; the real behaviour is the Xbox 360 platform layer
// (Direct3D 9 / XInput / file IO / sound).
// ==============================================================================
#include <string>
class InputStream;

class URL {
public:
	URL(...) {}
	virtual ~URL() {}
	virtual void* openConnection(...) { return 0; }
	virtual InputStream* openStream(...) { return 0; }
	virtual std::string toString(...) { return std::string(); }
	virtual std::string getPath(...) { return std::string(); }
	virtual std::string getFile(...) { return std::string(); }
	virtual std::string getProtocol(...) { return std::string(); }
	virtual std::string getHost(...) { return std::string(); }
	virtual std::string getQuery(...) { return std::string(); }
};
#endif // URL_H
