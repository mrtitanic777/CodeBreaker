#ifndef URLCONNECTION_H
#define URLCONNECTION_H

// ============================ EXTERNAL-LIBRARY STUB ============================
// Stands in for java.net.URLConnection.
// This is NOT part of the decompiled Minecraft source -- it is an external Java
// library. Symbol-only: methods are no-ops / return defaults so the converted game
// code compiles and links; the real behaviour is the Xbox 360 platform layer
// (Direct3D 9 / XInput / file IO / sound).
// ==============================================================================
#include <string>
class InputStream;
class OutputStream;

class URLConnection {
public:
	URLConnection(...) {}
	virtual ~URLConnection() {}
	virtual void connect(...) {}
	virtual void setDoInput(...) {}
	virtual void setDoOutput(...) {}
	virtual void setUseCaches(...) {}
	virtual void setRequestProperty(...) {}
	virtual void setConnectTimeout(...) {}
	virtual void setReadTimeout(...) {}
	virtual void disconnect(...) {}
	virtual InputStream* getInputStream(...) { return 0; }
	virtual OutputStream* getOutputStream(...) { return 0; }
	virtual int getContentLength(...) { return 0; }
	virtual int getResponseCode(...) { return 0; }
	virtual std::string getHeaderField(...) { return std::string(); }
};
#endif // URLCONNECTION_H
