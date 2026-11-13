// SoundSystem on SDL2 + libvorbisfile (see Audio.reference.h).
#include "stdafx.h"
#include "Audio.h"
#include <SDL2/SDL.h>
#include <vorbis/vorbisfile.h>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <map>
#include <memory>
#include <vector>
#include <mutex>

namespace {

const int RATE = 44100;

// a decoded sound: interleaved stereo float samples at its own rate
struct Clip {
	std::vector<float> data;
	int rate;
	Clip() : rate(RATE) {}
};

// an Ogg file opened for decoding as it plays (music, streaming)
struct Stream {
	OggVorbis_File vf;
	bool open;
	int rate, channels;
	std::vector<float> buf;                       // decoded, not yet consumed (stereo)
	size_t at;
	bool ended;
	Stream() : open(false), rate(RATE), channels(2), at(0), ended(false) {}
	~Stream() { if (open) ov_clear(&vf); }
	bool fill() {                                 // decode the next chunk; false at the end
		float** pcm = 0;
		int bit = 0;
		long n = ov_read_float(&vf, &pcm, 4096, &bit);
		if (n <= 0) return false;
		buf.erase(buf.begin(), buf.begin() + (long)at);
		at = 0;
		for (long i = 0; i < n; ++i) {
			buf.push_back(pcm[0][i]);
			buf.push_back(pcm[channels > 1 ? 1 : 0][i]);
		}
		return true;
	}
};

struct Source {
	std::shared_ptr<Clip> clip;
	std::shared_ptr<Stream> stream;
	std::string path;
	double pos;                                    // in source frames
	float volume, pitch, x, y, z, dist;
	int att;
	bool loop, playing, positional;
	Source() : pos(0), volume(1), pitch(1), x(0), y(0), z(0), dist(16), att(0), loop(false), playing(false),
	           positional(false) {}
};

SDL_AudioDeviceID g_dev = 0;
std::map<std::string, std::shared_ptr<Clip> > g_clips;      // by file path (g_clipLock)
std::mutex g_clipLock;
std::map<std::string, Source> g_sources;                     // by source name
float g_lx = 0, g_ly = 0, g_lz = 0, g_fx = 0, g_fy = 0, g_fz = -1, g_ux = 0, g_uy = 1, g_uz = 0;
float g_master = 1.0F;

// J2X_AUDIO_DUMP=FILE.wav (environment): also write everything the mixer plays to a 16-bit WAV -- what the
// game sounds like, checked without a person listening (tests, CI). J2X_AUDIO_LOG=1: one line per sound.
FILE* g_dump = 0;
unsigned g_dumped = 0;
bool g_log = false;

void dumpHeader(unsigned frames) {
	unsigned data = frames * 4, riff = 36 + data;
	unsigned char h[44] = {'R', 'I', 'F', 'F', 0, 0, 0, 0, 'W', 'A', 'V', 'E', 'f', 'm', 't', ' ', 16, 0, 0, 0, 1, 0, 2, 0,
	                       0x44, 0xAC, 0, 0, 0x10, 0xB1, 0x02, 0, 4, 0, 16, 0, 'd', 'a', 't', 'a', 0, 0, 0, 0};
	for (int k = 0; k < 4; ++k) { h[4 + k] = (unsigned char)(riff >> (8 * k)); h[40 + k] = (unsigned char)(data >> (8 * k)); }
	std::fseek(g_dump, 0, SEEK_SET);
	std::fwrite(h, 1, 44, g_dump);
	std::fseek(g_dump, 0, SEEK_END);
}

struct Lock {
	Lock() { if (g_dev) SDL_LockAudioDevice(g_dev); }
	~Lock() { if (g_dev) SDL_UnlockAudioDevice(g_dev); }
};

bool openOgg(const std::string& path, OggVorbis_File* vf, int* rate, int* channels) {
	if (ov_fopen(path.c_str(), vf) != 0) return false;
	vorbis_info* vi = ov_info(vf, -1);
	*rate = vi ? (int)vi->rate : RATE;
	*channels = vi ? vi->channels : 2;
	return true;
}

std::shared_ptr<Clip> decodeWav(const std::string& path) {
	SDL_AudioSpec spec;
	Uint8* buf = 0;
	Uint32 len = 0;
	if (!SDL_LoadWAV(path.c_str(), &spec, &buf, &len)) return std::shared_ptr<Clip>();
	SDL_AudioCVT cvt;
	SDL_BuildAudioCVT(&cvt, spec.format, spec.channels, spec.freq, AUDIO_F32SYS, 2, spec.freq);
	cvt.len = (int)len;
	std::vector<Uint8> work((size_t)len * (cvt.len_mult > 0 ? cvt.len_mult : 1) + 16);
	std::memcpy(&work[0], buf, len);
	cvt.buf = &work[0];
	SDL_ConvertAudio(&cvt);
	SDL_FreeWAV(buf);
	std::shared_ptr<Clip> c(new Clip());
	c->rate = spec.freq;
	const float* f = (const float*)&work[0];
	c->data.assign(f, f + cvt.len_cvt / sizeof(float));
	return c;
}

std::shared_ptr<Clip> clipFor(const std::string& path) {        // decoded outside the mixer's lock
	{
		std::lock_guard<std::mutex> g(g_clipLock);
		std::map<std::string, std::shared_ptr<Clip> >::iterator it = g_clips.find(path);
		if (it != g_clips.end()) return it->second;
	}
	std::shared_ptr<Clip> c;
	size_t dot = path.find_last_of('.');
	std::string ext = dot == std::string::npos ? "" : path.substr(dot + 1);
	if (ext == "wav" || ext == "WAV") {
		c = decodeWav(path);
	} else {
		OggVorbis_File vf;
		int rate, channels;
		if (openOgg(path, &vf, &rate, &channels)) {
			c.reset(new Clip());
			c->rate = rate;
			float** pcm = 0;
			int bit = 0;
			long n;
			while ((n = ov_read_float(&vf, &pcm, 4096, &bit)) > 0)
				for (long i = 0; i < n; ++i) {
					c->data.push_back(pcm[0][i]);
					c->data.push_back(pcm[channels > 1 ? 1 : 0][i]);
				}
			ov_clear(&vf);
		}
	}
	if (!c) std::fprintf(stderr, "[audio] cannot decode %s\n", path.c_str());
	std::lock_guard<std::mutex> g(g_clipLock);
	g_clips[path] = c;
	return c;
}

// gain and pan of a positional source as the listener hears it
void spatial(const Source& s, float* gl, float* gr) {
	float g = s.volume * g_master, pan = 0;
	if (s.positional) {
		float dx = s.x - g_lx, dy = s.y - g_ly, dz = s.z - g_lz;
		float d = std::sqrt(dx * dx + dy * dy + dz * dz);
		if (s.att == 2 && s.dist > 0) g *= d >= s.dist ? 0.0F : 1.0F - d / s.dist;      // linear fade
		if (d > 0.001F) {
			float rx = g_fy * g_uz - g_fz * g_uy, ry = g_fz * g_ux - g_fx * g_uz, rz = g_fx * g_uy - g_fy * g_ux;
			float rl = std::sqrt(rx * rx + ry * ry + rz * rz);
			if (rl > 0.0001F) pan = (dx * rx + dy * ry + dz * rz) / (d * rl);
		}
	}
	*gl = g * (pan > 0 ? 1.0F - pan * 0.6F : 1.0F);
	*gr = g * (pan < 0 ? 1.0F + pan * 0.6F : 1.0F);
}

void mix(void*, Uint8* bytes, int len) {
	float* out = (float*)bytes;
	int frames = len / (int)(2 * sizeof(float));
	std::memset(bytes, 0, (size_t)len);
	for (std::map<std::string, Source>::iterator it = g_sources.begin(); it != g_sources.end(); ++it) {
		Source& s = it->second;
		if (!s.playing) continue;
		float gl, gr;
		spatial(s, &gl, &gr);
		if (s.clip) {
			const std::vector<float>& d = s.clip->data;
			size_t n = d.size() / 2;
			double step = s.pitch * (double)s.clip->rate / RATE;
			for (int f = 0; f < frames && s.playing; ++f) {
				size_t i = (size_t)s.pos;
				if (i >= n) {
					if (s.loop && n) { s.pos = 0; i = 0; } else { s.playing = false; break; }
				}
				out[2 * f] += d[2 * i] * gl;
				out[2 * f + 1] += d[2 * i + 1] * gr;
				s.pos += step;
			}
		} else if (s.stream && s.stream->open) {
			Stream& st = *s.stream;
			double step = s.pitch * (double)st.rate / RATE;
			for (int f = 0; f < frames && s.playing; ++f) {
				size_t i = (size_t)s.pos;
				while (st.at + i >= st.buf.size() / 2) {
					if (!st.fill()) {
						if (s.loop) { ov_raw_seek(&st.vf, 0); continue; }
						s.playing = false;
						break;
					}
				}
				if (!s.playing) break;
				out[2 * f] += st.buf[2 * (st.at + i)] * gl;
				out[2 * f + 1] += st.buf[2 * (st.at + i) + 1] * gr;
				s.pos += step;
				size_t used = (size_t)s.pos;
				if (used > 0) { st.at += used; s.pos -= (double)used; }
			}
		}
	}
	for (int k = 0; k < frames * 2; ++k)                      // keep the sum in range
		out[k] = out[k] > 1.0F ? 1.0F : (out[k] < -1.0F ? -1.0F : out[k]);
	if (g_dump) {
		std::vector<short> s16((size_t)frames * 2);
		for (int k = 0; k < frames * 2; ++k) s16[(size_t)k] = (short)(out[k] * 32767.0F);
		std::fwrite(&s16[0], sizeof(short), s16.size(), g_dump);
		g_dumped += (unsigned)frames;
		dumpHeader(g_dumped);
		std::fflush(g_dump);
	}
}

void openDevice() {
	if (g_dev) return;
	if (!SDL_WasInit(SDL_INIT_AUDIO) && SDL_InitSubSystem(SDL_INIT_AUDIO) != 0) {
		std::fprintf(stderr, "[audio] SDL audio init failed: %s\n", SDL_GetError());
		return;
	}
	SDL_AudioSpec want, have;
	SDL_zero(want);
	want.freq = RATE;
	want.format = AUDIO_F32SYS;
	want.channels = 2;
	want.samples = 1024;
	want.callback = mix;
	g_dev = SDL_OpenAudioDevice(0, 0, &want, &have, 0);
	if (!g_dev) {
		std::fprintf(stderr, "[audio] no audio device: %s\n", SDL_GetError());
		return;
	}
	const char* dump = std::getenv("J2X_AUDIO_DUMP");
	if (dump && *dump && (g_dump = std::fopen(dump, "wb")) != 0) dumpHeader(0);
	g_log = std::getenv("J2X_AUDIO_LOG") != 0;
	SDL_PauseAudioDevice(g_dev, 0);
}

std::string pathOf(URL* url, const std::string& identifier) {
	return url && !url->j2xResourcePath().empty() ? url->j2xResourcePath() : identifier;
}

void makeSource(const std::string& name, URL* url, const std::string& id, bool loop, bool streaming,
                float x, float y, float z, int att, float dist) {
	std::string path = pathOf(url, id);
	if (g_log) std::fprintf(stderr, "[audio] %s <- %s\n", name.c_str(), path.c_str());
	std::shared_ptr<Clip> clip;
	std::shared_ptr<Stream> st;
	if (streaming) {
		st.reset(new Stream());
		st->open = openOgg(path, &st->vf, &st->rate, &st->channels);
		if (!st->open) std::fprintf(stderr, "[audio] cannot open %s\n", path.c_str());
	} else {
		clip = clipFor(path);
	}
	Lock l;
	Source s;
	s.path = path;
	s.loop = loop;
	s.x = x; s.y = y; s.z = z;
	s.att = att;
	s.dist = dist;
	s.positional = att != 0;
	s.clip = clip;
	s.stream = st;
	g_sources[name] = s;
}

} // namespace

