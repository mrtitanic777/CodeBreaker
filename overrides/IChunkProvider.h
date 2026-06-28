#ifndef ICHUNKPROVIDER_H
#define ICHUNKPROVIDER_H

// Auto-generated from IChunkProvider.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class Chunk;
class IProgressUpdate;

class IChunkProvider {
public:

	// Polymorphic deletion: WorldClient/World hold an IChunkProvider* and delete
	// through it (e.g. swapping the base-ctor-made provider for the client one).
	// Without a virtual dtor that is UB and leaks the concrete subclass.
	virtual ~IChunkProvider() {}

	virtual bool chunkExists(int var1, int var2) = 0;
	virtual Chunk* provideChunk(int var1, int var2) = 0;
	virtual void populate(IChunkProvider* var1, int var2, int var3) = 0;
	virtual bool saveChunks(bool var1, IProgressUpdate* var2) = 0;
	virtual bool unloadQueuedChunks() = 0;
	virtual bool canSave() = 0;
};

#endif // ICHUNKPROVIDER_H