#ifndef SOUNDSYSTEM_H
#define SOUNDSYSTEM_H
// Stub: Java sound system not used on Xbox 360
#include <string>

class SoundSystemConfig {
public:
    static int TYPE_STREAMING;
    static int TYPE_NORMAL;
    static void addLibrary(int lib) {}
    static void setCodec(const std::string& ext, int codec) {}
};

class SoundSystem {
public:
    SoundSystem() {}
    void init() {}
    void stop() {}
    void setListenerPosition(float x, float y, float z) {}
    void setListenerAngle(float yaw) {}
    void setMasterVolume(float v) {}
    void setVolume(const std::string& src, float v) {}
    void play(const std::string& src) {}
    void backgroundMusic(const std::string& name, const std::string& path, bool loop) {}
    void newSource(bool priority, const std::string& name, const std::string& path, float x, float y, float z, int type, float dist) {}
    void newStreamingSource(bool priority, const std::string& name, const std::string& path, float x, float y, float z, int type, float dist) {}
    bool playing(const std::string& src) { return false; }
};

class LibraryLWJGLOpenAL {};
class CodecJOrbis {};

#endif // SOUNDSYSTEM_H