SoundSystem::SoundSystem(...) { openDevice(); }
SoundSystem::~SoundSystem() {}

void SoundSystem::cleanup() {
	Lock l;
	g_sources.clear();
}

void SoundSystem::newSource(bool, const std::string& source, URL* url, const std::string& id, bool loop,
                            float x, float y, float z, int att, float dist) {
	makeSource(source, url, id, loop, false, x, y, z, att, dist);
}

void SoundSystem::newStreamingSource(bool, const std::string& source, URL* url, const std::string& id, bool loop,
                                     float x, float y, float z, int att, float dist) {
	makeSource(source, url, id, loop, true, x, y, z, att, dist);
}

void SoundSystem::backgroundMusic(const std::string& source, URL* url, const std::string& id, bool loop) {
	makeSource(source, url, id, loop, true, 0, 0, 0, 0, 0);
	play(source);
}

void SoundSystem::quickPlay(bool, URL* url, const std::string& id, bool loop, float x, float y, float z, int att, float dist) {
	static int n = 0;
	char name[32];
	std::snprintf(name, sizeof(name), "quick_%d", n++ & 255);
	makeSource(name, url, id, loop, false, x, y, z, att, dist);
	play(name);
}

void SoundSystem::play(const std::string& source) {
	Lock l;
	std::map<std::string, Source>::iterator it = g_sources.find(source);
	if (it != g_sources.end()) { it->second.playing = true; }
}

