#include "stdafx.h"
#include "JavaRT.h"

#include "ZipFile.h"
#include "File.h"        // File (public .path) + FileInputStream + InputStream (all in FileIO.h)
#include "Inflate.h"     // rawInflate

// ---- LITTLE-ENDIAN field readers (ZIP is LE; the 360 is big-endian -> never cast multi-byte) ----
static unsigned int le16(const unsigned char* p) {
	return (unsigned int)p[0] | ((unsigned int)p[1] << 8);
}
static unsigned int le32(const unsigned char* p) {
	return (unsigned int)p[0] | ((unsigned int)p[1] << 8) |
	       ((unsigned int)p[2] << 16) | ((unsigned int)p[3] << 24);
}

// InputStream over an owned byte buffer (the decompressed entry). The caller (ImageIO / BufferedReader)
// reads then close()s it; it owns its copy so the source ZIP buffer can be reused/freed.
namespace {
class MemInputStream : public InputStream {
	std::vector<unsigned char> _b;
	size_t _pos;
public:
	MemInputStream(const unsigned char* d, size_t n) : _b(d, d + n), _pos(0) {}
	virtual int read() { return _pos < _b.size() ? (int)_b[_pos++] : -1; }
	virtual int read(unsigned char* b, int off, int len) {
		int n = 0; while (n < len && _pos < _b.size()) b[off + n++] = _b[_pos++];
		return (n > 0) ? n : (len == 0 ? 0 : -1);
	}
	virtual int read(unsigned char* b, int len) { return read(b, 0, len); }
	virtual int available() { return (int)(_b.size() - _pos); }
	virtual void close() {}
};
} // namespace

void ZipFile::_slurp(const std::string& path) {
	if (path.empty()) return;
	FileInputStream fis(path);
	unsigned char tmp[8192]; int n;
	while ((n = fis.read(tmp, 0, (int)sizeof(tmp))) > 0)
		_data.insert(_data.end(), tmp, tmp + n);
	fis.close();
}

ZipFile::ZipFile(File* f) : _ok(false) {
	_slurp(f ? f->path : std::string());
	_parse();
}

ZipFile::ZipFile(const std::string& path) : _ok(false) {
	_slurp(path);
	_parse();
}

ZipFile::~ZipFile() {}
void ZipFile::close() {}
int ZipFile::size() { return (int)_entries.size(); }

void ZipFile::_parse() {
	const size_t N = _data.size();
	if (N < 22) return;                       // smaller than an empty End-Of-Central-Directory record
	const unsigned char* d = &_data[0];

	// Find the EOCD (signature PK\05\06 = 0x06054b50), scanning back from the end. The trailing
	// comment can be up to 65535 bytes, so bound the search.
	size_t eocd = (size_t)-1;
	size_t minPos = (N > (size_t)(22 + 65535)) ? (N - (22 + 65535)) : 0;
	for (size_t i = N - 22; ; --i) {
		if (d[i] == 0x50 && d[i + 1] == 0x4b && d[i + 2] == 0x05 && d[i + 3] == 0x06) { eocd = i; break; }
		if (i == minPos) break;
	}
	if (eocd == (size_t)-1 || eocd + 20 > N) return;

	unsigned int count = le16(d + eocd + 10);     // total central-directory entries
	unsigned int cdOff = le32(d + eocd + 16);     // offset of the central directory
	if (cdOff >= N) return;

	size_t p = cdOff;
	for (unsigned int e = 0; e < count; ++e) {
		if (p + 46 > N) break;
		if (!(d[p] == 0x50 && d[p + 1] == 0x4b && d[p + 2] == 0x01 && d[p + 3] == 0x02)) break;  // PK\01\02
		Entry en;
		en.method      = (int)le16(d + p + 10);
		en.compSize    = le32(d + p + 20);
		en.uncompSize  = le32(d + p + 24);
		unsigned int nameLen  = le16(d + p + 28);
		unsigned int extraLen = le16(d + p + 30);
		unsigned int cmtLen   = le16(d + p + 32);
		en.localOffset = le32(d + p + 42);
		if (p + 46 + nameLen > N) break;
		en.name.assign((const char*)(d + p + 46), nameLen);
		_entries.push_back(en);
		p += 46 + nameLen + extraLen + cmtLen;
	}
	_ok = true;
}

void* ZipFile::getEntry(const std::string& name) {
	for (size_t i = 0; i < _entries.size(); ++i)
		if (_entries[i].name == name) return &_entries[i];
	return 0;
}

InputStream* ZipFile::getInputStream(void* entryPtr) {
	if (!entryPtr || _data.empty()) return 0;
	Entry* en = (Entry*)entryPtr;
	const unsigned char* d = &_data[0];
	const size_t N = _data.size();

	// The local file header repeats name/extra lengths (often differing from the central dir), so the
	// data start MUST be computed from the local header, not the central one.
	size_t lo = en->localOffset;
	if (lo + 30 > N) return 0;
	if (!(d[lo] == 0x50 && d[lo + 1] == 0x4b && d[lo + 2] == 0x03 && d[lo + 3] == 0x04)) return 0;  // PK\03\04
	unsigned int lNameLen  = le16(d + lo + 26);
	unsigned int lExtraLen = le16(d + lo + 28);
	size_t dataOff = lo + 30 + lNameLen + lExtraLen;
	if (dataOff + en->compSize > N) return 0;
	const unsigned char* comp = d + dataOff;

	if (en->method == 0) {                          // stored (no compression)
		return new MemInputStream(comp, en->compSize);
	}
	if (en->method == 8) {                          // deflate
		if (en->uncompSize == 0) return new MemInputStream(comp, 0);
		std::vector<unsigned char> out(en->uncompSize);
		int got = rawInflate(comp, (int)en->compSize, &out[0], (int)out.size());
		if (got <= 0) return 0;
		return new MemInputStream(&out[0], (size_t)got);
	}
	return 0;                                        // unsupported method
}
