#ifndef SOUNDPOOLENTRY_H
#define SOUNDPOOLENTRY_H

// Auto-generated from SoundPoolEntry.java
// Target: Xbox 360 / C++03 / DirectX 9

#include <string>

class URL;

class SoundPoolEntry {
public:
	SoundPoolEntry();
	SoundPoolEntry(const std::string& var1, std::string /* PORT: was java.net.URL */* var2);
	virtual ~SoundPoolEntry();

	std::string m_soundName;
	std::string /* PORT: was java.net.URL */* m_soundUrl;


};

#endif // SOUNDPOOLENTRY_H