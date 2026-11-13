// The offline network (see Network.reference.h).
#include "stdafx.h"
#include "Network.h"
#include "URL.h"
#include "HttpURLConnection.h"
#include "IOException.h"

namespace {
class OfflineConnection : public HttpURLConnection {
public:
	void connect(...) { throw new IOException(); }
	int getResponseCode(...) { throw new IOException(); }
	InputStream* getInputStream(...) { throw new IOException(); }
	void disconnect(...) {}
};
}

URLConnection* j2xUrlOpenConnection(URL*) { return new OfflineConnection(); }
InputStream* j2xUrlOpenStream(URL*) { throw new IOException(); }
