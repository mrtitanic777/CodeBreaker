// The offline network (see Network.reference.h).
#include "stdafx.h"
#include "Network.h"
#include "URL.h"
#include "HttpURLConnection.h"
#include "IOException.h"
#include "ByteArrayInputStream.h"
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <string>
#include <system_error>
#include <vector>

namespace {
class OfflineConnection : public HttpURLConnection {
public:
	void connect(...) { throw new IOException(); }
	int getResponseCode(...) { throw new IOException(); }
	InputStream* getInputStream(...) { throw new IOException(); }
	void disconnect(...) {}
};

// minecraft.net/resources/ as early Minecraft (Indev) knew it: a listing of "name,size,time" lines, then each file under that address.
// The converter puts the game's sounds and music there (Content/resources, beside the program's working folder) from the workspace the
// game was converted from; this serves them, so the game's own resource thread registers them as if it had downloaded them.
const char* const kResourcesUrl = "http://www.minecraft.net/resources/";
const char* const kResourcesDir = "resources";

std::string percent_decoded(const std::string& s) {
	std::string out;
	for (size_t i = 0; i < s.size(); ++i) {
		unsigned v = 0;
		if (s[i] == '%' && i + 2 < s.size() && std::sscanf(s.c_str() + i + 1, "%2x", &v) == 1) {
			out += (char)v;
			i += 2;
		} else {
			out += s[i];
		}
	}
	return out;
}

InputStream* bytes_stream(const std::string& bytes) {
	JArray<unsigned char> a((int)bytes.size());
	if (!bytes.empty()) std::memcpy(a.data(), bytes.data(), bytes.size());
	return new ByteArrayInputStream(a);
}

std::string resources_listing() {
	namespace fs = std::filesystem;
	std::vector<std::string> lines;
	std::error_code ec;
	for (fs::recursive_directory_iterator it(kResourcesDir, ec), end; !ec && it != end; it.increment(ec)) {
		if (!it->is_regular_file(ec)) continue;
		const std::string name = it->path().lexically_relative(kResourcesDir).generic_string();
		const unsigned long long size = (unsigned long long)it->file_size(ec);
		char line[32];
		std::snprintf(line, sizeof line, ",%llu,0", size);
		lines.push_back(name + line);
	}
	std::sort(lines.begin(), lines.end());
	std::string out;
	for (size_t i = 0; i < lines.size(); ++i) out += lines[i] + "\n";
	return out;
}

bool read_resource(const std::string& name, std::string& bytes) {
	if (name.find("..") != std::string::npos) return false;   // (only what is under resources/)
	std::FILE* f = std::fopen((std::string(kResourcesDir) + "/" + name).c_str(), "rb");
	if (!f) return false;
	char buf[65536];
	size_t n;
	while ((n = std::fread(buf, 1, sizeof buf, f)) > 0) bytes.append(buf, n);
	std::fclose(f);
	return true;
}
}

URLConnection* j2xUrlOpenConnection(URL*) { return new OfflineConnection(); }

InputStream* j2xUrlOpenStream(URL* u) {
	const std::string spec = u ? u->j2xResourcePath() : std::string();
	const size_t base = std::strlen(kResourcesUrl);
	if (spec.compare(0, base, kResourcesUrl) != 0) throw new IOException();   // anything else: no network, as Java with none
	const std::string name = percent_decoded(spec.substr(base));
	if (name.empty()) return bytes_stream(resources_listing());
	std::string bytes;
	if (!read_resource(name, bytes)) throw new IOException();
	return bytes_stream(bytes);
}
