#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from Packet51MapChunk.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Packet51MapChunk.h"
#include "DataInputStream.h"
#include "DataOutputStream.h"
#include "IOException.h"
#include "NetHandler.h"
#include "Packet.h"

/* ---- ORIGINAL JAVA (stubbed below) ----
 *  // These were java.util.zip.Inflater / DataFormatException (real zlib inflate).
 *  // The shim below fakes the API and inflate() is a no-op returning 0.
 *  import java.util.zip.DataFormatException;
 *  import java.util.zip.Inflater;
 *  ---- end original Java ---- */
// --- Local shims (runtime-stub gaps; see final report) ---
// java.util.zip.Inflater / DataFormatException are not provided by the runtime
// headers. This shim preserves the call shape (setInput/inflate/end). The real
// zlib-backed decompression must be supplied centrally by the converter owner;
// until then inflate() is a no-op that returns 0.
namespace {
	class DataFormatException {};
	class Inflater {
		JArray<unsigned char> _in;
	public:
		Inflater() {}
		void setInput(const JArray<unsigned char>& b) { _in = b; }
		// Returns number of bytes inflated into out. TODO(runtime): real zlib inflate.
		int inflate(JArray<unsigned char>& /*out*/) { return 0; }
		void end() {}
	};
}

Packet51MapChunk::Packet51MapChunk()
	: xPosition(0)
	, yPosition(0)
	, zPosition(0)
	, xSize(0)
	, ySize(0)
	, zSize(0)
	, chunkSize(0)
{
	this->isChunkDataPacket = true;
}

Packet51MapChunk::~Packet51MapChunk()
{
}

void Packet51MapChunk::readPacketData(DataInputStream* var1)
{
	this->xPosition = var1->readInt();
	this->yPosition = var1->readShort();
	this->zPosition = var1->readInt();
	this->xSize = var1->read() + 1;
	this->ySize = var1->read() + 1;
	this->zSize = var1->read() + 1;
	int var2 = var1->readInt();
	JArray<unsigned char > var3 = JArray<unsigned char >(var2);
	var1->readFully(var3);
	this->chunk = JArray<unsigned char >(this->xSize * this->ySize * this->zSize * 5 / 2);
	/* ---- ORIGINAL JAVA (stubbed below) ----
	 *  // var4.inflate(this.chunk) really decompresses the chunk payload here.
	 *  // In the port the shim Inflater.inflate() is a no-op, so this.chunk is
	 *  // never populated (stays all-zero). The try/finally is preserved via the
	 *  // RAII guard so end() still runs on every path.
	 *  Inflater var4 = new Inflater();
	 *  var4.setInput(var3);
	 *
	 *  try {
	 *      var4.inflate(this.chunk);
	 *  } catch (DataFormatException var9) {
	 *      throw new IOException("Bad compressed data format");
	 *  } finally {
	 *      var4.end();
	 *  }
	 *  ---- end original Java ---- */
	Inflater* var4 = new Inflater();
	var4->setInput(var3);
	// try/finally -> RAII guard so end() runs on every path (normal, throw, rethrow).
	struct InflaterEndGuard {
		Inflater* inf;
		InflaterEndGuard(Inflater* i) : inf(i) {}
		~InflaterEndGuard() { inf->end(); }
	} var4guard(var4);
	try {
	var4->inflate(this->chunk);
	} catch (DataFormatException) {
	throw new IOException("Bad compressed data format");
	}
}

void Packet51MapChunk::writePacketData(DataOutputStream* var1)
{
	var1->writeInt(this->xPosition);
	var1->writeShort(this->yPosition);
	var1->writeInt(this->zPosition);
	var1->write(this->xSize - 1);
	var1->write(this->ySize - 1);
	var1->write(this->zSize - 1);
	var1->writeInt(this->chunkSize);
	var1->write(this->chunk, 0, this->chunkSize);
}

void Packet51MapChunk::processPacket(NetHandler* var1)
{
	var1->handleMapChunk(this);
}

int Packet51MapChunk::getPacketSize()
{
	return 17 + this->chunkSize;
}
