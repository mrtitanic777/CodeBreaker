#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from ThreadDownloadImage.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ThreadDownloadImage.h"
#include "BufferedImage.h"
#include "Exception.h"
#include "ImageBuffer.h"
#include "ImageIO.h"
#include "InputStream.h"
#include "Thread.h"
#include "ThreadDownloadImageData.h"
#include "URL.h"

/* ---- ORIGINAL JAVA (stubbed below) ----
 *  // java.net.HttpURLConnection used by run(); the real methods perform the HTTP request:
 *  HttpURLConnection var1 = null;
 *  URL var2 = new URL(this.location);
 *  var1 = (HttpURLConnection)var2.openConnection();
 *  var1.setDoInput(true);
 *  var1.setDoOutput(false);
 *  var1.connect();
 *  if(var1.getResponseCode() != 404) { ... ImageIO.read(var1.getInputStream()); ... }
 *  // finally: var1.disconnect();
 *  ---- end original Java ---- */
// Local shim: java.net.HttpURLConnection has no central runtime stub (see report). URL::open-
// Connection() returns void*; this models the methods used here. Network ops are no-ops for now.
// STUB: connect()/getResponseCode()/getInputStream()/disconnect() do no real networking.
struct HttpURLConnection {
	void setDoInput(bool) {}
	void setDoOutput(bool) {}
	void connect() {}
	int getResponseCode() { return 200; }
	InputStream* getInputStream() { return 0; }
	void disconnect() {}
};

ThreadDownloadImage::ThreadDownloadImage(ThreadDownloadImageData* var1, const JavaString& var2, ImageBuffer* var3)
	: buffer(NULL)
	, imageData(NULL)
{
	this->imageData = var1;
	this->location = var2;
	this->buffer = var3;
}

ThreadDownloadImage::~ThreadDownloadImage()
{
}

void ThreadDownloadImage::run()
{
	HttpURLConnection* var1 = NULL;
	// Java try/finally: the `finally { var1.disconnect(); }` must run on EVERY exit path
	// (normal return, early return, and the caught exception). C++ has no finally, so an RAII
	// guard runs disconnect() in its destructor regardless of how this scope is left.
	struct _Finally {
		HttpURLConnection** c;
		_Finally(HttpURLConnection** p) : c(p) {}
		~_Finally() { if(*c) (*c)->disconnect(); }
	} _finallyGuard(&var1);   // NOT '_finally': MSVC treats _finally as the __finally SEH keyword
	try {
	URL* var2 = new URL(this->location);
	var1 = (HttpURLConnection*)var2->openConnection();
	var1->setDoInput(true);
	var1->setDoOutput(false);
	var1->connect();
	if(var1->getResponseCode() != 404) {
	if(this->buffer == NULL) {
	this->imageData->image = ImageIO::read(var1->getInputStream());
	} else {
	this->imageData->image = this->buffer->parseUserSkin(ImageIO::read(var1->getInputStream()));
	}
	return;
	}
	} catch (Exception var6) {
	var6.printStackTrace();
	return;
	}
}
