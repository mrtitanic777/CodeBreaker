#ifndef ZIPFILE_H
#define ZIPFILE_H
// ============================ java.util.zip.ZipFile (REAL) =====================================
// Reads the whole archive into memory and parses the ZIP central directory; getInputStream()
// inflates an entry on demand with rawInflate() (Inflate.h). Enables custom texture packs:
// TexturePackCustom reads pack.png / pack.txt and every overridden texture through this.
//
// BIG-ENDIAN HAZARD: the ZIP format is LITTLE-ENDIAN and the 360 is big-endian, so every
// multi-byte field is assembled byte-by-byte (le16/le32 in the .cpp) -- never cast/load.
// ==============================================================================================
#include <string>
#include <vector>
class InputStream;
class File;

class ZipFile {
public:
	struct Entry {
		std::string  name;
		int          method;       // 0 = stored, 8 = deflate
		unsigned int compSize;
		unsigned int uncompSize;
		unsigned int localOffset;  // offset of this entry's local file header
	};
private:
	std::vector<unsigned char> _data;     // the entire .zip slurped into RAM
	std::vector<Entry>         _entries;
	bool                       _ok;
	void _parse();
	void _slurp(const std::string& path);
public:
	ZipFile(File* f);
	ZipFile(const std::string& path);
	virtual ~ZipFile();
	virtual void close();
	virtual void* getEntry(const std::string& name);   // -> Entry* (owned by this) or NULL
	virtual InputStream* getInputStream(void* entry);  // inflated stream (caller owns) or NULL
	virtual int size();
};
#endif // ZIPFILE_H