void SoundSystem::stop(const std::string& source) {
	Lock l;
	std::map<std::string, Source>::iterator it = g_sources.find(source);
	if (it != g_sources.end()) { it->second.playing = false; it->second.pos = 0; }
}

void SoundSystem::pause(const std::string& source) {
	Lock l;
	std::map<std::string, Source>::iterator it = g_sources.find(source);
	if (it != g_sources.end()) it->second.playing = false;
}

void SoundSystem::removeSource(const std::string& source) {
	Lock l;
	g_sources.erase(source);
}

void SoundSystem::setVolume(const std::string& source, float v) {
	Lock l;
	std::map<std::string, Source>::iterator it = g_sources.find(source);
	if (it != g_sources.end()) it->second.volume = v < 0 ? 0 : v;
}

void SoundSystem::setPitch(const std::string& source, float p) {
	Lock l;
	std::map<std::string, Source>::iterator it = g_sources.find(source);
	if (it != g_sources.end()) it->second.pitch = p > 0.01F ? p : 0.01F;
}

void SoundSystem::setPosition(const std::string& source, float x, float y, float z) {
	Lock l;
	std::map<std::string, Source>::iterator it = g_sources.find(source);
	if (it != g_sources.end()) { it->second.x = x; it->second.y = y; it->second.z = z; }
}

void SoundSystem::setListenerPosition(float x, float y, float z) {
	Lock l;
	g_lx = x; g_ly = y; g_lz = z;
}

void SoundSystem::setListenerOrientation(float lx, float ly, float lz, float ux, float uy, float uz) {
	Lock l;
	g_fx = lx; g_fy = ly; g_fz = lz; g_ux = ux; g_uy = uy; g_uz = uz;
}

bool SoundSystem::playing(const std::string& source) {
	Lock l;
	std::map<std::string, Source>::iterator it = g_sources.find(source);
	return it != g_sources.end() && it->second.playing;
}

void SoundSystem::setMasterVolume(float v) {
	Lock l;
	g_master = v;
}
