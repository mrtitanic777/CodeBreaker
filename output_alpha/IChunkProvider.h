#ifndef ICHUNKPROVIDER_H
#define ICHUNKPROVIDER_H

// Auto-generated from IChunkProvider.java
// Target: Xbox 360 / C++03 / DirectX 9

class Chunk;
class IProgressUpdate;

class IChunkProvider {
public:
	virtual ~IChunkProvider() {}
	virtual bool chunkExists(int var1, int var2) = 0;
	virtual Chunk* provideChunk(int var1, int var2) = 0;
	virtual void populate(IChunkProvider* var1, int var2, int var3) = 0;
	virtual bool saveChunks(bool var1, IProgressUpdate* var2) = 0;
	virtual bool func_532_a() = 0;
	virtual bool func_536_b() = 0;
};

#endif // ICHUNKPROVIDER_H